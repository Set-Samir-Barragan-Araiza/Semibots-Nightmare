# Semibots Nightmare

![Fondo Pantalla Inicio](assets/images/FondoPantallaInicio.jpg)

Una pequeña demo/juego en C++ usando SFML donde controlas a un "semibot" por un fondo tipo chimenea. Este repositorio contiene el código fuente, recursos (sprites, sonidos) y utilidades para compilar y ejecutar la aplicación en Windows/Linux.

**Estado:** jugable — movimiento A/D, agacharse con Left Ctrl, animaciones de caminar y apoyo de sonido.

--

**Características principales**

- Movimiento lateral con teclas `A` / `D`.
- Agachado con `Left Ctrl`.
- Animación de caminar (dos frames) y sprite de idle.
- Escalado uniforme por altura objetivo para mantener proporciones.
- Superposición en pantalla con ayuda gráfica (`GamesButtons.png`) que indica los controles.
- Integración de efectos de sonido para pasos (si `assets/sound/footsteps.*` está presente).

--

**Requisitos**

- Compilador C++ compatible con C++17 (`g++`, `clang++`, MSVC).
- SFML (graphics, window, system, audio) instalada o disponible como DLLs en `bin/`.
- Make (opcional) para facilitar la compilación.

En Windows puedes usar MinGW/WSL/Visual Studio; en Linux instalar `libsfml-dev` vía tu gestor.

--

**Estructura del proyecto**

- `src/` — archivos fuente C++.
- `include/` — encabezados (clases: `Pantalla`, `Semibot`, `WalkAnimation`, `Direccion`, ...).
- `assets/` — recursos: `images/`, `fonts/`, `sound/`.
- `bin/` — binarios y objetos generados por la compilación.
- `scripts/` — helper scripts (copiar DLLs, etc.).

--

Cómo compilar y ejecutar

1) Desde un entorno POSIX (Linux, WSL, MinGW/MSYS):

```bash
make
./bin/app.exe   # en Windows con MinGW o en WSL
```

2) En Windows (cmd / Powershell) si `bin/app.exe` ya fue creado:

```powershell
.\bin\app.exe
```

Notas:
- Si faltan DLLs de SFML en Windows, ejecuta `sh scripts/copy_sfml_dlls.sh "" bin` desde MSYS/MinGW o copia manualmente las DLLs al directorio `bin/`.
- El Makefile está preparado para compilar los objetos en `bin/objs` y enlazar `bin/app.exe`.

--

Controles del juego

- Mover izquierda/derecha: `A` / `D`.
- Agacharse: `Left Ctrl`.
- En el menú de inicio: `Enter` para comenzar.

--

Assets importantes

- `assets/images/FondoChimenea.png` — fondo principal (usado como 'suelo').
- `assets/images/SemibotParadoDerecha.png`, `SemibotParadoIzquierda.png` — frames de idle.
- `assets/images/SemibotPaso1*.png`, `SemibotPaso2*.png` — frames de caminar.
- `assets/images/GamesButtons.png` — imagen explicativa de controles (mostrada sobre el semibot).
- `assets/sound/footsteps.wav` u `.ogg` — sonido de pasos (opcional).

Puedes reemplazar estas imágenes manteniendo los mismos nombres para que el código las detecte automáticamente. Si usas distintos nombres, actualiza las llamadas en `include/Direccion.hpp`.

--

Detalles técnicos y notas para desarrolladores

- `Semibot` gestiona texturas, animaciones y entrada. La escala se calcula por altura objetivo en píxeles (`setIdleSizePixels`) para evitar compresión horizontal.
- `WalkAnimation` es una animación simple de dos frames que recalcula origen y escala al cambiar de frame para mantener los pies alineados al suelo.
- La pantalla de ayuda ahora se dibuja sobre el semibot desde `Pantalla::run()` y desaparece después de unos segundos.

Depuración rápida

- Para ver mensajes de depuración temporales (si aún existen), ejecuta el binario desde una consola para ver la salida estándar.
- Si la app no inicia, verifica la presencia de las DLLs de SFML en `bin/` o instala SFML en tu sistema.

--

Contribuciones

1. Haz un fork del repositorio.
2. Crea una rama feature/bugfix.
3. Haz tus cambios y abre un pull request con descripción clara.

Por favor incluye capturas si el cambio afecta a la UI/animaciones.

--

Licencia

Incluye la licencia que desees aquí (MIT por defecto). Si quieres que añada un archivo `LICENSE`, dime cuál prefieres.

--

Contacto y Créditos

- Autor: Equipo de desarrollo (local)
- Recursos: las imágenes y sonidos están en la carpeta `assets/`.

--

¡Gracias por trabajar en este proyecto! Si quieres, puedo añadir ejemplos de screenshots, badges o un GIF corto mostrando gameplay.

