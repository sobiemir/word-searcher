#ifndef __WSH_CONFIGURATION__
#define __WSH_CONFIGURATION__

// sprawdź z jakim systemem program ma do czynienia
#ifdef _WIN32
#   define MSD_SYSTEM_WINDOWS
#elif defined __linux__
#   define MSD_SYSTEM_LINUX
#endif

template <typename T> struct Vector2
{
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
};

#endif
