#include "glade/render/GladeRenderer.h"
#include "glade/render/DrawFrameHook.h"
#include "glade/Context.h"
#include "glade/util/FileManager.h"
#include "glade/util/DesktopFileManager.h"
#include "glade/util/ResourceManager.h"
#include "glade/glade-game.h"

FileManager *file_manager = NULL;

static Glade::Renderer *renderer = NULL;
static Context *gameContext = NULL;

class StandardDrawFrameHook : public DrawFrameHook
{
  private:
    Context* context;
  
  public:
    StandardDrawFrameHook(Context &context) : context(&context)
    {
    }
   
    virtual void onBeforeDraw(void)
    {
      context->processRequests();
      
      if (context->getCurrentState() != NULL) {
        if (context->enableSimulator) {
          context->getSimulator()->stepSimulation(context->timer.getDeltaTime());  
        }
        
        if (context->enableCollisionDetector) {
          context->getCollisionDetector()->detectCollisions(context->timer.getDeltaTime());
        }
        
        if (context->enableAiContainer) {
          //context->getAiContainer()->process(context->getCurrentState());
        }
        
        context->getCurrentState()->applyRules(*context);
        
        if (context->enableSoundPlayer) {
          //context->getSoundPlayer()->process();
        }
      }
    }
    
    virtual void onAfterDraw(void)
    {
      if (context->getCurrentState() != NULL) {
        context->timer.reset();
      }
    }
};

static StandardDrawFrameHook *drawHook = nullptr;

extern "C" void glade_init(int viewport_width, int viewport_height, const char* resourcesDir)
{
  printf("Glade init\n");

  renderer = new Glade::Renderer();
  renderer->onSurfaceCreated();
  renderer->onSurfaceChanged(viewport_width, viewport_height);

  gameContext = new Context(renderer);
  drawHook = new StandardDrawFrameHook(*gameContext);
  renderer->addDrawFrameHook(*drawHook);

  ::file_manager = new DesktopFileManager(resourcesDir);

  InitialGameData gameData;
  ::glade_game_init(&gameData, ::file_manager);

  gameContext->requestStateChange(std::move(gameData.initialState));
}

extern "C" void glade_tick()
{
  printf("Glade tick\n");
  renderer->onDrawFrame();
}

extern "C" void glade_deinit()
{
  delete renderer;
}

namespace gladen {
  namespace system {
    void exit()
    {
    }
  }
}
