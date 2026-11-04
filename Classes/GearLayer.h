#pragma once
#include "Compat/Compat2x.h"
#include "Defines.h"

USING_NS_CC;

class GameLayer;
class Hero;
class ScrewLayer;


enum gearbtnType{
	buyType,
	sellType
};

class GearLayer : public CCLayer{

public:
	GearLayer(void);
	~GearLayer(void);

	bool	init(CCRenderTexture* snapshoot);
	static GearLayer*	create(CCRenderTexture* snapshoot);
	
	CCLayer* gears_layer;
	CCLayer* currentGear_layer;
	CCLabelBMFont* coinLabel;

	CCSprite* gears_bg;

	gearType currentGear;
	void	updatePlayerGear();
	void	updateGearList();
	void	showGearDetail(gearType type);
	bool	isInGearList(CCTouch* touch);

	ScrewLayer* _screwLayer;

private:
	void	onResume(CCObject* sender);
	void	onGearBuy(CCObject* sender);
	void	onGearUpgrade(CCObject* sender);
	void	onGearSell(CCObject* sender);

	// Name and description from Element/gears.xml, drawn with a bundled TTF
	ax::ValueMap	_gearTexts;
	ax::Label*		_gearName;
	ax::Label*		_gearDesc;
	ax::Label*		_gearDescEn;
	ax::Label*		_gearInfo;
	// Buy for a gear not owned yet, Upgrade + Sell for an owned one
	CCMenu*			_buyMenu;
	CCMenu*			_ownedMenu;
	ax::MenuItemSprite*	_upgradeItem;
	ax::MenuItemSprite*	_sellItem;
	ax::Label*		_upgradePrice;
	ax::Label*		_sellPrice;
	CCNode*			_listClip;
	
	CC_SYNTHESIZE(GameLayer*,_delegate,Delegate);
	

};





class GearButton : public CCSprite, public CCTouchDelegate{

public:
	GearButton(void);
	~GearButton(void);

	gearType				_gearType;
	gearbtnType  			_btnType;

	virtual bool		init(const char *szImage);
	static GearButton*	create(const char *szImage);

	CCRect				getRect();
	void				setBtnType(gearType type,gearbtnType type2,int level);
	gearType 			getBtnType();
	void				playSound();

	bool				_isBuyed;
	CCPoint				_touchStart;

	CC_SYNTHESIZE(GearLayer*,_delegate,Delegate);

protected:

	virtual void	onEnter();
	virtual void	onExit();
	virtual bool	ccTouchBegan(CCTouch* touch,CCEvent* event);
	virtual void    ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void	ccTouchEnded(CCTouch* touch,CCEvent* event);

	bool			containsTouchLocation(CCTouch* touch);



};



class ScrewLayer : public CCLayer{

public:
	ScrewLayer(void);
	~ScrewLayer(void);

	float			prePosY;
	virtual	bool	init();
	CREATE_FUNC(ScrewLayer);

	CC_SYNTHESIZE(GearLayer*,_delegate,Delegate);

	CCSprite* screwBar;

	int totalRow;
	int gearNum;
	float	maxScroll();
	void	updateScrollBar();
	CC_SYNTHESIZE_RETAIN(CCArray*,_gearArray,GearArray);

protected:

	virtual bool	ccTouchBegan(CCTouch* touch,CCEvent* event);
	virtual void    ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void	ccTouchEnded(CCTouch* touch,CCEvent* event);


	void onEnter();
	void onExit();


};