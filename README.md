# geowrapper
Prototype that uses CGAL's Alpha Wrap

## Usage
```
geowrapper input_file.obj relative_alpha[optional] relative_offset[optional]
```

Default value for relative_alpha is 20 and relative_offset is 300. Larger numbers result in better geometries, but with a time penalty. My suggestion is to start with around 500 for relative_alpha and 12000 for relative_offset and raise numbers until you are satisfied with the results.

Allowed input formats: STL, OBJ, PLY, VTP, OFF

Output format hardcoded to OBJ, file name is `input_file_wrapped.obj`

## Install
```
mkdir build && cd build
cmake ..
make
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
