#include "Characters.h"
#include "AI/AIController.h"
#include "AI/SummonController.h"
USING_NS_CC;


bool Hero::init(){
	bool bRet = false;
	do {

		CC_BREAK_IF(!HeroElement::init());
		this->setAnchorPoint(ccp(0.5,0));		
		
		bRet=true;
	}while(0);

	return bRet;

};

Hero::~Hero(){
	delete _brain;
	delete _summon;
}

void Hero::setAI(float dt){

	if(_brainFor!=getCharacter()->getCString()){
		_brainFor=getCharacter()->getCString();
		delete _brain;
		delete _summon;
		_brain=ai::AIController::create(this);
		_summon=_brain ? nullptr : ai::SummonController::create(this);
	}
	if(_brain){
		_brain->tick(dt);
		return;
	}
	if(_summon){
		_summon->tick(dt);
		return;
	}

	// clones (cloneAI), DogWall and the guardians keep their small routines;
	// heroes and summons are handled above
	if(role()==Role::Clone){
		cloneAI();
		return;
	}

	if(isCharacter(CharacterId::DogWall)){
		AI_DogWall();
	}else if(isCharacter(CharacterId::Roshi) || isCharacter(CharacterId::Han)){
		AI_Guardian();
	}
}

void Hero::AI_DogWall(){
	
	this->attack(NAttack);

};

void Hero::AI_Guardian(){

	if(!this->findTargetEnemy("Hero",true)){
		if(!this->findTargetEnemy("Flog",true)){
			if(!this->findTargetEnemy("Flog",false)){
				if(!this->findTargetEnemy("Hero",false)){
					_mainTarget=NULL;
				}
			}
		}
	}
	

	if(_mainTarget){
		CCPoint moveDirection;
		CCPoint sp=ccpSub(_mainTarget->getPosition(),this->getPosition());



		if(abs(sp.x)> 128 || abs(sp.y)> 16 ){

			moveDirection =ccpNormalize(sp);
			this->walk(moveDirection);
			return;

		}else if( abs(sp.x)> 32 || abs(sp.y)>32 ){

			moveDirection =ccpNormalize(sp);
			this->walk(moveDirection);
			return;

		}else if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){
			bool isTurn=false;

			CCObject* pObject;
			CCARRAY_FOREACH(_delegate->_TowerArray,pObject){
				ActionManager* target=(ActionManager*) pObject;
				CharacterId gardTower;
				if(this->getDelegate()->zhenying>0){
					gardTower=CharacterId::AkatsukiCenter;
				}else{
					gardTower=CharacterId::KonohaCenter;
				}
				if(target->isCharacter(gardTower) && target->getHpPercent()<0.5f){
					isTurn=true;

				}
			}



			if(_isCanSkill1 && !_skillChangeBuffValue && (this->getHpPercent()<0.5f || isTurn)){
				this->changeSide(sp);
				this->attack(SKILL1);
				this->scheduleOnce(schedule_selector(Hero::enableSkill1),_sattackcoldDown1);
			}else if(_isCanSkill2 && _skillChangeBuffValue){
				this->changeSide(sp);
				this->attack(SKILL2);
				this->scheduleOnce(schedule_selector(Hero::enableSkill2),_sattackcoldDown2);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);
			}
		}
		return;

		

	}else{
		CCPoint moveDirection;
		CCPoint sp=ccpSub(this->getSpawnPoint(),this->getPosition());
		if(abs(sp.x)> 32 || abs(sp.y)> 32){

			moveDirection =ccpNormalize(sp);
			this->walk(moveDirection);
			return;
		}else{

			this->idle();
		}


	}

	
}

