CC = em++
SRC = src/c++
skeleton_project.js : obj/skeleton_main.bc
	em++ obj/skeleton_main.bc \
	-O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 \
	-s EXPORTED_FUNCTIONS='["_mainWasm"]' --preload-file assets -s 'EXTRA_EXPORTED_RUNTIME_METHODS=["ccall", "cwrap"]' \
	-o skeleton_project.js
skeleton_project : obj/skeleton_main.o
	g++ obj/skeleton_main.o -O2 -o skeleton_project
obj/skeleton_main.bc : $(SRC)/skeleton_main.cpp obj
	em++ -s USE_SDL=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 -c $(SRC)/skeleton_main.cpp -o obj/skeleton_main.bc
obj/skeleton_main.o : $(SRC)/skeleton_main.cpp obj
	g++ -c $(SRC)/skeleton_main.cpp -o obj/skeleton_main.o
obj :
	mkdir obj
clean:
	rm -rf obj skeleton_project skeleton_project.wasm skeleton_project.js skeleton_project.data



