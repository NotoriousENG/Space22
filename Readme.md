# Shooty Shoot
A simple space shooter made in C using SDL based on [Parallel Realities's tutorials](https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php) with multi-platform build support.

## Playing
[Play/Download on itch.io](https://notoriouseng.itch.io/shooty-shoot)

## Build for Multiple Platforms
You can build for multiple platforms using CMake, you will need the following installed to link for your platform of choice
* SDL2
* SDL2-image
* SDL2-mixer
### Windows
* You can install dependencies using [vcpkg](https://github.com/microsoft/vcpkg)
    * for SDL_Mixer use ./vcpkg.exe install sdl2-mixer[libvorbis]
    * you may need to remove SDL_Mixer first, sdl2-mixer[dynamic-load] causes issues with cmake
* Open Project in Visual Studio (cmake support installed)
* Build
* Run SimpleGame.exe

### Linux / MacOS
* install dependencies using package manager i.e. apt, pacman, etc. for MacOS this would be [brew](https://brew.sh/)
```
mkdir build
cd build
cmake ..
make
./SimpleGame
```
### Web
```
mkdir build
cd build
emcmake cmake ..
make
emrun SimpleGame.html
```
