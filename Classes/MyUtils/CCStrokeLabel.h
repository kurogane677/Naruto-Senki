#ifndef __CCSTROKELABEL_H__
#define __CCSTROKELABEL_H__
#include "Compat/Compat2x.h"


class CCStrokeLabel : public CCNode {
    
public:
    
	 virtual bool init();
	static CCStrokeLabel* create(CCLabelTTF* labelTTF,ccColor3B fullColor,ccColor3B StrokeColor,float strokeSize);
	float getStrokeSize();
	void setStrokeSize(float strokeSize);
    
private:

	CCSprite* m_sprite;
	CCLabelTTF* m_label;
	ccColor3B m_fullColor;
	ccColor3B m_StrokeColor;
	float m_strokeSize;
};

#endif
