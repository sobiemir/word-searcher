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

#include "../inc/searcher.hpp"

// =====================================================================================================================

Searcher::Searcher( string folder, string phrase, string filter )
{
	this->Printer   = NULL;
	this->Searching = false;

#ifdef WSD_SYSTEM_WINDOWS
	InitializeCriticalSection( &this->Mutex );
	InitializeConditionVariable( &this->Condition );
#else
	this->Mutex     = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	this->Condition = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
#endif

	this->Criteria( folder, phrase, filter );
}

// =====================================================================================================================

void Searcher::Criteria( string folder, string phrase, string filter )
{
	this->_Folder = folder;
	this->_Phrase = phrase;
	this->_Filter = filter;

	this->Trim( this->_Filter );
	this->Trim( this->_Folder );

	if( this->_Folder.empty() )
		this->_Folder = "./";
	if( this->_Filter.empty() )
		this->_Filter = "*";

	this->ParseExtensions( this->_Filter );

	char chr = this->_Folder[this->_Folder.size() - 1];

#ifdef WSD_SYSTEM_WINDOWS
	// windows lewy ukośnik traktuje tak samo jak prawy ukośnik, a więc separator pomiędzy folderami
	if( chr != '\\' && chr != '/' )
		this->_Folder += "/";
#else
	// w przypadku systemu linuks, nazwa pliku może zawierać lewy ukośnik, więc separatorem jest tylko prawy
	if( chr != '/' )
		this->_Folder += "/";
#endif
}

// =====================================================================================================================

void Searcher::Run( void )
{
	size_t dirnums  = 0;
	size_t filenums = 0;
	size_t foundpos;

	vector<string> dirlist;

	DIR   *dirptr;
	struct dirent *direntry;

	this->Searching          = true;
	this->Printer->Searching = true;

	this->Printer->ToggleHeaderMessage( true, " ...wyszukiwanie... " );
	this->Printer->ToggleFooterMessage( true, " Wcisnij ^C aby przerwac " );

#ifdef WSD_SYSTEM_WINDOWS
	WakeConditionVariable( &this->Condition );
#else
	// wyślij sygnał, że wszystko jest w porządku...
	pthread_cond_signal( &this->Condition );
#endif

	// wyczyść listę plików przed wyszukiwaniem
	this->FoundFiles.clear();

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

				string filename   = currdir + direntry->d_name;
				string shortfname = currdir + direntry->d_name;
				string textline;
				
				// przerwij wyszukiwanie gdy jest to konieczne
				if( !this->Searching )
					break;

				// sprawdź czy znaleziony plik jest folderem
				if( direntry->d_type == DT_DIR )
				{
					// jeżeli jest to wyszukiwanie bez podfolderów, pomiń...
					if( this->_Modifiers & WSD_MOD_NOINCREMENTAL )
						continue;

					if( strcmp(direntry->d_name, ".") && strcmp(direntry->d_name, "..") )
						dirlist.push_back( currdir + direntry->d_name + "/" ),
						dirnums++;
				}
				// tylko zwykłe pliki
				else if( direntry->d_type == DT_REG )
				{
					foundpos = 0;

					// pomijanie plików, które nie spełniają kryteriów
					if( !this->CheckExtension(filename) )
						continue;

					// otwórz plik
					filestream.open( filename.c_str(), std::ios::in );

					// problem z otwarciem pliku...
					if( filestream.fail() )
					{
						filestream.close();
						continue;
					}

					shortfname.erase( 0, this->_Folder.size() );
					filenums++;

					this->Printer->RefreshCurrentFile( &shortfname );

					// szukaj frazy
					while( getline(filestream, textline, '\n') )
						if( (foundpos = textline.find(this->_Phrase)) != std::string::npos )
						{
							// dodaj do listy znalezionych
							this->FoundFiles.push_back( shortfname );

							if( this->Printer != NULL )
								this->Printer->RefreshPrintedFiles();

							break;
						}

					filestream.close();
				}
			}

		// zamnkij folder
		closedir( dirptr );

		// usuwa pierwszy element
		if( this->Searching )
			dirlist.erase( dirlist.begin() );
		else
		{
			dirlist.clear();
			break;
		}
	}

	// zatrzymaj wyszukiwanie
	this->Printer->Searching = false;
	this->Printer->ToggleHeaderMessage( false );
	this->Printer->ToggleFooterMessage( false );
}

// =====================================================================================================================

string Searcher::GetFolder( void )
{
	return this->_Folder;
}

// =====================================================================================================================

string Searcher::GetPhrase( void )
{
	return this->_Phrase;
}

// =====================================================================================================================

string Searcher::GetFilter( void )
{
	return this->_Filter;
}

// =====================================================================================================================

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

// =====================================================================================================================

void Searcher::ParseExtensions( string ext )
{
	// wyczyść poprzednie filtry
	this->_ExtensionList.clear();
	this->_Modifiers = 0;

	size_t occ = ext.find( '/', 0 );
	while( occ != string::npos )
	{
		int start = occ;
		occ = ext.find( '/', occ + 1 );

		string data;
		if( occ != string::npos )
			data = ext.substr( start + 1, occ - (start + 1) );
		else
			data = ext.substr( start + 1 );

		if( !data.empty() )
			this->_ExtensionList.push_back( data );
	}

	for( int x = 0, y = ext.size(); x > -1 && x < y; ++x )
		switch( ext[x] )
		{
			case '/': x = -2; break;
			case '*': this->_Modifiers |= WSD_MOD_ALL; break;
			case '-': this->_Modifiers |= WSD_MOD_EXCLUDE; break;
			case '!': this->_Modifiers |= WSD_MOD_CASESENSITIVE; break;
			case '@': this->_Modifiers |= WSD_MOD_NOEXT; break;
			case '=': this->_Modifiers |= WSD_MOD_NOINCREMENTAL; break;
		}

	// zamień na małe litery w przypadku gdy wyszukiwanie nie uwzględnia wielkości znaków
	if( !(this->_Modifiers & WSD_MOD_CASESENSITIVE) )
		for( size_t x = 0; x < this->_ExtensionList.size(); ++x )
			for( size_t y = 0; y < this->_ExtensionList[x].size(); ++y )
				this->_ExtensionList[x][y] = tolower( this->_ExtensionList[x][y] );
}

// =====================================================================================================================

bool Searcher::CheckExtension( string filename )
{
	int dotpos = -1;

	if( (this->_Modifiers & (WSD_MOD_NOEXT | WSD_MOD_ALL)) == WSD_MOD_ALL )
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

	// plik bez rozszerzenia domyślnie jest odrzucany, po przekazaniu modyfikatora już nie
	if( dotpos == -1 )
	{
		if( (this->_Modifiers & (WSD_MOD_NOEXT | WSD_MOD_ALL)) == WSD_MOD_NOEXT )
			return true;
		else
			return false;
	}
	else if( this->_Modifiers & WSD_MOD_ALL )
		return true;

	const char *ext = &filename.c_str()[dotpos];
	bool        cmp = false;
	size_t      len = filename.size() - dotpos;

	// sprawdź czy rozszerzenie się zgadza, zgodnie z wybraną techniką
	// albo uwzględnia wielkość liter, albo nie
	if( this->_Modifiers & WSD_MOD_CASESENSITIVE )
	{
		for( size_t x = 0; x < this->_ExtensionList.size(); ++x )
			if( len == this->_ExtensionList[x].size() && strcmp(ext, this->_ExtensionList[x].c_str()) == 0 )
			{
				cmp = true;
				break;
			}
	}
	else
	{
		// w tym przypadku trzeba sprawdzać każdy znak po kolei
		for( size_t x = 0; x < this->_ExtensionList.size(); ++x )
			if( len == this->_ExtensionList[x].size() )
			{
				cmp = true;
				for( size_t y = 0; y < len; ++y )
					if( this->_ExtensionList[x][y] != tolower(ext[y]) )
					{
						cmp = false;
						break;
					}
				if( !cmp )
					break;
			}
	}

	// zaneguj, gdy pliki z podanym rozszerzeniem są pomijane
	return this->_Modifiers & WSD_MOD_EXCLUDE
		? !cmp
		: cmp;
}
