#include <errno.h>
#include <stdlib.h>

#ifdef UNIX
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

#ifdef WINDOWS
    #include <windows.h>
#endif

#include "utils.h"

bool get_console_size(size_t *width, size_t* height)
{
#ifdef UNIX
    errno = 0;

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    if (errno) {
        return false;
    }

    *width = (size_t) w.ws_col * 2;
    *height = (size_t) w.ws_row * 4;

    return true;

#elif defined WINDOWS
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    *width = (size_t) (csbi.srWindow.Right - csbi.srWindow.Left + 1) * 2;
    *height = (size_t) (csbi.srWindow.Bottom - csbi.srWindow.Top + 1) * 4;

    return true;

#else
    return false;
#endif
}
