[![CMake](https://github.com/lyquid/ktpUtils/actions/workflows/cmake.yml/badge.svg)](https://github.com/lyquid/ktpUtils/actions/workflows/cmake.yml)
# ktpUtils

Utility functions and classes.

## colors.hpp
Contains a Color class in RGBA format from 0 to 1 per channel.

## libppm.hpp
A library to create [ppm](https://en.wikipedia.org/wiki/Netpbm) image files.

## object_pool.hpp
2 classes for storing arbitrary objects and improve data locality. One pool is indexed, which always tries to fill the first elements so you don't need to traverse the full pool. The other isn't.

## timer.hpp
A timer class useful for video games.
