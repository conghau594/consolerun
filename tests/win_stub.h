#ifndef WIN_STUB_H
#define WIN_STUB_H
#include <stdint.h>
#include <unistd.h>

typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned int UINT;
typedef int INT;
typedef struct { short X; short Y; } COORD;
typedef struct { COORD dwSize; } CONSOLE_SCREEN_BUFFER_INFO;
typedef struct { unsigned long cbSize; short nFont; COORD dwFontSize; int FontFamily; int FontWeight; wchar_t FaceName[32]; } CONSOLE_FONT_INFOEX;
typedef struct { unsigned long dwSize; BOOL bVisible; } CONSOLE_CURSOR_INFO;
typedef struct { short Left; short Top; short Right; short Bottom; } SMALL_RECT;
typedef void* HANDLE;
typedef void* HWND;

#define STD_INPUT_HANDLE 0
#define STD_OUTPUT_HANDLE 1

static inline HANDLE GetStdHandle(int x) { return (HANDLE)0; }
static inline HWND GetConsoleWindow(void) { return (HWND)0; }
static inline int GetCurrentConsoleFontEx(HANDLE h, BOOL b, CONSOLE_FONT_INFOEX *c) { return 0; }
static inline int SetCurrentConsoleFontEx(HANDLE h, BOOL b, CONSOLE_FONT_INFOEX *c) { return 0; }
static inline int SetConsoleCursorInfo(HANDLE h, const CONSOLE_CURSOR_INFO *c) { return 0; }
static inline int SetConsoleScreenBufferSize(HANDLE h, COORD c) { return 0; }
static inline int SetConsoleWindowInfo(HANDLE h, int abs, const SMALL_RECT *r) { return 0; }
static inline long SetWindowLong(HWND hwnd, int idx, long newLong) { return 0; }
static inline long GetWindowLong(HWND hwnd, int idx) { return 0; }
static inline void* GetSystemMenu(HWND hwnd, int b) { return 0; }
static inline int DeleteMenu(void* menu, int pos, int flags) { return 0; }
static inline int ShowScrollBar(HWND hwnd, int bar, BOOL show) { return 0; }
static inline int SetConsoleMode(HANDLE h, int mode) { return 0; }
static inline int SetConsoleTitleW(const wchar_t* title) { return 0; }
static inline void FlushConsoleInputBuffer(HANDLE h) { (void)h; }
static inline short GetKeyState(int key) { return 0; }

#define VK_DOWN 0
#define GWL_STYLE 0
#define WS_SIZEBOX 0
#define SC_MAXIMIZE 0
#define MF_BYCOMMAND 0
#define SB_BOTH 0

static inline void Sleep(int ms) { usleep(ms * 1000); }

#endif /* WIN_STUB_H */
