#pragma once
#include "Defines.h"
#include "Compat/Compat2x.h"
#include "SelectLayer.h"
#include "NetworkLayer.h"
#include "CreditsLayer.h"

#include "MyUtils/KTools.h"
#include "MyUtils/CCStrokeLabel.h"
#include "MyUtils/CCScrewLayer.h"


USING_NS_CC;
using namespace rapidjson; 


// declare menuButton 
enum btnType{
	Network,
	Training,
	Exit,
	Credits,
	HardCore
};

class StartMenu;

class MenuButton : public CCSprite, public CCTouchDelegate{

public:
	MenuButton(void);
	~MenuButton(void);

	bool				_isTop;
	bool				_isBottom;
	btnType				_btnType;

	virtual bool		init(const char *szImage);
	static MenuButton*	create(const char *szImage);
	
	CCRect				getRect();
	
	
	void				setBtnType(btnType type);
	btnType 			getBtnType();
	void				playSound();

	float			prePosY;
	


	CC_SYNTHESIZE(StartMenu*,_delegate,Delegate);



	
	
protected:

	virtual void	onEnter();
	virtual void	onExit();
	virtual bool	ccTouchBegan(CCTouch* touch,CCEvent* event);
	virtual void    ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void	ccTouchEnded(CCTouch* touch,CCEvent* event);

	bool			containsTouchLocation(CCTouch* touch);

};







class StartMenu : public CCLayer{

public:
	StartMenu(void);
	~StartMenu(void);



	bool				init();
	void				onTrainingCallBack();
	void				onHardCoreOn(CCObject* sender);
	void				onHardCoreOff(CCObject* sender);
	void				onHardLayerCallBack();
	void				onHardCoreCallBack();
	void				onNormalCallBack(CCObject* sender);
	void				openHeroSelect(bool isHardCore);
	void				onCreditsCallBack();
	void				onExitCallBack();

	void				scrollMenu(int posY);
	CCSprite*			menuText;

	MenuButton* hardCore_btn;

	CCLayer* hardCoreLayer;

	static const int	_pos01=48;
	static const int	_pos02=92;
	static const int	_pos03=150;

	bool  isClockwise;
	bool  isDrag;

	void setNotice();
	CCLayer* notice_layer;
	void update(float dt);
	CCLabelTTF* noticeLabel;

	virtual void   keyBackClicked();

	CC_SYNTHESIZE_RETAIN(CCArray*,_menu_array, Menus);
	CREATE_FUNC(StartMenu);

    

protected:
	void onEnter();
	void onExit();


};

