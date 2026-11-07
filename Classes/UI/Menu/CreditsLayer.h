#pragma once
#include "Compat/Compat2x.h"
#include "Defines.h"
USING_NS_CC;


class GameLayer;
class CreditsLayer : public CCLayer{

public:
	CreditsLayer(void);
	~CreditsLayer(void);

	bool	init();


	CREATE_FUNC(CreditsLayer);

	virtual void keyBackClicked();

};
