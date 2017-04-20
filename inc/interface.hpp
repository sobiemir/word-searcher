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

#ifndef __WSH_INTERFACE__
#define __WSH_INTERFACE__

#include <cstdlib>
#include <cstring>
#include "configuration.hpp"
#include "textbox.hpp"
#include "panel.hpp"

#ifdef WSD_SYSTEM_WINDOWS
#	include "curses.h"
#else
#	include <ncurses.h>
#endif

/**
 * Klasa intefejsu programu.
 * Zawiera funkcje odpowiedzialne za zarządzanie interfejsem.
 * Funkcje odświeżające listę plików lub aktualny plik wywoływane są prosto z klasy Searcher.
 * Klasa pozwala na oddzielenie logiki aplikacji od jej wyglądu.
 */
class Interface
{
public:
	/**
	 * Okno główne.
	 * W oknie tym znajdują się pola tekstowe, odpowiedzialne za filtr, folder i frazę.
	 */
	WINDOW *MainWindow;
	/**
	 * Okno wyszukiwania.
	 * Do okna podpięty jest panel wyszukiwania, w którym wyświetlany jest aktualnie przetwarzany plik.
	 */
	WINDOW *SearchWindow;
	/**
	 * Okno wyników.
	 * Do okna podpięty jest panel wyników, w którym wyświetlana jest lista plików spełniających
	 * kryteria wyszukiwania.
	 */
	WINDOW *ResultWindow;

	/**
	 * Pole tekstowe dla folderu.
	 * Dostępne po wciśnięciu kombinacji klawiszy CTRL + D.
	 */
	TextBox Folder;
	/**
	 * Pole tekstowe dla frazy.
	 * Dostępne po wciśnięciu kombinacji klawiszy CTRL + F.
	 */
	TextBox Phrase;
	/**
	 * Pole tekstowe dla filtru.
	 * Dostępne po wciśnięciu kombinacji klawiszy CTRL + S.
	 */
	TextBox Filter;

	/**
	 * Panel wyszukiwania.
	 * Wyświetla aktualnie przeszukiwany plik przez klasę Searcher.
	 */
	Panel SearchPanel;
	/**
	 * Panel wyników.
	 * Wyświetla wszystkie pliki, spełniające kryteria wyszukiwania.
	 */
	Panel ResultPanel;

	/**
	 * Style czcionek.
	 * Zawiera style, w jakich wyświetlane są teksty.
	 * W przypadku gdy konsola nie obsługuje koloru, wszystkie wartości ustawione są na 0.
	 */
	int TextStyle[4];

	/**
	 * Informacja o uruchomieniu wyszukiwarki.
	 * Gdy klasa odpowiedzialna za wyszukiwanie zostanie uruchomiona, zmienna będzie zawierała wartość TRUE.
	 * Zmienna używana w pętli głównej programu.
	 */
	bool Searching;

// =====================================================================================================================

	/**
	 * Konstruktor klasy.
	 * Uzupełnia pola klasy domyślnymi wartościami.
	 */
	Interface( void );

	/**
	 * Destruktor klasy.
	 * Niszczy utworzone wcześniej okna aplikacji.
	 */
	~Interface( void );

	/**
	 * Wyświetla przekazaną informację w stopce.
	 * 
	 * @param show    Pokazać wiadomość czy wyczyścić ekran?
	 * @param message Wiadomość do wyświetlenia.
	 */
	void ToggleFooterMessage( bool show = true, const char *message = "" );

	/**
	 * Wyświetla przekazaną informację w nagłówku.
	 * 
	 * @param show    Pokazać wiadomość czy wyczyścić ekran?
	 * @param message Wiadomość do wyświetlenia.
	 */
	void ToggleHeaderMessage( bool show = false, const char *message = "" );

	/**
	 * Odświeża linię aktualnie przeszukiwanego pliku.
	 * Funkcja wywoływana z poziomu klasy Searcher.
	 * 
	 * @param file Nazwa pliku do wyświetlenia.
	 */
	void RefreshCurrentFile( string *file );

	/**
	 * Odświeża listę plików, w których znaleziona została fraza.
	 * Funkcja wywolywana z poziomu klasy Searcher.
	 * Odwołanie do niej występuje tylko, gdy w przetwarzanym pliku znaleziona została fraza.
	 * Pozwala na ponowne wypisanie listy plików w panelu i wyświetlenie aktualnych wyników.
	 */
	void RefreshPrintedFiles( void );

	/**
	 * Inicjalizuje kolory w terminalu.
	 * W przypadku gdy terminal nie obsługuje kolorów, funkcja zamyka się automatycznie.
	 */
	void InitColors( void );

	/**
	 * Zmienia wymiary okien aplikacji.
	 * Funkcja powinna być wywoływana w trakcie zmiany rozmiaru konsoli / terminala.
	 * Pozwala na dopasowanie interfejsu aplikacji do aktualnych wymiarów okna terminala.
	 */
	void TerminalResize( void );

	/**
	 * Zwalnia pamięć po utworzonych oknach.
	 * Funkcja wywoływana przez destruktor i funkcje dopasowującą interfejs do rozmiaru terminala.
	 */
	void DestroyWindows( void );
};

#endif
