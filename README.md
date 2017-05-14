# SFML Sample Code

* Sample sfml code to be used as a compilation test: [sfml_sample.cpp](https://raw.githubusercontent.com/Ohohcakester/gdg-sfml-workshop/master/compilation_tests/sample_game.cpp?token=AE989UeWcuWW9S6mQSNjnEadbe22VENZks5XUCg6wA%3D%3D)

Expected Result:

![sfml_sample](images/sfml_sample.gif)

# Compiling with SFML

SFML Download Page (Download the binary corresponding to your compiler, else it won't work)

* [SFML Download](https://www.sfml-dev.org/download/sfml/2.4.2/)

The official setup instructions for SFML can be found here.

* [SFML Tutorial (Setup)](https://www.sfml-dev.org/tutorials/2.4/)

NOTE: Don't try to compile the SFML source code yourself, especially if you don't know what you are doing. Download a precompiled binary from the [downloads](https://www.sfml-dev.org/download/sfml/2.4.2/) page. You'll have a much, much easier time.


Below are some additional pointers for setting up SFML.

## Windows:
Either

**M1: Visual Studio [Recommended]**

Install Visual Studio with C++, download the SFML binary for your Visual Studio version, follow all the setup instructions in the [SFML tutorial](http://www.sfml-dev.org/tutorials/2.3/start-vc.php).

Or

**M2: MinGW / g++, from command line**

Download [MinGW **6.1.0**](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/6.1.0/threads-posix/dwarf/i686-6.1.0-release-posix-dwarf-rt_v5-rev0.7z/download), and download the SFML binary for MinGW from the downloads page. Make sure you add the `bin` folder for MinGW to your path so that you can use g++. Ask me if you don't know how to do that.
- NOTE: You must use the exact version of MinGW (or TDM) corresponding to the SFML binary you downloaded. It won't work otherwise. (e.g. if you download SFML for GCC 6.1.0 MinGW (DW2) - 32-bit, make sure you use the 32-bit version of MinGW 6.1.0 along with it.

Place the sfml contents in a folder called "sfml" in the same directory as your game code, and run these two lines to build:
```
g++ -std=c++11 -c *.cpp -I sfml/include
g++ -std=c++11 *.o -o Game -L sfml/lib -lsfml-graphics -lsfml-window -lsfml-system
```

Note: The game won't run if you don't put the .dll files from the sfml/bin directory into the same directory as your Game.exe


## Mac OSX

Either

**M1: Brew**

Note: most convenient, but not recommended. Your compiled game will work on your computer only. (but okay if just for the workshop)
Make sure you are able to use g++ on your terminal. Then install SFML via `brew install sfml`.

To compile your code, run these two lines:
```
g++ -std=c++11 -c *.cpp
g++ -std=c++11 *.o -o Game -lsfml-graphics -lsfml-window -lsfml-system
```

Or

**M2: Xcode [Recommended]**

The recommended way for setting up SFML on a Mac. Follow the instructions on the [SFML tutorial](https://www.sfml-dev.org/tutorials/2.4/start-osx.php).


## Linux

**M1: apt-get**

Make sure you are able to use g++. Install SFML via `sudo apt-get install libsfml-dev`
then run these two lines to compile.
```
g++ -std=c++11 -c *.cpp
g++ -std=c++11 *.o -o Game -lsfml-graphics -lsfml-window -lsfml-system
```

If that doesn't work out for you, refer to the [SFML tutorial](https://www.sfml-dev.org/tutorials/2.4/start-linux.php)