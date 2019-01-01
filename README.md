# wasm_react_pwa_skeleton
Skeleton project for using SDL WebAssembly in a React project that can be used offline as a PWA

This project was created using https://lyceum-allotments.github.io/2016/06/emscripten-and-sdl2-tutorial-part-7-get-naked-owl/ as a base for the WebAssembly and using the create-react-app.

The current version of this can be found at https://cfogrady.github.io/wasm_react_pwa_skeleton/ as an installable app.

# Getting Started with Development
1. Make sure you have emscripten installed and working. Instructions can be found at: https://kripken.github.io/emscripten-site/docs/getting_started/Tutorial.html#tutorial
2. Install node (Recommend using nvm).
3. Navigate to `${projectRoot}/native` 
4. Run `make` which will compile the c++ code into webassembly and place the output in `${projectRoot}/node/public`.
5. Navigate to `${projectRoot}/node`
6. Run `npm start` to run locally (does not include service worker (pwa component)

*OR*
1. Make sure SDL2 is installed to your development environment
2. From the `${projectRoot}/native` directory you can run `make skeleton_project` which will compile the source into an executable binary which can be run natively.

## Significant Locations
### skeleton_project.cpp
This contains all of the native code. Of most significance are the items within `#ifdef __emscripten__` and `#endif` which handle items specific to our web assemly.
### WebAssemblyLoader.js
This file dynamically loads a web assembly module and runs it with the output tied to the canvas.
The meat of the code is in skeleton_project.cpp which handles code specific to emscripten and WebAssemblyLoader.js which loads that assembly to a canvas modal.
### index.html
The index.html is changed to register the serviceWorker making this a PWA.
### package.json
This is where our homepage is set. This is also where the sw-precache package has been added to the build script. This node package is what allows us to generate our own service-worker overriding the default one used by the react-scripts.
### sw-precache-config.js
This file is the template referenced in the package.json. Our SkeletonModule.* files (emscripten output) is marked here as staticFileGlobs, which allows us to fetch SkeletonModule.js, SkeletonModule.wasm, and SkeletonModule.data on install so our app can be used offline once installed.
### manifest.json
This file is the manifest used by Chrome on Desktops and Mobile to figure out the starting_url of the app, the icons, orientation, and how to open.


The docs directory is just a copy of the build directory from the last run. This is so that github pages can serve up our PWA. Everything in the project is currently configured for github pages. If changing to a different server or server location the start_url and homepage settings discussed above will need to be changed.

## Run in Production Mode (PWA component)
1. Remove the homepage in the package.json or change it to the correct path for your webserver.
2. Change the start_url in the manifest.json to "." or "/" if this is going to be the root of the webserver
3. Navigate to the `${projectRoot}/node` directory
3. Run `npm run build` to build the project.
4. Point a webserver to the newly created build directory or copy the build directory to a location for the webserver
*or*
4. Install serve `npm install -g serve` (-g installs globally) and run `serve -s build`.
