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

	// Naruto clones, DogWall and the guardians keep their small routines;
	// heroes and summons are handled above
	if(role()==Role::Clone){
		if(isCharacter("Naruto")){
			AI_NarutoClone();
		}else if(isCharacter("SageNaruto")){
			AI_SageNarutoClone();
		}else if(isCharacter("RikudoNaruto")){
			AI_RikudoNarutoClone();
		}
		return;
	}

	if(isCharacter("DogWall")){
		AI_DogWall();
	}else if(isCharacter("Roshi") || isCharacter("Han")){
		AI_Guardian();
	}
}

void Hero::AI_NarutoClone(){

	
	if(!this->findEnemy("Flog",0)){
		if(!this->findEnemy("Hero",0)){
			if(!this->findEnemy("Tower",0)){
				_mainTarget=NULL;
			};

		}
	};

	if(_mainTarget){

		CCPoint moveDirection;
		CCPoint sp;
		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}

		if(abs(sp.x)> 32 || abs(sp.y)>32){

			moveDirection =ccpNormalize(sp);
			this->walk(moveDirection);

		}else{
			if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){
				this->changeSide(sp);
				this->attack(NAttack);
			}
		}
		return;
	}

	this->stepOn();


};

void Hero::AI_SageNarutoClone(){

	if(!this->findEnemy("Hero",0)){
		if(!this->findEnemy("Flog",0)){
			if(!this->findEnemy("Tower",0)){
				_mainTarget=NULL;
			};
		}
	};

	if(_mainTarget){
			CCPoint moveDirection;
			CCPoint sp;
			if(_mainTarget->_originY){
				sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
			}else{
				sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
			}

			if(_mainTarget->role()==Role::Tower||
				_mainTarget->role()==Role::Flog){

					if(abs(sp.x)> 32 || abs(sp.y)>32){

						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);

					}else{
						if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){
						
							this->changeSide(sp);
							this->attack(NAttack);
						}
					}
					return;
			}else {

				if(abs(sp.x)> 128 || abs(sp.y)> 16 ){

					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;

				}else if((abs(sp.x)> 48  || abs(sp.y)> 8) && !_isCanSkill1 ){

					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){

					//后撤动作
					if(_isCanSkill1 && abs(sp.x)<32){
						this->stepBack();
						return;
					}

					if(_isCanSkill1  && _mainTarget->getGP()<5000){
						this->changeSide(sp);
						this->attack(SKILL1);
					}else{
						this->changeSide(sp);
						this->attack(NAttack);
					}
				}
				return;

			}

	};

	this->stepOn();


};


void Hero::AI_RikudoNarutoClone(){

	if(!this->findEnemy("Hero",0)){
		if(!this->findEnemy("Flog",0)){
			if(!this->findEnemy("Tower",0)){
				_mainTarget=NULL;
			};
		}
	};

	if(_mainTarget){
		CCPoint moveDirection;
		CCPoint sp;
		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}

		if(_mainTarget->role()==Role::Tower||
			_mainTarget->role()==Role::Flog){

				if(abs(sp.x)> 32 || abs(sp.y)>32){

					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);

				}else{
					if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){

						this->changeSide(sp);
						this->attack(NAttack);
					}
				}
				return;
		}else {

			if(abs(sp.x)> 128 || abs(sp.y)> 16 ){

				moveDirection =ccpNormalize(sp);
				this->walk(moveDirection);
				return;

			}else if((abs(sp.x)> 48  || abs(sp.y)> 8) && !_isCanSkill1 ){

				moveDirection =ccpNormalize(sp);
				this->walk(moveDirection);
				return;
			}else if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){


				if(_isCanSkill1  && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL1);
				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}
			}
			return;

		}

	};

	this->stepOn();


};


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
				const char* gardTower;
				if(this->getDelegate()->zhenying>0){
					gardTower="AkatsukiCenter";
				}else{
					gardTower="KonohaCenter";
				}
				if(strcmp(target->getCharacter()->getCString(),gardTower)==0 && target->getHpPercent()<0.5f){
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

