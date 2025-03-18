# Cerve - Serve with C

It's a simple HTTP server, for local workflows. Completely written in C.
I have built this as a replacement for the [serve npm package](https://www.npmjs.com/package/serve).

## Installation

Cerve is released on GitHub releases using tags. The releases includes pre-built binaries for Ubuntu (latest)
and MacOS (latest).
Windows binary is not included because cerve is not portable to Windows.

Here is the installation script. Run this after you have downloaded the binary.

```shell
# Extract the tar file
mkdir cerve-v0.1.0
tar -xf cerve-linux-v0.1.0.tar.gz -C cerve-v0.1.0

# Add cerve to PATH
mv cerve-v0.1.0/cerve ~/.local/bin/cerve
```

If you are on a older version of Ubuntu or MacOS or on  a different distro, the prebuild binaries might not work.
You will have to build the binaries yourselves. The instructions are provided below.

### Usage

`cerve help` will print the subcommands.

```
Cerve (0.1.0)
Serve with C

USAGE
   $ cerve help				- Prints this message
   $ cerve version			- Prints current version
   $ cerve [--listen PORT] folder_name	- Starts a HTTP server serving folder_name. Defaults to the cwd.

help and version commands don't take in any options.

OPTIONS FOR SERVING
   -l PORT, --listen PORT		 - Listens on PORT. Default 2000. Must be bigger than 1024.
```


## Development
### Prerequisites

These programs are required to build cerve. 

- [CMake](https://cmake.org/) v3.31.6
- Clang Format v18.1.3
 
### Build

```bash
mkdir build
cd build
cmake .. # if debug build
cmake -DCMAKE_BUILD_TYPE=Release .. # if release build
make
```

### Creating a Release

Cerve is released from a GitHub Workflow. The workflow is run whenever a tag is created in the form "v0.1.5".

## Authors

Sahithyan Kandathasan (https://sahithyan.dev)
