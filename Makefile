libglade.a: FreetypeFont.o Layout.o Widget.o log.o Matrix.o globals.o Path.o WavefrontObjReader.o CSVReader.o Simulator.o CollisionDetector.o Perception.o GladeRenderer.o GladeObject.o ResourceManager.o DesktopFileManager.o builddir
	libtool -static -o build/glade/libglade.a ../../vendor/lodepng/lodepng.o build/glade/FreetypeFont.o build/glade/ResourceManager.o build/glade/Layout.o build/glade/Widget.o build/glade/log.o build/glade/Matrix.o build/glade/globals.o build/glade/Path.o build/glade/WavefrontObjReader.o build/glade/CSVReader.o build/glade/Simulator.o build/glade/CollisionDetector.o build/glade/Perception.o build/glade/GladeRenderer.o build/glade/GladeObject.o build/glade/DesktopFileManager.o

######################

FreetypeFont.o: builddir
	clang -O0 -g -I ../../vendor/freetype-2.10.1/include -I include -DGLADE_MACOS -c src/ui/font/FreetypeFont.cpp -o build/glade/FreetypeFont.o

Layout.o: builddir
	clang -O0 -g -I include -DGLADE_MACOS -c src/ui/layout/Layout.cpp -o build/glade/Layout.o

Widget.o: builddir
	clang -O0 -g -I include -DGLADE_MACOS -c src/ui/Widget.cpp -o build/glade/Widget.o

log.o: builddir
	clang -O0 -g -I include -DGLADE_MACOS -c src/debug/log.cpp -o build/glade/log.o

Matrix.o: builddir
	clang -O0 -g -I include -DGLADE_MACOS -c src/math/Matrix.cpp -o build/glade/Matrix.o

globals.o: builddir
	clang -O0 -g -I include -DGLADE_MACOS -c src/math/globals.cpp -o build/glade/globals.o

Path.o: builddir
	clang -O0 -g -I include -DGLADE_MACOS -c src/util/Path.cpp -o build/glade/Path.o

WavefrontObjReader.o: builddir
	clang -O0 -g -I include -DGLADE_MACOS -c src/util/WavefrontObjReader.cpp -o build/glade/WavefrontObjReader.o

CSVReader.o: builddir
	clang -O0 -g -I include -DGLADE_MACOS -c src/util/CSVReader.cpp -o build/glade/CSVReader.o

ResourceManager.o: builddir
	clang -O0 -std=c++11 -g -I ../../vendor/assimp/include -I ../../vendor/freetype-2.10.1/include -I ../../vendor/lodepng -I include -DGLADE_MACOS -c src/util/ResourceManager.cpp -o build/glade/ResourceManager.o

DesktopFileManager.o: builddir
	clang -O0 -g -I include -DGLADE_MACOS -c src/util/DesktopFileManager.cpp -o build/glade/DesktopFileManager.o

Simulator.o: builddir
	clang -O0 -g -I include -DGLADE_MACOS -c src/physics/Simulator.cpp -o build/glade/Simulator.o

CollisionDetector.o: builddir
	clang -O0 -g -I include -DGLADE_MACOS -c src/physics/CollisionDetector.cpp -o build/glade/CollisionDetector.o

Perception.o: builddir
	clang -O0 -g -I include -DGLADE_MACOS -c src/render/Perception.cpp -o build/glade/Perception.o

GladeRenderer.o: builddir
	clang -O0 -g -I../../vendor/glew/include -I include -DGLADE_MACOS -DGL_SILENCE_DEPRECATION -c src/render/GladeRenderer.cpp -o build/glade/GladeRenderer.o

functions.o: builddir
	clang -O0 -g -I include -DGLADE_MACOS -DGL_SILENCE_DEPRECATION -c src/opengl/functions.cpp -o build/glade/functions.o

GladeObject.o: builddir
	clang -O0 -g -I include -DGLADE_MACOS -c src/GladeObject.cpp -o build/glade/GladeObject.o

######################

builddir: 
	mkdir -p build
	mkdir -p build/glade

clean:
	rm -rf build
