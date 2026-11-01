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
        // 16:9 window (720p) for the 568x320 design resolution
        pRenderView = ax::RenderViewImpl::createWithRect("NarutoSenki", ax::Rect(0, 0, 1280, 720));
#else
        pRenderView = ax::RenderViewImpl::create("NarutoSenki");
#endif
        pDirector->setRenderView(pRenderView);
    }
	// 16:9 design (the menu backgrounds are 570x320). SHOW_ALL keeps it 16:9 on every
	// device, with bars on wider/narrower screens: walk speed and AI ranges scale
	// with winSize.width, so letting the width follow the screen would change gameplay.
	pRenderView->setDesignResolutionSize(568,320,ResolutionPolicy::SHOW_ALL);

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
