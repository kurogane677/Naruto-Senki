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
	void   AI_NarutoClone();
	void	AI_SageNarutoClone();
	void	AI_RikudoNarutoClone();
	void	AI_DogWall();
	void	AI_Guardian();
	
};

