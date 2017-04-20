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

#include <vector>
#include <string>

#include "configuration.hpp"
#include "control.hpp"

#ifdef WSD_SYSTEM_WINDOWS
#   include "../extra/curses.h"
#else
#   include <ncurses.h>
#endif

/**
 * Klasa kontrolki pola tekstowego.
 * Pozwala na wyświetlenie aktualnego tekstu i jego zmianę.
 * W przypadku przepełnienia, tekst jest przesuwany o jeden znak w lewo, chowając lewą stronę.
 * Można go potem odczytać, przesuwając kursor w tamtą stronę.
 * Zasada działania jest taka sama jak w przypadku zwykłego pola tekstowego.
 * 
 * Klasa przechwytuje następujące klawisze:
 * - LEFT      : przesunięcie kursora o jeden znak w lewo
 * - RIGHT     : przesunięcie kursora o jeden znak w prawo 
 * - HOME      : powrót na początek tekstu
 * - END       : przejście na sam koniec tekstu
 * - BACKSPACE : usuwa jeden znak z lewej strony kursora
 * - DELETE    : usuwa jeden znak z prawej strony kursora
 */
class TextBox: public Control
{
private:
	/**
	 * Napis na kontrolce.
	 * Napis wyświetlany jest zaraz obok pola tekstowego.
	 */
	std::string _Label;
	/**
	 * Styl wyświetlania napisu.
	 * Style definiowane są w interfejsie.
	 */
	int _LabelStyle;
	/**
	 * Zawartość pola tekstowego.
	 * Tekst ten, wyświetlany w polu tekstowym, będzie możliwy do zmiany.
	 */
	std::string _Content;
	/**
	 * Styl wyświetlania tekstu w polu tekstowym.
	 * Style definiowane są w interfejsie.
	 */
	int _ContentStyle;
	/**
	 * Indeks wprowadzania tekstu.
	 * Generalnie jest to wartość pozycji kontolki względem osi X z dodaną wartością szerokości tekstu
	 * dla napisu, wyświetlanego przed kontrolką.
	 */
	size_t _TypeStart;
	/**
	 * Przesunięcie tekstu.
	 * Zmienna wkracza do akcji gdy wystąpi przepełnienie tekstu w oknie.
	 * Wartość ta zarazem reprezentuje ilość znaków, ukrytych z lewej strony.
	 */
	size_t _Shift;
	/**
	 * Ilość liter wyświetlana w polu tekstowym.
	 * W przypadku gdy wystąpi przepełnienie tekstu, ilość ta jest zawsze taka sama.
	 */
	size_t _Letters;
	/**
	 * Pozycja kursora względem pola tekstowego.
	 * Pozycja nie jest liczona względem całego tekstu, a więc realną pozycję należy obliczyć
	 * poprzez dodanie do tej wartości, wartości zmiennej TextBox::_Shift.
	 */
	size_t _CaretPosition;

public:
	/**
	 * Konstruktor klasy.
	 * Uzupełnia zmienne domyślnymi wartościami.
	 * 
	 * @param label Tekst wyświetlany obok kontrolki.
	 * @param content Tekst wyświetlany w kontrolce.
	 * @param size Domyślny rozmiar kontrolki.
	 */
	TextBox( std::string label = "", std::string content = "", size_t size = 0 );

	/**
	 * Zmienia pozycję kontrolki.
	 * 
	 * @param x Nowa pozycja względem osi X.
	 * @param y Nowa pozycja względem osi Y.
	 */
	void SetPosition( int x, int y );

	/**
	 * Zmienia rozmiar kontrolki.
	 * Wersja z podawaniem tylko szerokości i tylko tej wersji powinno się używać dla pola tekstowego.
	 * 
	 * @param width Nowa szerokosć kontrolki.
	 */
	void SetSize( int width );

	/**
	 * Zmienia styl wyświetlania tekstu w kontrolce.
	 * Pozwala na ustawienie napisu obok kontrolki i samego tekstu wyświetlanego w kontrolce.
	 * 
	 * @param label Styl dla napisu.
	 * @param text Styl dla tekstu w kontrolce.
	 */
	void SetStyle( int label, int text );

	/**
	 * Pobiera aktualną wartość, zapisaną w kontrolce.
	 * 
	 * @return Wartość zapisana w kontrolce.
	 */
	std::string GetContent( void );

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
};

#endif
