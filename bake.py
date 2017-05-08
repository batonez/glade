import os
import baker

baker.compilerOutputDir = os.path.join(os.environ['HATCHERY_BUILDS'], 'glade')

baker.compilerIncludes.extend([
os.path.join(os.environ['HATCHERY_SOURCES'], 'lodepng'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'include'),
os.path.join(os.environ['HATCHERY_TOOLS'], 'OpenGL', 'include')
])

baker.compilerSources = [
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'Context.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'GladeObject.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'render', 'GladeRenderer.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'render', 'Perception.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'render', 'Mesh.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'opengl', 'functions.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'physics', 'CollisionDetector.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'physics', 'Simulator.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'util', 'CSVReader.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'util', 'WavefrontObjReader.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'util', 'Path.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'util', 'ResourceManager.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'util', 'DesktopFileManager.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'math', 'Matrix.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'math', 'globals.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'debug', 'log.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'debug', 'crashHandler.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'ui', 'Widget.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'ui', 'layout', 'Layout.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'ui', 'layout', 'CenterLayout.cpp'),
]

baker.parseCommandLine()
baker.sanitizeBakeInput()    
baker.compile()