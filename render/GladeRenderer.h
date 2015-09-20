#pragma once

#include <vector>
#include "../opengl/functions.h"
#include "../math/Vector.h"
#include "../math/Transform.h"
#include "Drawable.h"
#include "../GladeObject.h"
#include "../#ui/Widget.h"
#include "DrawFrameHook.h"
#include "../render/Shader.h"

using namespace std;

enum ProjectionMode {PERSPECTIVE, ORTHO};

class GladeRenderer {
public:
  static const GLuint POS_SIZE_FLOATS;
  static const GLuint COLOR_SIZE_FLOATS;
  static const GLuint NORMAL_SIZE_FLOATS;
  static const GLuint TEXCOORD_SIZE_FLOATS;
  static const GLuint POS_OFFSET_FLOATS;
  static const GLuint COLOR_OFFSET_FLOATS;
  static const GLuint NORMAL_OFFSET_FLOATS;
  static const GLuint TEXCOORD_OFFSET_FLOATS;
  static const GLuint VERTEX_STRIDE_FLOATS;
  static const GLsizei POS_OFFSET_BYTES;
  static const GLsizei COLOR_OFFSET_BYTES;
  static const GLsizei NORMAL_OFFSET_BYTES;
  static const GLsizei TEXCOORD_OFFSET_BYTES;
  static const GLsizei VERTEX_STRIDE_BYTES;
	
	static const double DOUBLE_CUBE_DIAGONAL;
	
  typedef std::set<DrawFrameHook*> DrawFrameHooks;
  typedef DrawFrameHooks::iterator DrawFrameHooksI;
  
private:
    // shader program handle
	GLuint program;

	// handles to shader uniforms
	GLuint  uProjectionMatrix, uWorldViewMatrix,
			uMaterialAmbient, uMaterialDiffuse, uMaterialSpecular, uMaterialShininess,
			uReplaceColor, uColor, uLight, uLightDirection, uLightAmbient, uLightDiffuse, uLightSpecular, uLightHalfplane,
			uSamplerNumber, uTexOffsetX, uTexOffsetY, uTexScaleX, uTexScaleY;
	
	// handles to shader attributes
    GLuint aPosition, aNormal, aTexCoord;
    
	// directional light (hardcoded)
    float lightDirection[3], lightAmbient[4], lightDiffuse[4], lightSpecular[4];
	
	unsigned short viewportWidth, viewportHeight;
	float aspectRatio;
	ProjectionMode currentProjectionMode, sceneProjectionMode;
			
	float projectionMatrix[16], viewMatrix[16], worldViewMatrix[16];
	Vector3f backgroundColor;
	
	vector<GladeObject*> sceneObjects; // must be set
	vector<GladeObject*> uiElements;   // must be set
  
  DrawFrameHooks drawFrameHooks;
/*	Comparator<GladeObject> drawingOrderComparator; */
	
	bool initialized;

public:
	GladeRenderer(void);
  Transform camera;
  
  void add(GladeObject* sceneObject);
	void add(Widget* uiElement);
	void sortDrawables(void);
	void clear(void);
	void onSurfaceCreated(Shader &vertexShader, Shader &fragmentShader);
	void onSurfaceChanged(int width, int height);
	void onDrawFrame(void);
	void setSceneProjectionMode(ProjectionMode projectionMode);
	void addDrawFrameHook(DrawFrameHook* hook);
	int getViewportWidth(void);
	int getViewportHeight(void);
	void setBackgroundColor(float r, float g, float b);
	int percentToPixels(float percent);
	float pixelsToPercent(float pixels);
  Transform getTransformForRootWidget(void);
	void moveZeroToUpperLeftCorner(void);
	float getHalfViewportWidthCoords(void);
	float getHalfViewportHeightCoords(void);
	float getViewportWidthCoords(void);
	float getViewportHeightCoords(void);
	Vector2f getPointCoords(float screenX, float screenY);
// vector<Widget*>::iterator getWidgets(void);
private:
	void drawAll(vector<GladeObject*>::iterator i, vector<GladeObject*>::iterator end);
	void moveAllObjectsIntoVideoMemory(void);
	void moveIntoVideoMemory(GladeObject &sceneObject);
	void moveIntoVideoMemory(Drawable &drawable);
	void removeFromVideoMemory(Drawable &drawable);
	void removeAllObjectsFromVideoMemory(void);
	void draw(GladeObject::DrawablesI di, Transform &transform);
	void bindBuffers(Drawable &drawable);
	void switchProjectionMode(ProjectionMode projectionMode, bool force);
	void switchProjectionMode(ProjectionMode projectionMode);
	void getShaderHandles(void);
	void setLightUniforms(void);
	GLuint loadShader(GLuint type, Shader &shader);
	int checkGLError();
};
