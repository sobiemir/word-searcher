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

#ifndef __WSH_SEARCHER__
#define __WSH_SEARCHER__

#include <vector>
#include <string>
#include <cstring>
#include <fstream>

#include "interface.hpp"
#include "configuration.hpp"

#ifdef WSD_SYSTEM_WINDOWS
#   include "../extra/dirent.h"
#else
#   include <dirent.h>
#endif

using namespace std;

/**
 * Klasa wyszukiwarki frazy.
 * Udostępnia funkcje, pozwalające na wyszukiwanie frazy w plikach.
 * Domyślnie wyszukiwarka nie zwraca uwagi na wielkość liter w rozszerzeniach, oraz wyszukiwanie odbywa się
 * przyrostowo, a więc każdy napotkany folder dodatkowo jest przeszukiwany.
 * Pliki można filtrować poprzez nazwy rozszerzeń.
 *
 * Składnia filtru:
 * [modyfikatory]/[rozszerzenie1]/[rozszerzenie2]/...
 *
 * Gdzie dostępne modyfikatory filtrów to:
 * > ! : wyszukiwanie z uwzględnieniem wielkości znaków w rozszerzeniu
 * > @ : dołączanie do wyszukiwania plików bez rozszerzenia
 * > * : wyszukiwanie we wszystkich znalezionych plikach
 * > + : wyszukiwanie w plikach zawierających podane rozszerzenia
 * > - : wyszukiwanie w plikach, które nie zawierają podanych rozszerzeń
 * > = : przeszukiwanie tylko podanego folderu, bez podfolderów
 *
 * Szczególne przypadki modyfikatorów bez listy rozszerzeń:
 * > @   : wyszukuje tylko w plikach bez rozszerzeń
 * > *@  : wyszukuje tylko w plikach, które mają jakiekolwiek rozszerzenie
 * > -   : wolniejszy odpowiednik *@
 * > *=  : przeszukuje wszystkie plików tylko w podanym folderze
 * > *=@ : przeszukuje wszystkie pliki z jakimkolwiek rozszerzeniem w podanym folderze
 * > @=  : przeszukuje wszystkie pliki bez rozszerzenia w podanym folderze
 *
 * Przykłady filtrów:
 *   *@
 *   !+/html/css
 *   -/jpg/png
 *   /php/js/ts
 *   !@-/cpp/hpp
 *   @=+/c/h
 */
class Searcher
{
private:
	/**
	 * Nazwa folderu.
	 * W tym folderze program rozpocznie wyszukiwanie plików.
	 */
	string _Folder;
	/**
	 * Fraza do znalezienia.
	 * W przypadku pustej frazy wypisuje wszystkie pliki.
	 */
	string _Phrase;
	/**
	 * Filtr stosowany na nazwach plików.
	 * W przypadku gdy plik nie spełnia oczekiwań filtru, zostaje on odrzucony.
	 * W ten sposób można przyspieszyć wyszukiwanie w przypadku dużych ilości plików.
	 */
	string _Filter;
	/**
	 * Wyodrębniona z filtru lista rozszerzeń.
	 * Na jej podstawie program odrzuca pliki do wyszukiwania. 
	 */
	vector<string> _ExtensionList;
	/**
	 * Modyfikatory dla filtru.
	 * Dzięki nim, filtr wie co ma zrobić z danym plikiem w szczególnych przypadkach.
	 */
	int _Modifiers;

// =====================================================================================================================

	/**
	 * Czyści prawą i lewą stronę łańcucha z białych znaków.
	 * Dzięki temu ciąg końcowy wygląda tak jak powinien wyglądać.
	 *
	 * @param str Ciąg znaków do obcięcia.
	 */
	void Trim( string &str );

	/**
	 * Przetwarza filtr podany przez użytkownika.
	 * Wyodrębnia z niego listę rozszerzeń i modyfikatory.
	 * 
	 * @param ext 
	 */
	void ParseExtensions( string ext );

	/**
	 * Sprawdza, czy rozszerzenie pliku spełnia oczekiwania filtra.
	 * Dodatkowo bierze pod uwagę przekazane do filtra modyfikatory.
	 *
	 * @param filename Nazwa pliku do sprawdzenia.
	 */
	bool CheckExtension( string filename );

public:

// =====================================================================================================================

	/**
	 * Lista plików, które spełniają kryteria wyszukiwania.
	 * Lista ta wykorzystywana jest do wyświetlania w panelu.
	 */
	vector<string> FoundFiles;
	/**
	 * Klasa zawierająca funkcje pozwalające na wyświetlanie danych.
	 * Zmienna używana przy raportowaniu przeszukiwanego pliku lub do wyświetlania listy plików.
	 * Można do niej przypisać tylko klasę interfejsu.
	 */
	Interface *Printer;
	/**
	 * Informacja o tym, czy funkcja aktualnie zajmuje się wyszukiwaniem frazy.
	 * Przydatne w komunikacji między wątkami, dzięki temu w prosty sposób można przerwać wyszukiwanie.
	 */
	bool Searching;

#ifdef WSD_SYSTEM_WINDOWS
	/**
	 * Sekcja krytyczna.
	 * Używana przy zmiennej warunkowej dla wątku.
	 * Wersja dla systemu Windows.
	 */
	CRITICAL_SECTION   Mutex;
	/**
	 * Zmienna warunkowa.
	 * Odpowiada za przekazanie informacji o zmianie statusu zmiennej z poziomu wątku.
	 * Wersja dla systemu Windows.
	 */
	CONDITION_VARIABLE Condition;
#else
	/**
	 * Sekcja krytyczna.
	 * Używana przy zmiennej warunkowej dla wątku.
	 * Wersja standardu POSIX.
	 */
	pthread_mutex_t Mutex;
	/**
	 * Zmienna warunkowa.
	 * Odpowiada za przekazanie informacji o zmianie statusu zmiennej z poziomu wątku.
	 * Wersja standardu POSIX.
	 */
	pthread_cond_t  Condition;
#endif

// =====================================================================================================================

	/**
	 * Konstruktor klasy Searcher.
	 * Pozwala na przypisanie domyślnych wartości zmiennym w klasie.
	 * Wywołuje funkcję Searcher::Criteria.
	 * 
	 * @param folder Nazwa folderu, który program będzie przeszukiwał.
	 * @param phrase Fraza wyszukiwana w plikach.
	 * @param filter Filtr stosowany na rozszerzeniach plików.
	 */
	Searcher( string folder = "./", string phrase = "", string filter = "*" );

	/**
	 * Uruchamia wyszukiwarkę.
	 * Funkcja otwiera foldery w poszukiwaniu plików, w których testowana jest podana fraza.
	 * Gdy fraza zostaje znaleziona, raportowana jest do klasy Interface przypisanej do zmiennej Printer.
	 * Dzięki temu wyniki wyszukiwania pojawiają się od razu po znalezieniu konkretnego elementu,
	 * więc nie trzeba czekać na zakończenie wyszukiwania.
	 */
	void Run( void );

	/**
	 * Zmienia kryteria wyszukiwania.
	 * Funkcja pozwala na zmianę kryteriów wyszukiwania przypisanych w konstruktorze.
	 * Należy ją wywołać przed uruchomieniem funkcji Searcher::Run.
	 * 
	 * @param folder Nazwa folderu, który program będzie przeszukiwał.
	 * @param phrase Fraza wyszukiwana w plikach.
	 * @param filter Filtr stosowany na rozszerzeniach plików.
	 */
	void Criteria( string folder = "./", string phrase = "", string filter = "*" );

	/**
	 * Pobiera nazwę folderu przekazanego do kryterium wyszukiwania.
	 *
	 * @return Nazwa folderu.
	 */
	string GetFolder( void );

	/**
	 * Pobiera nazwę frazy przekazanej do kryterium wyszukiwania.
	 *
	 * @return Nazwa frazy.
	 */
	string GetPhrase( void );

	/**
	 * Pobiera nazwę filtru przekazanego do kryterium wyszukiwania.
	 *
	 * @return Nazwa filtru.
	 */
	string GetFilter( void );
};

#endif
