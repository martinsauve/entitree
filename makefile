CFLAGS += -I./raylib/include -L./raylib/lib -Wall -Wextra -Wpedantic -O3 --std=c11
STATIC_LFLAGS += -l:libraylib.a -lm
LFLAGS += -lraylib -lm


static: main.c makefile dl
	gcc $(CFLAGS) main.c -o invaders $(STATIC_LFLAGS)

invaders: main.c makefile dl
	gcc $(CFLAGS) main.c -o invaders $(LFLAGS)

web: main.c makefile
	/home/martin/sourcecode/emsdk/upstream/emscripten/emcc -o game.html main.c -Os -Wall /home/martin/sourcecode/raylibWeb/src/libraylib.web.a -I/home/martin/sourcecode/raylibWeb/src -Iraylib/include -s USE_GLFW=3 -s ASYNCIFY --shell-file /home/martin/sourcecode/raylibWeb/src/minshell.html -DPLATFORM_WEB


dl: download-raylib.sh
	@if [ ! -d "raylib" ]; then \
		./download-raylib.sh;\
		fi

linter:
	bear -- make -B static

run: static
	./invaders
clean:
	rm -f invaders
	rm -rf raylib
