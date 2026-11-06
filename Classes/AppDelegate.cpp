#include "AppDelegate.h"
#include "GameScene.h"

USING_NS_CC;
using namespace CocosDenshion;



AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

void AppDelegate::initGfxContextAttrs()
{
    // red,green,blue,alpha,depth,stencil,multisamplesCount; stencil is needed by CCClippingNode
    GfxContextAttrs gfxContextAttrs = {8, 8, 8, 8, 24, 8, 0};
    ax::RenderView::setGfxContextAttrs(gfxContextAttrs);
}


bool AppDelegate::applicationDidFinishLaunching() {
    // seed rand() once (critical hits, effects, save keys)
    srand((unsigned)time(nullptr));

    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    ax::RenderView* pRenderView = pDirector->getRenderView();
    if (!pRenderView) {
#if (AX_TARGET_PLATFORM != AX_PLATFORM_ANDROID) && (AX_TARGET_PLATFORM != AX_PLATFORM_IOS)
        // 16:9 window (720p); NS_WINDOW=WxH (e.g. 1280x576 for 20:9) tests other screen shapes
        int windowWidth = 1280, windowHeight = 720;
        if (const char* windowSize = getenv("NS_WINDOW"))
            sscanf(windowSize, "%dx%d", &windowWidth, &windowHeight);
        pRenderView = ax::RenderViewImpl::createWithRect("NarutoSenki", ax::Rect(0, 0, windowWidth, windowHeight));
#else
        pRenderView = ax::RenderViewImpl::create("NarutoSenki");
#endif
        pDirector->setRenderView(pRenderView);
    }
	// The design is 568x320. On wider screens the height stays 320 and the width follows
	// the screen, so there are no black bars (#20); gameplay ranges use kGameWidth, not
	// the screen width. Narrower screens (4:3/16:10 tablets) keep SHOW_ALL: a taller view
	// would show past the top of the map.
	ax::Size frameSize = pRenderView->getFrameSize();
	bool isWideScreen = frameSize.width * 320 >= frameSize.height * 568;
	pRenderView->setDesignResolutionSize(568,320,isWideScreen ? ResolutionPolicy::FIXED_HEIGHT : ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    pDirector->setStatsDisplay(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = GameScene::create();

    // run
    pDirector->runWithScene(pScene);

    return true;
}


// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
    void AppDelegate::applicationDidEnterBackground() {


		

    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();

	

}

// this function will be called when the app is active again
    void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();

}
