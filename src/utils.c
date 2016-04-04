#include <errno.h>
#include <stdlib.h>

#define bit_SSE41 0x00080000
#if defined(TARGET_COMPILER_MSVC)
    #include <intrin.h>
#else
    #include <cpuid.h>
#endif

#ifdef UNIX
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

#ifdef WINDOWS
    // We cant load windows.h as it also pulls in GDI headers, which
    // conflict with some of our own structs.
    #include <windef.h>
    #include <winbase.h>
    #include <wincon.h>
#endif

#include "utils.h"
#include "stamp.h"
#include "x86/x86.h"

// Initialize with the default c based algorithm.
void(*fill_triangle_implementation)(Canvas*, const Point, const Point,
    const Point, void(*set_pixel)(Canvas*, const int, const int)) = fill_triangle;

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

void init_library() {
    unsigned int registers[4] = {1,0,0,0};

#if defined(TARGET_COMPILER_MSVC)
    __cpuid((int*)registers, registers[0]);
#else
    unsigned int level = 0;
    __get_cpuid(level, registers, registers+1, registers+2, registers+3);
#endif

    if (registers[2] & bit_SSE41) {
        fill_triangle_implementation = fill_triangle_sse4;
    } else {
        fill_triangle_implementation = fill_triangle;
    }
}

void get_clamped_min_max(int a, int b, int c, const int clamp_min,
                         const int clamp_max, int *min, int *max) {
    if (a > b) {
        int tmp = a;
        a = b;
        b = tmp;
    }

    if (b > c) {
        int tmp = b;
        b = c;
        c = tmp;
    }

    if (a > b) {
        a = b;
    }

    *min = (clamp_min > a) ? clamp_min : a;
    *max = (clamp_max < c) ? clamp_max : c;
}
