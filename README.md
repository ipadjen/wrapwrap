# geowrapper
[3D Alpha Wrapping](https://inria.hal.science/hal-03688637) from [CGAL](https://www.cgal.org) wrapped into a small application.

It creates watertight and valid geometries from the input mesh.

More information about the algorithm and parameters can be found in [CGAL's documentation](https://doc.cgal.org/latest/Alpha_wrap_3/index.html).

## Usage
```
geowrapper -i input_file.obj -alpha relative_alpha -offset relative_offset -o output_file.obj
```

Larger numbers result in better geometries, but with a time penalty. My suggestion is to start with around 500 for relative_alpha and 1200 for relative_offset and raise numbers until you are satisfied with the results. For quick checks, try even lower values (e.g. 100 for alpha and 200 for offset).

Allowed formats: STL, OBJ, PLY, VTP, OFF

## Install
```
mkdir build && cd build
cmake ..
make
./geowrapper
```

## Dependencies
CGAL >= 5.0

Linux - Ubuntu/Debian
```
sudo apt-get install libcgal-dev 
```

Mac - Homebrew
```
brew install cgal
```
