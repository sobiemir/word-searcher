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

#ifndef __WSH_CONTROL__
#define __WSH_CONTROL__

#include <vector>
#include "configuration.hpp"

#ifdef WSD_SYSTEM_WINDOWS
#	include "../extra/curses.h"
#else
#	include <ncurses.h>
#endif

/**
 * Klasa główna wszystkich kontrolek dostępnych w programie.
 * Zawiera podstawowe zmienne i funkcje, które powinna posiadać każda kontrolka.
 * Dzięki niej, możliwe jest przypisanie kontrolki do zmiennej, ustawienie skupienia oraz
 * ręczne przechwytywanie znaków, dzięki czemu skupienie nie blokuje globalnych skrótów programu.
 */
class Control
{
protected:
	/**
	 * Pozycja kontrolki w oknie.
	 * Pozycja liczona jest relatywnie do okna, w którym się znajduje.
	 */
	Vector2<int> _Position;
	/**
	 * Rozmiar kontrolki.
	 * W przypadku, gdy rozmiar jest za duży, kontrolka nie narysuje się w całości.
	 */
	Vector2<int> _Size;
	/**
	 * Okno, do którego przypisana jest kontrolka.
	 * W przypadku gdy kontrolka nie jest przypisana do żadnego okna, wyświetlana jest na stdscr.
	 */
	WINDOW *_Window;

public:
	/**
	 * Konstruktor klasy.
	 * Uzupełnia zmienne domyślnymi wartościami.
	 */
	Control( void ) :
		_Position(), _Size(), _Window(NULL) {}

	/**
	 * Zmienia rozmiar kontrolki.
	 * 
	 * @param width Nowa szerokość.
	 * @param height Nowa wysokość.
	 */
	void SetSize( int width, int height )
		{ this->_Size.Width = width, this->_Size.Height = height; }

	/**
	 * Zmienia pozycję kontrolki.
	 * 
	 * @param x Nowa pozycja względem osi X.
	 * @param y Nowa pozycja względem osi Y.
	 */
	void SetPosition( int x, int y )
		{ this->_Position.X = x, this->_Position.Y = y; }

	/**
	 * Zmienia okno, do którego klasa przynależy.
	 * Dzięki temu klasa wie, gdzie ma się pojawić kontrolka.
	 * 
	 * @param window Nowe okno.
	 */
	void SetWindow( WINDOW *window )
		{ this->_Window = window; }

	/**
	 * Pobiera wymiary kontrolki.
	 * 
	 * @return Wymiary kontrolki.
	 */
	Vector2<int> GetSize( void )
		{ return this->_Size; }

	/**
	 * Zwraca pozycję kontrolki.
	 * 
	 * @return Pozycja kontrolki.
	 */
	Vector2<int> GetPosition( void )
		{ return this->_Position; }

	/**
	 * Zwraca okno, do którego przypisana została kontrolka.
	 * 
	 * @return Okno kontrolki.
	 */
	WINDOW *GetWindow( void )
		{ return this->_Window; }

	/**
	 * Przetwarza znak podany w argumencie.
	 * Funkcja wywoływana ręcznie lub automatycznie przez funkcję Control::Focus.
	 * Funkcja służy przede wszystkim obsłudze skrótów klawiaturowych, obsługiwanych przez kontrolkę.
	 * Implementacja tej funkcji musi się znaleźć w klasie pochodnej.
	 * 
	 * @param chr Znak do przetworzenia.
	 */
	virtual void ParseKey( int chr ) = 0;

	/**
	 * Ustawia skupienie na kontrolkę.
	 * Generalnie funkcja ustawia kursor i rozpoczyna przechwytywanie znaków.
	 * Przechwytywanie powinno się jednak dać wyłączyć poprzez podanie wartości false w argumencie.
	 * Dzięki temu kontrolka nie będzie blokowała innych skrótów klawiszowych, zdefiniowanych przez program.
	 * Jednak ta metoda wymaga ręcznego wywoływania funkcji ParseKey.
	 * Implementacja tej funkcji musi się znaleźć w klasie pochodnej.
	 * 
	 * @param capture Przechwytywanie znaków.
	 */
	virtual void Focus( bool capture = true ) = 0;

	/**
	 * Wyświetla kontrolkę.
	 * Standardowa funkcja wyświetlania kontrolki.
	 * Implementacja tej funkcji musi się znaleźć w klasie pochodnej.
	 */
	virtual void Print( void ) = 0;
};

#endif
