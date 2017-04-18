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

#ifdef MSD_SYSTEM_WINDOWS
#   include "dirent.h"
#else
#   include <dirent.h>
#endif

using namespace std;

class Searcher
{
private:
    string _Folder;
    string _Phrase;
    string _Filter;

    vector<string> _ExtensionList;

    int _Modifiers;

    void Trim( string &str );
    void ParseExtensions( string ext );
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
