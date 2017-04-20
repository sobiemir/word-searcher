/*
 *  WordSearcher
 *  Copyright (C) 2013-2017 sobiemir <sobiemir@aculo.pl>
 * 
 *  This program is free software: you can redistribute it and/or modify  
 *  it under the terms of the GNU Lesser General Public License as   
 *  published by the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include <string>

#include "../inc/configuration.hpp"
#include "../inc/interface.hpp"
#include "../inc/searcher.hpp"
#include "../inc/control.hpp"

#ifdef WSD_SYSTEM_WINDOWS
#	include "../extra/curses.h"
#else
#	include <ncurses.h>
#	include <pthread.h>
#	include <signal.h>
#	include <errno.h>
#endif

using namespace std;

#ifdef WSD_SYSTEM_WINDOWS
	/**
	 * Funkcja wywoływana w osobnym wątku.
	 * Odpowiada za wyszukiwanie frazy w plikach.
	 * Wersja dla systemu Windows.
	 * 
	 * @param ptr Wskaźnik do klasy wyszukiwarki.
	 * @return Kod błędu.
	 */
	DWORD WINAPI non_blocking_search( void *ptr )
	{
		Searcher *searcher = static_cast<Searcher*>(ptr);
		searcher->Run();

		return 0;
	}
#else
	/**
	 * Funkcja wywoływana w osobnym wątku.
	 * Odpowiada za wyszukiwanie frazy w plikach.
	 * Wersja dla systemu Linux i standardu POSIX.
	 * 
	 * @param ptr Wskaźnik do klasy wyszukiwarki.
	 * @return Status wyjścia.
	 */
	void *non_blocking_search( void *ptr )
	{
		Searcher *searcher = static_cast<Searcher*>(ptr);
		searcher->Run();

		return NULL;
	}
#endif

/**
 * Funkcja główna aplikacji.
 * 
 * @param argc Ilość argumentów przekazywanych do programu.
 * @param argv Argumenty przekazywane do programu.
 * 
 * @return Kod błędu.
 */
int main( int argc, char *argv[] )
{
	Control *focused = NULL;

	// inicjalizacja ncurses
	if( initscr() == NULL )
		return EXIT_FAILURE;
	
	raw();
	nonl();
	noecho();
	keypad( stdscr, TRUE );
	set_escdelay( 0 );

	Interface ws;
	Searcher  searcher;

	// źródło wyświetlanych danych
	searcher.Printer = &ws;
	ws.ResultPanel.SetTextSource( &searcher.FoundFiles );

	ws.InitColors();
	ws.TerminalResize();

	bool run    = true;
	bool really = false;

#ifdef WSD_SYSTEM_WINDOWS
	DWORD  threadid = 0;
	HANDLE searchthread = NULL;
#else
	pthread_t searchthread;
#endif

	while( run )
	{
		int chr = wgetch( ws.MainWindow );

#	ifdef WSD_SYSTEM_WINDOWS
		if( WaitForSingleObject(searchthread, 0) == WAIT_OBJECT_0 )
			ws.Searching       = false,
			searcher.Searching = false,

			ws.ToggleHeaderMessage( false ),
			ws.ToggleFooterMessage( false );
#	else
		// nie pozwól, aby błąd w wątku spowodował zawieszenie programu
		if( ws.Searching && pthread_kill(searchthread, 0) == ESRCH )
			ws.Searching       = false,
			searcher.Searching = false,

			ws.ToggleHeaderMessage( false ),
			ws.ToggleFooterMessage( false );
#	endif

		// reakcje na specjalne zdarzenia, działające podczas wyszukiwania
		if( chr == 3 ) // ^C
			if( ws.Searching )
				searcher.Searching = false;
			else
				really = true,
				ws.ToggleFooterMessage( true, " Sekwencja ^C + ESC " );
#	ifdef KEY_RESIZE
		else if( chr == KEY_RESIZE )
			ws.TerminalResize();
#	endif
		else if( chr == 27 && really == true ) // ESC
			run = false;
		else
			really = false,
			ws.ToggleFooterMessage( false );

		// jeżeli wyszukiwanie się rozpoczęło, pozostaw akcje pod spodem w spokoju
		if( ws.Searching )
			continue;

		switch( chr )
		{
			break;
			case 9: // ^I
				ws.Folder.Focus( false );
				focused = &ws.Folder;
			break;
			case 15: // ^O
				ws.Phrase.Focus( false );
				focused = &ws.Phrase;
			break;
			case 16: // ^P
				ws.Filter.Focus( false );
				focused = &ws.Filter;
			break;
			case 17: // ^Q
				ws.ResultPanel.Focus( false );
				focused = &ws.ResultPanel;
			break;
			case 18: // ^R
			{
				focused = NULL;

				// ustaw kryteria wyszukiwania
				searcher.Criteria( ws.Folder.GetContent(), ws.Phrase.GetContent(), ws.Filter.GetContent() );

#			ifdef WSD_SYSTEM_WINDOWS
				BOOL error = FALSE;

				// zamknij uchwyt do poprzedniego wątku
				if( searchthread )
					CloseHandle( searchthread );
				searchthread = NULL;
				
				// próbuj utworzyć wątek, jeżeli się nie uda, trudno
				if( !(searchthread = CreateThread(NULL, 0, non_blocking_search, &searcher, 0, &threadid)) )
					break;

				EnterCriticalSection( &searcher.Mutex );
				
				// czekaj maksymalnie 4 sekundy na zamianę statusu
				while( !error && !ws.Searching )
					error = SleepConditionVariableCS( &searcher.Condition, &searcher.Mutex, 4000 );
				
				// jeżeli do tej pory odpowiedzi nie było, próbuj go zamknąć
				if( GetLastError() == ERROR_TIMEOUT )
					TerminateThread( searchthread, 0 );
				
				LeaveCriticalSection( &searcher.Mutex );
#			else
				struct timespec ts;
				int    ercode = 0;

				// próbuj utworzyć wątek, jeżeli się nie uda, trudno
				if( pthread_create(&searchthread, NULL, non_blocking_search, &searcher) )
					break;

				pthread_mutex_lock( &searcher.Mutex );

				clock_gettime( CLOCK_REALTIME, &ts );
				ts.tv_sec += 4;

				// czekaj maksymalnie 4 sekundy na zamianę statusu
				while( ercode == 0 && !ws.Searching )
					ercode = pthread_cond_timedwait( &searcher.Condition, &searcher.Mutex, &ts );
				
				// jeżeli do tej pory odpowiedzi nie było, próbuj go zamknąć
				if( ercode == ETIMEDOUT )
					pthread_cancel( searchthread );

				pthread_mutex_unlock( &searcher.Mutex );
#			endif
			}
			break;
		}

		// przetwarzaj wciśnięty klawisz w kontrolce
		if( focused )
			focused->ParseKey( chr );
	}

#ifdef WSD_SYSTEM_WINDOWS
	if( searchthread )
		CloseHandle( searchthread );
#endif

	clear();
	refresh();
	endwin();

	return EXIT_SUCCESS;
}
