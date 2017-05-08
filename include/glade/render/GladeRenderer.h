#pragma once

#include <vector>
#include <memory>
#include <set>

#include "glade/render/Perception.h"
#include "glade/math/Transform.h"
#include "glade/GladeObject.h"

class Vector3f;
class ShaderProgram;
class Texture;
class Widget;
class Perception;

namespace Glade {
  class Mesh;

  class Renderer {
  public:
    enum ProjectionMode {PERSPECTIVE, ORTHO};

    const float zNear, zFar;

  private:
    Perception  *perception;
    unsigned short viewportWidth, viewportHeight;
    float aspectRatio;
    ProjectionMode currentProjectionMode, sceneProjectionMode;
        
    float projectionMatrix[16], viewMatrix[16], viewWorldMatrix[16];
    Vector3f backgroundColor;
    
    std::vector<GladeObject*> sceneObjects; // maybe set?
    std::vector<GladeObject*> uiElements;   //  maybe set?

    std::unique_ptr<GladeObject::Comparator> drawingOrderComparator;

    bool initialized;

  public:
    Renderer(void);
    void onSurfaceCreated();
    void onSurfaceChanged(int width, int height);
    void add(GladeObject *sceneObject);
    void add(Widget *uiElement);
    void remove(GladeObject *sceneObject);
    void sortDrawables(void);
    void clear(void);
    void onDrawFrame(void);
    void setSceneProjectionMode(ProjectionMode projectionMode);
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
    bool unprojectPoint(float x, float y, float z, Vector3f &result);
    void setDrawingOrderComparator(std::unique_ptr<GladeObject::Comparator> &comparator);

    Transform *getCamera()
    {
      return perception ? perception->getTransform() : NULL;
    }

    Perception  *getPerception() { return perception; }
    void setPerception(Perception *perception) { this->perception = perception; }

  private:
    void moveAllObjectsIntoVideoMemory(void);
    void moveIntoVideoMemory(GladeObject &sceneObject);

    void compileShaderProgram(ShaderProgram *program);
    void bindBuffers(Mesh &mesh);
    void moveIntoVideoMemory(std::shared_ptr<Mesh> mesh);
    void moveIntoVideoMemory(std::shared_ptr<Texture> texture);
    void writeUniformsToVideoMemory(Drawable *drawable, ShaderProgram &program);

    int checkGLError();

    void drawAll(std::vector<GladeObject*>::iterator i, std::vector<GladeObject*>::iterator end, bool ui);
    void removeFromVideoMemory(Drawable &drawable);
    void removeAllObjectsFromVideoMemory(void);
    void draw(GladeObject::DrawablesI di, Transform &transform, bool ui);
    void switchProjectionMode(ProjectionMode projectionMode, bool force);
    void switchProjectionMode(ProjectionMode projectionMode);
    void getShaderHandles(ShaderProgram &shaderProgram);
  };
}