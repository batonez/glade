#include "glade/opengl/functions.h"
#include "glade/log/log.h"
#include "glade/math/Matrix.h"
#include "glade/render/GladeRenderer.h"
#include <math.h>

const GLuint  GladeRenderer::POS_SIZE_FLOATS        = 3;
const GLuint  GladeRenderer::NORMAL_SIZE_FLOATS     = 3;
const GLuint  GladeRenderer::TEXCOORD_SIZE_FLOATS   = 2;
const GLuint  GladeRenderer::POS_OFFSET_FLOATS      = 0;
const GLuint  GladeRenderer::NORMAL_OFFSET_FLOATS   = POS_SIZE_FLOATS;
const GLuint  GladeRenderer::TEXCOORD_OFFSET_FLOATS = POS_SIZE_FLOATS + NORMAL_SIZE_FLOATS;
const GLuint  GladeRenderer::VERTEX_STRIDE_FLOATS 	= POS_SIZE_FLOATS + NORMAL_SIZE_FLOATS + TEXCOORD_SIZE_FLOATS;
const GLsizei GladeRenderer::POS_OFFSET_BYTES       = POS_OFFSET_FLOATS * sizeof(float);
const GLsizei GladeRenderer::NORMAL_OFFSET_BYTES    = NORMAL_OFFSET_FLOATS * sizeof(float);
const GLsizei GladeRenderer::TEXCOORD_OFFSET_BYTES  = TEXCOORD_OFFSET_FLOATS * sizeof(float);
const GLsizei GladeRenderer::VERTEX_STRIDE_BYTES    = VERTEX_STRIDE_FLOATS * sizeof(float);

GladeRenderer::GladeRenderer(void)
{
	camera.invertTranslation(true);
	
	lightDirection[0] = -1.0f; lightDirection[1]= 0.0f; lightDirection[2] = 1.0f;
	lightAmbient[0] = 0.5f; lightAmbient[1] = 0.5f; lightAmbient[2] = 0.5f; lightAmbient[3] = 0.5f; lightAmbient[4] = 1.0f;
	lightDiffuse[0] = 0.5f; lightDiffuse[1] = 0.5f; lightDiffuse[2] = 0.5f; lightDiffuse[3] = 1.0f;
	lightSpecular[0] = 1.0f; lightSpecular[1] = 1.0f; lightSpecular[2] = 1.0f; lightSpecular[3] = 1.0f;
	
	sceneProjectionMode = PERSPECTIVE;
	initialized = false;
}

void GladeRenderer::add(GladeObject* pSceneObject)
{
  log("Adding object");
	if (this->initialized) {
		moveIntoVideoMemory(*pSceneObject);
	}
	
	sceneObjects.push_back(pSceneObject);
}

void GladeRenderer::add(Widget* uiElement)
{
  if (this->initialized) {
    moveIntoVideoMemory(*uiElement);
  }
  
  uiElements.push_back(uiElement);
}

void GladeRenderer::clear(void)
{
	log("Clearing renderer");
	removeAllObjectsFromVideoMemory();
  GladeObject::Drawables* drawables;
  vector<GladeObject*>::iterator oi = sceneObjects.begin(); // must be set
  
  while (oi != sceneObjects.end()) {
  	drawables = (*oi)->getDrawables();
		
    for (GladeObject::DrawablesI di = drawables->begin(); di != drawables->end(); di++) {
			(*di)->getTextureTransform()->removeAnimationCallbacks();
		}
    
		oi = sceneObjects.erase(oi);
  }
  
  vector<GladeObject*>::iterator wi = uiElements.begin(); // must be set
  
  while (wi != uiElements.end()) {
  	drawables = (*wi)->getDrawables();
		
    for (GladeObject::DrawablesI di = drawables->begin(); di != drawables->end(); di++) {
			(*di)->getTextureTransform()->removeAnimationCallbacks();
		}
    
		wi = uiElements.erase(wi);
  }
	
	log("Done clearing renderer");
}

void GladeRenderer::onSurfaceCreated(Shader &vertexShader, Shader &fragmentShader)
{
  glFrontFace(GL_CW);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glEnable(GL_BLEND);
//	glEnable(GL_CULL_FACE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	getShaderHandles();
	setLightUniforms();
	moveAllObjectsIntoVideoMemory();
  log("Initialized renderer");
	initialized = true;
}

void GladeRenderer::onSurfaceChanged(int width, int height)
{
	viewportWidth = width;
	viewportHeight = height;
	aspectRatio = (float) viewportWidth / viewportHeight;
	glViewport(0, 0, viewportWidth, viewportHeight);
	
	switchProjectionMode(sceneProjectionMode, true);
}

void GladeRenderer::onDrawFrame(void)
{
/*	for (DrawFrameHooksI hook = drawFrameHooks.begin(); hook != drawFrameHooks.end(); ++hook) {
		(*hook)->onBeforeDraw();
	}*/	

	glClearColor(this->backgroundColor.x, this->backgroundColor.y, this->backgroundColor.z, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switchProjectionMode(sceneProjectionMode);
	camera.getMatrix(viewMatrix);
  
	drawAll(sceneObjects.begin(), sceneObjects.end());
	
	switchProjectionMode(ORTHO);
	Matrix::setIdentityM(viewMatrix, 0);
	
	drawAll(uiElements.begin(), uiElements.end());
	
/*  for (DrawFrameHooksI hook = drawFrameHooks.begin(); hook != drawFrameHooks.end(); ++hook) {
		(*hook)->onAfterDraw();
	}*/
}

void GladeRenderer::setSceneProjectionMode(ProjectionMode projectionMode)
{
	this->sceneProjectionMode = projectionMode;
}

int GladeRenderer::getViewportWidth(void)
{
	return viewportWidth;
}

int GladeRenderer::getViewportHeight(void)
{
	return viewportHeight;
}

void GladeRenderer::setBackgroundColor(float r, float g, float b)
{
	this->backgroundColor.set(r, g, b);
}

int GladeRenderer::percentToPixels(float percent)
{
	return (int)((viewportWidth < viewportHeight ? viewportWidth / 2 : viewportHeight / 2) * percent); 
}

float GladeRenderer::pixelsToPercent(float pixels)
{
	return pixels / (viewportWidth < viewportHeight ? viewportWidth / 2 : viewportHeight / 2);
}

Transform GladeRenderer::getTransformForRootWidget(void)
{
	return Transform(
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			getHalfViewportWidthCoords(), getHalfViewportHeightCoords(), 1.0f
	);
}

void GladeRenderer::moveZeroToUpperLeftCorner(void)
{
	camera.setPosition(getHalfViewportWidthCoords(),  getHalfViewportHeightCoords(), 0);
}

float GladeRenderer::getHalfViewportWidthCoords(void)
{
	return aspectRatio > 1 ? aspectRatio : 1;
}

float GladeRenderer::getHalfViewportHeightCoords(void)
{
	return aspectRatio > 1 ? 1 : aspectRatio;
}
	
float GladeRenderer::getViewportWidthCoords(void)
{
	return 2 * aspectRatio;
}

float GladeRenderer::getViewportHeightCoords(void)
{
	return 2;
}

Vector2f GladeRenderer::getPointCoords(float screenX, float screenY)
{
	Vector2f result;
	
	result.x = screenX / viewportWidth * 2 * aspectRatio - aspectRatio;
	result.y = screenY / viewportHeight * 2 - 1;
	
	return result;
}

/*
vector<GladeObject>::iterator GladeRenderer::getWidgets(void)
{
	return uiElements.begin();
}*/

void GladeRenderer::drawAll(vector<GladeObject*>::iterator i, vector<GladeObject*>::iterator end)
{
	Transform finalWorldTransformForDrawable;
  GladeObject::Drawables* drawables;
  
	while (i != end) {
		if ((*i)->isEnabled()) {
			drawables = (*i)->getDrawables();
		
			for (GladeObject::DrawablesI di = drawables->begin(); di != drawables->end(); di++) {
				if ((*i)->isViewEnabled(*di)) {
          (*i)->getTransform()->combineWith(
						*(*di)->getTransform(),
						(*di)->preservePosition,
						(*di)->preserveRotation,
						(*di)->preserveScale,
						finalWorldTransformForDrawable
					);
					
					draw(di, finalWorldTransformForDrawable);
				}
			}
		}
		
		i++;
	}
}

void GladeRenderer::moveAllObjectsIntoVideoMemory(void)
{
	vector<GladeObject*>::iterator i = sceneObjects.begin();
	
	while (i != sceneObjects.end()) {
		moveIntoVideoMemory(**i);
		i++;
	}
	
	vector<GladeObject*>::iterator wi = uiElements.begin();
	
	while (wi != uiElements.end()) {
		moveIntoVideoMemory(**wi);
		wi++;
	}
}

void GladeRenderer::moveIntoVideoMemory(GladeObject &sceneObject)
{
	GladeObject::DrawablesI di = sceneObject.getDrawables()->begin();
	
	while (di != sceneObject.getDrawables()->end()) {
		moveIntoVideoMemory((*di)->getVertexObject());
    moveIntoVideoMemory((*di)->getTexture());
    compileShaderProgram((*di)->getShaderProgram());
		di++;
	}
  
  glReleaseShaderCompiler();
}

void GladeRenderer::compileShaderProgram(ShaderProgram *program)
{
  GLuint vertexShaderPointer = loadShader(GL_VERTEX_SHADER, program->vertexShader);
	GLuint fragmentShaderPointer = loadShader(GL_FRAGMENT_SHADER, program->fragmentShader);
	
	programHandle = glCreateProgram();
  glAttachShader(programHandle, vertexShaderPointer);
  glAttachShader(programHandle, fragmentShaderPointer);
  glLinkProgram(programHandle);
  program.gpuHandle = programHandle;
}

void GladeRenderer::moveIntoVideoMemory(VertexObject *mesh)
{
  if (NULL == mesh || mesh->hasVideoBufferHandle()) {
    return;
  }
  
  log("Creating VBO");
  GLuint vboIds[2];
  glGenBuffers(2, vboIds);
  mesh->vertexVboId = vboIds[0];
  mesh->indexVboId = vboIds[1];
    
  bindBuffers(*mesh);
  
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->getVertexBufferSize(), mesh->getVertices(), GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * mesh->getIndexBufferSize(), mesh->getIndices(), GL_STATIC_DRAW);
  
  mesh->erase();
}

void GladeRenderer::moveIntoVideoMemory(Texture *texture)
{
  if (NULL == texture || texture->hasVideoBufferHandle() || NULL == texture->getData()) {
    return;
  }
  
  log("Loading texture into video memory");
  GLvoid* data = (GLvoid*) texture->getData();
  
  GLuint texIds[1];
  glGenTextures(1, texIds);
  texture->setVideoBufferHandle(texIds[0]);
  
  glBindTexture(GL_TEXTURE_2D, texture->getVideoBufferHandle());
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  
  if (texture->isTextureSizePowerOfTwo()) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
  }
  
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA,
    texture->textureWidth,
    texture->textureHeight,
    0,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    data
  );
  
  texture->erase();
}

void GladeRenderer::removeFromVideoMemory(Drawable &drawable)
{
	Texture *texture = drawable.getTexture();
  VertexObject *mesh = drawable.getVertexObject();
	
	if (NULL != texture && texture->hasVideoBufferHandle()) {
      GLuint texIds[1];
      texIds[0] = texture->getVideoBufferHandle();
      glDeleteTextures(1, texIds);
      texture->setVideoBufferHandle(-1);
	}
		
	if (mesh->hasVideoBufferHandle()) {
		GLuint vboIds[1];
		vboIds[0] = mesh->vertexVboId;
		vboIds[1] = mesh->indexVboId;
		glDeleteBuffers(2, vboIds);
		mesh->vertexVboId = -1;
		mesh->indexVboId = -1;
	}
}

void GladeRenderer::removeAllObjectsFromVideoMemory(void)
{
	vector<GladeObject*>::iterator i = sceneObjects.begin();
	
	while (i != sceneObjects.end()) {
		GladeObject::Drawables* drawables = (*i)->getDrawables();
		GladeObject::DrawablesI di = drawables->begin();
		
		while (di != drawables->end()) {
			removeFromVideoMemory(**di);
      di++;
		}
		
		i++;
	}

  vector<GladeObject*>::iterator wi = uiElements.begin();
	
	while (wi != uiElements.end()) {
		GladeObject::Drawables* drawables = (*wi)->getDrawables();
		GladeObject::DrawablesI di = drawables->begin();
		
		while (di != drawables->end()) {
			removeFromVideoMemory(**di);
      di++;
		}
		
		wi++;
	}
}

void GladeRenderer::draw(GladeObject::DrawablesI di, Transform &transform)
{
  ShaderProgram *program = di->getShaderProgram();
  
  if (NULL == program) {
    log("Could not render a drawable: no GPU program");
    return;
  }
  
  if (program->gpuHandle < 0) {
    log("Could not render a drawable: GPU program was not loaded to video memory");
    return;
  }
  
  glUseProgram(program->gpuHandle);
  
	static float worldMatrix[16];
	
  transform.getMatrix(worldMatrix);
	Matrix::multiplyMM(worldViewMatrix, 0, viewMatrix, 0, worldMatrix, 0);
  
  glUniformMatrix4fv(uProjectionMatrix, 4, GL_FALSE, projectionMatrix);
  glUniformMatrix4fv(uWorldViewMatrix, 4, GL_FALSE, worldViewMatrix);
  
	bindBuffers(*(*di)->getVertexObject());
	
	glVertexAttribPointer(
		aPosition, POS_SIZE_FLOATS,
		GL_FLOAT, GL_FALSE,
		VERTEX_STRIDE_BYTES, (const GLvoid *)POS_OFFSET_BYTES
	);
  
	glEnableVertexAttribArray(aPosition);
	
  glVertexAttribPointer(
    aNormal, NORMAL_SIZE_FLOATS,
    GL_FLOAT, GL_FALSE,
    VERTEX_STRIDE_BYTES, (const GLvoid *)NORMAL_OFFSET_BYTES
  );
  
  glEnableVertexAttribArray(aNormal);
  
	Texture* texture = (*di)->getTexture();
	
	if (texture != NULL) {
		if (texture->hasVideoBufferHandle()) {
      glActiveTexture(GL_TEXTURE0);
		  glBindTexture(GL_TEXTURE_2D, texture->getVideoBufferHandle());

		  TextureTransform* texTransform = (*di)->getTextureTransform();
      texTransform->executeCallbacks();
			
      glUniform1i(uSamplerNumber, 0);
      glUniform1f(uTexScaleX, texTransform->textureScaleX  / (float) texture->numberOfFrames * texTransform->getTextureScaleXModifierForFrame(*texture));
      glUniform1f(uTexScaleY, texTransform->textureScaleY  / (float) texture->numberOfAnimations * texTransform->getTextureScaleYModifierForFrame(*texture));
			glUniform1f(uTexOffsetX, texTransform->getCurrentFrameNumber(*texture) * texture->texCoordFrameWidth);
			glUniform1f(uTexOffsetY, texTransform->getCurrentAnimationNumber(*texture) * texture->texCoordFrameHeight);
			
			glVertexAttribPointer(
				aTexCoord, TEXCOORD_SIZE_FLOATS,
				GL_FLOAT, GL_FALSE,
				VERTEX_STRIDE_BYTES, (const GLvoid*)TEXCOORD_OFFSET_BYTES
			);
						
			glEnableVertexAttribArray(aTexCoord);
    }
  }
  
  /*
  Vector4f* constantColor = (*di)->getConstantColor();
	glUniform4f(uColor, constantColor->x, constantColor->y, constantColor->z, constantColor->w);
  
	glUniform1i(uReplaceColor, (*di)->replaceOriginalColor ? 1 : 0);
  
  if ((*di)->isLit()) {
    Material* material = (*di)->getMaterial();
		
    //halfplane = normalize(dir - look) // <- Dynamic
		float halfplaneXZ = (float)(-1 / sqrt(2.0));
		glUniform3f(uLightHalfplane, halfplaneXZ, 0, halfplaneXZ);
		
		glUniform4f(uMaterialAmbient, material->ambient[0], material->ambient[1], material->ambient[2], material->ambient[3]);
		glUniform4f(uMaterialDiffuse, material->diffuse[0], material->diffuse[1], material->diffuse[2], material->diffuse[3]);
		glUniform4f(uMaterialSpecular, material->specular[0], material->specular[1], material->specular[2], material->specular[3]);
		glUniform1f(uMaterialShininess, material->shininess);
  
    glUniform1i(uLight, 1);
    
  } else {
		glUniform1i(uLight, 0);
	}
  */
  
  program->setUniforms();
  
	glDrawElements(GL_TRIANGLES, (*di)->getVertexObject()->getIndexBufferSize(), GL_UNSIGNED_SHORT, 0);
	
	glDisableVertexAttribArray(aPosition);
	glDisableVertexAttribArray(aNormal);
	glDisableVertexAttribArray(aTexCoord);
}

void GladeRenderer::bindBuffers(VertexObject &mesh)
{
	if (!mesh.hasVideoBufferHandle()) {
		log("Render error: trying to use a mesh which was not loaded into video buffer");
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexVboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexVboId);
}

void GladeRenderer::switchProjectionMode(ProjectionMode projectionMode, bool force)
{
	if (force || projectionMode != currentProjectionMode) {
		switch (projectionMode) {
			case ORTHO:
				Matrix::orthoM(projectionMatrix, 0, -aspectRatio, aspectRatio, 1, -1, -(float)DOUBLE_CUBE_DIAGONAL, (float)DOUBLE_CUBE_DIAGONAL);
				glDisable(GL_DEPTH_TEST);
				break;
			case PERSPECTIVE:
				Matrix::perspectiveM(projectionMatrix, 0, 45, aspectRatio, 1, 150);
				glEnable(GL_DEPTH_TEST);
				break;
			default: ;
				//throw new RuntimeException("Unexpected projection mode. Use ORTHO or PERSPECTIVE constants");
		}
		
		currentProjectionMode = projectionMode;
	}
}

void GladeRenderer::switchProjectionMode(ProjectionMode projectionMode)
{
	switchProjectionMode(projectionMode, false);
}

void GladeRenderer::getShaderHandles(ShaderProgram &program)
{
  uProjectionMatrix 	= glGetUniformLocation(program.gpuHandle, "uProjectionMatrix");
	uWorldViewMatrix	= glGetUniformLocation(program.gpuHandle, "uWorldViewMatrix");

/*	
	uColor				= glGetUniformLocation(program.gpuHandle, "uColor");
	uReplaceColor		= glGetUniformLocation(program.gpuHandle, "uReplaceColor");
	uLight 				= glGetUniformLocation(program.gpuHandle, "uLight");
	uLightDirection 	= glGetUniformLocation(program.gpuHandle, "uLightDirection");
	uLightHalfplane		= glGetUniformLocation(program.gpuHandle, "uLightHalfplane");
	uLightAmbient 		= glGetUniformLocation(program.gpuHandle, "uLightAmbient");
	uLightDiffuse 		= glGetUniformLocation(program.gpuHandle, "uLightDiffuse");
	uLightSpecular 		= glGetUniformLocation(program.gpuHandle, "uLightSpecular");
	uMaterialAmbient 	= glGetUniformLocation(program.gpuHandle, "uMaterialAmbient");
	uMaterialDiffuse 	= glGetUniformLocation(program.gpuHandle, "uMaterialDiffuse");
	uMaterialSpecular 	= glGetUniformLocation(program.gpuHandle, "uMaterialSpecular");
	uMaterialShininess 	= glGetUniformLocation(program.gpuHandle, "uMaterialShininess");
*/

	aPosition			= glGetAttribLocation(program.gpuHandle, "aPosition");
	aNormal				= glGetAttribLocation(program.gpuHandle, "aNormal");
	aTexCoord			= glGetAttribLocation(program.gpuHandle, "aTexCoord0");
	
	uSamplerNumber 		= glGetUniformLocation(program.gpuHandle, "uTextureSampler0");
	uTexScaleX 			= glGetUniformLocation(program.gpuHandle, "uTexCoordScaleX0");
	uTexScaleY 			= glGetUniformLocation(program.gpuHandle, "uTexCoordScaleY0");
	uTexOffsetX 		= glGetUniformLocation(program.gpuHandle, "uTexCoordOffsetX0");
	uTexOffsetY 		= glGetUniformLocation(program.gpuHandle, "uTexCoordOffsetY0");
  
  program.getUniformLocations();
} 

//void GladeRenderer::setLightUniforms(void)
{
	glUniform3f(uLightDirection, lightDirection[0], lightDirection[1], lightDirection[2]);
	glUniform4f(uLightAmbient, lightAmbient[0], lightAmbient[1], lightAmbient[2], lightAmbient[3]);
	glUniform4f(uLightDiffuse, lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
	glUniform4f(uLightSpecular, lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
}

GLuint GladeRenderer::loadShader(GLuint shaderType, Shader &shader)
{
	GLuint shaderHandle = glCreateShader(shaderType);
  
  if (shaderHandle) {
    const char* source = shader.getSource();
    glShaderSource(shaderHandle, 1, &source, NULL);
    glCompileShader(shaderHandle);
    GLint compiled = 0;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compiled);
    
    if (!compiled) {
      GLint infoLen = 0;
      glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &infoLen);
      
      if (infoLen) {
        char* buf = (char*) malloc(infoLen);
        
        if (buf) {
            glGetShaderInfoLog(shaderHandle, infoLen, NULL, buf);
            log("shaderHandle compilation error:");
            log(buf);
            free(buf);
        }
        
        glDeleteShader(shaderHandle);
        shaderHandle = 0;
      }
    }
  }

  return shaderHandle;
}

int GladeRenderer::checkGLError(void)
{
	int errorCode = glGetError();
	
	switch (errorCode) {
		case GL_INVALID_ENUM:
			log("GL_INVALID_ENUM: A GLenum argument is out of range. The command that generated this error was ignored.");
			break;
		case GL_INVALID_VALUE:
			log("GL_INVALID_VALUE: A numeric argument is out of range. The command that generated this error was ignored.");
			break;
		case GL_INVALID_OPERATION:
			log("GL_INVALID_OPERATION: The command cannot be performed in the current OpenGL state. The command that generated this error was ignored.");
			break;
		case GL_OUT_OF_MEMORY:
			log("GL_OUT_OF_MEMORY: There is insufficient memory to execute this command. The state of the OpenGL pipeline is undefined after this point.");
			break;
	}
	
	return errorCode;
}

void GladeRenderer::addDrawFrameHook(DrawFrameHook* hook)
{
	drawFrameHooks.insert(hook);
}

/*
void GladeRenderer::setDrawingOrderComparator(Comparator<GladeObject> drawingOrderComparator)
{
	this->drawingOrderComparator = drawingOrderComparator;
}
*/	
void GladeRenderer::sortDrawables()
{
  /*
	if (drawingOrderComparator != NULL) {
		Collections.sort(sceneObjects, drawingOrderComparator);
	}*/
}
