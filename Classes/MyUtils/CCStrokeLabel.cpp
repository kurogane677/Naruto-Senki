#include "CCStrokeLabel.h"



 CCStrokeLabel* CCStrokeLabel::create(CCLabelTTF* labelTTF,ccColor3B fullColor,ccColor3B StrokeColor,float strokeSize)
 {
	 CCStrokeLabel* tmp = new CCStrokeLabel;
	 tmp->autorelease();
	 tmp->m_label = labelTTF;
	 tmp->m_fullColor = fullColor;
	 tmp->m_StrokeColor = StrokeColor;
	 tmp->m_strokeSize = strokeSize;
	 tmp->init();
	 return tmp;
 }


bool  CCStrokeLabel::init()
{
	// cocos2d-x 2.x drew the label 24 times into a CCRenderTexture to fake a stroke.
	// Axmol labels support outlines natively. Note the legacy naming: m_fullColor is
	// the outline colour and m_StrokeColor the text colour.
	m_label->setColor(m_StrokeColor);
	m_label->enableOutline(ccc4(m_fullColor.r, m_fullColor.g, m_fullColor.b, 255), (int)getStrokeSize());
	m_label->setAnchorPoint(ccp(0.5f, 0.5f));
	m_label->setPosition(ccp(0, 0));
	setContentSize(m_label->getContentSize());
	addChild(m_label);
	m_sprite = NULL;
	return true;
}

float CCStrokeLabel::getStrokeSize()
{
	return m_strokeSize;
}
void CCStrokeLabel::setStrokeSize(float strokeSize)
{
	m_strokeSize = strokeSize;
}
