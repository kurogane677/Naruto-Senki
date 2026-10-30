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
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    ax::RenderView* pRenderView = pDirector->getRenderView();
    if (!pRenderView) {
#if (AX_TARGET_PLATFORM != AX_PLATFORM_ANDROID) && (AX_TARGET_PLATFORM != AX_PLATFORM_IOS)
        // 2x the 480x320 design resolution so the window is usable on modern screens
        pRenderView = ax::RenderViewImpl::createWithRect("NarutoSenki", ax::Rect(0, 0, 960, 640));
#else
        pRenderView = ax::RenderViewImpl::create("NarutoSenki");
#endif
        pDirector->setRenderView(pRenderView);
    }
	pRenderView->setDesignResolutionSize(480,320,ResolutionPolicy::FIXED_HEIGHT);

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
