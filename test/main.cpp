#include "glade/os/windows-inc.h"

#include <fstream>
#include <stdio.h>
#include <tchar.h>

#include "glade/exception/GladeException.h"
#include "glade/log/log.h"
#include "glade/opengl/functions.h"
#include "glade/render/GladeRenderer.h"
#include "glade/render/DrawFrameHook.h"
#include "glade/GladeObject.h"
#include "glade/render/Drawable.h"
#include "glade/render/meshes/Triangle.h"
#include "glade/render/Texture.h"
#include "glade/util/TextureLoader.h"
#include "glade/render/Shader.h"
#include "glade/Context.h"
#include "glade/State.h"
#include "glade/ui/Panel.h"
#include "glade/ui/Label.h"
#include "glade/ui/layout/SequenceLayout.h"
#include "glade/ui/font/BitmapFont.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// todo implement
class StandardDrawFrameHook : public DrawFrameHook
{
  virtual void onBeforeDraw(void)
  {
    
  }
  
	virtual void onAfterDraw(void)
  {
  }
};

class MyState : public State
{
  private:
    Texture* tex = NULL;
    Widget* wi = NULL;
    Drawable* wiView = NULL;
    Layout* layout = NULL;
    
  public:
    MyState(Context* context) : State(context)
    {
    }

    virtual void init()
    {
      log("State init");
      
      layout = new SequenceLayout();
     // wi = new Panel(layout);
     // wiView = new Drawable(Rectangle::INSTANCE);
      
     // tex = TextureLoader::loadTexture("notexture.png");
     // wiView->setTexture(tex);
     // wiView->setConstantColor(0.0f, 1.0f, 0.0f, 0.4f);
     // wi->getTransform()->setScale(0.5f, 0.5f, 1.0f);
     // wi->addDrawable(wiView);
      
      BitmapFont font("fonts/exo.gigantic.png", "fonts/exo.gigantic.csv");
      font.setFontSize(0.3f);
     
      wi = Label::forString(std::string("One Player"), font, layout, 0.0f, 1.0f, 0.0f);
      context->renderer->setBackgroundColor(0.05f, 0.4f, 0.2f);
      context->add(wi);
    }
    
    virtual void applyRules()
    {
      if (tex)
        delete tex;
      //if (wi)
      //  delete wi;
      if (wiView)
        delete wiView;
      if (layout)
        delete layout;
        
      if (wi) {
        for (GladeObject::DrawablesI di = wi->getDrawables()->begin(); di != wi->getDrawables()->end(); ++di)
          delete *di;
          
        delete wi;
      }
    }
    
    virtual void shutdown()
    {
    
    }
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch (message)
  {
  case WM_DESTROY:
      PostQuitMessage(0);
      break;
  default:
      return DefWindowProc(hWnd, message, wParam, lParam);
      break;
  }

  return 0;
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
  try {
    static TCHAR szWindowClass[] = _T("win32app");
    static TCHAR szTitle[] = _T("Glade native testing Application");  
    
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_OWNDC;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    
    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Glade native"),
            NULL);

        return 1;
    }

    // The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 500: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    
    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Glade native"),
            NULL);

        return 1;
    }
        
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
        PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
        32,                        //Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                        //Number of bits for the depthbuffer
        8,                        //Number of bits for the stencilbuffer
        0,                        //Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    
    HDC hdc = GetDC(hWnd);
    
    int pfNumber = ChoosePixelFormat(hdc, &pfd);
    
    if (!pfNumber) {
      MessageBox(NULL,
          _T("Couldn't get pixel format number"),
          _T("Glade native"),
          NULL);

      return 1;
    }
    
    SetPixelFormat(hdc, pfNumber, &pfd);
    
    HGLRC renderingContext = wglCreateContext(hdc);
    
    if (!renderingContext) {
      MessageBox(NULL,
        _T("Failed to create OpenGL context"),
        _T("Glade native"),
        NULL);
        
        return 1;
    }
    
    if (!wglMakeCurrent(hdc, renderingContext)) {
      MessageBox(NULL,
        _T("Failed to make OpenGL context current"),
        _T("Glade native"),
        NULL);
    }

    // OpenGL calls may start here
    
    //MessageBox(0,(TCHAR*)glGetString(GL_VERSION), "OPENGL VERSION",0);
    
    // load necessary extensions and function pointers
    loadFunctions();
    // also, re-create context using pixel format extensions if necessary
    
    // Create renderer and objects
    GladeRenderer renderer;
    
    std::ifstream inVertexShader("vertex.glsl");
    std::ifstream inFragmentShader("fragment.glsl");
    
    renderer.onSurfaceCreated(
      Shader(inVertexShader),
      Shader(inFragmentShader)
    );
        
    renderer.onSurfaceChanged(SCREEN_WIDTH, SCREEN_HEIGHT);

    /* 
    GladeObject object;
    Drawable view(Triangle::INSTANCE);
    view.setConstantColor(1.0f, 0.0f, 0.0f, 0.5f);
    view.setLit(false);
    
    Texture* texture = TextureLoader::loadTexture("notexture.png");
    view.setTexture(texture);
    
    object.addDrawable(&view);
    renderer.add(&object);
    
    renderer.setSceneProjectionMode(ORTHO);
    */
  
    Context context(&renderer);
    MyState state(&context);
    context.requestStateChange(&state);
    
    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    MSG msg;
    
    // we may start rendering
    for (;;) {
      while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT)
          goto cleanup;
        
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
            
      renderer.onDrawFrame();
      SwapBuffers(hdc);
      context.processRequests();
      
      Sleep(100);
    }
    
    cleanup:
    
    renderer.clear();
    
    if (!wglMakeCurrent(hdc, NULL)) { // no OpenGL calls beyond this point
      MessageBox(NULL,
        _T("Failed to make OpenGL context noncurrent"),
        _T("Glade native"),
        NULL);
    }
    
    wglDeleteContext(renderingContext); 
    
    OutputDebugString("Program is finishing");
    
    return (int) msg.wParam;
    
  } catch (GladeException e) {
    log("Uncaught GladeException: %s", e.getMessage()); // FIXME!! FREE GL CONTEXT AND RESOURCES!
  } catch (...) {
    log("Uncaught unknown exception");  // FIXME!! FREE GL CONTEXT AND RESOURCES!
  }
  
  return 1;
}

namespace gladen
{
  namespace system
  {
    void exit()
    {
      ::PostQuitMessage(0);
    }
  }
}