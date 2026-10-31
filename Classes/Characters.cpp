#include "Characters.h"
#include "AI/AIController.h"
USING_NS_CC;


bool Hero::init(){
	bool bRet = false;
	do {

		CC_BREAK_IF(!HeroElement::init());
		this->setAnchorPoint(ccp(0.5,0));		
		
		//this->schedule(schedule_selector(Hero::neicun),0.5f);
		bRet=true;
	}while(0);

	return bRet;

};

Hero::~Hero(){
	delete _brain;
}

void Hero::setAI(float dt){

	if(_brainFor!=getCharacter()->getCString()){
		_brainFor=getCharacter()->getCString();
		delete _brain;
		_brain=ai::AIController::create(this);
	}
	if(_brain){
		_brain->tick(dt);
		return;
	}

	// clones, summons and guardians keep their small routines; heroes with a
	// profile in Resources/AI/profiles.json are handled above
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

	if(isCharacter("MaskRaidon") || isCharacter("MaskFudon") || isCharacter("MaskKadon")){
		AI_Mask();
	}else if(isCharacter("Slug")){
		AI_Slug();
	}else if(isCharacter("Centipede")){
		AI_Centipede();
	}else if(isCharacter("DogWall")){
		AI_DogWall();
	}else if(isCharacter("DevaPath")){
		AI_DevaPath();
	}else if(isCharacter("AsuraPath")){
		AI_AsuraPath();
	}else if(isCharacter("AnimalPath")){
		AI_AnimalPath();
	}else if(isCharacter("Kurama")){
		AI_Kurama();
	}else if(isCharacter("Akamaru")){
		AI_Akamaru();
	}else if(isCharacter("Karasu")){
		AI_Karasu();
	}else if(isCharacter("Saso")){
		AI_Saso();
	}else if(isCharacter("Parents")){
		AI_Parents();
	}else if(isCharacter("Sanshouuo")){
		AI_Sanshouuo();
	}else if(isCharacter("Roshi") || isCharacter("Han")){
		AI_Guardian();
	}
}

void Hero::AI_DevaPath(){

	if(!this->findEnemy("Hero",0)){
		if(!this->findEnemy("Flog",0)){
			if(!this->findEnemy("Tower",0)){
				_mainTarget=NULL;

			};
		}
	};

	if(_mainTarget){
			CCPoint moveDirection;
			CCPoint sp=ccpSub(_mainTarget->getPosition(),this->getPosition());

			if(_mainTarget->role()==Role::Tower||
				_mainTarget->role()==Role::Flog){


					if(abs(sp.x)> 32 || abs(sp.y)>32){

						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);

					}else{
						if( this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){
							this->changeSide(sp);
							this->attack(NAttack);
						}
					}
					return;


			}else {

				if(abs(sp.x)> 256  || abs(sp.y)> 16 ){

					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;

				}else if(abs(sp.x)> 32 || abs(sp.y)>32){

					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK ){

					bool isHaveKuilei1=false;
					bool isHaveKuilei2=false;

					if(getMonsterArray() && getMonsterArray()->count()>0){
						CCObject* pObject;
						CCARRAY_FOREACH(getMonsterArray(),pObject){
							Monster* mo=(Monster*) pObject;
							if (mo->isCharacter("AnimalPath")){
								isHaveKuilei1=true;
							}

							if (mo->isCharacter("AsuraPath")){
								isHaveKuilei2=true;
							}

						};
					}


					if(_isCanSkill1 && !isHaveKuilei1){

						this->attack(SKILL1);
						this->scheduleOnce(schedule_selector(Hero::enableSkill1),_sattackcoldDown1);

					}else if(_isCanSkill2 && !isHaveKuilei2){

						this->changeSide(sp);
						this->attack(SKILL2);
						this->scheduleOnce(schedule_selector(Hero::enableSkill2),_sattackcoldDown2);

					}else{

						this->changeSide(sp);
						this->attack(NAttack);
					}


				}
				return;

			}

	};

	this->stepOn();
}


void Hero::AI_AsuraPath(){


	if(!this->findEnemy("Hero",0)){
		if(!this->findEnemy("Flog",0)){
			if(!this->findEnemy("Tower",0)){
				_mainTarget=NULL;

			};
		}
	};

	if(_mainTarget){
			CCPoint moveDirection;
			CCPoint sp=ccpSub(_mainTarget->getPosition(),this->getPosition());

			if(_mainTarget->role()==Role::Tower||
				_mainTarget->role()==Role::Flog){


					if(abs(sp.x)> 32 || abs(sp.y)>32){

						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);

					}else{
						if( this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){
							this->changeSide(sp);
							this->attack(NAttack);
						}
					}
					return;


			}else {

				if(abs(sp.x)> 256  || abs(sp.y)> 32 ){

					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;

				}else if((abs(sp.x)> 32 || abs(sp.y)>32) && !_isCanSkill1  ){

					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK ){

					//后撤动作
					if(_isCanSkill1){

						if(abs(sp.x)<64 && this->getPositionX()>64 &&
							this->getPositionX()<(95*32-64)){
								if(this->stepBack())
									return;
						}
					}

					if(_isCanSkill1){
						this->changeSide(sp);
					
						this->attack(SKILL1);
						this->scheduleOnce(schedule_selector(Hero::enableSkill1),_sattackcoldDown1);

					}else{

						this->changeSide(sp);
						this->attack(NAttack);
					}


				}
				return;

			}

	};

	this->stepOn();
}

void Hero::AI_AnimalPath(){

	if(!this->findEnemy("Flog",0)){
		if(!this->findEnemy("Tower",0)){
			if(!this->findEnemy("Hero",0)){
				_mainTarget=NULL;

			};
		}
	};

	if(_mainTarget){
			CCPoint moveDirection;
			CCPoint sp=ccpSub(_mainTarget->getPosition(),this->getPosition());

			if(_mainTarget->role()==Role::Tower||
				_mainTarget->role()==Role::Flog){


					if(abs(sp.x)> 32 || abs(sp.y)>32){

						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);

					}else{
						if( this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){

							if(_isCanSkill1 && _mainTarget->role()!=Role::Tower ){
							
								this->attack(SKILL1);
								this->scheduleOnce(schedule_selector(Hero::enableSkill1),_sattackcoldDown1);

							}else{
								this->changeSide(sp);
								this->attack(NAttack);
							}
							
						}
					}
					return;


			}else {

				if(abs(sp.x)> 256  || abs(sp.y)> 32 ){

					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;

				}else if(abs(sp.x)> 32 || abs(sp.y)>32){

					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){

					if(_isCanSkill1){
					
						this->attack(SKILL1);
						this->scheduleOnce(schedule_selector(Hero::enableSkill1),10.0f);

					}else{

						this->changeSide(sp);
						this->attack(NAttack);
					}


				}
				return;

			}

	};

	this->stepOn();
}




void Hero::AI_Akamaru(){

	if(!this->findEnemy("Hero",winSize.width/2 -32,true)){
		if(!this->findEnemy("Flog",winSize.width/2 -32,true)){
			if(!this->findEnemy("Tower",winSize.width/2 -32,true)){
				this->_mainTarget=NULL;
			}
		}
	};


	CCPoint moveDirection;

	if(abs(ccpSub(_master->getPosition(),this->getPosition()).x) >winSize.width/2 -48){
		if( this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){
			moveDirection =ccpNormalize(ccpSub(_master->getPosition(),this->getPosition()));	
			this->walk(moveDirection);
			return;
		}
	}

	if(_mainTarget){
		
		CCPoint sp;
		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}

		if(_mainTarget->role()==Role::Flog){

			if(abs(sp.x)> 48 || abs(sp.y)> 32 ){		
				moveDirection =ccpNormalize(sp);
				this->walk(moveDirection);
			
			}else{	
				if(this->getActionState()==ACTION_STATE_IDLE || this->getActionState()==ACTION_STATE_WALK  || _actionState==ACTION_STATE_ATTACK){
					this->changeSide(sp);
					this->attack(NAttack);
				}
						
			}
			return;


		}else {

			if(abs(sp.x)>  48  || abs(sp.y)> 32 ){
			 
				moveDirection =ccpNormalize(sp);
				this->walk(moveDirection);
				return;
				
			}else{
				if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){
					
					if(_master->_isCanSkill2 && _mainTarget->getGP()<5000 && (_master->_isControled || _master->_isAI==true) && 
						(_master->getActionState()==ACTION_STATE_IDLE ||  _master->getActionState()==ACTION_STATE_WALK ||_master->getActionState()==ACTION_STATE_ATTACK)
						){
							this->changeSide(sp);
							if(_master->role()!=Role::Player){
								_master->attack(SKILL2);
							}else if(_master->role()==Role::Player){
								_master->attack(SKILL2);
							}

							
					}else{
						this->changeSide(sp);
						this->attack(NAttack);

					}

				}


			}

			return;

		}

	};

	if(abs(ccpSub(_master->getPosition(),this->getPosition()).x) >winSize.width/2 -64){

		CCPoint moveDirection =ccpNormalize(ccpSub(_master->getPosition(),this->getPosition()));	
		this->walk(moveDirection);
		return;
	}else{
		
		this->stepOn();
	}


};

void Hero::AI_Karasu(){

	

	if(!this->findEnemy("Hero",winSize.width/2 -32,true)){
		if(!this->findEnemy("Flog",winSize.width/2 -32,true)){
				this->_mainTarget=NULL;
		}
	};

	
	if(_mainTarget){
			CCPoint moveDirection;
			if(abs(ccpSub(_master->getPosition(),this->getPosition()).x) >winSize.width/2 -48){
			    if( this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){
				moveDirection =ccpNormalize(ccpSub(_master->getPosition(),this->getPosition()));	
				this->walk(moveDirection);
				return;
				}
			}
			CCPoint sp;
			if(_mainTarget->_originY){
				sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
			}else{
				sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
			}

			

			if(_mainTarget->role()==Role::Flog){

					if(abs(sp.x)> 32 || abs(sp.y)>32 ){
						if(this->_master->getActionState()== ACTION_STATE_IDLE ||
							this->_master->getActionState()== ACTION_STATE_WALK|| 
							this->_master->getActionState()== ACTION_STATE_ATTACK ||
							this->_master->getActionState()== ACTION_STATE_SATTACK ||
							this->_master->getActionState()== ACTION_STATE_OATTACK||
							this->_master->getActionState()== ACTION_STATE_O2ATTACK
							){
					
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
						}

					}else{
						if( this->getActionState()==ACTION_STATE_IDLE ||  
							this->getActionState()==ACTION_STATE_WALK || 
							this->getActionState()==ACTION_STATE_ATTACK){
							this->changeSide(sp);
							this->attack(NAttack);
						}
					}
					return;


			}else {

				if(abs(sp.x)> 32  || abs(sp.y)> 16 ){
					if(this->_master->getActionState()== ACTION_STATE_IDLE ||
						this->_master->getActionState()== ACTION_STATE_WALK || 
						this->_master->getActionState()== ACTION_STATE_ATTACK ||
						this->_master->getActionState()== ACTION_STATE_SATTACK ||
						this->_master->getActionState()== ACTION_STATE_OATTACK||
						this->_master->getActionState()== ACTION_STATE_O2ATTACK
						){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
					}
				}else if(this->getActionState()==ACTION_STATE_IDLE ||  
					this->getActionState()==ACTION_STATE_WALK || 
					this->getActionState()==ACTION_STATE_ATTACK){

					if(_master->getActionState()==ACTION_STATE_IDLE ||  
						_master->getActionState()==ACTION_STATE_WALK ||
						_master->getActionState()==ACTION_STATE_ATTACK){


						if(_master->_isCanSkill2  && _mainTarget->getGP()<5000 && (_master->_isControled || _master->_isAI==true)){

							this->changeSide(sp);
						
							if(_master->role()!=Role::Player){
								_master->attack(SKILL2);
							}else if(_master->role()==Role::Player){
								_master->attack(SKILL2);
							}

						}else{

							this->changeSide(sp);
							this->attack(NAttack);
						}

					}
					


				}
				return;

			}

	};

	if(abs(ccpSub(_master->getPosition(),this->getPosition()).x) >winSize.width/2 -64){
		CCPoint moveDirection =ccpNormalize(ccpSub(_master->getPosition(),this->getPosition()));	
		this->walk(moveDirection);
		return;
	}else{
		if(
			this->_master->getActionState()== ACTION_STATE_WALK  || 
			this->_master->getActionState()== ACTION_STATE_ATTACK ||
			this->_master->getActionState()== ACTION_STATE_SATTACK ||
			this->_master->getActionState()== ACTION_STATE_OATTACK
			){
			this->stepOn();
		}else{
			if (this->_actionState==ACTION_STATE_WALK || 
				this->_actionState==ACTION_STATE_ATTACK){
				this->idle();
			}
			
		}
	}
	

};

void Hero::AI_Saso(){



	if(!this->findEnemy("Hero",winSize.width/2 -32,true)){
		if(!this->findEnemy("Flog",winSize.width/2 -32,true)){
			this->_mainTarget=NULL;
		}
	};

	CCPoint moveDirection;
	if(abs(ccpSub(_master->getPosition(),this->getPosition()).x) >64){
		if( this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){
			moveDirection =ccpNormalize(ccpSub(_master->getPosition(),this->getPosition()));	
			this->walk(moveDirection);
			return;
		}
	}

	if(_mainTarget){
			

			CCPoint sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY?_mainTarget->_originY:_mainTarget->getPositionY()),
			ccp(this->getPositionX(),_originY?_originY:this->getPositionY()));

			if(_mainTarget->role()==Role::Flog){

				if(abs(sp.x)> 48 || abs(sp.y)> 16 ){
					if(this->_master->getActionState()== ACTION_STATE_IDLE ||
						this->_master->getActionState()== ACTION_STATE_WALK|| 
						this->_master->getActionState()== ACTION_STATE_ATTACK ||
						this->_master->getActionState()== ACTION_STATE_SATTACK ||
						this->_master->getActionState()== ACTION_STATE_OATTACK||
						this->_master->getActionState()== ACTION_STATE_O2ATTACK
						){

							moveDirection =ccpNormalize(sp);
							this->walk(moveDirection);

					}

				}else{
					if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){
						this->changeSide(sp);
						this->attack(NAttack);
					}
				}
				return;


			}else {

				if((abs(sp.x)> 48 || abs(sp.y)> 16)){
					if(this->_master->getActionState()== ACTION_STATE_IDLE ||
						this->_master->getActionState()== ACTION_STATE_WALK || 
						this->_master->getActionState()== ACTION_STATE_ATTACK ||
						this->_master->getActionState()== ACTION_STATE_SATTACK ||
						this->_master->getActionState()== ACTION_STATE_OATTACK||
						this->_master->getActionState()== ACTION_STATE_O2ATTACK
						){
							moveDirection =ccpNormalize(sp);
							this->walk(moveDirection);
							return;
					}
				}else if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){

					if(_isCanSkill1  && _mainTarget->getGP()<5000){
						this->changeSide(sp);
					
						this->attack(SKILL1);
						this->scheduleOnce(schedule_selector(Hero::enableSkill1),this->_sattackcoldDown1);

					}else{

						this->changeSide(sp);
						this->attack(NAttack);
					}


				}
				return;

			}

	};

	if(abs(ccpSub(_master->getPosition(),this->getPosition()).x) >winSize.width/2 -64){
		CCPoint moveDirection =ccpNormalize(ccpSub(_master->getPosition(),this->getPosition()));	
		this->walk(moveDirection);
		return;
	}else{
		
			if (this->_actionState==ACTION_STATE_WALK || 
				this->_actionState==ACTION_STATE_ATTACK){
				this->idle();
			}

	}


};

void Hero::AI_Parents(){



	if(!this->findEnemy("Hero",winSize.width/2 -32,true)){

		if(!this->findEnemy("Flog",48,true)){
			if(!this->findEnemy("Tower",48,true)){
			this->_mainTarget=NULL;
			}
		}
	};
	
	CCPoint moveDirection;
	
	if(abs(ccpSub(_master->getPosition(),this->getPosition()).x) >9 && !_skillChangeBuffValue){
		if( this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){
			moveDirection =ccpNormalize(ccpSub(_master->getPosition(),this->getPosition()));	
			this->walk(moveDirection);
			return;
		}
	}
	if(_mainTarget){


		CCPoint sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY?_mainTarget->_originY:_mainTarget->getPositionY()),
			ccp(this->getPositionX(),_originY?_originY:this->getPositionY()));

		if(_mainTarget->role()==Role::Flog || _mainTarget->role()==Role::Tower ){

			
			if(abs(sp.x)> 48 || abs(sp.y)> 32 ){
			
			}else{
				if((this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK )  && !_skillChangeBuffValue){
					this->changeSide(sp);
					this->attack(NAttack);
				}
			}
			return;


		}else {
			if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK ){
				if(_master->getActionState()==ACTION_STATE_IDLE ||  
					_master->getActionState()==ACTION_STATE_WALK ||
					_master->getActionState()==ACTION_STATE_ATTACK){

					if(_master->_isCanSkill3  && _mainTarget->getGP()<5000 && (_master->_isControled || _master->_isAI==true) && !_skillChangeBuffValue){

							this->changeSide(sp);

							_master->attack(SKILL3);
							

					}else if(abs(sp.x)> 48 || abs(sp.y)> 32  ){
						if(_skillChangeBuffValue &&  this->getActionState()!=ACTION_STATE_ATTACK){
							moveDirection =ccpNormalize(sp);
							this->walk(moveDirection);
							return;
						}
					}else{
					
						if(_master->_isCanSkill2  && _mainTarget->getGP()<5000 && (_master->_isControled || _master->_isAI==true) && !_skillChangeBuffValue){

							this->changeSide(sp);				
							_master->attack(SKILL2);
						
						}else{

							this->changeSide(sp);
							this->attack(NAttack);
						}


					}
				}
			}
			
			return;

		}

	};

	if(abs(ccpSub(_master->getPosition(),this->getPosition()).x) >9){
		 if( this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK ){
			moveDirection =ccpNormalize(ccpSub(_master->getPosition(),this->getPosition()));	
			this->walk(moveDirection);
			return;
		}
	}
	
	if (this->_actionState==ACTION_STATE_WALK ){
			this->idle();
	}


};

void Hero::AI_Sanshouuo(){


	if(!this->findEnemy("Hero",winSize.width/2 -32,true)){
		if(!this->findEnemy("Flog",winSize.width/2 -32,true)){
			this->_mainTarget=NULL;
		}
	};

	if(_mainTarget){
			CCPoint moveDirection;
			if(abs(ccpSub(_master->getPosition(),this->getPosition()).x) >winSize.width/2 -64){
				if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){
				moveDirection =ccpNormalize(ccpSub(_master->getPosition(),this->getPosition()));	
				this->walk(moveDirection);
				return;
				}
			}

			CCPoint sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY?_mainTarget->_originY:_mainTarget->getPositionY()),
				ccp(this->getPositionX(),_originY?_originY:this->getPositionY()));
					

			if(_mainTarget->role()==Role::Flog){


					if(abs(sp.x)> 32 || abs(sp.y)>32 ){
						 if(this->_master->getActionState()== ACTION_STATE_IDLE ||
							 this->_master->getActionState()== ACTION_STATE_WALK|| 
							this->_master->getActionState()== ACTION_STATE_ATTACK ||
							this->_master->getActionState()== ACTION_STATE_SATTACK ||
							this->_master->getActionState()== ACTION_STATE_OATTACK||
							this->_master->getActionState()== ACTION_STATE_O2ATTACK
							){
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						}

					}else{
						if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){
							this->changeSide(sp);
							this->attack(NAttack);
						}
					}
					return;


			}else {

				if(abs(sp.x)> 96  || abs(sp.y)> 32 ){
					if(this->_master->getActionState()== ACTION_STATE_IDLE ||
						this->_master->getActionState()== ACTION_STATE_WALK  || 
						this->_master->getActionState()== ACTION_STATE_ATTACK ||
						this->_master->getActionState()== ACTION_STATE_SATTACK ||
						this->_master->getActionState()== ACTION_STATE_OATTACK||
						this->_master->getActionState()== ACTION_STATE_O2ATTACK
						){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
					}
				}else if((abs(sp.x)> 32 || abs(sp.y)>32) && !_isCanSkill1 ){
					if(this->_master->getActionState()== ACTION_STATE_WALK || 
						this->_master->getActionState()== ACTION_STATE_ATTACK ||
						this->_master->getActionState()== ACTION_STATE_SATTACK ||
						this->_master->getActionState()== ACTION_STATE_OATTACK||
						this->_master->getActionState()== ACTION_STATE_O2ATTACK
						){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
					}
				}else if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){

					if(_isCanSkill1  && _mainTarget->getGP()<5000){
						this->changeSide(sp);
					
						this->attack(SKILL1);
						this->scheduleOnce(schedule_selector(Hero::enableSkill1),_sattackcoldDown1);
					}else{
						this->changeSide(sp);
						this->attack(NAttack);
					}

				}
				return;

			}

	};

	if(abs(ccpSub(_master->getPosition(),this->getPosition()).x) >winSize.width/2 -64){
		CCPoint moveDirection =ccpNormalize(ccpSub(_master->getPosition(),this->getPosition()));	
		this->walk(moveDirection);
		return;
	}else{
		if(this->_master->getActionState()== ACTION_STATE_WALK  || 
			this->_master->getActionState()== ACTION_STATE_ATTACK ||
			this->_master->getActionState()== ACTION_STATE_SATTACK ||
			this->_master->getActionState()== ACTION_STATE_OATTACK
			){
			this->stepOn();
		}else{
			if (this->_actionState==ACTION_STATE_WALK || this->_actionState==ACTION_STATE_ATTACK){
				this->idle();
			}
			
		}
	}
	

};


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

void Hero::AI_Kurama(){



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

		if(abs(sp.x)> 156 || abs(sp.y)> 48 ){

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


void Hero::AI_Mask(){

	if(this->isCharacter("MaskRaidon")){

		if(!this->findEnemy("Flog",0)){
			if(!this->findEnemy("Hero",0)){
				if(!this->findEnemy("Tower",0)){
					_mainTarget=NULL;
				};
			}
		};
	}else{

		
		if(!this->findEnemy("Hero",0)){
			if(!this->findEnemy("Flog",0)){
				if(!this->findEnemy("Tower",0)){
					_mainTarget=NULL;
				};
			}
		};
	}
	

	if(_mainTarget){
		CCPoint moveDirection;
		CCPoint sp;
		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}

		if(_mainTarget->role()==Role::Tower){

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

			if(abs(sp.x)> 96 || abs(sp.y)> 16 ){

				moveDirection =ccpNormalize(sp);
				this->walk(moveDirection);
				return;

			}else if((abs(sp.x)> 48  || abs(sp.y)> 16) && !_isCanSkill1 ){

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



void Hero::AI_Slug(){

	
	if(!this->findEnemy("Flog",0)){
		if(!this->findEnemy("Tower",0)){
			_mainTarget=NULL;
		};
	};
	

	if(_mainTarget){
			CCPoint moveDirection;
			CCPoint sp=ccpSub(_mainTarget->getPosition(),this->getPosition());

			if(_mainTarget->role()==Role::Tower||
				_mainTarget->role()==Role::Flog){


					if(abs(sp.x)> 32 || abs(sp.y)>32){

						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);

					}else{
						if(this->getActionState()==ACTION_STATE_IDLE ||  this->getActionState()==ACTION_STATE_WALK || this->getActionState()==ACTION_STATE_ATTACK){

							if(_isCanSkill1){
							
								this->attack(SKILL1);
								this->scheduleOnce(schedule_selector(Hero::enableSkill1),_sattackcoldDown1);

							}else if(_isCanSkill2){
								
								this->attack(SKILL2);
								this->scheduleOnce(schedule_selector(Hero::enableSkill2),_sattackcoldDown2);

							}else{
								this->changeSide(sp);
								this->attack(NAttack);
							}
						

						}
					}

					return;


			}
	};

	this->stepOn();


};


void Hero::AI_Centipede(){


	if(!this->findEnemy("Flog",0)){
		if(!this->findEnemy("Tower",0)){
			_mainTarget=NULL;
		};
	};


	if(_mainTarget){
		CCPoint moveDirection;
		CCPoint sp=ccpSub(_mainTarget->getPosition(),this->getPosition());

		if(_mainTarget->role()==Role::Tower||
			_mainTarget->role()==Role::Flog){


				if(abs(sp.x)> 32 || abs(sp.y)>32){

					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);

				}else{

					this->changeSide(sp);
					this->attack(NAttack);
					
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

