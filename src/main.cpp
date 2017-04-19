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

// sprawdź z jakim systemem program ma do czynienia
#ifdef _WIN32
#   define MSD_SYSTEM_WINDOWS
#elif defined __linux__
#   define MSD_SYSTEM_LINUX
#endif

#define WSD_MOD_ALL           0x01
#define WSD_MOD_CASESENSITIVE 0x02
#define WSD_MOD_EXCLUDE       0x04
#define WSD_MOD_NOEXT         0x08
#define WSD_MOD_NOINCREMENTAL 0x10

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <climits>

#ifdef MSD_SYSTEM_WINDOWS
#   include "dirent.h"
#else
#   include <dirent.h>
#endif

using namespace std;

/**
 * Trim obustronny.
 * @param  data Dane do sprawdzenia i obcięcia.
 * @param  len  Długość danych.
 * @return      Długość danych po obcięciu.
 */
int trim_charstr( char *data, size_t len )
{
    size_t pos = 0;

    if( len == 0 )
        len = strlen( data );

    for( ; pos < len; ++pos )
        if( data[pos] != ' ' )
            break;

    if( pos != 0 && data[pos] != '\0' )
        memmove( data, &data[pos], len - pos );

    len -= pos;

    // należy zakończyć ciąg, gdyż może powodować późniejsze błędy
    data[len] = '\0';

    for( pos = len; pos > 0; --pos )
        if( data[pos] != ' ' && data[pos] != '\0' )
        {
            data[++pos] = '\0';
            break;
        }

    if( pos == 0 && (data[0] == ' ' || data[0] == '\0') )
        data[0] = '\0';
    else if( pos == 0 )
        data[++pos] = '\0';

    return pos;
}

/**
 * Parsuje rozszerzenia zamieniając je na listę.
 * !+/html/css
 * Taki ciąg po parsowaniu tworzy dwa rozszerzenia: html, css
 * Dodatkowo dwa modyfikatory.
 *
 * Lista modyfikatorów:
 * > ! : sprawdzanie rozszerzeń zgodnie z wielkością liter - szybsze przetwarzanie
 * > + : modyfikator opcjonalny, tylko pliki z podanymi rozszerzeniami są przeszukiwane
 * > - : tylko pliki o rozszerzeniach innych niż podane są przesukiwane
 * > * : przeszukiwane są wszystkie rozszerzenia
 * > @ : uwzględnij w filtrze również pliki bez rozszerzeń
 * > = : przeszukiwanie tylko podanego folderu, bez podfolderów
 *
 * Szczególne przypadki podawania modyfikatorów bez rozszerzeń
 * > @   : bez podawania rozszerzeń - szuka tylko w plikach bez rozszerzeń
 * > *@  : szuka tylko w plikach z rozszerzeniami
 * > -   : bez podawania rozszerzeń - szuka tylko w plikach z rozszerzeniami, wolniejsza wersja *@
 * > *=  : przeszukuje wszystkie plików tylko w podanym folderze
 * > *=@ : przeszukuje wszystkie pliki z jakimkolwiek rozszerzeniem w podanym folderze
 * > @=  : przeszukuje wszystkie pliki bez rozszerzenia w podanym folderze
 *
 * @param  str    Ciąg znaków zawierający rozszerzenia.
 * @param  extvec Lista rozszerzeń wyciągniętych z ciągu.
 * @return        Modyfikatory zamienione na liczbę.
 */
int parse_extensions( char *str, vector<string> &extvec )
{
    int modifier  = 0;
    string extstr = str;

    // wyczyść poprzednie filtry
    extvec.clear();

    size_t occ = extstr.find( '/', 0 );
    while( occ != string::npos )
    {
        int start = occ;
        occ = extstr.find( '/', occ + 1 );

        string data;
        if( occ != string::npos )
            data = extstr.substr( start + 1, occ - (start + 1) );
        else
            data = extstr.substr( start + 1 );

        if( !data.empty() )
            extvec.push_back( data );
    }

    for( int x = 0, y = extstr.size(); x > -1 && x < y; ++x )
        switch( extstr[x] )
        {
            case '/': x = -2; break;
            case '*': modifier |= WSD_MOD_ALL;           break;
            case '-': modifier |= WSD_MOD_EXCLUDE;       break;
            case '!': modifier |= WSD_MOD_CASESENSITIVE; break;
            case '@': modifier |= WSD_MOD_NOEXT;         break;
            case '=': modifier |= WSD_MOD_NOINCREMENTAL; break;
        }

    // zamień na małe litery w przypadku gdy wyszukiwanie nie uwzględnia wielkości znaków
    if( !(modifier & WSD_MOD_CASESENSITIVE) )
        for( size_t x = 0; x < extvec.size(); ++x )
            for( size_t y = 0; y < extvec[x].size(); ++y )
                extvec[x][y] = tolower( extvec[x][y] );

    return modifier;
}

/**
 * Sprawdza, czy rozszerzenie pasuje do zdefiniowanych kryteriów.
 *
 * @param  filename Nazwa pliku.
 * @param  extvec   Lista rozszerzeń wyciągniętych z ciągu.
 * @param  modifier Modyfikatory.
 * @return          Modyfikatory zamienione na liczbę.
 */
bool file_extension_check( string filename, vector<string> &extvec, int modifier )
{
    int dotpos = -1;

    if( (modifier & (WSD_MOD_NOEXT | WSD_MOD_ALL)) == WSD_MOD_ALL )
        return true;

    // wyszukaj kropkę w nazwie pliku
    for( int x = filename.size() - 1; x >= 0; --x )
    {
        if( filename[x] == '/' )
            break;
        else if( filename[x] == '.' )
        {
            dotpos = x + 1;
            break;
        }
    }

    // plik bez rozszerzenia domyślnie są odrzucane, po przekazaniu modyfikatora już nie
    if( dotpos == -1 )
    {
        if( (modifier & (WSD_MOD_NOEXT | WSD_MOD_ALL)) == WSD_MOD_NOEXT )
            return true;
        else
            return false;
    }
    else if( modifier & WSD_MOD_ALL )
        return true;

    const char *ext = &filename.c_str()[dotpos];
    bool        cmp = false;
    size_t      len = filename.size() - dotpos;

    // sprawdź czy rozszerzenie się zgadza, zgodnie z wybraną techniką
    // albo uwzględnia wielkość liter, albo nie
    if( modifier & WSD_MOD_CASESENSITIVE )
    {
        for( size_t x = 0; x < extvec.size(); ++x )
            if( len == extvec[x].size() && strcmp(ext, extvec[x].c_str()) == 0 )
            {
                cmp = true;
                break;
            }
    }
    else
    {
        // w tym przypadku trzeba sprawdzać każdy znak po kolei
        for( size_t x = 0; x < extvec.size(); ++x )
            if( len == extvec[x].size() )
            {
                cmp = true;
                for( size_t y = 0; y < len; ++y )
                    if( extvec[x][y] != tolower(ext[y]) )
                    {
                        cmp = false;
                        break;
                    }
                if( !cmp )
                    break;
            }
    }

    // zaneguj, gdy pliki z podanym rozszerzeniem są pomijane
    return modifier & WSD_MOD_EXCLUDE
        ? !cmp
        : cmp;
}

/**
 * Funkcja główna...
 * @return  Kod błędu
 */
int main( void )
{
    bool continueSearch = true;
    bool saveLastFolder = false;
    bool saveLastWord   = false;
    bool saveLastFilter = false;
    
    char phrase[2048];
    char folder[2048];
    char filter[2048];

    size_t folderLen;
    size_t filterLen;

    vector<string> foundFolders;
    vector<string> phraseList;
    vector<string> extList;
    
    int fileNums     = 0;
    int folderNums   = 0;
    int extModifiers = 0;

    DIR *dirPointer;
    struct dirent *dirEntry;

    // informacje o programie
    cout << "===============================================================================" << endl;
    cout << ": Program : WordSearcher                                                      :" << endl;
    cout << ": Author  : sobiemir                                                          :" << endl;
    cout << ": Version : 0.1.4                                                             :" << endl;
    cout << ": Desc    : Searching for phrase in files located in given directory.         :" << endl;
    cout << "===============================================================================" << endl;
    cout << endl;

    while( continueSearch )
    {
        // ścieżka do folderu
        if( !saveLastFolder )
        {
            cout << "Folder [./]: ";
            cin.getline( folder, sizeof(folder) );

            folderLen = trim_charstr( folder, 0 );

            // w przypadku gdy użytkownik nie poda ścieżki do folderu
            // przyjmuj aktualną ścieżkę
            if( folderLen == 0 )
                folder[0] = '.',
                folder[1] = '/',
                folder[2] = '\0',
                folderLen = 2;

            // dodaj ukośnik gdy go brakuje
#       ifdef MSD_SYSTEM_WINDOWS
            if( folderLen > 1 && folder[folderLen-1] != '/' && folder[folderLen-1] != '\\' && folderLen < 2047 )
                folder[folderLen] = '/',
                folder[++folderLen] = '\0';
#       else
            // jako że linux w nazwie folderu lub pliku może zawierać lewy ukośnik, nie uwzględniaj go
            if( folderLen > 1 && folder[folderLen-1] != '/' && folderLen < 2047 )
                folder[folderLen] = '/',
                folder[++folderLen] = '\0';
#       endif
        }
        // szukana fraza
        if( !saveLastWord )
        {
            cout << "Fraza: ";
            cin.getline( phrase, sizeof(phrase) );
        }
        // filtr dla plików
        if( !saveLastFilter )
        {
            cout << "Filtr [*]: ";
            cin.getline( filter, sizeof(filter) );

            filterLen = trim_charstr( filter, 0 );
            if( filterLen == 0 )
                filter[0] = '*',
                filter[1] = '\0';

            extModifiers = parse_extensions( filter, extList );
        }

        // dodaj szukany folder do listy
        foundFolders.push_back( folder );
        cout << endl;
        cout << "Wyszukiwanie..." << endl;
        cout << "-------------------------------------------------------------------------------" << endl;

        // szukanie frazy
        while( foundFolders.size() > 0 )
        {
            string currFolder   = foundFolders.at( 0 );
            dirPointer          = opendir( currFolder.c_str() );

            // wyświetlanie postępu - foldery
            cout << endl;
            cout << "Folder: " << currFolder << endl;
            cout << "===============================================================================" << endl;

            if( dirPointer != NULL )
            {
                while( (dirEntry = readdir(dirPointer)) )
                {
                    string  fileName      = currFolder + dirEntry->d_name;
                    string  shortFileName = currFolder + dirEntry->d_name;
                    string  textLine;
                    fstream fileStream;

                    // sprawdź czy znaleziony plik jest folderem
                    if( dirEntry->d_type == DT_DIR )
                    {
                        // jeżeli ma wyszukiwać bez podfolderów, pomiń
                        if( extModifiers & WSD_MOD_NOINCREMENTAL )
                            continue;

                        if( strcmp(dirEntry->d_name, ".") && strcmp(dirEntry->d_name, "..") )
                            foundFolders.push_back( currFolder + dirEntry->d_name + "/" ),
                            folderNums++;
                    }
                    else
                    {
                        // pomijanie plików, które nie spełniają kryteriów
                        if( !file_extension_check(fileName, extList, extModifiers) )
                            continue;

                        // otwórz plik i wyświetl postęp - pliki
                        fileStream.open( fileName.c_str(), std::ios::in );
                        shortFileName.erase( 0, folderLen );
                        fileNums++;
                        cout << "   - " << shortFileName << "..." << endl;

                        // szukaj frazy
                        while( getline(fileStream, textLine, '\n') )
                        {
                            size_t foundPos = textLine.find( phrase );
                            if( foundPos != std::string::npos )
                            {
                                // dodaj do listy znalezionych
                                phraseList.push_back( shortFileName );
                                break;
                            }
                        }
                        fileStream.close( );
                    }
                }
            }
            else
                cout << "Folder o podanej nazwie nie istnieje!";

            // zamnkij folder
            closedir( dirPointer );
            foundFolders.erase( foundFolders.begin() );
        }

        int  phraseListSize = phraseList.size();
        char userOption     = -1;

        cout << endl;
        cout << "-------------------------------------------------------------------------------" << endl;
        cout << "Przeszukiwany katalog: " << folder << endl;
        cout << "Szukana fraza: " << phrase << endl;
        cout << "Uzyty filtr: " << filter << endl;
        cout << "Przeszukano " << fileNums + folderNums << " elementow, w tym:" << endl;
        cout << "   - " << fileNums << " elementow to pliki." << endl;
        cout << "   - " << folderNums << " elementow to foldery." << endl;
        cout << "   - " << phraseListSize << " plikow spelnia kryteria wyszukiwania" << endl;
        cout << "-------------------------------------------------------------------------------" << endl;

        // wyświetlanie wyników
        if( phraseListSize > 0 )
        {
            cout << "Lista plikow spelniajacych kryteria wyszukiwania:" << endl;
            cout << "-------------------------------------------------------------------------------" << endl;

            for( int x = 0; x < phraseListSize; ++x )
                cout << "   - " << phraseList.at( x ) << endl;
        }
        else
            cout << "Podana fraza nie zostala znaleziona w zadnym pliku!" << endl;

        cout << "-------------------------------------------------------------------------------" << endl;

        bool chooseMenu = true;
        while( chooseMenu )
        {
            // menu dodatkowe
            cout << endl;
            cout << "[1] Nowe wyszukiwanie" << endl;
            cout << "[2] Szukaj ponownie" << endl;
            cout << "[3] Szukaj w innym folderze" << endl;
            cout << "[4] Szukaj innej frazy" << endl;
            cout << "[5] Szukaj z uzyciem innego filtra" << endl;
            cout << "---" << endl;
            cout << "[6] Szukaj innej frazy z innym filtrem" << endl;
            cout << "[7] Szukaj w innym folderze z uzyciem innego filtra" << endl;
            cout << "[8] Szukaj innej frazy w innym folderze" << endl;
            cout << "---" << endl;
            cout << "[0] Wyjdz" << endl;
            cout << endl;
            cout << "Wybierz opcje: ";
            cin  >> userOption;

            // od nowa...
            chooseMenu = false;
            switch( userOption )
            {
                case '0':  continueSearch = false; break;
                case '1':  saveLastFolder = false; saveLastWord = false; saveLastFilter = false; break;
                case '2':  saveLastFolder = true;  saveLastWord = true;  saveLastFilter = true;  break;
                case '3':  saveLastFolder = false; saveLastWord = true;  saveLastFilter = true;  break;
                case '4':  saveLastFolder = true;  saveLastWord = false; saveLastFilter = true;  break;
                case '5':  saveLastFolder = true;  saveLastWord = true;  saveLastFilter = false; break;
                case '6':  saveLastFolder = true;  saveLastWord = false; saveLastFilter = false; break;
                case '7':  saveLastFolder = false; saveLastWord = true;  saveLastFilter = false; break;
                case '8':  saveLastFolder = false; saveLastWord = false; saveLastFilter = true;  break;
                default:
                    chooseMenu = true;
                    cin.ignore( INT_MAX, '\n' );
                    cin.clear();
            }
        }
        // czyszczenie listy wyników
        phraseList.clear();

        cin.ignore( INT_MAX, '\n' );
        cin.clear();

        cout << endl;
    }
    return 0;
}
