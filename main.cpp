/*
 *  WordSearcher
 *  Copyright (C) 2013-2017 sobiemir <sobiemir@aculo.pl>
 * 
 *  WordSearcher is free software: you can redistribute it and/or modify  
 *  it under the terms of the GNU Lesser General Public License as   
 *  published by the Free Software Foundation, version 3.
 *
 *  Word Searcher is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "dirent.h"

using namespace std;

int main( void )
{
    bool continueSearch = true;
    bool saveLastFolder = false;
    bool saveLastWord   = false;

    string folder;
    string phrase;

    int fileNums   = 0;
    int folderNums = 0;

    vector<string> foundFolders;
    vector<string> phraseList;

    DIR *dirPointer;
    struct dirent *dirEntry;

    // informacje o programie
    cout << "===============================================================================" << endl;
    cout << ": Program : WordSearcher                                                      :" << endl;
    cout << ": Author  : sobiemir                                                          :" << endl;
    cout << ": Version : 0.1.1                                                             :" << endl;
    cout << ": Desc    : Searching for phrase in files located in given directory.         :" << endl;
    cout << "===============================================================================" << endl;
    
    while( continueSearch )
    {
        // scieżka do folderu
        if( !saveLastFolder )
        {
            cout << endl << "Sciezka do folderu: ";
            cin  >> folder;
        }
        // szukana fraza
        if( !saveLastWord )
        {
            cout << endl << "Szukana fraza: ";
            cin  >> phrase;
        }

        // dodaj szukany folder do listy
        foundFolders.push_back( folder );
        cout << endl;
        cout << "Prosze czekac... trwa wyszukiwanie..." << endl;
        cout << "===============================================================================" << endl;

        // szukanie frazy
        while( foundFolders.size( ) > 0 )
        {
            string currFolder   = foundFolders.at( 0 );
            dirPointer          = opendir( currFolder.c_str( ) );

            // wyświetlanie postępu - foldery
            cout << endl;
            cout << "Folder: " << currFolder << endl;
            cout << "-------------------------------------------------------------------------------" << endl;

            if( dirPointer != NULL )
            {
                while( dirEntry = readdir( dirPointer ) )
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
                        shortFileName.erase( 0, folder.size() );
                        fileNums++;
                        cout << "   - " << shortFileName << endl;

                        // szukaj frazy
                        while( getline( fileStream, textLine, '\n' ) )
                        {
                            size_t foundPos = textLine.find( phrase );
                            if( foundPos != std::string::npos )
                            {
                                // dodaj do listy znalezionych
                                phraseList.push_back( shortFileName );
                                break;
                            }
                        }
                        fileStream.close();
                    }
                }
            }
            else
                cout << "Nie znaleziono folderu o takiej nazwie!";

            // zamnkij folder
            closedir( dirPointer );
            foundFolders.erase( foundFolders.begin( ) );
        }

        int phraseListSize = phraseList.size( );
        int userOption     = 0;

        // wyświetlanie wyników
        if( phraseListSize > 0 )
        {
            cout << endl;
            cout << "-------------------------------------------------------------------------------" << endl;
            cout << "Przeszukiwany katalog: " << folder << endl;
            cout << "Szukana fraza: " << phrase << endl;
            cout << "Przeszukano " << fileNums  folderNums << " elementow, w tym:" << endl;
            cout << "   - " << fileNums << " elementow to pliki." << endl;
            cout << "   - " << folderNums << " elementow to foldery." << endl;
            cout << "   - " << phraseListSize << " plikow spelnia kryteria wyszukiwania" << endl;
            cout << "-------------------------------------------------------------------------------" << endl;
            cout << "Lista plikow spelniajacych kryteria wyszukiwania:" << endl;
            cout << "-------------------------------------------------------------------------------" << endl;
            for( int x = 0; x < phraseListSize; ++x ) cout << "   - " << phraseList.at( x ) << endl;
        }
        else
            cout << endl << "Podana fraza nie zostala znaleziona w zadnym pliku!" << endl;

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

        // Od nowa...
        switch( userOption )
        {
            case 1:  saveLastFolder = false; saveLastWord = false; break;
            case 2:  saveLastFolder = true;  saveLastWord = true;  break;
            case 3:  saveLastFolder = false; saveLastWord = true;  break;
            case 4:  saveLastFolder = true;  saveLastWord = false; break;
            default: continueSearch = false;
        }
        // Czyszczenie listy wyników
        phraseList.clear( );
    }
    return 0;
}
