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
- SQLite (Public Domain, https://sqlite.org)

## Structure
```
\prog1-beleg
|--doc     - Contains LaTeX sources for the documentation summary mentioned above.
|--include - Contains public and private include files for the core / data library.
|--src     - Contains source files for the included modules.
|--tests   - Contains test cases that can also be called using the `test` make target.
|--ui      - Contains the Glade definition(s) for the frontend graphical user interface.
|
|-CMakeLists.txt - Use this file to build the project using CMake
```

## How to build
Simply type `cmake .` followed by `make` to build this source tree and run `./pb-frontend`
to execute the program.
To execute all tests, run `make test` instead of `./pb-frontend`. You should see an
evaluation of all test cases returned to you.
