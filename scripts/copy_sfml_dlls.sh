#!/usr/bin/env sh
# Script simple para copiar DLLs de SFML a un destino.
# Uso: copy_sfml_dlls.sh [SFML_BIN_HINT] [DEST_DIR]

SFML_HINT="$1"
DEST="$2"

CP="cp -u"

do_copy() {
    src="$1"
    if [ -d "$src" ]; then
        echo "Copiando DLLs desde $src a $DEST"
        $CP "$src"/*.dll "$DEST" 2>/dev/null || true
        exit 0
    fi
}

if [ -n "$SFML_HINT" ] && [ -d "$SFML_HINT" ]; then
    do_copy "$SFML_HINT"
fi

# Rutas comunes
COMMONS="/c/SFML/bin C:/SFML/bin /c/Program\ Files/SFML/bin /c/Program\ Files\ \(x86\)/SFML/bin C:/Program\ Files/SFML/bin C:/Program\ Files\ \(x86\)/SFML/bin"
for p in $COMMONS; do
    if [ -d "$p" ]; then
        do_copy "$p"
    fi
done

# Buscar en PATH (separadores ':' o ';')
echo "$PATH" | tr ':' '\n' | while read d; do
    if [ -d "$d" ] && ls "$d" 2>/dev/null | grep -i sfml-graphics >/dev/null 2>&1; then
        do_copy "$d"
    fi
done

echo "$PATH" | tr ';' '\n' | while read d; do
    if [ -d "$d" ] && ls "$d" 2>/dev/null | grep -i sfml-graphics >/dev/null 2>&1; then
        do_copy "$d"
    fi
done

echo "No se encontró directorio con DLLs de SFML; omitiendo copia"
exit 0
