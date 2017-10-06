# Cylinders (In Progress)

This program is designed to test if cylinders intersect.

## Compile
```
mkdir build
cd build
ccmake .. -DWITH_FPS=[ON|OFF] -DWITH_AA=[ON|OFF]
make
./exe [1|2|3|...]
```

Note: <br />
&nbsp;&nbsp;`WITH_FPS` displays frame per second, default to `OFF`, `WITH_AA` enables anti aliasing, default to `ON`. <br />
&nbsp;&nbsp;Run `./exe` with optional argument `-r` will load file `cylinders.txt`. <br />

## Controls
Press `c` to create cylinder.<br />
Press `0` up to `9` to select cylinders (This will put program in OBJECT mode, press `tab` to switch back to SCENE mode)<br />
Press `backspace` to delete selected cylinders.<br />
Press `Delete` to delete all cylinders.<br />
Press `o` to output current cylinder data<br />
Use `[`, `]` or scroll wheel to scale selected cylinders.<br />
Scroll wheel in general change the depth.<br />
Hold right button to translate selected cylinder.<br />
Hold left button to rotate selected cylinder.<br />

## Screen shots
![alt text](https://github.com/Guo-Haowei/Cylinders/blob/master/ScreenShot.png)

## Features
* Trackball rotation
* Use unionfind data structure to label intersected cylinders with same color

# Issues
* Cylinder disappears occasionally when rotating
* glm::lookAt function is not perfect for this program
* cylIntersect raises "Not a point/vector problem"

## Future
* Color picking for selecting cylinders
* Hightlight the border of the selected cylinder
* Raytracing for refraction effect
