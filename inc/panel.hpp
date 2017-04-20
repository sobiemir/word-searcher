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
#include "control.hpp"

#ifdef WSD_SYSTEM_WINDOWS
#	include "../extra/curses.h"
#else
#	include <ncurses.h>
#endif

/**
 * Klasa kontrolki panelu.
 * Pozwala na wyświetlanie tekstu i przewijanie w razie przepełnienia.
 * Na razie pozwala na przesuwanie tekstu tylko w górę i dół.
 * 
 * Klasa przechwytuje następujące klawisze:
 * - UP   : przewinięcie tekstu o linię w górę
 * - DOWN : przewinięcie tekstu o linię w dół
 * - PGUP : przejście do poprzedniej strony
 * - PGDN : przejście do następnej strony
 * - HOME : powrót na początek tekstu
 * - END  : przejście na sam koniec tekstu
 */
class Panel: public Control
{
private:
	/**
	 * Informacja o aktualnym przesunięciu tekstu.
	 * Dzięki temu obliczana jest pozycja, od której tekst ma być wyświetlany.
	 */
	Vector2<int> _Scroll;
	/**
	 * Limit przesunięcia tekstu.
	 * Tekst nie może zostać przesuwany w nieskończoność, tu są limity, obliczane
	 * względem wielkości źródła i samej kontrolki.
	 */
	Vector2<int> _ScrollLimit;
	/**
	 * Indeks ostatniego przetworzonego elementu.
	 * Z racji że tekst pobierany jest ze źródła, wyświetlany jest tylko do tego momentu,
	 * do którego został wcześniej sprawdzony (wywołana została funkcja odświeżenia).
	 */
	size_t _LastIndex;
	/**
	 * Źródło tekstu, zawierające linie do wyświetlenia w programie.
	 * Każdą linię powinien reprezentować nowy element w tablicy.
	 */
	std::vector<std::string> *_Lines;
	/**
	 * Wyświetlany tekst w kontrolce.
	 * Tekst jest wyświetlany w przypadku gdy panel nie posiada źródła.
	 */
	std::string _Text;

public:
	/**
	 * Konstruktor klasy.
	 * Pozwala na uzupełnienie zmiennych domyślnymi wartościami.
	 */
	Panel( void ): Control(),
		_Scroll(), _ScrollLimit(), _LastIndex(0) {}

	/**
	 * Zmienia rozmiar kontrolki.
	 * Funkcja nadpisuje funkcję z klasy bazowej.
	 * 
	 * @param width Nowa szerokość.
	 * @param height Nowa wysokość.
	 */
	void SetSize( int width, int height );

	/**
	 * Przesuwa teksto do podanych współrzędnych.
	 * Na razie działa tylko przesuwanie względem osi Y.
	 * 
	 * @param x Pozycja X przesunięcia tekstu.
	 * @param y Pozycja Y przesunięcia tekstu.
	 */
	void SetScroll( int x, int y );

	/**
	 * Ustawia domyślny tekst statyczny wyświetlany na panelu.
	 * Tekst ten jest wyświetlany, gdy panel nie posiada żadnego źródła danych.
	 * 
	 * @param text Tekst do zapisania.
	 */
	void SetText( std::string text );

	/**
	 * Ustawia źródło tekstu, wyświetlanego w panelu.
	 * Funkcja ta została stworzona z myślą o dynamicznych źródłach danych.
	 * Przed każdą aktualizacją podpiętego źródła, należy jednak wywołać funkcję Panel::RefreshTextSoruce.
	 * W przeciwnym wypadku wprowadzone zmiany nie zostaną wyświetlone.
	 * 
	 * @param source Źródło danych do wyświetlenia.
	 */
	void SetTextSource( std::vector<std::string> *source );

	/**
	 * Odświeża informacje o podpiętym źródle.
	 * Dzięki temu klasa dostaje informacje o wprowadzonych zmianach w źródle.
	 * Pozwala to na odświeżenie wyświetlanych danych w oknie.
	 * Funkcję tą powinno się wywoływać przy każdej zmianie w źródle, która ma zostać wyświetlona,
	 * lub gdy na tym źródle mają być wykonywane przez klasę jakiekolwiek operacje.
	 */
	void RefreshTextSource( void );

	/**
	 * Przetwarza znak podany w argumencie.
	 * Dzięki tej funkcji można przesuwać wypisany tekst, który nie mieści się w oknie.
	 * 
	 * @param chr Znak do przetworzenia.
	 */
	void ParseKey( int chr );

	/**
	 * Ustawia skupienie na kontrolkę.
	 * Generalnie funkcja ustawia kursor i rozpoczyna przechwytywanie znaków.
	 * Przechwytywanie powinno się jednak dać wyłączyć poprzez podanie wartości false w argumencie.
	 * Dzięki temu kontrolka nie będzie blokowała innych skrótów klawiszowych, zdefiniowanych przez program.
	 * Jednak ta metoda wymaga ręcznego wywoływania funkcji ParseKey.
	 * 
	 * @param capture Przechwytywanie znaków.
	 */
	void Focus( bool capture = true );

	/**
	 * Wyświetla kontrolkę.
	 * Standardowa funkcja wyświetlania kontrolki.
	 */
	void Print( void );

	/**
	 * Wyświetla w kontrolce tekst podany w argumencie.
	 * Tekst działa na podobnej zasadzie jak źródło, gdyż funkcja nie operuje bezpośrednio na jego kopii.
	 * 
	 * @param data Wskaźnik do tekstu.
	 */
	void Print( std::string *data );
};

#endif
