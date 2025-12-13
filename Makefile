# Makefile para compilar el proyecto en bin/

CXX ?= g++
CXXFLAGS ?= -std=c++17 -Iinclude -Wall -g
LDFLAGS ?= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

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

# Ruta al directorio donde están las DLLs de SFML (configurable)
# Ejemplo en Windows: C:/SFML/bin
SFML_BIN ?=
# Comando para copiar (usa cp por compatibilidad con MSYS/MinGW)
CP ?= cp -u

.PHONY: all clean run dirs

all: $(TARGET) copy-dlls

dirs:
	$(MKDIR_P) bin bin/objs

bin/objs/%.o: src/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS) | dirs
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

run: all
	./$(TARGET)

# Copiar DLLs de SFML a bin/ para facilitar la ejecución en Windows
.PHONY: copy-dlls
copy-dlls: | dirs
	# Delegar la detección y la copia a un script auxiliar para evitar complejidad en Makefile
	sh scripts/copy_sfml_dlls.sh "$(SFML_BIN)" bin || true

clean:
	$(RM) bin
