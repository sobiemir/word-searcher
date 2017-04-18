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

#include <ncurses.h>
#include <panel.h>
#include <cstdlib>
#include <string>

#include "interface.hpp"
#include "searcher.hpp"

using namespace std;

int main( int argc, char *argv[] )
{
    if( initscr() == NULL )
        return EXIT_FAILURE;
    
    raw();
    nonl();
    noecho();
    keypad( stdscr, FALSE );

    Interface ws;
    Searcher  searcher;

    searcher.Printer = &ws;
    // ws.ResultPanel.SetTextSource( &searcher.FoundFiles );

    ws.InitColors();
    ws.TerminalResize();

    int chr;

    while( (chr = wgetch(ws.MainWindow)) != 3 ) /* chr != ^C */
    {
        switch( chr )
        {
            case 4: /* chr != ^D */
                ws.Folder.Focus();
            break;
            case 6: /* chr != ^F */
                ws.Phrase.Focus();
            break;
            case 17: /* chr != ^Q */
                // ws.ResultPanel.Focus();
            break;
            case 23: /* chr != ^W */
            break;
            case 18:
            {
                // wprintw( ws.MainWindow, "START\n" );
                // wrefresh( ws.MainWindow );
                searcher.Criteria( ws.Folder.GetContent(), ws.Phrase.GetContent(), ws.Filter.GetContent() );
                searcher.Run();
                // wprintw( ws.MainWindow, "STOP - %d\n", searcher.FoundFiles.size() );
                // wrefresh( ws.MainWindow );
            }
            break;
            case 10: case 13: /* RETURN */
            break;
#       ifdef KEY_RESIZE
            case KEY_RESIZE:
                ws.TerminalResize();
            break;
#       endif
        }
        // wprintw( ws.MainWindow, "%d %c\n", chr, chr );
        // wrefresh( ws.MainWindow );
    }

    clear();
    refresh();
    endwin();

    return EXIT_SUCCESS;
}
