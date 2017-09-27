# Cylinders (In Progress)

This program is designed to test if cylinders intersect.

## Compile
```
mkdir build
cd build
ccmake .. -DWITH_FPS=[ON|OFF] -DWITH_AA=[ON|OFF]
make
./exe
```

Note: `WITH_FPS` displays frame per second, default to `OFF`, `WITH_AA` enables anti aliasing, default to `ON`.

## Controls
Press `c` to create cylinder.<br />
Press `0` up to `9` to select cylinders (This will put program in OBJECT mode, press `tab` to switch back to SCENE mode)<br />
Press `backspace` to delete selected cylinders.<br />
Press `Delete` to delete all cylinders.<br />
Use `[`, `]` or scroll wheel to scale selected cylinders.<br />
Scroll wheel in general change the depth.<br />
Hold right button to translate selected cylinder.<br />
Hold left button to rotate selected cylinder.<br />

## Screen shots
![alt text](https://github.com/Guo-Haowei/Cylinders/blob/master/ScreenShot.png)

## Future
1. Read in and write out cylinders
2. Raytracing to pick cylinders
3. Raytracing for refraction effect
