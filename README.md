# wasm_react_pwa_skeleton
Skeleton project for using SDL WebAssembly in a React project that can be used offline as a PWA

This project was created using https://lyceum-allotments.github.io/2016/06/emscripten-and-sdl2-tutorial-part-7-get-naked-owl/ as a base for the WebAssembly.

# This is still a WIP
TODO: Make PWA

# Getting Started
1. Make sure you have emscripten installed and working. Instructions can be found at: https://kripken.github.io/emscripten-site/docs/getting_started/Tutorial.html#tutorial
2. Install node (Recommend using nvm).
3. Navigate to `${projectRoot}/native` 
4. Run `make` which will compile the c++ code into webassembly and place the output in `${projectRoot}/node/public`.
5. Navigate to `${projectRoot}/node`
6. Run `npm start`

*OR*
1. Make sure SDL2 is installed to your development environment
2. From the `${projectRoot}/native` directory you can run `make skeleton_project` which will compile the source into an executable binary which can be run natively.

The meat of the code is in skeleton_project.cpp which handles code specific to emscripten and WebAssemblyLoader.js which loads that assembly to a canvas modal.
