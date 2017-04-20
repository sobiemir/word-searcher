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

TextBox::TextBox( std::string label, std::string content, size_t size ): Control(),
	_Label(label), _LabelStyle(0), _Content(content), _ContentStyle(0), _Shift(0), _Letters(0)
{
	// oblicz rozmiar pola tekstowego, odejmując rozmiar napisu od podanej wartości
	this->_Size.Width = this->_Label.size() > size
		? 0
		: size - this->_Label.size();
	this->_TypeStart = this->_Position.X + this->_Label.size();
}

// =====================================================================================================================

void TextBox::SetPosition( int x, int y )
{
	this->_Position.X = x;
	this->_Position.Y = y;

	this->_TypeStart = this->_Position.X + (int)this->_Label.size();
}

// =====================================================================================================================

void TextBox::SetSize( int width )
{
	if( (int)this->_Label.size() > width )
		this->_Size.Width = 0,
		this->_Letters    = 0;
	else
		this->_Size.Width = width - this->_Label.size(),
		this->_Letters    = (int)this->_Content.size() > width
			? width
			: this->_Content.size();
}

// =====================================================================================================================

void TextBox::SetStyle( int label, int text )
{
	this->_ContentStyle = text;
	this->_LabelStyle   = label;
}

// =====================================================================================================================

std::string TextBox::GetContent( void )
{
	return this->_Content;
}

// =====================================================================================================================

void TextBox::ParseKey( int chr )
{
	// znaki kontrolne
	switch( chr )
	{
		// przejdź na koniec
		case KEY_END:
			this->_CaretPosition = this->_Letters;
			this->_Shift         = this->_Content.size() - this->_Letters;
		break;
		// przejdź na początek
		case KEY_HOME:
			this->_CaretPosition = 0;
			this->_Shift         = 0;
		break;
		// przesuń kursor o jeden znak w lewo
		case KEY_LEFT:
			if( this->_CaretPosition > 0 )
				this->_CaretPosition--,
				wmove( this->_Window, this->_Position.Y, this->_TypeStart + this->_CaretPosition );
			else if( this->_Shift > 0 )
			{
				this->_Shift--;
				break;
			}
			return;
		break;
		// przesuń kursor o jeden znak w prawo
		case KEY_RIGHT:
			if( this->_CaretPosition < this->_Letters )
				this->_CaretPosition++,
				wmove( this->_Window, this->_Position.Y, this->_TypeStart + this->_CaretPosition );
			else if( this->_Content.size() > this->_Shift + this->_Letters )
			{
				this->_Shift++;
				break;
			}
			return;
		break;
		// Backspace
		case 8: case 127: case KEY_BACKSPACE:
			// w przypadku gdy wartość jest pusta, nie rób nic...
			if( this->_Content.size() == 0 || (this->_CaretPosition == 0 && this->_Shift == 0) )
				return;
			
			this->_Content.erase( this->_Shift + this->_CaretPosition - 1, 1 );
			
			if( this->_Shift == 0 )
				this->_Letters--,
				this->_CaretPosition--;
			else
				this->_Shift--;
		break;
		// Delete - zachowanie trochę inne niż Backspace
		case KEY_DC:
			// w przypadku gdy wartość jest pusta, nie rób nic...
			if( this->_Letters <= this->_CaretPosition )
				return;
			
			this->_Content.erase( this->_Shift + this->_CaretPosition, 1 );
			
			if( this->_Shift == 0 )
				this->_Letters--;
			else
				this->_Shift--;
		break;
		default:
			// przyjmuj na razie tylko znaki ASCII
			if( chr < 32 || chr > 127 )
				return;

			this->_Content.insert( this->_Shift + this->_CaretPosition, 1, chr );

			if( (int)this->_Content.size() < this->_Size.Width )
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

// =====================================================================================================================

void TextBox::Focus( bool capture )
{
	int chr = 0;

	// przenieś kursor na ostatni znak w polu
	this->_CaretPosition = this->_Letters;
	wmove( this->_Window, this->_Position.Y, this->_TypeStart + this->_CaretPosition );

	if( !capture )
		return;

	// przechwytuj znaki, dopóki program nie napotka znaku LF lub CR
	while( (chr = wgetch(this->_Window)), chr != 10 && chr != 13 )
		this->ParseKey( chr );
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
	mvwhline( this->_Window, this->_Position.Y, this->_TypeStart, ' ', this->_Size.Width );

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
