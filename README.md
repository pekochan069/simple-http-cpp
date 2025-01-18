# Simple http server using socket

## Build project

### Requirements

[clang](https://clang.llvm.org/) 19+

### Build step

1 Clone repo

```sh
git clone https://github.com/pekochan069/simple-http-cpp
```

2 Build nobpp

```sh
clang++ ./build.cpp -O3 -o build.exe # Windows
clang++ ./build.cpp -O3 -o build     # Linux (Currently not supported)
```

3 Build project

```sh
./build
```

4 Run project

```sh
./build/main
```
