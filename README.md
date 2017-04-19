# WordSearcher

Program pozwala na wyszukiwanie frazy w plikach, znajdujących się w podanym katalogu.
Program działa w konsoli zarówno na systemie Windows, jak i na systemie Linux.

W przypadku systemu Linux, jedyne co wystarczy zrobić to odpalić skrypt `compile.sh`.
Po poprawnej kompilacji utworzony zostanie plik `wordsearcher`.

### Filtrowanie

Od wersji 0.1.4 możliwe jest filtrowanie rozszerzeń plików.
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

### Wygląd

Po uruchomieniu program będzie wyglądał mniej więcej tak:

![WordSearcher v0.1.4](http://img.aculo.pl/word-searcher/v0.1.4-start.png)

Program na początku przejdzie w tryb interakcji z użytkownikiem, prosząc o podanie kolejno:

  * Folderu w którym program będzie szukał plików.
  * Frazy, która będzie wyszukiwana w plikach.
  * Filtru, stosowanego na rozszerzeniach wszystkich znalezionych plików.

Po filtru i wciśnięciu przycisku ENTER, aplikacja rozpocznie wyszukiwanie.
Po zakończonym wyszukiwaniu pojawią się statystyki i menu wyboru kolejnej akcji.

![WordSearcher v0.1.4](http://img.aculo.pl/word-searcher/v0.1.4-menu.png)

Program po podaniu wartości spoza zakresu (innej niż wypisane w menu) powtórzy pytanie i będzie oczekiwał na porawną wartość.
