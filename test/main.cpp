#include "../os/windows-inc.h"

#include <stdio.h>
#include <tchar.h>

#include "../exception/GladeException.h"
#include "../log/log.h"
#include "../opengl/functions.h"
#include "../render/GladeRenderer.h"
#include "../GladeObject.h"
#include "../render/Drawable.h"
#include "../render/samples/Cube.h"
#include "../render/samples/Triangle.h"

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
        500, 500,
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
    renderer.onSurfaceCreated();
    renderer.onSurfaceChanged(500, 500);
    renderer.setBackgroundColor(1.0, 1.0, 1.0);
    
    /*
    GladeObject object;
    Drawable view(Triangle::INSTANCE);
    view.setConstantColor(1.0, 0, 0);
    object.addDrawable(&view);
    renderer.add(&object);
    renderer.setSceneProjectionMode(ORTHO);
    renderer.camera.setPosition(0.0f, 0.0f, -1.0f);
    view.setLit(false);*/
    
    GladeObject object;
    Drawable view(Triangle::INSTANCE);
    view.setConstantColor(1.0f, 0.0f, 0.0f, 1.0f);
    view.setLit(false);
    object.addDrawable(&view);
    renderer.add(&object);
    
    renderer.setSceneProjectionMode(PERSPECTIVE);
    renderer.camera.setPosition(0.0f, 0.0f, 2.0f);
    
    float param = 0.0f;
    
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
          break;
        
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      
      if (msg.message == WM_QUIT)
        break;
      
      
      renderer.onDrawFrame();
      SwapBuffers(hdc);
      
      //view.getTransform()->setRotation(param, param, 0.0f);
      renderer.camera.setPosition(0.0f, 0.0f, 2.0f);
      param -= 0.1f;
      Sleep(100);
    }
    
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
    log("Uncaught GladeException: %s", e.getMessage());
  } catch (...) {
    log("Uncaught unknown exception");
  }
  
  return 1;
}