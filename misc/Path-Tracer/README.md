# Overview 

Simple path tracer in C++. It ports ForLoop multi-threads and progress reporter from pbrt-v3.



## Features
 - Triangle Mesh 
 - Obj Model Formart
 - Textures (PNG only at the moment)
 - KD tree accelarator from pbrt for mesh.
 - Diffuse, specular, and emission material types
 - Multi-Threading
 - solve 3x3 matrix linear equation
 
## Defects

- Ray and Shape intersection test is done in world space.
- Only sphere and triangle are supported.
- not support refraction yet

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
$ ./pathtracer <number-of-samples>
```
This will render the Stanford Dragon scene which is included.
Take a look at src/main.cpp to see how to create a scene amd import objs.

## Intersection test

The test happens in world space.

### Ray-Sphere test
-----

![ray-sphere](./ray-sphere.png)

### Ray-Triangle test
-----

![ray-triangle](./ray-triangle.png)

### solve 3x3 matrix linear equation
-----

![solve equation](./solve3x3MatrixEquation.png)

## Reference

- [pbrt book v3]() 
- [fundamental of computer graphics]()
