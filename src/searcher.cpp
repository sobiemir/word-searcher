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

#include "searcher.hpp"

Searcher::Searcher( string folder, string phrase, string filter )
{
    this->_Folder = folder;
    this->_Phrase = phrase;
    this->_Filter = filter;

    this->Printer = NULL;
}

void Searcher::Criteria( string folder, string phrase, string filter )
{
    this->_Folder = folder;
    this->_Phrase = phrase;
    this->_Filter = filter;
}

void Searcher::Run( void )
{
    size_t dirnums  = 0,
           filenums = 0;

    vector<string> dirlist;
    
    DIR   *dirptr;
    struct dirent *direntry;

    // wyczyść listę plików przed wyszukiwaniem
    this->FoundFiles.clear();

    // wyczyść nazwę folderu ze zbędnych spacji
    this->Trim( this->_Folder );

    // dodaj folder do listy
    dirlist.push_back( this->_Folder );

    while( dirlist.size() )
    {
        string currdir = dirlist.at( 0 );
        dirptr = opendir( currdir.c_str() );

        if( dirptr != NULL )
            // czytaj pliki z katalogu
            while( (direntry = readdir(dirptr)) )
            {
                fstream filestream;

                string  filename   = currdir + direntry->d_name,
                        shortfname = currdir + direntry->d_name,
                        textline;

                // sprawdź czy znaleziony plik jest folderem
                if( direntry->d_type == DT_DIR )
                {
                    if( strcmp(direntry->d_name, ".") && strcmp(direntry->d_name, "..") )
                        dirlist.push_back( currdir + direntry->d_name + "/" ),
                        dirnums++;
                }
                else
                {
                    // otwórz plik
                    filestream.open( filename.c_str(), std::ios::in );
                    shortfname.erase( 0, this->_Folder.size() );
                    filenums++;

                    this->Printer->RefreshCurrentFile( &shortfname );

                    // szukaj frazy
                    while( getline(filestream, textline, '\n') )
                    {
                        size_t foundPos = textline.find( this->_Phrase );
                        if( foundPos != std::string::npos )
                        {
                            // dodaj do listy znalezionych
                            this->FoundFiles.push_back( shortfname );

                            if( this->Printer != NULL )
                                this->Printer->RefreshPrintedFiles();

                            break;
                        }
                    }
                    filestream.close( );
                }
            }

        // zamnkij folder
        closedir( dirptr );

        // usuwa pierwszy element
        dirlist.erase( dirlist.begin() );
    }
}

string Searcher::GetFolder( void )
{
    return this->_Folder;
}

string Searcher::GetPhrase( void )
{
    return this->_Phrase;
}

string Searcher::GetFilter( void )
{
    return this->_Filter;
}

void Searcher::Trim( string &str )
{
    if( str.size() == 0 )
        return;

    size_t leftshift = str.find_first_not_of( " \t\f\v\n\r" );

    // w przypadku gdy żaden znak nie został znaleziony, wyczyść cały łańcuch
    if( leftshift == string::npos )
        return str.clear();
    
    str.erase( 0, leftshift );

    // wcześniej warunek obejmował przypadek dla całego ciągu wypełnionego białymi znakami
    size_t rightshift = str.find_last_not_of( " \t\f\v\n\r" );

    if( rightshift + 1 < str.size() )
        str.erase( rightshift + 1 );
}
