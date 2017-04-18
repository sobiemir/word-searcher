#include "interface.hpp"

Interface::Interface( void )
{
    this->Folder = TextBox( "Folder : ", "./", 0 );
    this->Phrase = TextBox( "Fraza  : ", ""  , 0 );
    this->Filter = TextBox( "Filtr  : ", "*" , 0 );
    
    this->MainWindow = this->SearchWindow =
    this->HelpWindow = this->ResultWindow = NULL;

    this->TextStyle[0] = this->TextStyle[1] =
    this->TextStyle[2] = this->TextStyle[3] = 0;
}

void Interface::InitColors( void )
{
    int colors    = has_colors();
    int defcolors = (use_default_colors() != ERR);

    if( !colors )
        return;

    start_color();

    if( defcolors )
    {
        init_pair( 1, COLOR_CYAN, -1 );
        init_pair( 2, COLOR_YELLOW, -1 );
        init_pair( 3, COLOR_BLACK, COLOR_WHITE );
        init_pair( 4, COLOR_BLACK, COLOR_YELLOW );
    }
    else
    {
        init_pair( 1, COLOR_CYAN, COLOR_BLACK );
        init_pair( 2, COLOR_YELLOW, COLOR_BLACK );
        init_pair( 3, COLOR_BLACK, COLOR_WHITE );
        init_pair( 4, COLOR_BLACK, COLOR_YELLOW );
    }

    this->TextStyle[0] = COLOR_PAIR(1);
    this->TextStyle[1] = COLOR_PAIR(2);
    this->TextStyle[2] = COLOR_PAIR(3);
    this->TextStyle[3] = A_BOLD | COLOR_PAIR(4);
    
    this->Folder.SetStyle( this->TextStyle[0], 0 );
    this->Phrase.SetStyle( this->TextStyle[0], 0 );
    this->Filter.SetStyle( this->TextStyle[0], 0 );
}

void Interface::TerminalResize( void )
{
    this->DestroyWindows();

    if( !(this->MainWindow = newwin(LINES, COLS, 0, 0)) )
        exit( EXIT_FAILURE );

    keypad( this->MainWindow, TRUE );

    this->Folder.SetSize( COLS - 2 );
    this->Folder.SetWindow( this->MainWindow );
    this->Folder.SetPosition( 1, 1 );

    this->Phrase.SetSize( COLS - 2 );
    this->Phrase.SetWindow( this->MainWindow );
    this->Phrase.SetPosition( 1, 2 );

    this->Filter.SetSize( COLS - 2 );
    this->Filter.SetWindow( this->MainWindow );
    this->Filter.SetPosition( 1, 3 );

    wattron( this->MainWindow, this->TextStyle[1] );
    wborder(
        this->MainWindow,
        ACS_VLINE,    ACS_VLINE,
        ACS_HLINE,    ACS_HLINE,
        ACS_ULCORNER, ACS_URCORNER,
        ACS_LLCORNER, ACS_LRCORNER
    );

    mvwprintw( this->MainWindow, 0, COLS - 23, " Interface v0.2.0 " );
    wattroff( this->MainWindow, this->TextStyle[1] );

    this->Folder.Print();
    this->Phrase.Print();
    // this->Filter.Print();

    mvwhline( this->MainWindow, 3, 1, ACS_HLINE, COLS - 2 );
    mvwhline( this->MainWindow, (LINES - 3) / 2 + 2, 1, ACS_HLINE, COLS - 2 );

    wattron( this->MainWindow, this->TextStyle[2] );
    mvwprintw( this->MainWindow, ((LINES - 3) / 2) / 2 + 3, COLS / 2 - 9, " [^H] Okno pomocy " );
    wattroff( this->MainWindow, this->TextStyle[2] );

    wrefresh( this->MainWindow );
}

void Interface::DestroyWindows( void )
{
    if( this->MainWindow )
        delwin( this->MainWindow );
    if( this->SearchWindow )
        delwin( this->SearchWindow );
    if( this->ResultWindow )
        delwin( this->ResultWindow );
    if( this->HelpWindow )
        delwin( this->HelpWindow );
    
    this->MainWindow = this->SearchWindow =
    this->HelpWindow = this->ResultWindow = NULL;
}
