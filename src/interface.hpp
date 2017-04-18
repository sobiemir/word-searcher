#ifndef __WSH_INTERFACE__
#define __WSH_INTERFACE__

#include <ncurses.h>
#include <stdlib.h>
#include "textbox.hpp"

class Interface
{
public:
    WINDOW *MainWindow;
    WINDOW *SearchWindow;
    WINDOW *ResultWindow;
    WINDOW *HelpWindow;

    TextBox Folder;
    TextBox Phrase;
    TextBox Filter;

    int TextStyle[4];

    Interface( void );

    void InitColors( void );
    void TerminalResize( void );
    void DestroyWindows( void );
};

#endif
