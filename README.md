# geowrapper
![Build Status](https://github.com/ipadjen/geowrapper/workflows/build/badge.svg)

geowrapper is the [3D Alpha Wrapping](https://inria.hal.science/hal-03688637) from [CGAL](https://www.cgal.org) wrapped into a simple application.

<p align="center">
  <img width="800" alt="wankel_cad" src="https://github.com/ipadjen/geowrapper/assets/49401914/39f04369-b4df-4938-b57a-c6718372b361">
</p>

It creates watertight and valid geometries from the input mesh. It fills holes and can even simplify geometries depending on wrapping parameters. Can be useful for CFD applications.

Also works with bad quality and missing data:

<p align="center">
  <img width="400" alt="wankel_cad" src="https://github.com/ipadjen/geowrapper/assets/49401914/3f8d461f-67b5-4c8c-a0f7-bfcf3aaa7b44">
</p>

More information about the algorithm and parameters can be found in [CGAL's documentation](https://doc.cgal.org/latest/Alpha_wrap_3/index.html).

## Getting started
Windows users can download and run the binary `geowrapper.exe` provided in the [release](https://github.com/ipadjen/geowrapper/releases/latest). Unix (Linux/MacOS) users have to build from source.

To run the tool, type the following:

```
geowrapper -i input_file.obj -alpha relative_alpha -offset relative_offset -o output_file.obj
```

Larger numbers result in better geometries, but with a time penalty. My suggestion is to start with around 500 for relative_alpha and 1200 for relative_offset and raise numbers until you are satisfied with the results. For quick checks and simplifications, try even lower values (e.g. 20-100 for alpha and 1000 for offset).

Allowed formats: STL, OBJ, PLY, VTP, OFF


## Building from source
#### Dependencies
CGAL >= 5.0

Linux - Ubuntu/Debian
```
sudo apt-get install libcgal-dev 
```

Mac - Homebrew
```
brew install cgal
```
#### Build
With installed dependencies, build the code with the following:
```
mkdir build && cd build
cmake ..
make
./geowrapper
```
