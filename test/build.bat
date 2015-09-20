%DEVTOOLS%\"Microsoft Visual Studio 12.0"\VC\bin\cl.exe ^
/I"%DEVTOOLS%\Microsoft Visual Studio 12.0\VC\include" ^
/I"%DEVTOOLS%\Microsoft SDKs\Windows\v7.1A\Include" ^
/I%DEVTOOLS%\OpenGL\include ^
/I%DEVTOOLS%\lodepng\include ^
/EHsc ^
main.cpp ^
..\render\GladeRenderer.cpp ^
..\render\meshes\Cube.cpp ^
..\render\meshes\Triangle.cpp ^
..\render\meshes\Rectangle.cpp ^
..\util\CSVReader.cpp ^
..\math\Matrix.cpp ^
..\log\log.cpp ^
..\opengl\functions.cpp ^
%DEVTOOLS%\lodepng\lodepng.cpp ^
/link /LIBPATH:"%DEVTOOLS%\Microsoft Visual Studio 12.0\VC\lib" ^
/LIBPATH:"%DEVTOOLS%\Microsoft SDKs\Windows\v7.1A\Lib" ^
/WX ^
kernel32.lib user32.lib gdi32.lib Opengl32.lib