# Cerve - Serve with C

It's a simple HTTP server, for local workflows. Completely written in C.
I have built this as a replacement for the [serve npm package](https://www.npmjs.com/package/serve).

## Prerequisites

These programs are required to build cerve. 

- [CMake](https://cmake.org/) v3.31.6
- [Cwalk library](https://github.com/likle/cwalk/tree/stable) v1.2.9
- Clang Format v18.1.3
 
## Development

```bash
mkdir build
cd build
cmake ..
make
```

The above commands builds the executable.

## Release

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

The above command builds the executable for release.

## Usage

After following the above steps, the executable is located at `build/cerve`.

`cerve help` will print the subcommands.

## Authors

Sahithyan Kandathasan (https://sahithyan.dev)
