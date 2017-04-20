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

#ifndef __WSH_CONFIGURATION__
#define __WSH_CONFIGURATION__

// sprawdź z jakim systemem program ma do czynienia
#ifdef _WIN32
#	define WSD_SYSTEM_WINDOWS
#elif defined __linux__
#	define WSD_SYSTEM_LINUX
#endif

/**
 * Definicje wartości modyfikatorów dla filtrów.
 * Pozwalają na szybkie rozpoznanie z jakim modyfikatorem funkcja ma do czynienia.
 */
#define WSD_MOD_ALL           0x01  // Wszystkie znalezione pliki.
#define WSD_MOD_CASESENSITIVE 0x02  // Sprawdzanie rozszerzeń z uwzględnieniem wielkości znaków.
#define WSD_MOD_EXCLUDE       0x04  // Pliki z każdym rozszerzeniem oprócz uwzględnionych w liście.
#define WSD_MOD_NOEXT         0x08  // Wyszukiwanie razem z plikami bez rozszerzeń.
#define WSD_MOD_NOINCREMENTAL 0x10  // Wyszukiwanie tylko w podanym folderze - domyślnie jest przyrostowe.

/**
 * Struktura zmiennej składającej się z dwóch elementów.
 * Przykładami takich zmiennych są wymiary lub pozycja.
 *
 * Struktura zawiera dwie unie, zawierające elementy tego samego typu.
 * Dzięki temu nazwa jednego elementu jest aliasem drugiego.
 */
template <typename T> struct Vector2
{
public:
	union
	{
		T X;
		T Width;
	};
	union
	{
		T Y;
		T Height;
	};
	Vector2<T>( void ):
		X(0), Y(0) {}
};

#endif
