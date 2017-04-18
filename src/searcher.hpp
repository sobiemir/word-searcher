#ifndef __WSH_SEARCHER__
#define __WSH_SEARCHER__

#include <vector>
#include <string>

using namespace std;

class Searcher
{
private:
    

public:
    Searcher( string folder = "./", string phrase = "", string filter = "*" );

    void Run( void );
};

#endif
