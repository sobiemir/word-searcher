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
    int pos = 0;

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

    if( pos == 0 )
        data[0] = '\0';

    return pos;
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
    
    char phrase[2048];
    char folder[2048];

    size_t folderLen;

    vector<string> foundFolders;
    vector<string> phraseList;
    
    int fileNums = 0;
    int folderNums = 0;

    DIR *dirPointer;
    struct dirent *dirEntry;

    // informacje o programie
    cout << "===============================================================================" << endl;
    cout << ": Program : WordSearcher                                                      :" << endl;
    cout << ": Author  : sobiemir                                                          :" << endl;
    cout << ": Version : 0.1.3                                                             :" << endl;
    cout << ": Desc    : Searching for phrase in files located in given directory.         :" << endl;
    cout << "===============================================================================" << endl;

    while( continueSearch )
    {
        // ścieżka do folderu
        if( !saveLastFolder )
        {
            cout << endl << "Folder [./]: ";
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
            if( folderLen > 1 && folder[folderLen-1] != '/' && folder[folderLen-1] != '\\' && folderLen < 2047 )
                folder[folderLen] = '/',
                folder[++folderLen] = '\0';
        }
        // szukana fraza
        if( !saveLastWord )
        {
            cout << endl << "Fraza: ";
            cin.getline( phrase, sizeof(phrase) );
        }

        // dodaj szukany folder do listy
        foundFolders.push_back( folder );
        cout << endl;
        cout << "Wyszukiwanie..." << endl;
        cout << "===============================================================================" << endl;

        // szukanie frazy
        while( foundFolders.size() > 0 )
        {
            string currFolder   = foundFolders.at( 0 );
            dirPointer          = opendir( currFolder.c_str() );

            // wyświetlanie postępu - foldery
            cout << endl;
            cout << "Folder: " << currFolder << endl;
            cout << "-------------------------------------------------------------------------------" << endl;

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
                        if( strcmp(dirEntry->d_name, ".") && strcmp(dirEntry->d_name, "..") )
                            foundFolders.push_back( currFolder + dirEntry->d_name + "/" ),
                            folderNums++;
                    }
                    else
                    {
                        // otwórz plik i wyświetl postęp - pliki
                        fileStream.open( fileName, std::ios::in );
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

        int phraseListSize = phraseList.size();
        int userOption     = 0;

        // wyświetlanie wyników
        if( phraseListSize > 0 )
        {
            cout << endl;
            cout << "-------------------------------------------------------------------------------" << endl;
            cout << "Przeszukiwany katalog: " << folder << endl;
            cout << "Szukana fraza: " << phrase << endl;
            cout << "Przeszukano " << fileNums + folderNums << " elementow, w tym:" << endl;
            cout << "   - " << fileNums << " elementow to pliki." << endl;
            cout << "   - " << folderNums << " elementow to foldery." << endl;
            cout << "   - " << phraseListSize << " plikow spelnia kryteria wyszukiwania" << endl;
            cout << "-------------------------------------------------------------------------------" << endl;
            cout << "Lista plikow spelniajacych kryteria wyszukiwania:" << endl;
            cout << "-------------------------------------------------------------------------------" << endl;

            for( int x = 0; x < phraseListSize; ++x )
                cout << "   - " << phraseList.at( x ) << endl;
        }
        else
        {
            cout << endl;
            cout << "-------------------------------------------------------------------------------" << endl;
            cout << "Przeszukiwany katalog: " << folder << endl;
            cout << "Szukana fraza: " << phrase << endl;
            cout << "Przeszukano " << fileNums + folderNums << " elementow, w tym:" << endl;
            cout << "   - " << fileNums << " elementow to pliki." << endl;
            cout << "   - " << folderNums << " elementow to foldery." << endl;
            cout << "   - " << phraseListSize << " plikow spelnia kryteria wyszukiwania" << endl;
            cout << "-------------------------------------------------------------------------------" << endl;
            cout << "Podana fraza nie zostala znaleziona w zadnym pliku!" << endl;
        }

        // menu dodatkowe
        cout << endl;
        cout << "[0] Wyjdz" << endl;
        cout << "[1] Nowe wyszukiwanie" << endl;
        cout << "[2] Szukaj ponownie" << endl;
        cout << "[3] Szukaj w innym folderze" << endl;
        cout << "[4] Szukaj innej frazy" << endl;
        cout << endl;
        cout << "Wybierz opcje: ";
        cin  >> userOption;

        // od nowa...
        switch( userOption )
        {
            case 1:  saveLastFolder = false; saveLastWord = false; break;
            case 2:  saveLastFolder = true;  saveLastWord = true;  break;
            case 3:  saveLastFolder = false; saveLastWord = true;  break;
            case 4:  saveLastFolder = true;  saveLastWord = false; break;
            default: continueSearch = false;
        }
        // czyszczenie listy wyników
        phraseList.clear();

        cin.ignore( INT_MAX, '\n' );
        cin.clear();
    }
    return 0;
}