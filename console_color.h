#ifndef CONSOLE_COLOR_H
#define CONSOLE_COLOR_H

#ifndef ENABLE_COLOR
#define ENABLE_COLOR 1
#endif


typedef enum {
    COLOR_DEFAULT,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN
} ConsoleColor;

void setTextColor(ConsoleColor color);
void resetTextColor(void);
void printColoredText(const char *text, ConsoleColor color);
void printColoredChar(char symbol, ConsoleColor color);

#endif
