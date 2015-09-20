#include <math.h>
#include <assert.h>

#include "glade/log/log.h"
#include "glade/math/util.h"
#include "glade/math/Matrix.h"
#include "glade/ui/Widget.h"
#include "glade/render/GladeRenderer.h"
#include "glade/render/Drawable.h"
#include "glade/render/ShaderProgram.h"
#include "glade/render/DrawFrameHook.h"
#include "glade/math/Vector.h"

const GLuint  GladeRenderer::POS_SIZE_FLOATS        = 3;
const GLuint  GladeRenderer::NORMAL_SIZE_FLOATS     = 3;
const GLuint  GladeRenderer::TEXCOORD_SIZE_FLOATS   = 2;
const GLuint  GladeRenderer::POS_OFFSET_FLOATS      = 0;
const GLuint  GladeRenderer::NORMAL_OFFSET_FLOATS   = POS_SIZE_FLOATS;
const GLuint  GladeRenderer::TEXCOORD_OFFSET_FLOATS = POS_SIZE_FLOATS + NORMAL_SIZE_FLOATS;
const GLuint  GladeRenderer::VERTEX_STRIDE_FLOATS   = POS_SIZE_FLOATS + NORMAL_SIZE_FLOATS + TEXCOORD_SIZE_FLOATS;
const GLsizei GladeRenderer::POS_OFFSET_BYTES       = POS_OFFSET_FLOATS      * sizeof(float);
const GLsizei GladeRenderer::NORMAL_OFFSET_BYTES    = NORMAL_OFFSET_FLOATS   * sizeof(float);
const GLsizei GladeRenderer::TEXCOORD_OFFSET_BYTES  = TEXCOORD_OFFSET_FLOATS * sizeof(float);
const GLsizei GladeRenderer::VERTEX_STRIDE_BYTES    = VERTEX_STRIDE_FLOATS   * sizeof(float);

GladeRenderer::GladeRenderer(void)
{
  camera.invertTranslation(true);
    
  sceneProjectionMode = PERSPECTIVE;
  initialized = false;
}


void GladeRenderer::onSurfaceCreated()
{
  glFrontFace(GL_CCW);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  moveAllObjectsIntoVideoMemory();
  log("Initialized renderer");
  initialized = true;
  checkGLError();
}


void GladeRenderer::onSurfaceChanged(int width, int height)
{
  viewportWidth = width;
  viewportHeight = height;
  aspectRatio = (float) viewportWidth / viewportHeight;
  glViewport(0, 0, viewportWidth, viewportHeight);
  
  switchProjectionMode(sceneProjectionMode, true);
  checkGLError();
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
    checkGLError();
  }
  
  uiElements.push_back(uiElement);
}

void GladeRenderer::clear(void)
{
  log("Clearing renderer");
  removeAllObjectsFromVideoMemory();
  
  GladeObject::Drawables* drawables;
  std::vector<GladeObject*>::iterator oi = sceneObjects.begin(); // must be a 'set'
  
  while (oi != sceneObjects.end()) {
    drawables = (*oi)->getDrawables();
    
    for (GladeObject::DrawablesI di = drawables->begin(); di != drawables->end(); di++) {
      (*di)->getTextureTransform()->removeAnimationCallbacks();
    }
    
    oi = sceneObjects.erase(oi);
  }

  checkGLError();
  
  std::vector<GladeObject*>::iterator wi = uiElements.begin(); // must be a 'set'
  
  while (wi != uiElements.end()) {
    drawables = (*wi)->getDrawables();
    
    for (GladeObject::DrawablesI di = drawables->begin(); di != drawables->end(); di++) {
      (*di)->getTextureTransform()->removeAnimationCallbacks();
    }
    
    wi = uiElements.erase(wi);
  }

  checkGLError();
  
  log("Done clearing renderer");
}

void GladeRenderer::onDrawFrame(void)
{
  for (DrawFrameHooksI hook = drawFrameHooks.begin(); hook != drawFrameHooks.end(); ++hook) {
    (*hook)->onBeforeDraw();
  }

  glClearColor(this->backgroundColor.x, this->backgroundColor.y, this->backgroundColor.z, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  switchProjectionMode(sceneProjectionMode);
  camera.getMatrix(viewMatrix);
  
  drawAll(sceneObjects.begin(), sceneObjects.end());
  
  switchProjectionMode(ORTHO);
  Matrix::setIdentityM(viewMatrix, 0);
  
  drawAll(uiElements.begin(), uiElements.end());
  
  for (DrawFrameHooksI hook = drawFrameHooks.begin(); hook != drawFrameHooks.end(); ++hook) {
    (*hook)->onAfterDraw();
  }
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


//std::vector<GladeObject>::iterator GladeRenderer::getWidgets(void)
//{
//  return uiElements.begin();
//}

void GladeRenderer::drawAll(std::vector<GladeObject*>::iterator i, std::vector<GladeObject*>::iterator end)
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

void GladeRenderer::compileShaderProgram(ShaderProgram *program)
{
  
  if (program == nullptr || program == NULL) {
    log("Warning: tried to compile a shader program, but it's null");
  }
  
  if (program->hasGpuHandle()) {
    return;
  }
    
  GLuint vertexShaderPointer = loadShader(GL_VERTEX_SHADER, program->vertexShaderSource);
  GLuint fragmentShaderPointer = loadShader(GL_FRAGMENT_SHADER, program->fragmentShaderSource);
  
  program->gpuHandle = glCreateProgram();
  glAttachShader(program->gpuHandle, vertexShaderPointer);
  glAttachShader(program->gpuHandle, fragmentShaderPointer);
  glLinkProgram(program->gpuHandle);
  
  glUseProgram(program->gpuHandle);
  
  GLint numberOfUniforms;
  glGetProgramiv(program->gpuHandle, GL_ACTIVE_UNIFORMS, &numberOfUniforms);
  
  GLchar buffer[128];
  GLsizei nameLength;
  GLenum type;
  GLint uniformSize;
  
  for (int i = 0; i < numberOfUniforms; ++i) {
    memset(buffer, '\0', 128);
    glGetActiveUniform(program->gpuHandle, i, 128, &nameLength, &uniformSize, &type, buffer);
    
    int uniformHandle = glGetUniformLocation(program->gpuHandle, buffer);
    assert(uniformHandle >= 0);
    program->saveUniformHandle(buffer, uniformHandle); //!!!
  }
}


void GladeRenderer::moveAllObjectsIntoVideoMemory(void)
{
  std::vector<GladeObject*>::iterator i = sceneObjects.begin();
  
  while (i != sceneObjects.end()) {
    moveIntoVideoMemory(**i);
    i++;
  }
  
  std::vector<GladeObject*>::iterator wi = uiElements.begin();
  
  while (wi != uiElements.end()) {
    moveIntoVideoMemory(**wi);
    wi++;
  }

  checkGLError();
}

void GladeRenderer::moveIntoVideoMemory(GladeObject &sceneObject)
{
  GladeObject::DrawablesI di = sceneObject.getDrawables()->begin();
  
  while (di != sceneObject.getDrawables()->end()) {
    moveIntoVideoMemory((*di)->getVertexObject());
    moveIntoVideoMemory((*di)->getTexture());
    compileShaderProgram((*di)->getShaderProgram().get());
    di++;
  }
  
  glReleaseShaderCompiler();
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

void GladeRenderer::moveIntoVideoMemory(std::shared_ptr<Texture> texturePtr)
{
  Texture *texture = texturePtr.get();
  
  if (nullptr == texture) {
    return;
  }
  
  if (texture->hasVideoBufferHandle()) {
    return;
  }
  
  if (NULL == texture->getData()) {
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
  Texture *texture = drawable.getTexture().get();
  VertexObject *mesh = drawable.getVertexObject();
  
  if (nullptr != texture && texture->hasVideoBufferHandle()) {
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
  std::vector<GladeObject*>::iterator i = sceneObjects.begin();
  
  while (i != sceneObjects.end()) {
    GladeObject::Drawables* drawables = (*i)->getDrawables();
    GladeObject::DrawablesI di = drawables->begin();
    
    while (di != drawables->end()) {
      removeFromVideoMemory(**di);
      di++;
    }
    
    i++;
  }

  std::vector<GladeObject*>::iterator wi = uiElements.begin();
  
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
  static bool firstCycle = true;

  if (firstCycle) {
    log("Started drawing");
    checkGLError();
  }
  
  std::shared_ptr<ShaderProgram> program = (*di)->getShaderProgram();
  
  if (nullptr == program) {
    log("Could not render a drawable: no GPU program");
    return;
  }
  
  if (program->gpuHandle < 0) {
    log("Could not render a drawable: GPU program was not loaded to video memory");
    return;
  }
  
  glUseProgram(program->gpuHandle);
  getShaderHandles(*program);

  if (firstCycle) {
    checkGLError();
  }
  
  static float worldMatrix[16];
  
  transform.getMatrix(worldMatrix);
  Matrix::multiplyMM(worldViewMatrix, 0, viewMatrix, 0, worldMatrix, 0);
  
  glUniformMatrix4fv(uProjectionMatrix, 1, GL_FALSE, projectionMatrix);// WHY it worked with 4 in GL?
  glUniformMatrix4fv(uWorldViewMatrix, 1, GL_FALSE, worldViewMatrix);
  
  if (firstCycle) {
    checkGLError();
  }
  
  bindBuffers(*(*di)->getVertexObject());
  
  if (firstCycle) {
    checkGLError();
  }
  
  glVertexAttribPointer(
    aPosition, POS_SIZE_FLOATS,
    GL_FLOAT, GL_FALSE,
    VERTEX_STRIDE_BYTES, (const GLvoid *)POS_OFFSET_BYTES
  );
  
  glEnableVertexAttribArray(aPosition);
  
  if (firstCycle) {
    checkGLError();
  }
  
  if (aNormal >= 0) {
    glVertexAttribPointer(
      aNormal, NORMAL_SIZE_FLOATS,
      GL_FLOAT, GL_FALSE,
      VERTEX_STRIDE_BYTES, (const GLvoid *)NORMAL_OFFSET_BYTES
    );
    
    glEnableVertexAttribArray(aNormal);
  }
  
  if (firstCycle) {
    checkGLError();
  }
  
  std::shared_ptr<Texture> texture = (*di)->getTexture();
  
  if (texture != nullptr) {
    if (texture->hasVideoBufferHandle()) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture->getVideoBufferHandle());

      if (firstCycle) {
        checkGLError();
      }
      
      TextureTransform* texTransform = (*di)->getTextureTransform();
      texTransform->executeCallbacks();
      
      glUniform1i(uSamplerNumber, 0);
      glUniform1f(uTexScaleX, texTransform->textureScaleX  / (float) texture->numberOfFrames * texTransform->getTextureScaleXModifierForFrame(*texture));
      glUniform1f(uTexScaleY, texTransform->textureScaleY  / (float) texture->numberOfAnimations * texTransform->getTextureScaleYModifierForFrame(*texture));
      glUniform1f(uTexOffsetX, texTransform->getCurrentFrameNumber(*texture) * texture->texCoordFrameWidth);
      glUniform1f(uTexOffsetY, texTransform->getCurrentAnimationNumber(*texture) * texture->texCoordFrameHeight);

      if (firstCycle) {
        checkGLError();
      }
      
      glVertexAttribPointer(
        aTexCoord, TEXCOORD_SIZE_FLOATS,
        GL_FLOAT, GL_FALSE,
        VERTEX_STRIDE_BYTES, (const GLvoid*)TEXCOORD_OFFSET_BYTES
      );

      if (firstCycle) {
        checkGLError();
      }
      
      glEnableVertexAttribArray(aTexCoord);
    }
  }
  
  Drawable::ShaderFloatUniformsCI fi = (*di)->floatUniformsBegin();
  GLint uniformHandle = -1;
  
  while (fi != (*di)->floatUniformsEnd()) {
    uniformHandle = program->getUniformHandle(fi->first);
    glUniform1f(uniformHandle, fi->second);
    ++fi;
  }
  
  if (firstCycle) {
    checkGLError();
  }
  
  Drawable::ShaderBoolUniformsCI bi = (*di)->boolUniformsBegin();
  
  while (bi != (*di)->boolUniformsEnd()) {
    uniformHandle = program->getUniformHandle(bi->first);
    glUniform1i(uniformHandle, bi->second ? 1 : 0);
    ++bi;
  }
  
  if (firstCycle) {
    checkGLError();
  }
  
  Drawable::ShaderIntUniformsCI ii = (*di)->intUniformsBegin();
  
  while (ii != (*di)->intUniformsEnd()) {
    uniformHandle = program->getUniformHandle(ii->first);
    glUniform1i(uniformHandle, ii->second);
    ++ii;
  }

  Drawable::ShaderVec3UniformsCI v3i = (*di)->vec3UniformsBegin();
  
  if (firstCycle) {
    checkGLError();
  }
  
  while (v3i != (*di)->vec3UniformsEnd()) {
    uniformHandle = program->getUniformHandle(v3i->first);
    glUniform3f(uniformHandle, v3i->second.x, v3i->second.y, v3i->second.z);
    ++v3i;
  }
  
  Drawable::ShaderVec4UniformsCI v4i = (*di)->vec4UniformsBegin();
  
  while (v4i != (*di)->vec4UniformsEnd()) {
    uniformHandle = program->getUniformHandle(v4i->first);
    glUniform4f(uniformHandle, v4i->second.x, v4i->second.y, v4i->second.z, v4i->second.w);
    ++v4i;
  }

  glDrawElements(GL_TRIANGLES, (*di)->getVertexObject()->getIndexBufferSize(), GL_UNSIGNED_SHORT, 0);
  
  glDisableVertexAttribArray(aPosition);
  glDisableVertexAttribArray(aNormal);
  glDisableVertexAttribArray(aTexCoord);

  if (firstCycle) {
    checkGLError();
  }
  
  firstCycle = false;
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
  uProjectionMatrix   = glGetUniformLocation(program.gpuHandle, "uProjectionMatrix");
  uWorldViewMatrix  = glGetUniformLocation(program.gpuHandle, "uWorldViewMatrix");

  aPosition      = glGetAttribLocation(program.gpuHandle, "aPosition");
  aNormal        = glGetAttribLocation(program.gpuHandle, "aNormal");
  aTexCoord      = glGetAttribLocation(program.gpuHandle, "aTexCoord0");
  
  uSamplerNumber  = glGetUniformLocation(program.gpuHandle, "uTextureSampler0");
  uTexScaleX       = glGetUniformLocation(program.gpuHandle, "uTexCoordScaleX0");
  uTexScaleY       = glGetUniformLocation(program.gpuHandle, "uTexCoordScaleY0");
  uTexOffsetX     = glGetUniformLocation(program.gpuHandle, "uTexCoordOffsetX0");
  uTexOffsetY     = glGetUniformLocation(program.gpuHandle, "uTexCoordOffsetY0");
} 

GLuint GladeRenderer::loadShader(GLuint shaderType, std::vector<char> &shader_source)
{
  GLuint shaderHandle = glCreateShader(shaderType);
  
  if (shaderHandle) {
    const char *shaderSourceLinePtr = shader_source.data();
    GLint shaderSourceSize = shader_source.size();
    glShaderSource(shaderHandle, 1, &shaderSourceLinePtr, &shaderSourceSize);
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
            log("Shader compilation error:");
            log(buf);
            free(buf);
            throw GladeException("Shader compilation error");
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
    default:
      log("No OpenGL error at this point");
      break;
  }
  
  return errorCode;
}

void GladeRenderer::addDrawFrameHook(DrawFrameHook &hook)
{
  drawFrameHooks.insert(&hook);
}


void GladeRenderer::setDrawingOrderComparator(std::unique_ptr<GladeObject::Comparator> &comparator)
{
  this->drawingOrderComparator = std::move(comparator);
}

void GladeRenderer::sortDrawables()
{
  log("Fixme: Sorting drawables not implemented");
  
  //if (drawingOrderComparator != NULL) {
  //  Collections.sort(sceneObjects, drawingOrderComparator);
  //}
}
