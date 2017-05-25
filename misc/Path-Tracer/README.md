# Path Tracer

Simple Monte Carlo path tracer in C++.

## Features
 - Mesh Rendering
 - Obj Model Import
 - Textures (PNG only at the moment)
 - Simple naive KD tree for meshes.
 - Diffuse, specular, and emission material types
 
## Compiling and Running
Requires cmake and OpenMP for multithreading.

Compile:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```
Run:
```
$ ./pathtracer <number of samples>
```
This will render the Stanford Dragon scene which is included.
Take a look at src/main.cpp to see how to create a scene amd import objs.

