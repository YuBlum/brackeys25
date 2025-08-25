if [ "$#" -ne 1 ]; then
  echo "usage: ./build win|lin|web"
  exit 1
fi

NAME="game"
FLAGS="-Wall -Wextra -Wno-override-init -Werror -Wpedantic -std=c23"
DEF="-DDEV"
SRCS=$(find ./src/ -type f -name "*.c" ! -path "./src/engine/x64win.c" ! -path "./src/engine/x64lin.c" ! -path "./src/engine/wasm.c" | tr '\n' ' ')
LIBS="\
-L./vendor/glfw/ \
"
INCS="\
--embed-dir=./assets \
-I./vendor/glfw/ \
-I./include/ \
-I./vendor/glad/include \
-I./vendor/miniaudio \
"
RUN="y"

if [ "$1" = "win" ]; then
  DEF+=" -DWINDOWS"
  SRCS+=" ./src/engine/x64win.c"
  FLAGS+=" -Wno-override-init-side-effects"
  LIBS+="\
vendor/glad/win-glad.o \
vendor/miniaudio/win-miniaudio.o \
-static-libgcc \
-lwin-glfw3 \
-lgdi32 \
-static \
-lpthread \
"
  OUT="./builds/$NAME.exe"
  RUN="./$OUT"
  CC=x86_64-w64-mingw32-gcc
elif [ "$1" = "lin" ]; then
  DEF+=" -DLINUX"
  SRCS+=" ./src/engine/x64lin.c"
  FLAGS+=" -Wno-override-init-side-effects -ggdb"
  LIBS+="\
vendor/glad/lin-glad.o \
vendor/miniaudio/lin-miniaudio.o \
-llin-glfw3 \
-lm \
"
  OUT="./builds/$NAME"
  RUN="./$OUT"
  CC=gcc
elif [ "$1" = "web" ]; then
  DEF+=" -DWASM"
  SRCS+=" ./src/engine/wasm.c"
  LIBS+="\
vendor/miniaudio/web-miniaudio.o \
-s USE_GLFW=3 \
-s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s FULL_ES3=1 \
"
  OUT="./builds/web/$NAME.html"
  RUN="emrun --no_browser --port 8080 $OUT"
  CC=emcc
else
  echo "invalid target platform $1"
fi

if ! ./generate_systems.py; then
  echo "updating the atlas failed"
  exit 1
fi

if ! ./generate_atlas.py; then
  echo "updating the atlas failed"
  exit 1
fi

if ! $CC $FLAGS $DEF $SRCS $LIBS $INCS -o $OUT; then
  echo "compilation failed"
  exit 1
fi

$RUN
