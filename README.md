# KojakSpectrumViewer
MS/MS spectral search results viewer. Applicable for any search results in pepXML. Specializes in cross-linked search results.

## Install

The software works great, but this repo is a work in progress. Compilation may be a little rough. If all you know how to do is: 
```
configure
make
make install
```
then maybe you should ask for help from someone who better knows their way around a computer. :)

In Linux: There is a Makefile, you can try to make right from the outset. Just be sure to do the following:

1. Clone and compile the MSToolkit: https://github.com/mhoopmann/mstoolkit
2. Install SDL and dev kit: https://www.libsdl.org/
3. Install SDL_ttf and dev kit: https://www.libsdl.org/projects/SDL_ttf/
4. Install PNG and dev kit: http://www.libpng.org/pub/png/libpng.html
5. Update the Makefile MSTOOLKITPATH

In Windows: Much the same as above, if using a Linux-like environment (MinGW/MSYS/CygWin etc). 
If using Visual Studio (works great this way), try the following:

1. Clone and compile the MSToolkit as a library.
2. Download and compile the libpng as a library.
3. Download the precompiled SDL dev and runtime libraries.
4. Download the precompiled SDL_ttf dev and runtime libraries.
5. Compile all .cpp source and link the MSToolkit,libpng, and SDL libraries.
6. Add the SDL dlls to the binary folder prior to running KojakSpectrumViewer.

I apologize in advance for the lack of user friendliness to the entire process. 
Over time I plan on improving this repo to reduce the complexity for compilation and automate
more of the processes.

## Binaries

Of course, if you just want the binaries, just go to: http://kojak-ms.org

## License

KojakSpectrumViewer code is available under the Apache License, Version 2.0. SDL_SavePNG code retains its zlib/libpng license.
