CC = em++
SRC = src
OBJ = obj
skeleton_project.js : $(OBJ)/skeleton_main.bc
	em++ $(OBJ)/skeleton_main.bc \
	-O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 \
	--preload-file assets -s MODULARIZE=1 -s EXPORT_NAME='"SkeletonModule"' \
	-o ../node/public/SkeletonModule.js
skeleton_project : $(OBJ)/skeleton_main.o
	g++ $(OBJ)/skeleton_main.o -lSDL2 -lSDL2_image -lSDL2_ttf -O2 -o skeleton_project
$(OBJ)/skeleton_main.bc : $(SRC)/skeleton_main.cpp $(OBJ)
	em++ -s USE_SDL=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 -c $(SRC)/skeleton_main.cpp -o $(OBJ)/skeleton_main.bc
$(OBJ)/skeleton_main.o : $(SRC)/skeleton_main.cpp $(OBJ)
	g++ -c $(SRC)/skeleton_main.cpp -o $(OBJ)/skeleton_main.o
$(OBJ) :
	mkdir $(OBJ)
clean:
	rm -rf $(OBJ) ./skeleton_project ../node/public/SkeletonModule.wasm ../node/public/SkeletonModule.js ../node/public/SkeletonModule.data



