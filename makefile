CFLAGS += -I./raylib/include -L./raylib/lib -Wall -Wextra -Wpedantic -O3 --std=c11
STATIC_LFLAGS += -l:libraylib.a -lm
LFLAGS += -lraylib -lm
BUILD_DIR += build

build/cJSON.o: ./third_party/cJSON/cJSON.c
	gcc -o $@ -c $<

build/uuid4.o: ./third_party/uuid4/src/uuid4.c
	gcc -o $@ -c $<

build/test: src/test.c src/entity.c src/entity.h build/uuid4.o build/cJSON.o
	gcc -o $@ $^ && ./build/test

#static: main.c makefile dl
#	gcc $(CFLAGS) main.c -o invaders $(STATIC_LFLAGS)
#
#invaders: main.c makefile dl
#	gcc $(CFLAGS) main.c -o invaders $(LFLAGS)
#
#web: main.c makefile
#	/home/martin/sourcecode/emsdk/upstream/emscripten/emcc -o game.html main.c -Os -Wall /home/martin/sourcecode/raylibWeb/src/libraylib.web.a -I/home/martin/sourcecode/raylibWeb/src -Iraylib/include -s USE_GLFW=3 -s ASYNCIFY --shell-file /home/martin/sourcecode/raylibWeb/src/minshell.html -DPLATFORM_WEB
#
#
#dl: download-raylib.sh
#	@if [ ! -d "raylib" ]; then \
#		./download-raylib.sh;\
#		fi

linter:
	bear -- make -B build/test

#run: static
#	./invaders
#clean:
#	rm -f invaders
#	rm -rf raylib
