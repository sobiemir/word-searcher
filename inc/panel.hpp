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

#ifndef __WSH_PANEL__
#define __WSH_PANEL__

#include <string>
#include <vector>

#include "configuration.hpp"

#ifdef WSD_SYSTEM_WINDOWS
#   include "curses.h"
#else
#   include <ncurses.h>
#endif

using namespace std;

class Panel
{
private:
    Vector2<int>    _Position;
    Vector2<int>    _Dimension;
    Vector2<int>    _Scroll;
    Vector2<int>    _ScrollLimit;
    vector<string> *_Lines;
    string          _Text;
    size_t          _LastIndex;
    WINDOW         *_Window;

public:
    Panel( void );

    void SetDimension( int cols, int rows );
    void SetPosition( int x, int y );
    void SetScroll( int x, int y );
    void SetWindow( WINDOW *window );

    void SetText( string text );
    void SetTextSource( vector<string> *source );
    void RefreshTextSource( void );

    void Focus( void );
    void Print( string *data = NULL );
};

#endif
