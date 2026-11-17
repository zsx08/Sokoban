#include "game.h"
#include <stdio.h>

#ifdef _WIN32
#include <conio.h>

static int readKey(void) {
    int ch = _getch();
    if (ch == 0 || ch == 0xE0) {
        return 256+_getch();
    }
    return ch;
}

#else
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#define ESCAPE_KEY 27
#define KEY_SEQUENCE_TIMEOUT_MS 100

static struct termios originalTerminal;
static volatile sig_atomic_t terminalChanged = 0;
static int terminalInitialized = 0;
static int pendingKey = EOF;

/* 正常退出和信号退出都要恢复回显及按行输入，避免影响后续终端使用。 */
static void restoreTerminal(void) {
    if (terminalChanged) {
        int result;
        do {
            result = tcsetattr(STDIN_FILENO, TCSANOW, &originalTerminal);
        } while (result == -1 && errno == EINTR);
        if (result == 0) terminalChanged = 0;
    }
}

static void handleTerminalSignal(int signalNumber) {
    restoreTerminal();
    _exit(128 + signalNumber);
}

static int prepareTerminal(void) {
    struct termios inputMode;
    if (!terminalInitialized) {
        if (!isatty(STDIN_FILENO) ||
            tcgetattr(STDIN_FILENO, &originalTerminal) == -1) {
            fprintf(stderr, "Please run in an interactive terminal.\n");
            return 0;
        }
        if (atexit(restoreTerminal) != 0 ||
            signal(SIGINT, handleTerminalSignal) == SIG_ERR ||
            signal(SIGTERM, handleTerminalSignal) == SIG_ERR ||
            signal(SIGQUIT, handleTerminalSignal) == SIG_ERR) {
            fprintf(stderr, "Failed to configure terminal cleanup.\n");
            return 0;
        }
        terminalInitialized = 1;
    }
    if (terminalChanged) return 1;

    inputMode = originalTerminal;
    /* 关闭按行输入和回显；保留 Ctrl+C 等信号处理。 */
    inputMode.c_lflag &= ~(ICANON | ECHO | ECHONL);
    inputMode.c_cc[VMIN] = 1;
    inputMode.c_cc[VTIME] = 0;
    terminalChanged = 1;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &inputMode) == -1) {
        restoreTerminal();
        fprintf(stderr, "Failed to configure single-key input.\n");
        return 0;
    }
    return 1;
}

static int readByte(void) {
    unsigned char ch;
    ssize_t count;
    do {
        count = read(STDIN_FILENO, &ch, 1);
    } while (count == -1 && errno == EINTR);
    return count == 1 ? ch : EOF;
}

/* 只在消费特殊键序列时短暂等待，单独按 Esc 不会一直卡住。 */
static int readSequenceByte(void) {
    fd_set input;
    struct timeval timeout;
    int ready;
    do {
        FD_ZERO(&input);
        FD_SET(STDIN_FILENO, &input);
        timeout.tv_sec = 0;
        timeout.tv_usec = KEY_SEQUENCE_TIMEOUT_MS * 1000;
        ready = select(STDIN_FILENO + 1, &input, NULL, NULL, &timeout);
    } while (ready == -1 && errno == EINTR);
    return ready > 0 ? readByte() : EOF;
}

static int readKey(void) {
    int ch;
    if (!prepareTerminal()) return EOF;
    if (pendingKey != EOF) {
        ch = pendingKey;
        pendingKey = EOF;
    } else {
        ch = readByte();
    }
    if (ch == EOF || ch == 4) {
        /* 非按行模式下 Ctrl+D 是普通字符，将它作为退出处理。 */
        restoreTerminal();
        return EOF;
    }
    if (ch == ESCAPE_KEY) {
        int prefix = readSequenceByte();
        if (prefix == '[' || prefix == 'O') {
            /* 消费方向键和功能键的序列，不把末尾的 A/D 当作移动。 */
            do {
                ch = readSequenceByte();
            } while (ch != EOF && (ch < 0x40 || ch > 0x7E));
        } else if (prefix != EOF) {
            /* Esc 后若是普通按键，留给下一次调用，不清空输入缓冲区。 */
            pendingKey = prefix;
        }
        return 0;
    }
    return ch;
}
#endif
//读取输入
int readCommand(void) {
    int ch = readKey();
    if (ch == EOF) return 'q';
    if (ch >= 'A' && ch <= 'Z') ch = ch - 'A' + 'a';
    if (ch == 'w' || ch == 'a' || ch == 's' || ch == 'd' ||
        ch == 'r' || ch == 'q' || ch == ' ' || ch == 27  ||
        ch == 'n' || ch == 331 || ch == 333) {
        return ch;
    }
    return '\0';
}
