# igneous
Igneous is an open source game engine written in C++.

## Environment Variables
The install directories for the OpenAL SDK and for Mono should be added as environment variables in your system. This is not necessary, but is a safeguard in the case that your system cannot find the install directories in one of the default search paths.

The OpenAL variable should be named 'OPENALDIR' exactly as written here without the quotes.
The Mono variable should be named 'MONO_ROOT' exactly as written here without the quotes.

## Checkout

```
git clone https://github.com/MissingBitStudios/igneous.git
cd igneous
git submodule update --init --recursive
mkdir build
cd build
cmake ..
```
