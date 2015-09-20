%DEVTOOLS%\"Microsoft Visual Studio 12.0"\VC\bin\cl.exe ^
/I"%DEVTOOLS%\Microsoft Visual Studio 12.0\VC\include" ^
/I"%DEVTOOLS%\Microsoft SDKs\Windows\v7.1A\Include" ^
/I%DEVTOOLS%\OpenGL\include ^
/I%DEVTOOLS%\lodepng\include ^
/I%CODESPACE%\Gladen\include ^
/EHsc ^
main.cpp ^
..\src\render\GladeRenderer.cpp ^
..\src\render\meshes\Cube.cpp ^
..\src\render\meshes\Triangle.cpp ^
..\src\render\meshes\Rectangle.cpp ^
..\src\util\CSVReader.cpp ^
..\src\math\Matrix.cpp ^
..\src\log\log.cpp ^
..\src\opengl\functions.cpp ^
%DEVTOOLS%\lodepng\lodepng.cpp ^
/link /LIBPATH:"%DEVTOOLS%\Microsoft Visual Studio 12.0\VC\lib" ^
/LIBPATH:"%DEVTOOLS%\Microsoft SDKs\Windows\v7.1A\Lib" ^
/WX ^
kernel32.lib user32.lib gdi32.lib Opengl32.lib