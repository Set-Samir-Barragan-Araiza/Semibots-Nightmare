# Makefile para compilar el proyecto en bin/

CXX ?= g++
CXXFLAGS ?= -std=c++17 -Iinclude -Wall -g
LDFLAGS ?= -lsfml-graphics -lsfml-window -lsfml-system

ifeq ($(OS),Windows_NT)
EXE_EXT := .exe
else
EXE_EXT :=
endif

TARGET := bin/app$(EXE_EXT)
SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,bin/objs/%.o,$(SRCS))

MKDIR_P := mkdir -p
RM := rm -rf

.PHONY: all clean run dirs

all: $(TARGET)

dirs:
	$(MKDIR_P) bin bin/objs

bin/objs/%.o: src/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS) | dirs
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

run: all
	./$(TARGET)

clean:
	$(RM) bin
