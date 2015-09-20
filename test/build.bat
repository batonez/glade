%HATCHERY_TOOLS%\"Microsoft Visual Studio 12.0"\VC\bin\cl.exe ^
/I"%HATCHERY_TOOLS%\Microsoft Visual Studio 12.0\VC\include" ^
/I"%HATCHERY_TOOLS%\Microsoft SDKs\Windows\v7.1A\Include" ^
/I%HATCHERY_TOOLS%\OpenGL\include ^
/I%HATCHERY_TOOLS%\lodepng\include ^
/I%HATCHERY_CODESPACE%\Gladen\include ^
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
%HATCHERY_TOOLS%\lodepng\lodepng.cpp ^
/link /LIBPATH:"%HATCHERY_TOOLS%\Microsoft Visual Studio 12.0\VC\lib" ^
/LIBPATH:"%HATCHERY_TOOLS%\Microsoft SDKs\Windows\v7.1A\Lib" ^
/WX ^
kernel32.lib user32.lib gdi32.lib Opengl32.lib