qmake ChessGame.pro
mingw64-make
mingw64-make install
cd release 
del *.o
del *.cpp
windeployqt ChessGame.exe