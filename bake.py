import os
import baker

baker.compilerOutputDir = os.path.join(os.environ['HATCHERY_BUILDS'], 'glade')

baker.compilerIncludes.extend([
os.path.join(os.environ['HATCHERY_TOOLS'], 'OpenGL', 'include'),
os.path.join(os.environ['HATCHERY_TOOLS'], 'freetype-2.5.5', 'include', 'freetype2'),
os.path.join(os.environ['HATCHERY_TOOLS'], 'freetype-2.5.5', 'include'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'include'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'lodepng'),
])

baker.compilerSources = [
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'render', 'GladeRenderer.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'render', 'Perception.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'GladeObject.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'physics', 'CollisionDetector.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'physics', 'Simulator.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'util', 'CSVReader.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'util', 'Path.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'math', 'Matrix.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'math', 'globals.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'debug', 'log.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'debug', 'crashHandler.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'opengl', 'functions.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'ui', 'Widget.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'ui', 'layout', 'Layout.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'ui', 'font', 'FreetypeFont.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'util', 'ResourceManager.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'util', 'DesktopFileManager.cpp'),
]

baker.parseCommandLine()
baker.sanitizeBakeInput()    
baker.compile()