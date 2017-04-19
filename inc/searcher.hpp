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
#   include "dirent.h"
#else
#   include <dirent.h>
#endif

using namespace std;

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
    vector<string> FoundFiles;
    Interface *Printer;

    Searcher( string folder = "./", string phrase = "", string filter = "*" );

    void Run( void );

    void Criteria( string folder = "./", string phrase = "", string filter = "*" );

    string GetFolder( void );
    string GetPhrase( void );
    string GetFilter( void );
};

#endif
