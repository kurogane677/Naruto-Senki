#pragma once
#include "Compat/Compat2x.h"
#include "Element.h"


namespace ai { class AIController; class SummonController; }

class Hero : public HeroElement {
public:
	~Hero();
	CREATE_FUNC(Hero);
	bool	init();
	ai::AIController*	brain() const { return _brain; }

	

protected:
	// data-driven AI (Classes/AI); rebuilt when the hero transforms
	ai::AIController*	_brain=nullptr;
	ai::SummonController*	_summon=nullptr;
	std::string			_brainFor;

	void setAI(float dt);
	// the routine of a clone or summon that no AI controller drives; none by default
	virtual void	ownAI() {}
	void	AI_Guardian();
	
};

