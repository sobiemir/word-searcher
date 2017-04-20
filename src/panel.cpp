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

void Panel::SetSize( int width, int height )
{
	// odśwież limity przesuwania tekstu
	this->_ScrollLimit.X = this->_ScrollLimit.X + this->_Size.Width - width > 1
		? this->_ScrollLimit.X + this->_Size.Width - width
		: 0;
	this->_ScrollLimit.Y = this->_ScrollLimit.Y + this->_Size.Height - height > 1
		? this->_ScrollLimit.Y + this->_Size.Height - height
		: 0;

	// zmień przesunięcie tekstu w razie potrzeby
	if( width > this->_Size.Width && this->_Scroll.X > 0 )
		this->_Scroll.X = width - this->_Size.Width > this->_Scroll.X
			? 0
			: this->_Scroll.X - (width - this->_Size.Width);
	if( height > this->_Size.Height && this->_Scroll.Y > 0 )
		this->_Scroll.Y = height - this->_Size.Height > this->_Scroll.Y
			? 0
			: this->_Scroll.Y - (width - this->_Size.Height);

	this->_Size.Width  = width;
	this->_Size.Height = height;
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

void Panel::SetText( std::string text )
{
	this->_Text = text;
}

// =====================================================================================================================

void Panel::SetTextSource( std::vector<std::string> *source )
{
	this->_Lines = source;
	this->_LastIndex = this->_Lines->size();
}

// =====================================================================================================================

void Panel::RefreshTextSource( void )
{
	this->_LastIndex = this->_Lines->size();

	this->_ScrollLimit.Y = this->_Lines->size() > (size_t)this->_Size.Height
		? this->_Lines->size() - (size_t)this->_Size.Height
		: 0;
	this->_Scroll.Y = this->_ScrollLimit.Y;
}

// =====================================================================================================================

void Panel::ParseKey( int chr )
{
	bool changed = true;
	switch( chr )
	{
		// przesuń do samej góry
		case KEY_HOME:
			this->SetScroll( this->_Scroll.X, 0 );
		break;
		// przesuń na sam dół
		case KEY_END:
			this->SetScroll( this->_Scroll.X, this->_ScrollLimit.Y );
		break;
		// idź na następną stronę
		case KEY_NPAGE:
			this->SetScroll( this->_Scroll.X, this->_Scroll.Y + this->_Size.Height );
		break;
		// wróć na poprzednią stronę
		case KEY_PPAGE:
			this->SetScroll( this->_Scroll.X, this->_Scroll.Y - this->_Size.Height );
		break;
		// w lewo
		case KEY_LEFT:
			this->SetScroll( this->_Scroll.X - 1, this->_Scroll.Y );
		break;
		// w prawo
		case KEY_RIGHT:
			this->SetScroll( this->_Scroll.X + 1, this->_Scroll.Y );
		break;
		// do góry o jeden wiersz
		case KEY_UP:
			this->SetScroll( this->_Scroll.X, this->_Scroll.Y - 1 );
		break;
		// w dół o jeden wiersz
		case KEY_DOWN:
			this->SetScroll( this->_Scroll.X, this->_Scroll.Y + 1 );
		break;
		default:
			changed = false;
	}
	if( changed )
		this->Print();
}

// =====================================================================================================================

void Panel::Focus( bool capture )
{
	if( !this->_Lines )
		return;

	int chr = 0;

	// przetwarzaj znaki, dopóki program nie napotka znaku LF lub CR
	while( (chr = wgetch(this->_Window)), chr != 10 && chr != 13 )
		this->ParseKey( chr );
}

// =====================================================================================================================

void Panel::Print( void )
{
	if( !this->_Lines )
		return;

	std::string *line = NULL;

	size_t start = this->_Scroll.Y;
	size_t pos   = 0;
	size_t limit = (size_t)this->_Size.Height > this->_LastIndex
		? this->_LastIndex
		: start + this->_Size.Height;

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

// =====================================================================================================================

void Panel::Print( std::string *data )
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
	this->Print();
}
