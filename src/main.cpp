#include <ncurses.h>
#include <panel.h>
#include <cstdlib>
#include <string>

#include "textbox.hpp"

using namespace std;

WINDOW *mainwindow   = NULL,
       *searchwindow = NULL,
       *resultwindow = NULL;
int     sizechanged  = 0,
        textstyle[4] = {0};

string displayFolder;

TextBox folder( "Folder : ", "./", 0 );
TextBox phrase( "Fraza  : ", "", 0 );

void destroy_windows()
{
    if( mainwindow )
        delwin( mainwindow );
    if( searchwindow )
        delwin( searchwindow );
    if( resultwindow )
        delwin( searchwindow );

    mainwindow = searchwindow = resultwindow = NULL;
}

void init_colors()
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

    textstyle[0] = COLOR_PAIR(1);
    textstyle[1] = COLOR_PAIR(2);
    textstyle[2] = COLOR_PAIR(3);
    textstyle[3] = A_BOLD | COLOR_PAIR(4);
}

void terminal_resize()
{
    sizechanged = 1;

    destroy_windows();

    if( !(mainwindow = newwin(LINES, COLS, 0, 0)) )
        exit( EXIT_FAILURE );

    keypad( mainwindow, TRUE );

    folder.SetSize( COLS - 2 );
    folder.SetWindow( mainwindow );

    phrase.SetSize( COLS - 2 );
    phrase.SetWindow( mainwindow );

    // mvwhline( mainwindow, 0, 0, ACS_HLINE, COLS );

    wattron( mainwindow, textstyle[1] );
    wborder( mainwindow, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER );
    mvwprintw( mainwindow, 0, COLS - 23, " WordSearcher v0.2.0 " );
    wattroff( mainwindow, textstyle[1] );

    folder.Print();
    phrase.Print();

    mvwhline( mainwindow, 3, 1, ACS_HLINE, COLS - 2 );
    mvwhline( mainwindow, (LINES - 3) / 2 + 2, 1, ACS_HLINE, COLS - 2 );

    wattron( mainwindow, textstyle[2] );
    mvwprintw( mainwindow, ((LINES - 3) / 2) / 2 + 3, COLS / 2 - 9, " [^H] Okno pomocy " );
    wattroff( mainwindow, textstyle[2] );

    wrefresh( mainwindow );
}

void loop()
{
    int chr;

    while( (chr = wgetch(mainwindow)) != 3 ) /* chr != ^C */
    {
        switch( chr )
        {
            case 4: /* chr != ^D */
                folder.Focus();
            break;
            case 6: /* chr != ^F */
                phrase.Focus();
            break;
            case 10: case 13: /* RETURN */

            break;
#       ifdef KEY_RESIZE
            case KEY_RESIZE:
                terminal_resize();
            break;
#       endif
        }
        // wprintw( mainwindow, "%d %c\n", chr, chr );
        // wrefresh( mainwindow );
    }
}

int main( int argc, char *argv[] )
{
    if( initscr() == NULL )
        return EXIT_FAILURE;
    
    raw();
    nonl();
    noecho();
    keypad( stdscr, FALSE );

    init_colors();

    folder.SetPosition( 1, 1 );
    phrase.SetPosition( 1, 2 );
    folder.SetStyle( textstyle[0], 0 );
    phrase.SetStyle( textstyle[0], 0 );

    terminal_resize();

    loop();

    clear();
    refresh();
    endwin();

    return EXIT_SUCCESS;
}
