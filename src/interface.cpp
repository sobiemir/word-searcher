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

#include "../inc/interface.hpp"

// =====================================================================================================================

Interface::Interface( void )
{
    this->Folder = TextBox( "Folder : ", "./", 0 );
    this->Phrase = TextBox( "Fraza  : ", ""  , 0 );
    this->Filter = TextBox( "Filtr  : ", "*" , 0 );
    
    this->MainWindow = this->SearchWindow =
    this->HelpWindow = this->ResultWindow = NULL;

    this->TextStyle[0] = this->TextStyle[1] =
    this->TextStyle[2] = this->TextStyle[3] = 0;
}

// =====================================================================================================================

Interface::~Interface( void )
{
    this->DestroyWindows();
}

// =====================================================================================================================

void Interface::RefreshCurrentFile( string *file )
{
    this->SearchPanel.Print( file );
}

// =====================================================================================================================

void Interface::RefreshPrintedFiles( void )
{
    this->ResultPanel.RefreshTextSource();
    this->ResultPanel.Print();
}

// =====================================================================================================================

void Interface::InitColors( void )
{
    int colors    = has_colors();
    int defcolors = (use_default_colors() != ERR);

    // jeżeli terminal nie obsługuje albo nie pozwala na wyświetlanie kolorów, pomiń
    if( !colors )
        return;

    start_color();

    // jeżeli terminal pozwala na wyświetlanie domyślnego koloru
    if( defcolors )
    {
        init_pair( 1, COLOR_CYAN, -1 );
        init_pair( 2, COLOR_YELLOW, -1 );
        init_pair( 3, COLOR_BLACK, COLOR_WHITE );
        init_pair( 4, COLOR_BLACK, COLOR_YELLOW );
    }
    else
    {
        init_pair( 1, COLOR_CYAN, COLOR_BLACK );
        init_pair( 2, COLOR_YELLOW, COLOR_BLACK );
        init_pair( 3, COLOR_BLACK, COLOR_WHITE );
        init_pair( 4, COLOR_BLACK, COLOR_YELLOW );
    }

    this->TextStyle[0] = COLOR_PAIR(1);
    this->TextStyle[1] = COLOR_PAIR(2);
    this->TextStyle[2] = COLOR_PAIR(3);
    this->TextStyle[3] = A_BOLD | COLOR_PAIR(4);
    
    this->Folder.SetStyle( this->TextStyle[0], 0 );
    this->Phrase.SetStyle( this->TextStyle[0], 0 );
    this->Filter.SetStyle( this->TextStyle[0], 0 );
}

// =====================================================================================================================

void Interface::TerminalResize( void )
{
    int resultw,
        resulth;

    // usuń stare okna - trzeba odświeżyć rozmiary
    this->DestroyWindows();

    resultw = COLS - 2;
    resulth = LINES - 8;

    // utwórz nowe okna
    if( !(this->MainWindow = newwin(LINES, COLS, 0, 0)) )
        exit( EXIT_FAILURE );
    if( !(this->SearchWindow = newwin(1, resultw, 5, 1)) )
        exit( EXIT_FAILURE );
    if( !(this->ResultWindow = newwin(resulth, resultw, 7, 1)) )
        exit( EXIT_FAILURE );

    keypad( this->MainWindow, TRUE );
    keypad( this->ResultWindow, TRUE );

    // położenie pól tekstowych
    this->Folder.SetSize( COLS - 2 );
    this->Folder.SetWindow( this->MainWindow );
    this->Folder.SetPosition( 1, 1 );

    this->Phrase.SetSize( COLS - 2 );
    this->Phrase.SetWindow( this->MainWindow );
    this->Phrase.SetPosition( 1, 2 );

    this->Filter.SetSize( COLS - 2 );
    this->Filter.SetWindow( this->MainWindow );
    this->Filter.SetPosition( 1, 3 );

    // ramka wokół programu
    wattron( this->MainWindow, this->TextStyle[1] );
    wborder(
        this->MainWindow,
        ACS_VLINE,    ACS_VLINE,
        ACS_HLINE,    ACS_HLINE,
        ACS_ULCORNER, ACS_URCORNER,
        ACS_LLCORNER, ACS_LRCORNER
    );
    mvwprintw( this->MainWindow, 0, COLS - 23, " WordSearcher v0.2.1 " );
    wattroff( this->MainWindow, this->TextStyle[1] );

    // wyświetl pola tekstowe
    this->Folder.Print();
    this->Phrase.Print();
    this->Filter.Print();

    // linie oddzielające aktualnie przeszukiwany folder i listę znalezionych plików
    mvwhline( this->MainWindow, 4, 1, ACS_HLINE, COLS - 2 );
    mvwhline( this->MainWindow, 6, 1, ACS_HLINE, COLS - 2 );

    // położenie paneli
    this->ResultPanel.SetWindow( this->ResultWindow );
    this->ResultPanel.SetDimension( resultw, resulth );
    this->ResultPanel.SetPosition( 0, 0 );

    this->SearchPanel.SetWindow( this->SearchWindow );
    this->SearchPanel.SetDimension( resultw, 1 );
    this->SearchPanel.SetPosition( 0, 0 );

    // wyświetl panele
    this->ResultPanel.Print();
    this->SearchPanel.Print();

    // wattron( this->SearchWindow, this->TextStyle[1] );
    // mvwprintw( this->SearchWindow, 0, (COLS - 2) / 2 - 9, " [^H] Okno pomocy " );
    // wattroff( this->SearchWindow, this->TextStyle[1] );

    // odśwież wszystkie okna
    wrefresh( this->MainWindow );
    wrefresh( this->SearchWindow );
    wrefresh( this->ResultWindow );
}

// =====================================================================================================================

void Interface::DestroyWindows( void )
{
    if( this->MainWindow )
        delwin( this->MainWindow );
    if( this->SearchWindow )
        delwin( this->SearchWindow );
    if( this->ResultWindow )
        delwin( this->ResultWindow );
    if( this->HelpWindow )
        delwin( this->HelpWindow );
    
    this->MainWindow = this->SearchWindow =
    this->HelpWindow = this->ResultWindow = NULL;
}
