
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

    vector<string> foundFolders;
    vector<string> phraseList;

    DIR *dirPointer;
    struct dirent *dirEntry;

    // Informacje o programie
    cout << "===============================================================================" << endl;
    cout << ": Program  : WordSearcher                                                     :" << endl;
    cout << ": Autor    : Sobiemir                                                         :" << endl;
    cout << ": Wersja   : 0.1                                                              :" << endl;
    cout << ": Opis     : Program wyszukuje podana fraze w plikach (wyszukiwanie grupowe)  :" << endl;
    cout << "===============================================================================" << endl;
    
    // Informacje o programie nr. 2
    cout << endl;
    cout << "-------------------------------------------------------------------------------" << endl;
    cout << ": Witaj w programie WordSearcher!                                             :" << endl;
    cout << ": Zacial ci sie komputer? Podawaj to o co Cie program poprosi!                :" << endl;
    cout << ": Podajac sciezke do folderu zawsze koncz nazwe slashem!                      :" << endl;
    cout << "-------------------------------------------------------------------------------" << endl;
    
    while( continueSearch )
    {
        // Ścieżka do folderu
        if( !saveLastFolder )
        {
            cout << endl << "Sciezka do folderu: ";
            cin >> folder;
        }
        // Szukana fraza
        if( !saveLastWord )
        {
            cout << endl;
            cout << "Szukana fraza: ";
            cin >> phrase;
        }

        // Dodaj szukany folder do listy
        foundFolders.push_back( folder );
        cout << endl;
        cout << "Prosze czekac... trwa wyszukiwanie..." << endl;
        cout << "===============================================================================" << endl;

        // Szukanie frazy
        while( foundFolders.size( ) > 0 )
        {
            string currFolder   = foundFolders.at( 0 );
            dirPointer          = opendir( currFolder.c_str( ) );

            // Wyświetlanie postępu - foldery
            cout << endl;
            cout << "Folder: " << currFolder << endl;
            cout << "-------------------------------------------------------------------------------" << endl;

            if( dirPointer != NULL )
            {
                while( dirEntry = readdir( dirPointer ) )
                {
                    string  fileName = currFolder + dirEntry->d_name;
                    string  textLine;
                    fstream fileStream;

                    // Sprawdź czy znaleziony plik jest folderem
                    if( dirEntry->d_type == DT_DIR && strcmp( dirEntry->d_name, "." ) && strcmp( dirEntry->d_name, ".." ) )
                    {
                        foundFolders.push_back( currFolder + dirEntry->d_name + "/" );
                        continue;
                    }

                    // Otwórz plik i wyświetl postęp - pliki
                    fileStream.open( fileName, std::ios::in );
                    cout << "   - " << fileName << endl;

                    // Szukaj frazy
                    while( getline( fileStream, textLine ) )
                    {
                        size_t foundPos = textLine.find( phrase );
                        if( foundPos != std::string::npos )
                        {
                            // Dodaj do listy znalezionych
                            phraseList.push_back( fileName );
                            break;
                        }
                    }
                    fileStream.close( );
                }
            }
            else cout << "Nie znaleziono folderu o takiej nazwie!";

            // Zamnkij folder
            closedir( dirPointer );
            foundFolders.erase( foundFolders.begin( ) );
        }

        int phraseListSize  = phraseList.size( );
        int userOption      = 0;

        // Wyświetlanie wyników
        if( phraseListSize > 0 )
        {
            cout << endl << "Podana fraze znaleziono w nastepujacych plikach:" << endl;
            cout << "-------------------------------------------------------------------------------" << endl;
            for( int x = 0; x < phraseListSize; ++x ) cout << "   - " << phraseList.at( x ) << endl;
        }
        else cout << endl << "Podana fraza nie zostala znaleziona w zadnym pliku!" << endl;

        // Menu dodatkowe
        cout << endl;
        cout << "[0] Wyjdz" << endl;
        cout << "[1] Nowe wyszukiwanie" << endl;
        cout << "[2] Szukaj ponownie" << endl;
        cout << "[3] Szukaj w innym folderze" << endl;
        cout << "[4] Szukaj innej frazy" << endl;
        cout << endl;
        cout << "Wybierz opcje: ";
        cin >> userOption;

        // Od nowa...
        switch( userOption )
        {
        case 1: saveLastFolder = false; saveLastWord = false; break;
        case 2: saveLastFolder = true; saveLastWord = true; break;
        case 3: saveLastFolder = false; saveLastWord = true; break;
        case 4: saveLastFolder = true; saveLastWord = false; break;
        default: continueSearch = false;
        }
        // Czyszczenie listy wyników
        phraseList.clear( );
    }
    return 0;
}
