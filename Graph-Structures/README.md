### About

This repo contains some boilerplate code for graph representations


### Code:
Edit main.cpp

### Compile: 
```
meson ./build # First time only
ninja -C ./build
```

### Structures
+ structures - structures code
+ tests - gunit tests
+ main.cpp - example implementation, will generate executable `main`
+ build, subprojects, third_party - ignore

### Dependencies installation:
```
cd build
meson wrap install gtest
```

### Run tests:
```
ninja -C ./build test
```

### Before commiting, format and test:
```
ninja -C ./build format
ninja -C ./build tidy
ninja -C ./build test
```