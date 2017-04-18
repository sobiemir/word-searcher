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

#include <ncurses.h>
#include <stdlib.h>
#include "textbox.hpp"
#include "panel.hpp"

class Interface
{
private:
    // size_t FoundPosition;

public:
    WINDOW *MainWindow;
    WINDOW *SearchWindow;
    WINDOW *ResultWindow;
    WINDOW *HelpWindow;

    TextBox Folder;
    TextBox Phrase;
    TextBox Filter;

    Panel SearchPanel;
    Panel ResultPanel;

    int TextStyle[4];

    Interface( void );
    ~Interface( void );

    void RefreshCurrentFile( string *file );
    void RefreshPrintedFiles( void );

    void InitColors( void );
    void TerminalResize( void );
    void DestroyWindows( void );
};

#endif
