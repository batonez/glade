import os
import importlib.util
bakerModuleSpec = importlib.util.spec_from_file_location("baker", os.path.join(os.environ['HATCHERY_SOURCES'], 'baker', 'baker.py'))
baker = importlib.util.module_from_spec(bakerModuleSpec)
bakerModuleSpec.loader.exec_module(baker)

baker.compilerOutputDir = os.path.join(os.environ['HATCHERY_BUILDS'], 'glade')

baker.compilerIncludes.extend([
os.path.join(os.environ['HATCHERY_VENDOR'], 'OpenGL', 'include'),
os.path.join(os.environ['HATCHERY_VENDOR'], 'freetype-2.10.1', 'include', 'freetype2'),
os.path.join(os.environ['HATCHERY_VENDOR'], 'freetype-2.10.1', 'include'),
os.path.join(os.environ['HATCHERY_VENDOR'], 'lodepng'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'include'), #wtf
])

baker.compilerSources = [
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'render', 'GladeRenderer.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'render', 'Perception.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'render', 'DynamicMesh.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'GladeObject.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'physics', 'CollisionDetector.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'physics', 'Simulator.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'util', 'CSVReader.cpp'),
os.path.join(os.environ['HATCHERY_SOURCES'], 'glade', 'src', 'util', 'WavefrontObjReader.cpp'),
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
