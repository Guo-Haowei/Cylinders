# Cylinders (In Progress)

This program is designed to test if cylinders intersect.

## Compile
```
mkdir build
cd build
ccmake ..
make
./exe [-r] [path/to/cylinder/file]
```

Note: <br />
&nbsp;&nbsp;Run `./exe -r` will load file `cylinders.txt` by default. <br />

## Controls
Press `c` to create cylinder.<br />
Press `0` up to `9` to select cylinders (Or simply left click on the cylinder)<br />
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
* Trackball rotation to rotate both camera and cylinders
* Use unionfind data structure to label intersected cylinders with the same color
* Color picking cylinders (Considering using ray tracing later on)

# Issues
* Trackball rotation doesn't work properly
* GLM library sometimes gives nan matrix and vector

## Future
* Hightlight the border of the selected cylinder(Object outlining)
* Raytracing for reflaction and refraction effect
