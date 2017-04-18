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
            case 18:
            break;
            case 10: case 13: /* RETURN */
            {
                Searcher searcher(
                    ws.Folder.GetContent(),
                    ws.Phrase.GetContent(),
                    ws.Filter.GetContent()
                );
            }
            break;
#       ifdef KEY_RESIZE
            case KEY_RESIZE:
                ws.TerminalResize();
            break;
#       endif
        }
        wprintw( ws.MainWindow, "%d %c\n", chr, chr );
        wrefresh( ws.MainWindow );
    }

    clear();
    refresh();
    endwin();

    return EXIT_SUCCESS;
}
