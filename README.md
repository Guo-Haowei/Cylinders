# Cylinders (In Progress)

This program is designed to test if cylinders intersect.

## Installation

### MacOS
If you already have `brew` installed, skip this step. Otherwise, do
```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```
to install `brew`.

After installing `brew`, you need to download a few more packages.

```
brew install cmake
brew install pkg-config
brew install glew
brew install glfw
brew install glm
```
### Linux
Similar to `brew install`, do `apt-get` to download desired packages.
```
sudo apt-get update
sudo apt-get install cmake
sudo apt-get install pkgconf
sudo apt-get install libglfw-dev
sudo apt-get install libglew-dev
sudo apt-get install libglm-dev
```
If you do not have GLFW version 3, download the source distribution and run cmake:
```
wget https://github.com/glfw/glfw/releases/download/3.1.2/glfw-3.1.2.zip
unzip glfw-3.1.2.zip
cd glfw*
mkdir build
cd build
ccmake ..
```

### Compile Code

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
Press `0` up to `9` to select cylinders (Or simply double click on the cylinder)<br />
Press `backspace` to delete selected cylinders.<br />
Press `Delete` to delete all cylinders.<br />
Press `o` to output current cylinder data.<br />
Press `m` to print manual to console.<br />
Press `f` to turn on/off circles.<br />
Use `[`, `]` or scroll wheel to scale selected cylinders.<br />
Scroll wheel in general change the depth.<br />
Hold right button to translate selected cylinder.<br />
Hold left button to rotate selected cylinder.<br />

## Screen shots
![alt text](https://github.com/Guo-Haowei/Cylinders/blob/master/ScreenShot.png)

## Features
* Phong shading
* Trackball rotation to rotate both scene and cylinders
* Use unionfind data structure to label intersected cylinders with the same color
* Color picking cylinders (Considering using ray tracing later on)
* Hightlight the border of the selected cylinder(Object outlining)

## Future
* Raytracing for reflaction and refraction effect
