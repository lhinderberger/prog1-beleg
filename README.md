Lucas Hinderberger, HTW Dresden, winter term 2016/17

# Beleg Programmierung 1

The source code documentation for this program is written in English.
You should have received a german-language documentation summary as a pdf file.
If you haven't, please compile `doc/dokumentation.tex`.

#### Copyright + Third Party Libraries
This program was created by Lucas Hinderberger in the winter term of 2016/17 at
HTW Dresden for purely educational purposes and isn't intended to be used in any
commercial, industrial or other real-life environment. Do not distribute this software!

This program links against and might include code of the following third-party libraries:

- GTK+ 3 (LGPL, https://www.gtk.org)

## Structure
```
\prog1-beleg
|--doc - Contains LaTeX sources for the documentation summary mentioned above.
|--include - Contains public and private include files for the core / data library
|
|-CMakeLists.txt - Use this file to build the project using CMake
|-main.c - "Hello World!"
```

## How to build
Simply type `cmake .` to build this source tree and run `./prog1_beleg` to execute the program.