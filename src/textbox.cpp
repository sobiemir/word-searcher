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

#include "../inc/textbox.hpp"

// =====================================================================================================================

TextBox::TextBox( string label, string content, size_t size )
{
    this->_Content = content;
    this->_Label   = label;
    this->_Window  = NULL;
    this->_Shift   = 0;
    this->_Letters = 0;

    this->_ContentStyle = 0;
    this->_LabelStyle   = 0;

    // oblicz rozmiar pola tekstowego, odejmując rozmiar napisu od podanej wartości
    this->_Size = this->_Label.size() > size
        ? 0
        : size - this->_Label.size();
    this->_TypeStart = this->_Position.X + this->_Label.size();

    this->_Position.X = 0;
    this->_Position.Y = 0;
}

// =====================================================================================================================

TextBox &TextBox::operator = ( const TextBox &copy )
{
    this->_Label         = copy._Label;
    this->_LabelStyle    = copy._LabelStyle;
    this->_Content       = copy._Content;
    this->_ContentStyle  = copy._ContentStyle;
    this->_TypeStart     = copy._TypeStart;
    this->_Size          = copy._Size;
    this->_Position      = copy._Position;
    this->_Window        = copy._Window;
    this->_Shift         = copy._Shift;
    this->_Letters       = copy._Letters;
    this->_CaretPosition = copy._CaretPosition;

    return *this;
}

// =====================================================================================================================

void TextBox::SetPosition( int x, int y )
{
    this->_Position.X = x;
    this->_Position.Y = y;

    this->_TypeStart = this->_Position.X + this->_Label.size();
}

// =====================================================================================================================

void TextBox::SetWindow( WINDOW *window )
{
    this->_Window = window;
}

// =====================================================================================================================

void TextBox::SetSize( size_t size )
{
    if( this->_Label.size() > size )
        this->_Size    = 0,
        this->_Letters = 0;
    else
        this->_Size    = size - this->_Label.size(),
        this->_Letters = this->_Content.size() > size
            ? size
            : this->_Content.size();
}

// =====================================================================================================================

void TextBox::SetStyle( int label, int text )
{
    this->_ContentStyle = text;
    this->_LabelStyle   = label;
}

// =====================================================================================================================

string TextBox::GetContent( void )
{
    return this->_Content;
}

// =====================================================================================================================

void TextBox::Focus( void )
{
    int chr = 0;

    // przenieś kursor na ostatni znak w polu
    this->_CaretPosition = this->_Letters;
    wmove( this->_Window, this->_Position.Y, this->_TypeStart + this->_CaretPosition );

    // przetwarzaj znaki, dopóki program nie napotka znaku LF lub CR
    while( (chr = wgetch(this->_Window)), chr != 10 && chr != 13 )
    {
        // własne funkcje do wywołania (na kiedyś)
        // np. przechwytywanie CTRL + D / CTRL + F

        // znaki kontrolne
        switch( chr )
        {
            case KEY_END:
                this->_CaretPosition = this->_Letters;
                this->_Shift         = this->_Content.size() - this->_Letters;
            break;
            case KEY_HOME:
                this->_CaretPosition = 0;
                this->_Shift         = 0;
            break;
            case KEY_LEFT:
                if( this->_CaretPosition > 0 )
                    this->_CaretPosition--,
                    wmove( this->_Window, this->_Position.Y, this->_TypeStart + this->_CaretPosition );
                else if( this->_Shift > 0 )
                {
                    this->_Shift--;
                    break;
                }
                continue;
            break;
            case KEY_RIGHT:
                if( this->_CaretPosition < this->_Letters )
                    this->_CaretPosition++,
                    wmove( this->_Window, this->_Position.Y, this->_TypeStart + this->_CaretPosition );
                else if( this->_Content.size() > this->_Shift + this->_Letters )
                {
                    this->_Shift++;
                    break;
                }
                continue;
            break;
            case 8: case 127: case KEY_BACKSPACE: // backspace
                // w przypadku gdy wartość jest pusta, nie rób nic...
                if( this->_Content.size() == 0 || (this->_CaretPosition == 0 && this->_Shift == 0) )
                    continue;
                
                this->_Content.erase( this->_Shift + this->_CaretPosition - 1, 1 );
                
                if( this->_Shift == 0 )
                    this->_Letters--,
                    this->_CaretPosition--;
                else
                    this->_Shift--;
            break;
            case KEY_DC: // DELETE
                // w przypadku gdy wartość jest pusta, nie rób nic...
                if( this->_Letters <= this->_CaretPosition )
                    continue;
                
                this->_Content.erase( this->_Shift + this->_CaretPosition, 1 );
                
                if( this->_Shift == 0 )
                    this->_Letters--;
                else
                    this->_Shift--;
            break;
            case KEY_UP: case KEY_DOWN:
                continue;
            break;
            default:
                // przyjmuj na razie tylko znaki ANSII
                if( chr < 32 || chr > 127 )
                    continue;

                this->_Content.insert( this->_Shift + this->_CaretPosition, 1, chr );

                if( this->_Content.size() < this->_Size )
                    this->_CaretPosition++,
                    this->_Letters++;
                else
                    this->_Shift++;
            break;
        }

        this->Print();
        wmove( this->_Window, this->_Position.Y, this->_TypeStart + this->_CaretPosition );

        wrefresh( this->_Window );
    }
}

// =====================================================================================================================

void TextBox::Print( void )
{
    // wyświetl najpierw napis
    if( this->_LabelStyle )
        wattron( this->_Window, this->_LabelStyle );
    mvwprintw( this->_Window, this->_Position.Y, this->_Position.X, this->_Label.c_str() );
    if( this->_LabelStyle )
        wattroff( this->_Window, this->_LabelStyle );

    // nadpisuj litery aby w razie usuwania wymazać istniejące
    mvwhline( this->_Window, this->_Position.Y, this->_TypeStart, ' ', this->_Size );

    if( this->_Letters == 0 )
        return;

    // a potem zawartość pola tekstowego
    if( this->_ContentStyle )
        wattron( this->_Window, this->_ContentStyle );
    mvwaddnstr
    (
        this->_Window,
        this->_Position.Y,
        this->_TypeStart,
        &this->_Content.c_str()[this->_Shift],
        this->_Letters
    );
    if( this->_ContentStyle )
        wattroff( this->_Window, this->_ContentStyle );
}
