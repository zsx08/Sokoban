# 使用 GNU Make；Windows 可使用 MinGW-w64 提供的 mingw32-make。
.DEFAULT_GOAL := all
.DELETE_ON_ERROR:

SOURCES := main.cpp game.cpp input.cpp render.cpp console_color.cpp
HEADERS := game.h console_color.h

ifeq ($(OS),Windows_NT)
    EXEEXT := .exe
    DEFAULT_CXX := g++
else
    EXEEXT :=
    ifeq ($(shell uname -s),Darwin)
        DEFAULT_CXX := clang++
    else
        DEFAULT_CXX := g++
    endif
endif

# 替换 Make 内置的 CXX 默认值，保留环境变量和命令行指定的编译器。
ifeq ($(origin CXX),default)
    CXX := $(DEFAULT_CXX)
endif
CXX ?= $(DEFAULT_CXX)
CXXFLAGS ?= -std=c++11 -Wall -Wextra -g -O0

TARGET := sokoban$(EXEEXT)

# MinGW 的原生 cmd 和 MSYS/Git Bash 使用不同的运行、删除命令。
ifneq ($(filter cmd cmd.exe CMD CMD.EXE,$(notdir $(subst \,/,$(SHELL)))),)
    RUN_COMMAND = .\$(TARGET)
    CLEAN_COMMAND = if exist "$(TARGET)" del /Q "$(TARGET)"
else
    RUN_COMMAND = ./$(TARGET)
    CLEAN_COMMAND = $(RM) "$(TARGET)"
endif

.PHONY: all run clean

all: $(TARGET)

# 小型项目直接编译并链接；任一源文件、头文件或 Makefile 改动都会重建。
$(TARGET): $(SOURCES) $(HEADERS) Makefile
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(SOURCES) -o "$@" $(LDLIBS)

# 编译后直接运行，示例关卡已内置，不依赖外部数据文件。
run: $(TARGET)
	$(RUN_COMMAND)

clean:
	$(CLEAN_COMMAND)

