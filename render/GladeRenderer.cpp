#include "../opengl/functions.h"
#include <math.h>
#include "../log/log.h"
#include "../math/Matrix.h"
#include "GladeRenderer.h"

const GLuint GladeRenderer::POS_SIZE_FLOATS   		= 3;
const GLuint GladeRenderer::COLOR_SIZE_FLOATS 		= 4;
const GLuint GladeRenderer::NORMAL_SIZE_FLOATS 		= 3;
const GLuint GladeRenderer::TEXCOORD_SIZE_FLOATS 	= 2;
const GLuint GladeRenderer::POS_OFFSET_FLOATS 		= 0;
const GLuint GladeRenderer::COLOR_OFFSET_FLOATS 	= POS_SIZE_FLOATS;
const GLuint GladeRenderer::NORMAL_OFFSET_FLOATS 	= POS_SIZE_FLOATS + COLOR_SIZE_FLOATS;
const GLuint GladeRenderer::TEXCOORD_OFFSET_FLOATS 	= POS_SIZE_FLOATS + COLOR_SIZE_FLOATS + NORMAL_SIZE_FLOATS;
const GLuint GladeRenderer::VERTEX_STRIDE_FLOATS 	= POS_SIZE_FLOATS + COLOR_SIZE_FLOATS + NORMAL_SIZE_FLOATS + TEXCOORD_SIZE_FLOATS;
const GLsizei GladeRenderer::POS_OFFSET_BYTES 	  = POS_OFFSET_FLOATS * sizeof(float);
const GLsizei GladeRenderer::COLOR_OFFSET_BYTES 	= COLOR_OFFSET_FLOATS * sizeof(float);
const GLsizei GladeRenderer::NORMAL_OFFSET_BYTES 	= NORMAL_OFFSET_FLOATS * sizeof(float);
const GLsizei GladeRenderer::TEXCOORD_OFFSET_BYTES 	= TEXCOORD_OFFSET_FLOATS * sizeof(float);
const GLsizei GladeRenderer::VERTEX_STRIDE_BYTES 	= VERTEX_STRIDE_FLOATS * sizeof(float);

const double GladeRenderer::DOUBLE_CUBE_DIAGONAL = sqrt(3.0);

static const char vertexShaderSource[] =
"const float ZERO = 0.0, ONE = 1.0;\n"

"uniform mat4 uProjectionMatrix;\n"
"uniform mat4 uWorldViewMatrix;\n"

"uniform float uTexCoordScaleX0;\n"
"uniform float uTexCoordScaleY0;\n"
"uniform float uTexCoordOffsetX0;\n"
"uniform float uTexCoordOffsetY0;\n"

"uniform int     uLight;\n"
"uniform vec3    uLightDirection;\n"
"uniform vec3    uLightHalfplane;\n"
"uniform vec4    uLightAmbient;\n"
"uniform vec4    uLightDiffuse;\n"
"uniform vec4    uLightSpecular;\n"
"uniform vec4    uMaterialAmbient;\n"
"uniform vec4    uMaterialDiffuse;\n"
"uniform vec4    uMaterialSpecular;\n"
"uniform float   uMaterialShininess;\n"

"attribute vec4  aPosition;\n"
"attribute vec3  aNormal;\n"
"attribute vec2  aTexCoord0;\n"

"varying vec2    vTexCoord0;\n"
"varying vec4    vColor;\n"

"vec4 calculateLight(vec3 normal) {\n"
   "vec4 computedColor = vec4(ZERO, ZERO, ZERO, ZERO);\n"
   "float ndotl;\n"
   "float ndoth;\n"
   "ndotl = max(ZERO, dot(normal, uLightDirection));\n"
   "ndoth = max(ZERO, dot(normal, uLightHalfplane));\n"
   "computedColor += (uLightAmbient * uMaterialAmbient);\n"
   "computedColor += (ndotl * uLightDiffuse * uMaterialDiffuse);\n"
   
   "if (ndoth > ZERO) {\n"
      "computedColor += (pow(ndoth, uMaterialShininess) *\n"
      "uMaterialSpecular *\n"
      "uLightSpecular);\n"
   "}\n"
   
   "return computedColor;\n"
"}\n"

"vec4 transformTexCoords(vec2 texCoord, vec2 offset, vec2 scale) {\n"
   "vec4 result;\n"
   "result.y = 0;\n"
   "result.z = 0;\n"
   
   "if (scale.x < ZERO) {\n"
      "result.x = ONE + texCoord.x * scale.x - offset.x;\n"
   "} else {\n"
      "result.x = texCoord.x * scale.x + offset.x;\n"
   "}\n"
      
   "if (scale.y < ZERO) {\n"
      "result.y = ONE + texCoord.y * scale.y - offset.y;\n"
   "} else {\n"
      "result.y = texCoord.y * scale.y + offset.y;\n"
   "}\n"
      "\n"
   "return result;\n"
"}\n"

"void main() {\n"
  "gl_Position = uProjectionMatrix * uWorldViewMatrix * aPosition;\n"
  "vTexCoord0 = transformTexCoords(aTexCoord0, vec2(uTexCoordOffsetX0, uTexCoordOffsetY0), vec2(uTexCoordScaleX0, uTexCoordScaleY0));\n"
  
  "if (uLight != ZERO) {\n"
     "vec4 rotatedNormal = normalize(uWorldViewMatrix * vec4(aNormal.xyz, ZERO));\n"
     "vColor = calculateLight(rotatedNormal.xyz);\n"
  "} else {\n"
     "vColor = vec4(ZERO, ZERO, ZERO, ZERO);\n"
  "}\n"
"}\n";

static char fragmentShaderSource[] = 
"uniform sampler2D uTextureSampler0;\n"
"uniform vec4 uColor;\n"
"uniform int uReplaceColor = 0;\n"

"varying vec4 vColor;\n"
"varying vec2 vTexCoord0;\n"

"vec4 blendColors(vec4 baseColor, vec4 overlayColor) {\n"
   "return vec4(\n"
      "baseColor.r * (1.0 - overlayColor.a) + overlayColor.r * overlayColor.a, \n"
      "baseColor.g * (1.0 - overlayColor.a) + overlayColor.g * overlayColor.a, \n"
      "baseColor.b * (1.0 - overlayColor.a) + overlayColor.b * overlayColor.a, \n"
      "baseColor.a * (1.0 - overlayColor.a) + overlayColor.a * overlayColor.a);\n"
"}\n"

"void main(void)\n"
"{\n"
  "vec4 texColor = texture2D(uTextureSampler0, vTexCoord0);\n"

//"if (uReplaceColor == 1) {\n"
//		"gl_FragColor = vec4(uColor.r, uColor.g, uColor.b, texColor.a);\n"
//	"} else {\n"
  	"gl_FragColor = texColor;\n"
//	"}\n"
  
  "gl_FragColor = blendColors(gl_FragColor, uColor);\n"
	
//	"if (uReplaceColor == 0) {\n"
//		"gl_FragColor = blendColors(gl_FragColor, uColor);\n"
//	"}\n"
"}\n";

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
/*
void GladeRenderer::add(Widget uiElement)
{
	if (uiElement.getDrawables() != NULL) {
		if (this->initialized) {
			moveIntoVideoMemory(uiElement);
		}
		
		uiElements.add(uiElement);
	}
}
*/
void GladeRenderer::clear(void)
{
	log("Clearing renderer");
	removeAllObjectsFromVideoMemory();
  vector<Drawable*>* drawables;
  vector<GladeObject*>::iterator oi = sceneObjects.begin();
  
  while (oi != sceneObjects.end()) {
  	drawables = (*oi)->getDrawables();
		
    for (vector<Drawable*>::iterator di = drawables->begin(); di != drawables->end(); di++) {
			(*di)->getTextureTransform()->removeAnimationCallbacks();
		}
    
		oi = sceneObjects.erase(oi);
  }
  
	/*
	i = uiElements.iterator();
	
	while (i.hasNext()) {
		object = i.next();
		
		for (Drawable drawable: object.getDrawables()) {
			drawable.getTextureTransform().removeAnimationCallbacks();
		}
		
		i.remove();
	}*/
	
	log("Done clearing renderer");
}

void GladeRenderer::onSurfaceCreated(void)
{
  GLuint vertexShaderPointer = loadShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShaderPointer = loadShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	
	program = glCreateProgram();
  glAttachShader(program, vertexShaderPointer);
  glAttachShader(program, fragmentShaderPointer);
  glLinkProgram(program);
  glReleaseShaderCompiler();
	glUseProgram(program);
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
/*		for (DrawFrameHook hook: drawFrameHooks) {
		hook.onBeforeDraw();
	}
*/		

	glClearColor(this->backgroundColor.x, this->backgroundColor.y, this->backgroundColor.z, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switchProjectionMode(sceneProjectionMode);
	camera.getMatrix(viewMatrix);
  
	drawAll(sceneObjects.begin(), sceneObjects.end());
	
  /*
	switchProjectionMode(ORTHO);
	Matrix::setIdentityM(viewMatrix, 0);
	
	drawAll(uiElements.iterator());*/
	
/*		for (DrawFrameHook hook: drawFrameHooks) {
		hook.onAfterDraw();
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

/*
Transform GladeRenderer::getTransformForRootWidget(void)
{
	return new Transform(
			new Vector3f(),
			new Vector3f(),
			new Vector3f(getHalfViewportWidthCoords(), getHalfViewportHeightCoords(), 1)
	);
}*/

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
  vector<Drawable*>* drawables;
  
	while (i != end) {
		if ((*i)->isEnabled()) {
			drawables = (*i)->getDrawables();
		
			for (vector<Drawable*>::iterator di = drawables->begin(); di != drawables->end(); di++) {
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
	
	/*
	i = uiElements.iterator();
		
	while (i.hasNext()) {
		moveIntoVideoMemory(i.next());
	}*/
}

void GladeRenderer::moveIntoVideoMemory(GladeObject &sceneObject)
{
	vector<Drawable*>::iterator di = sceneObject.getDrawables()->begin();
	
	while (di != sceneObject.getDrawables()->end()) {
		if (!(**di).isInitialized()) {
			moveIntoVideoMemory(**di);
		}
		
		di++;
	}
}

void GladeRenderer::moveIntoVideoMemory(Drawable &drawable)
{
  log("Moving drawable into video memory");
	Texture* texture = drawable.getTexture();
	
  if (texture == NULL)
    log("Texture is null");
    
  if (texture->hasVideoBufferHandle())
    log("Has video buffer handle");
    
  if (texture->getData() == NULL)
    log("Texture data is null");
    
	if (texture != NULL && !texture->hasVideoBufferHandle() && texture->getData() != NULL) {
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
	
	if (drawable.vertexVboId == 0 && drawable.indexVboId == 0) {
    log("Creating VBO");
		GLuint vboIds[2];
		glGenBuffers(2, vboIds);
		drawable.vertexVboId = vboIds[0];
		drawable.indexVboId = vboIds[1];
		
		VertexObject* vertexObject = drawable.getVertexObject();
		
		bindBuffers(drawable);
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexObject->getVertexBufferSize(), vertexObject->getVertices(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * vertexObject->getIndexBufferSize(), vertexObject->getIndices(), GL_STATIC_DRAW);
		
		drawable.getVertexObject()->erase();
	}
}

void GladeRenderer::removeFromVideoMemory(Drawable &drawable)
{
	Texture* texture = drawable.getTexture();
	
	if (texture != NULL) {
    if (texture->hasVideoBufferHandle()) {
      GLuint texIds[1];
      texIds[0] = texture->getVideoBufferHandle();
      glDeleteTextures(1, texIds);
      texture->setVideoBufferHandle(0);
    }
    
    texture->free();
	}
		
	if (drawable.indexVboId != 0 || drawable.vertexVboId != 0) {
		GLuint vboIds[1];
		vboIds[0] = drawable.vertexVboId;
		vboIds[1] = drawable.indexVboId;
		glDeleteBuffers(2, vboIds);
		drawable.vertexVboId = 0;
		drawable.indexVboId = 0;
	}
	
	
}

void GladeRenderer::removeAllObjectsFromVideoMemory(void)
{
	vector<GladeObject*>::iterator i = sceneObjects.begin();
	
	while (i != sceneObjects.end()) {
		vector<Drawable*>* drawables = (*i)->getDrawables();
		vector<Drawable*>::iterator di = drawables->begin();
		
		while (di != drawables->end()) {
			removeFromVideoMemory(**di);
      di++;
		}
		
		i++;
	}
/*
	i = uiElements.iterator();
	
	while (i.hasNext()) {
		for (Drawable drawable: i.next().getDrawables()) {
			removeFromVideoMemory(drawable);
		}
	}*/
}

void GladeRenderer::draw(vector<Drawable*>::iterator di, Transform &transform)
{
	static float worldMatrix[16];
	
  transform.getMatrix(worldMatrix);
	Matrix::multiplyMM(worldViewMatrix, 0, viewMatrix, 0, worldMatrix, 0);
  
  glUniformMatrix4fv(uProjectionMatrix, 4, GL_FALSE, projectionMatrix);
  glUniformMatrix4fv(uWorldViewMatrix, 4, GL_FALSE, worldViewMatrix);
  
  Vector4f* constantColor = (*di)->getConstantColor();
	glUniform4f(uColor, constantColor->x, constantColor->y, constantColor->z, constantColor->w);
  
	glUniform1i(uReplaceColor, (*di)->replaceOriginalColor ? 1 : 0);
  
	bindBuffers(**di);
	
	glVertexAttribPointer(
		aPosition, POS_SIZE_FLOATS,
		GL_FLOAT, GL_FALSE,
		VERTEX_STRIDE_BYTES, (const GLvoid *)POS_OFFSET_BYTES
	);
  
	glEnableVertexAttribArray(aPosition);
	
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
	
  glVertexAttribPointer(
    aNormal, NORMAL_SIZE_FLOATS,
    GL_FLOAT, GL_FALSE,
    VERTEX_STRIDE_BYTES, (const GLvoid *)NORMAL_OFFSET_BYTES
  );
  
  glEnableVertexAttribArray(aNormal);
  
	glDrawElements(GL_TRIANGLES, (*di)->getVertexObject()->getIndexBufferSize(), GL_UNSIGNED_SHORT, 0);
	
	glDisableVertexAttribArray(aPosition);
	glDisableVertexAttribArray(aNormal);
	glDisableVertexAttribArray(aTexCoord);
}

void GladeRenderer::bindBuffers(Drawable &drawable)
{
	if (drawable.vertexVboId == 0 || drawable.indexVboId == 0) {
		log("Drawable was not initialized. Run initBuffers to create VBOs.");
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, drawable.vertexVboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable.indexVboId);
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

void GladeRenderer::getShaderHandles(void)
{
	uProjectionMatrix 	= glGetUniformLocation(program, "uProjectionMatrix");
	uWorldViewMatrix	= glGetUniformLocation(program, "uWorldViewMatrix");
	
	uColor				= glGetUniformLocation(program, "uColor");
	uReplaceColor		= glGetUniformLocation(program, "uReplaceColor");
	uLight 				= glGetUniformLocation(program, "uLight");
	uLightDirection 	= glGetUniformLocation(program, "uLightDirection");
	uLightHalfplane		= glGetUniformLocation(program, "uLightHalfplane");
	uLightAmbient 		= glGetUniformLocation(program, "uLightAmbient");
	uLightDiffuse 		= glGetUniformLocation(program, "uLightDiffuse");
	uLightSpecular 		= glGetUniformLocation(program, "uLightSpecular");
	uMaterialAmbient 	= glGetUniformLocation(program, "uMaterialAmbient");
	uMaterialDiffuse 	= glGetUniformLocation(program, "uMaterialDiffuse");
	uMaterialSpecular 	= glGetUniformLocation(program, "uMaterialSpecular");
	uMaterialShininess 	= glGetUniformLocation(program, "uMaterialShininess");

	aPosition			= glGetAttribLocation(program, "aPosition");
	aNormal				= glGetAttribLocation(program, "aNormal");
	aTexCoord			= glGetAttribLocation(program, "aTexCoord0");
	
	uSamplerNumber 		= glGetUniformLocation(program, "uTextureSampler0");
	uTexScaleX 			= glGetUniformLocation(program, "uTexCoordScaleX0");
	uTexScaleY 			= glGetUniformLocation(program, "uTexCoordScaleY0");
	uTexOffsetX 		= glGetUniformLocation(program, "uTexCoordOffsetX0");
	uTexOffsetY 		= glGetUniformLocation(program, "uTexCoordOffsetY0");
} 

void GladeRenderer::setLightUniforms(void)
{
	glUniform3f(uLightDirection, lightDirection[0], lightDirection[1], lightDirection[2]);
	glUniform4f(uLightAmbient, lightAmbient[0], lightAmbient[1], lightAmbient[2], lightAmbient[3]);
	glUniform4f(uLightDiffuse, lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], lightDiffuse[3]);
	glUniform4f(uLightSpecular, lightSpecular[0], lightSpecular[1], lightSpecular[2], lightSpecular[3]);
}

GLuint GladeRenderer::loadShader(GLuint shaderType, const char* shaderCode)
{
	GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    log("Shader compilation error:");
                    log(buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
	
    return shader;
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

/*
void GladeRenderer::setDrawingOrderComparator(Comparator<GladeObject> drawingOrderComparator)
{
	this->drawingOrderComparator = drawingOrderComparator;
}
	
void GladeRenderer::sortDrawables()
{
	if (drawingOrderComparator != NULL) {
		Collections.sort(sceneObjects, drawingOrderComparator);
	}
}

	
void GladeRenderer::addDrawFrameHook(DrawFrameHook hook)
{
	
}

*/