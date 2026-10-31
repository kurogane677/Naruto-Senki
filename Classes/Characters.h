#pragma once
#include "Compat/Compat2x.h"
#include "Element.h"


namespace ai { class AIController; }

class Hero : public HeroElement {
public:
	~Hero();
	CREATE_FUNC(Hero);
	bool	init();
	ai::AIController*	brain() const { return _brain; }

	

protected:
	// data-driven AI (Classes/AI); rebuilt when the hero transforms
	ai::AIController*	_brain=nullptr;
	std::string			_brainFor;

	void setAI(float dt);
	void   AI_NarutoClone();
	void   AI_Kurama();
	void   AI_Centipede();
	void	AI_Karasu();
	void	AI_Saso();
	void	AI_Parents();
	void	AI_Sanshouuo();
	void	AI_SageNarutoClone();
	void	AI_RikudoNarutoClone();
	void	AI_DevaPath();
	void	AI_AnimalPath();
	void	AI_AsuraPath();
	void	AI_Slug();
	void	AI_DogWall();
	void	AI_Guardian();
	void	AI_Akamaru();
	void    AI_Mask();
	
};

