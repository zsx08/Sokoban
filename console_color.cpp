#include "console_color.h"
#include <stdio.h>

/* ANSI 协议中的 ESC 字符，控制序列统一封装在本文件中。 */
#define ANSI_ESCAPE_CHARACTER 27

void setTextColor(ConsoleColor color) {
    int code;
    if (!ENABLE_COLOR) return;
    switch (color) {
        case COLOR_RED:     code = 31; break;
        case COLOR_GREEN:   code = 32; break;
        case COLOR_YELLOW:  code = 33; break;
        case COLOR_BLUE:    code = 34; break;
        case COLOR_MAGENTA: code = 35; break;
        case COLOR_CYAN:    code = 36; break;
        default:            code = 0; break;
    }

    /* 输出设置文字颜色的 ANSI 指令 */
    printf("%c[%dm", ANSI_ESCAPE_CHARACTER, code);
}

void resetTextColor(void) {
    setTextColor(COLOR_DEFAULT);
}

void printColoredText(const char *text, ConsoleColor color) {
    setTextColor(color);
    printf("%s", text);
    resetTextColor();
}

void printColoredChar(char symbol, ConsoleColor color) {
    setTextColor(color);
    putchar(symbol);
    resetTextColor();
}
