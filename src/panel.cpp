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

#include "../inc/panel.hpp"

// =====================================================================================================================

Panel::Panel()
{
    this->_Dimension.Width  = 0;
    this->_Dimension.Height = 0;

    this->_Position.X = 0;
    this->_Position.Y = 0;

    this->_Scroll.X = 0;
    this->_Scroll.Y = 0;

    this->_ScrollLimit.X = 0;
    this->_ScrollLimit.Y = 0;

    this->_LastIndex = 0;
}

// =====================================================================================================================

void Panel::SetDimension( int cols, int rows )
{
    // odśwież limity przesuwania tekstu
    this->_ScrollLimit.X = this->_ScrollLimit.X + this->_Dimension.Width - cols > 1
        ? this->_ScrollLimit.X + this->_Dimension.Width - cols
        : 0;
    this->_ScrollLimit.Y = this->_ScrollLimit.Y + this->_Dimension.Height - rows > 1
        ? this->_ScrollLimit.Y + this->_Dimension.Height - rows
        : 0;

    // zmień przesunięcie tekstu w razie potrzeby
    if( cols > this->_Dimension.Width && this->_Scroll.X > 0 )
        this->_Scroll.X = cols - this->_Dimension.Width > this->_Scroll.X
            ? 0
            : this->_Scroll.X - (cols - this->_Dimension.Width);
    if( rows > this->_Dimension.Height && this->_Scroll.Y > 0 )
        this->_Scroll.Y = rows - this->_Dimension.Height > this->_Scroll.Y
            ? 0
            : this->_Scroll.Y - (cols - this->_Dimension.Height);

    this->_Dimension.Width  = cols;
    this->_Dimension.Height = rows;
}

// =====================================================================================================================

void Panel::SetPosition( int x, int y )
{
    this->_Position.X = x;
    this->_Position.Y = y;
}

// =====================================================================================================================

void Panel::SetScroll( int x, int y )
{
    this->_Scroll.X = x > this->_ScrollLimit.X
        ? this->_ScrollLimit.X
        : (x < 0 ? 0 : x);
    this->_Scroll.Y = y > this->_ScrollLimit.Y
        ? this->_ScrollLimit.Y
        : (y < 0 ? 0 : y);
}

// =====================================================================================================================

void Panel::SetWindow( WINDOW *window )
{
    this->_Window = window;
}

// =====================================================================================================================

void Panel::SetText( string text )
{
    this->_Text = text;
}

// =====================================================================================================================

void Panel::SetTextSource( vector<string> *source )
{
    this->_Lines = source;
    this->_LastIndex = this->_Lines->size();
}

// =====================================================================================================================

void Panel::RefreshTextSource( void )
{
    this->_LastIndex = this->_Lines->size();

    this->_ScrollLimit.Y = this->_Lines->size() > (size_t)this->_Dimension.Height
        ? this->_Lines->size() - (size_t)this->_Dimension.Height
        : 0;
    this->_Scroll.Y = this->_ScrollLimit.Y;
}

// =====================================================================================================================

void Panel::Focus( void )
{
    if( !this->_Lines )
        return;

    int chr = 0;

    // przetwarzaj znaki, dopóki program nie napotka znaku LF lub CR
    while( (chr = wgetch(this->_Window)), chr != 10 && chr != 13 )
    {
        bool changed = true;
        switch( chr )
        {
            case KEY_HOME:
                this->SetScroll( this->_Scroll.X, 0 );
            break;
            case KEY_END:
                this->SetScroll( this->_Scroll.X, this->_ScrollLimit.Y );
            break;
            case KEY_NPAGE:
                this->SetScroll( this->_Scroll.X, this->_Scroll.Y + this->_Dimension.Height );
            break;
            case KEY_PPAGE:
                this->SetScroll( this->_Scroll.X, this->_Scroll.Y - this->_Dimension.Height );
            break;
            case KEY_LEFT:
                this->SetScroll( this->_Scroll.X - 1, this->_Scroll.Y );
            break;
            case KEY_RIGHT:
                this->SetScroll( this->_Scroll.X + 1, this->_Scroll.Y );
            break;
            case KEY_UP:
                this->SetScroll( this->_Scroll.X, this->_Scroll.Y - 1 );
            break;
            case KEY_DOWN:
                this->SetScroll( this->_Scroll.X, this->_Scroll.Y + 1 );
            break;
            default:
                changed = false;
        }
        if( changed )
            this->Print();
    }
}

// =====================================================================================================================

void Panel::Print( string *data )
{
    // w przypadku gdy źródło tekstu nie istnieje, wyświetl zwykły tekst
    if( data || this->_Text.size() > 0 )
    {
        if( !data )
            data = &this->_Text;

        werase( this->_Window );
        mvwaddnstr( this->_Window, this->_Position.Y, this->_Position.X, data->c_str(), -1 );
        wrefresh( this->_Window );
        return;
    }
    else if( !this->_Lines )
        return;

    string *line  = NULL;
    size_t  start = this->_Scroll.Y,
            pos   = 0,
            limit = (size_t)this->_Dimension.Height > this->_LastIndex
                ? this->_LastIndex
                : start + this->_Dimension.Height;

    // wyczyść okno
    werase( this->_Window );

    // wypisz tekst w panelu
    while( start < limit )
    {
        line = &this->_Lines->at( start );
        if( (size_t)this->_Scroll.X < line->size() )
            mvwaddnstr(
                this->_Window,
                this->_Position.Y + pos,
                this->_Position.X,
                &line->c_str()[this->_Scroll.X],
                line->size() - this->_Scroll.X
            );
        ++pos;
        ++start;
    }

    // odśwież całe okno
    wrefresh( this->_Window );
}
