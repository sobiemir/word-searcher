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

#ifndef __WSH_TEXTBOX__
#define __WSH_TEXTBOX__

#include <ncurses.h>
#include <vector>
#include <string>

//#define WSD_USE_WCHAR

using namespace std;

struct Vector2I
{
    int X;
    int Y;
};

class TextBox
{
private:
    string   _Label;            // zawartość napisu
    int      _LabelStyle;       // styl wyświetlania napisu
    string   _Content;          // zawartość pola
    int      _ContentStyle;     // styl wyświetlania zawartości pola
    size_t   _TypeStart;        // indeks od którego rozpoczynać się będzie wprowadzanie tekstu
    size_t   _Size;             // rozmiar pola tekstowego
    Vector2I _Position;         // pozycja pola wraz z napisem
    WINDOW  *_Window;           // okno w którym pole tekstowe będzie wyświetlane
    size_t   _Shift;            // przesunięcie wyświetlanej treści w elementach względem początku
    size_t   _Letters;          // ilość liter wyświetlana w polu tekstowym
    size_t   _CaretPosition;    // pozycja kursora

public:
    TextBox( string label = "", string content = "", size_t size = 0 );
    TextBox &operator = ( const TextBox &copy );

    void SetPosition( int x, int y );
    void SetPosition( Vector2I vec );
    void SetWindow( WINDOW *window );
    void SetSize( size_t size );
    void SetStyle( int label, int text );
    string GetContent( void );

    void Focus( void );
    void Print( void );
};

#endif
