import os
import baker

baker.compilerOutputDir = os.path.join(os.environ['HATCHERY_BUILDS'], 'Gladen')

baker.compilerIncludes.extend([
os.path.join(os.environ['HATCHERY_TOOLS'], 'OpenGL', 'include'),
os.path.join(os.environ['HATCHERY_TOOLS'], 'freetype-2.5.5', 'include', 'freetype2'),
os.path.join(os.environ['HATCHERY_TOOLS'], 'freetype-2.5.5', 'include'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'include'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'lodepng'),
])

baker.compilerSources = [
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'src', 'render', 'GladeRenderer.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'src', 'GladeObject.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'src', 'physics', 'CollisionDetector.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'src', 'physics', 'Simulator.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'src', 'util', 'CSVReader.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'src', 'util', 'Path.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'src', 'math', 'Matrix.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'src', 'math', 'globals.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'src', 'debug', 'log.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'src', 'debug', 'crashHandler.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'src', 'opengl', 'functions.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'src', 'ui', 'Widget.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'src', 'ui', 'layout', 'Layout.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'src', 'ui', 'font', 'FreetypeFont.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'src', 'util', 'ResourceManager.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'Gladen', 'src', 'util', 'DesktopFileManager.cpp'),
]

baker.parseCommandLine()
baker.sanitizeBakeInput()    
baker.compile()