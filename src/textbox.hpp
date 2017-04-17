#ifndef __WSH_PROMPT__
#define __WSH_PROMPT__

#include <ncurses.h>
#include <vector>
#include <string>

#define WSD_CTRL(X) (X & 0x1F)

//#define WSD_USE_WCHAR

using namespace std;

struct Vector2I
{
    int X;
    int Y;
};

class TextBox
{
private:
    string   _Label;            // zawartość napisu
    int      _LabelStyle;       // styl wyświetlania napisu
    string   _Content;          // zawartość pola
    int      _ContentStyle;     // styl wyświetlania zawartości pola
    size_t   _TypeStart;        // indeks od którego rozpoczynać się będzie wprowadzanie tekstu
    size_t   _Size;             // rozmiar pola tekstowego
    Vector2I _Position;         // pozycja pola wraz z napisem
    WINDOW  *_Window;           // okno w którym pole tekstowe będzie wyświetlane
    size_t   _Shift;            // przesunięcie wyświetlanej treści w elementach względem początku
    size_t   _Letters;          // ilość liter wyświetlana w polu tekstowym
    size_t   _CaretPosition;    // pozycja kursora

public:
    TextBox( string label = "", string content = "", size_t size = 0 );

    void SetPosition( int x, int y );
    void SetPosition( Vector2I vec );
    void SetWindow( WINDOW *window );
    void SetSize( size_t size );
    void SetStyle( int label, int text );
    string GetContent( void );

    void Focus( void );
    void Print( void );
};

#endif
