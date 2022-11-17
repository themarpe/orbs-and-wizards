# Orbs And Wizards

C++ based game using SDL2 library for graphics and input handling

Modern port. Originally done in 2015/2016

## Credits

Font: Return To Castle (https://www.dafont.com/return-to-castle.font)

Artwork: https://opengameart.org (and a couple of other sources)


![In-Game Screenshot](screens/screenshot1.png)

## Building

First make sure that all submodules are cloned

```
git submodule update --init --recursive
```

Then build
```
cmake -S . -B build
cmake --build build --config Debug --parallel
```

To run using development resources
```
# Linux/macOS
./build/OrbsAndWizards
# Windows
./build/Debug/OrbsAndWizards.exe
```

## Package

To create a package to release/share, build first and then install with below command
```
cmake --build build --config Release --target install
```

Resulting game can be found in `build/install/OrbsAndWizards` folder
