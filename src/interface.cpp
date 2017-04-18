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

#include "interface.hpp"

Interface::Interface( void )
{
    this->Folder = TextBox( "Folder : ", "./", 0 );
    this->Phrase = TextBox( "Fraza  : ", ""  , 0 );
    this->Filter = TextBox( "Filtr  : ", "*" , 0 );
    
    this->MainWindow = this->SearchWindow =
    this->HelpWindow = this->ResultWindow = NULL;

    this->TextStyle[0] = this->TextStyle[1] =
    this->TextStyle[2] = this->TextStyle[3] = 0;

    // this->FoundPosition = 4;
}

Interface::~Interface( void )
{
    this->DestroyWindows();
}

void Interface::RefreshPrintedFiles( void )
{
    // mvwprintw( this->MainWindow, this->FoundPosition++, 1, file.c_str() );
    // this->ResultPanel.RefreshSource();
    // this->ResultPanel.PrintText();
}

void Interface::InitColors( void )
{
    int colors    = has_colors();
    int defcolors = (use_default_colors() != ERR);

    if( !colors )
        return;

    start_color();

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

void Interface::TerminalResize( void )
{
    int resultw,
        resulth;

    this->DestroyWindows();

    resultw = COLS - 2;
    resulth = (LINES - 4) / 2 - 2;

    if( !(this->MainWindow = newwin(LINES, COLS, 0, 0)) )
        exit( EXIT_FAILURE );
    if( !(this->ResultWindow = newwin(resulth, resultw, 4, 1)) )
        exit( EXIT_FAILURE );

    keypad( this->MainWindow, TRUE );
    keypad( this->ResultWindow, TRUE );

    this->Folder.SetSize( COLS - 2 );
    this->Folder.SetWindow( this->MainWindow );
    this->Folder.SetPosition( 1, 1 );

    this->Phrase.SetSize( COLS - 2 );
    this->Phrase.SetWindow( this->MainWindow );
    this->Phrase.SetPosition( 1, 2 );

    this->Filter.SetSize( COLS - 2 );
    this->Filter.SetWindow( this->MainWindow );
    this->Filter.SetPosition( 1, 3 );

    wattron( this->MainWindow, this->TextStyle[1] );
    wborder(
        this->MainWindow,
        ACS_VLINE,    ACS_VLINE,
        ACS_HLINE,    ACS_HLINE,
        ACS_ULCORNER, ACS_URCORNER,
        ACS_LLCORNER, ACS_LRCORNER
    );

    mvwprintw( this->MainWindow, 0, COLS - 23, " WordSearcher v0.2.0 " );
    wattroff( this->MainWindow, this->TextStyle[1] );

    this->Folder.Print();
    this->Phrase.Print();
    // this->Filter.Print();

    mvwhline( this->MainWindow, 3, 1, ACS_HLINE, COLS - 2 );
    mvwhline( this->MainWindow, 4 + resulth, 1, ACS_HLINE, COLS - 2 );

    wattron( this->MainWindow, this->TextStyle[2] );
    wattroff( this->MainWindow, this->TextStyle[2] );

    // this->ResultPanel.SetWindow( this->ResultWindow );
    // this->ResultPanel.SetDimension( COLS - 2, (LINES - 4) / 2 );
    // this->ResultPanel.SetPosition( 0, 0 );

    mvwprintw( this->ResultWindow, resulth / 2, resultw / 2 - 9, " [^H] Okno pomocy " );

    wrefresh( this->MainWindow );
    wrefresh( this->ResultWindow );
}

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
