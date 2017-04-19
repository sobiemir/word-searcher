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

#include "./inc/configuration.hpp"
#include "./inc/interface.hpp"
#include "./inc/searcher.hpp"

#ifdef WSD_SYSTEM_WINDOWS
#   include "./inc/curses.h"
#else
#   include <ncurses.h>
#endif

using namespace std;

int main( int argc, char *argv[] )
{
    if( initscr() == NULL )
        return EXIT_FAILURE;
    
    raw();
    nonl();
    noecho();
    keypad( stdscr, TRUE );

    Interface ws;
    Searcher  searcher;

    searcher.Printer = &ws;
    ws.ResultPanel.SetTextSource( &searcher.FoundFiles );

    ws.InitColors();
    ws.TerminalResize();

    int chr;

    while( (chr = wgetch(ws.MainWindow)) != 3 ) // ^C
    {
        switch( chr )
        {
            case 4: // ^D
                ws.Folder.Focus();
            break;
            case 6: // ^F
                ws.Phrase.Focus();
            break;
            case 17: // ^Q
                ws.ResultPanel.Focus();
            break;
            case 19: // ^S
                ws.Filter.Focus();
            break;
            case 18: // ^R
            {
                searcher.Criteria( ws.Folder.GetContent(), ws.Phrase.GetContent(), ws.Filter.GetContent() );
                searcher.Run();
            }
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
