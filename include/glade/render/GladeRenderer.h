#pragma once

#include <vector>
#include <memory>

#ifndef ANDROID
#include "glade/opengl/functions.h"
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include "glade/math/globals.h"
#include "glade/math/Vector.h"
#include "glade/math/Transform.h"
#include "glade/GladeObject.h"
#include "glade/ui/Widget.h"
#include "DrawFrameHook.h"
#include "Drawable.h"
#include "ShaderProgram.h"

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
  
  typedef std::set<DrawFrameHook*> DrawFrameHooks;
  typedef DrawFrameHooks::iterator DrawFrameHooksI;
  
private:
    // shader program handle
  GLuint program;

  // handles to shader uniforms
  GLuint  uProjectionMatrix, uWorldViewMatrix,
      uSamplerNumber, uTexOffsetX, uTexOffsetY, uTexScaleX, uTexScaleY;
  
  // handles to shader attributes
    GLuint aPosition, aNormal, aTexCoord;
  
  unsigned short viewportWidth, viewportHeight;
  float aspectRatio;
  ProjectionMode currentProjectionMode, sceneProjectionMode;
      
  float projectionMatrix[16], viewMatrix[16], worldViewMatrix[16];
  Vector3f backgroundColor;
  
  vector<GladeObject*> sceneObjects; // maybe set?
  vector<GladeObject*> uiElements;   //  maybe set?
  
  DrawFrameHooks drawFrameHooks;
  std::unique_ptr<GladeObject::Comparator> drawingOrderComparator;
  
  bool initialized;

public:
  GladeRenderer(void);
  Transform camera;
  
  void add(GladeObject *sceneObject);
  void add(Widget *uiElement);
  void sortDrawables(void);
  void clear(void);
  void onSurfaceCreated();
  void onSurfaceChanged(int width, int height);
  void onDrawFrame(void);
  void setSceneProjectionMode(ProjectionMode projectionMode);
  void addDrawFrameHook(DrawFrameHook &hook);
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
  void setDrawingOrderComparator(std::unique_ptr<GladeObject::Comparator> &comparator);
  
private:
  void drawAll(vector<GladeObject*>::iterator i, vector<GladeObject*>::iterator end);
  void moveAllObjectsIntoVideoMemory(void);
  void moveIntoVideoMemory(GladeObject &sceneObject);
  void moveIntoVideoMemory(VertexObject *mesh);
  void moveIntoVideoMemory(std::shared_ptr<Texture> texture);
  void removeFromVideoMemory(Drawable &drawable);
  void removeAllObjectsFromVideoMemory(void);
  void draw(GladeObject::DrawablesI di, Transform &transform);
  void bindBuffers(VertexObject &mesh);
  void switchProjectionMode(ProjectionMode projectionMode, bool force);
  void switchProjectionMode(ProjectionMode projectionMode);
  void getShaderHandles(ShaderProgram &shaderProgram);
  GLuint loadShader(GLuint type, std::vector<char> &shader_source);
  void compileShaderProgram(ShaderProgram *program);
  int checkGLError();
};
