# WordSearcher

Program pozwala na wyszukiwanie frazy w plikach, znajdujących się w podanym katalogu.
Program działa w konsoli zarówno na systemie Windows, jak i na systemie Linux.

Do projektu dołączony został plik Makefile.
W przypadku systemów, w których zainstalowany jest `make` oraz `g++` z `lpthread` i `ncurses`, wystarczy uruchomić w konsoli polecenie `make`, aby skompilować program i utworzyć potrzebne foldery.
W przypadku pierwszej kompilacji, warto jednak uruchomić polecenie `make all`, aby utworzyć potrzebne foldery, do których zapisywane będą pliki tworzone przez `g++`.
Po poprawnej kompilacji utworzony zostanie plik `wordsearcher`.

Od wersji 0.2.0 aplikacja działa w oparciu o bibliotekę NCurses *(New Curses)*.
Biblioteka ta nie jest dostępna dla systemu Windows, jednak istnieje jej odpowiednik PDCurses *(Public Domain Curses)*, który może z powodzeniem zastąpić działanie oryginalnej biblioteki.

Aby poprawnie skompilować projekt na systemie Microsoft Windows, należy pobrać wspomnianą powyżej bibliotekę [*PDCurses*](https://pdcurses.sourceforge.io/) i dołączyć ją do projektu. Plik nagłókowy jest już dołączony do źródeł.

### Filtrowanie

Od wersji 0.1.4 / 0.2.1 możliwe jest filtrowanie rozszerzeń plików.
Filtr można wpisać zaraz po podaniu frazy i powinien on mieć następujący format:

    [modyfikatory]/[rozszerzenie1]/[rozszerzenie2]/...

Gdzie modyfikatorami są:

  * `!` : Porównywanie rozszerzeń z uwzględnieniem wielkości liter - szybsze niż zwykłe.
  * `@` : Uwzględnienie w wyszukiwaniu plików, nie posiadających rozszerzeń.
  * `*` : Wyszukiwanie we wszystkich plikach - domyślne gdy filtr jest pusty.
  * `+` : Przeszukiwanie w plikach o podanych rozszerzeniach - parametr domyślny.
  * `-` : Przeszukiwanie w plikach o rozszerzeniach innych niż podane.
  * `=` : Przeszukiwanie tylko w podanym katalogu, bez podkatalogów.

Modyfikatory można łączyć oraz podawać bez listy rozszerzeń.
Przypadki podawania modyfikatorów bez listy rozszerzeń:

  * `@  ` : Wyszukiwanie tylko w plikach bez rozszerzeń.
  * `*@ ` : Wyszukiwanie w plikach posiadających rozszerzenia.
  * `-  ` : Wolniejszy odpowiednik pozycji `*@`.
  * `*= ` : Wyszukiwanie we wszystkich plikach, ale tylko w podanym folderze.
  * `*=@` : Wyszukiwanie we wszystkich plikach o dowolnym rozszerzeniu w podanym folderze.
  * `@= ` : Wyszukiwanie tylko w plikach bez rozszerzenia w podanym folderze.

Przykłady poprawnych filtrów:

    *@
    !+/html/css
    -/jpg/png
    /php/js/ts
    !@-/cpp/hpp
    =+/c/h

### Wygląd i nawigacja

Po uruchomieniu program w przypadku systemu Linux wyświetlonym na konsoli *[Terminator v1.0](https://gnometerminator.blogspot.com/p/introduction.html)* będzie wyglądał mniej więcej tak:

![WordSearcher v0.2.2-linux](http://img.aculo.pl/word-searcher/v0.2.2-linux.png)

Na samej górze znajdują się pola tekstowe dla folderu, frazy i filtru.
Uruchomienie wyszukiwarki z domyślnymi wartościami tych pól, spowoduje wypisanie wszystkich plików, możliwych do odczytu.
Od wersji 0.2.2 możliwe jest przerwanie uruchomionego wyszukiwania bez konieczności zamykania programu lub zabijania procesu, z racji przeniesienia wyszukiwarki do osobnego wątku.

Lista skrótów klawiaturowych, dostępnych w programie:
  
  * `CTRL + C` - kończy działanie programu lub przerywa wyszukiwanie.
  * `CTRL + I` - ustawia skupienie na polu tekstowym, pozwalającym na zmianę filtra.
  * `CTRL + O` - ustawia skupienie na polu tekstowym, pozwalającym na zmianę folderu.
  * `CTRL + P` - ustawia skupienie na polu tekstowym, pozwalającym na zmianę frazy.
  * `CTRL + Q` - ustawia skupienie na polu, dzięki czemu możliwe jest przewijanie wyników wyszukiwania.

Funkcja przerywa wyszukiwanie, dopiero gdy skończy szukać frazy w otwartym pliku.
Aby wyjść z trybu skupienia pola tekstowego, należy wcisnąć klawisz `ENTER`. To samo tyczy się pola zawierającego wyniki wyszukiwania.

Program skompilowany i uruchomiony na systemie Microsoft Windows wygląda tak:

![WordSearcher v0.2.2-windows](http://img.aculo.pl/word-searcher/v0.2.2-windows.png)

Pod polami tekstowymi znajduje się panel, w którym wyświetlana jest nazwa pliku, który aktualnie jest przeszukiwany. Pod nim znajduje sę panel, zawierający listę wszystkich plików, które spełniają kryterium wyszukiwania. Wyniki wyświetlane są na bierząco.

Obok wersji 0.2.x rozwijana jest wersja 0.1.x, która działa bezpośrednio na konsoli.

## Lista zmian w programie

  * _v0.2.2_
    * Przeniesiono działanie kodu wyszukiwarki do osobnego wątku.
    * Kombinacja klawiszy CTRL + C powoduje przerwanie wyszukiwania.
    * Dodano informację o wyszukiwaniu (napis po rozpoczęciu wyszukiwania)
    * Dodano zabezpieczenie przed przypadkowym zamknięciem programu, teraz po wciśnięciu kombinacji klawiszy CTRL + C,
      należy dodatkowo wcisnąć klawisz ESC.
    * Zmieniono klawisze dostępu do pól tekstowych na CTRL + (I/O/P).
  * _v0.1.4_
    * Utworzono filtrowanie rozszerzeń plików.
    * Dodano możliwość wyłączenia wyszukiwania przyrostowego.
  * _v0.2.1_
    * Naprawiono błędy kompilacji na systemie _Windows_.
    * Naprawiono błąd związany z ostatnim ukośnikiem w ścieżce.
    * Utworzono filtrowanie rozszerzeń plików.
    * Dodano możliwość wyłączenia wyszukiwania przyrostowego.
  * _v0.2.0_
    * Przeniesiono kod z wersji 0.1.3 na wersję 0.2.0.
    * Zmieniono wygląd interfejsu - teraz program używa biblioteki ncurses.
    * Utworzono pola tekstowe dla folderu i frazy.
    * Utworzono pole, w którym wyświetlane są wyniki wyszukiwania.
  * _v0.1.3_
    * Naprawiono błąd związany z ostatnim ukośnikiem w ścieżce.
    * Utworzono plik README.
  * _v0.1.2_
    * Naprawiono błędy w kompilacji na systemie _Linux_.
    * W przypadku nie podania ścieżki, przyjmowana jest aktualna `./`.
  * _v0.1.1_
    * Dodano statystyki wyszukiwania.
    * Naprawiono błąd związany ze spacją w ścieżce folderu.
  * _v0.1.0_
    * Pierwsza wersja programu.
