#include "Characters.h"
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

void Hero::setAI(float dt){

	if(isCharacter("Sakura")){
		AI_Sakura();
	}else if(isCharacter("Pain")){
		AI_Pain();
	}else if(isCharacter("MaskRaidon")){
		AI_Mask();
	}else if(isCharacter("MaskFudon")){
		AI_Mask();
	}else if(isCharacter("MaskKadon")){
		AI_Mask();
	}else if(isCharacter("Slug")){
		AI_Slug();
	}else if(isCharacter("Centipede")){
		AI_Centipede();
	}else if(isCharacter("Nagato")){
		AI_Nagato();
	}else if(isCharacter("Asuma")){
		AI_Asuma();
	}else if(isCharacter("DevaPath")){
		AI_DevaPath();
	}else if(isCharacter("AsuraPath")){
		AI_AsuraPath();
	}else if(isCharacter("AnimalPath")){
		AI_AnimalPath();
	}else if(isCharacter("Sai")){
		AI_Sai();
	}else if(isCharacter("Tenten")){
		AI_Tenten();
	}else if(isCharacter("Suigetsu")){
		AI_Suigetsu();
	}else if(isCharacter("Konan")){
		AI_Konan();
	}else if(isCharacter("Jiraiya")){
		AI_Jiraiya();
	}else if(isCharacter("Shikamaru")){
		AI_Shikamaru();
	}else if(isCharacter("Sasuke")){
		AI_Sasuke();
	}else if(isCharacter("ImmortalSasuke")){
		AI_ImmortalSasuke();
	}else if(isCharacter("Deidara")){
		AI_Deidara();
	}else if(isCharacter("Minato")){
		AI_Minato();
	}else if(isCharacter("DogWall")){
		AI_DogWall();
	}else if(isCharacter("Kakashi")){
		AI_Kakashi();
	}else if(isCharacter("Tobi")){
		AI_Tobi();
	}else if(isCharacter("Hinata")){
		AI_Hinata();
	}else if(isCharacter("Neji")){
		AI_Neji();
	}else if(isCharacter("Choji")){
		AI_Choji();
	}else if(isCharacter("Itachi")){
		AI_Itachi();
	}else if(isCharacter("Orochimaru")){
		AI_Orochimaru();
	}else if(isCharacter("Naruto") &&
		role()==Role::Clone 
		){
			AI_NarutoClone();
	}else if(isCharacter("Naruto")){
		AI_Naruto();
	}else if(isCharacter("Kurama")){
		AI_Kurama();
	}else if(isCharacter("SageNaruto") &&
		role()==Role::Clone 
		){
			AI_SageNarutoClone();
	}else if(isCharacter("RikudoNaruto") &&
		role()==Role::Clone 
		){
			AI_RikudoNarutoClone();
	}else if(isCharacter("SageNaruto")
		){
			AI_SageNaruto();
	}else if(isCharacter("RikudoNaruto")){
		AI_RikudoNaruto();
	}else if(isCharacter("Gaara")){
		AI_Gaara();
	}else if(isCharacter("Tobirama")){
		AI_Tobirama();
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
	}else if(isCharacter("Kankuro")){
		AI_Kankuro();
	}else if(isCharacter("Chiyo")){
		AI_Chiyo();
	}else if(isCharacter("Kiba")){
		AI_Kiba();
	}else if(isCharacter("SageJiraiya")){
		AI_SageJiraiya();
	}else if(isCharacter("Karin")){
		AI_Karin();
	}else if(isCharacter("Lee")){
		AI_Lee();
	}else if(isCharacter("RockLee")){
		AI_RockLee();
	}else if(isCharacter("Tsunade")){
		AI_Tsunade();
	}else if(isCharacter("Jugo")){
		AI_Jugo();
	}else if(isCharacter("Kisame")){
		AI_Kisame();
	}else if(isCharacter("Ino")){
		AI_Ino();
	}else if(isCharacter("Hidan")){
		AI_Hidan();
	}else if(isCharacter("Shino")){
		AI_Shino();
	}else if(isCharacter("Hiruzen")){
		AI_Hiruzen();
	}else if(isCharacter("Kakuzu")){
		AI_Kakuzu();
	}else if(isCharacter("Roshi") ||
		isCharacter("Han")){
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

void Hero::AI_Naruto(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear03);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear07);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}
	

	if(this->checkRetri()){

		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			
			if(this->stepBack()){
				return;
			};
		}
	};
	
	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			
			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game  &&  !_isBati){
				this->changeSide(sp);
				this->attack(OUGIS2);
				return;
			}else if(_isCanSkill2 && !_isBati){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL2);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isBati && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {
					this->idle();
					return;
				}
			}else if(abs(sp.x)<128){

				//交战策略
				if(abs(sp.x)> 96 || abs(sp.y)> 32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if(abs(sp.x)<32 && ( _isCanSkill1 || _isCanOugis1 || _isCanSkill3) ){
					this->stepBack();
					return;
				}

				if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 && !_isBati){
					this->changeSide(sp);
					this->attack(OUGIS1);
				}else if(_isCanGear03){
					this->useGear(gear03);
				}else if(_isCanSkill3 && !_isBati && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL3);
				}else if(_isCanSkill1 && !_isBati && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL1);
				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
			
		}
			

	}
	//无玩家情况
	
	_mainTarget=NULL;
	if(!this->findEnemy2("Flog")){
		this->findEnemy2("Tower");
	}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill2 && !_isBati){
				this->changeSide(sp);
				this->attack(SKILL2);
				return;
			}else {
				this->changeSide(sp);
				this->attack(NAttack);
			}	
		}

		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}
	
}

void Hero::AI_SageNaruto(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear03);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear07);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}


	if(this->checkRetri()){
			if(_mainTarget!=NULL){
				if(this->stepBack2()){
					return;
				};
			}else{
				
				if(this->stepBack()){
					return;
				};
			}
	};
	
	
	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}


	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){

			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game &&  !_isBati){
				this->changeSide(sp);
				this->attack(OUGIS2);
				return;
			}else if(_isCanSkill2 && !_isBati){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL2);
				return;
			}else if(_isCanSkill3 && !_isBati && _mainTarget->getGP()<5000){	
				this->changeSide(sp);
				this->attack(SKILL3);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isBati && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {
					this->idle();
					return;
				}
			}else if(abs(sp.x)<128){

				//交战策略
				if(abs(sp.x)> 96 || abs(sp.y)> 32){				
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if(abs(sp.x)<32 && _isCanSkill1){
					this->stepBack();
					return;
				}
				if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 && !_isBati){
					this->changeSide(sp);
					this->attack(OUGIS1);
				}else if(_isCanGear03){
					this->useGear(gear03);
				}else if(_isCanSkill1 && !_isBati && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL1);
				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill2 && !_isBati){
				this->changeSide(sp);
				this->attack(SKILL2);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);
			}

			
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}




}


void Hero::AI_RikudoNaruto(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear03);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear07);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}


	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(_isCanGear00){
				this->useGear(gear00);
			};
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}


	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){

			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game &&  !_isBati){
				this->changeSide(sp);
				this->attack(OUGIS2);
				return;
			}else if(_isCanSkill2 && !_isBati){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL2);
				return;
			}else if(_isCanSkill3 && !_isBati  && _mainTarget->getGP()<5000){
				this->changeSide(sp);
				this->attack(SKILL3);
				return;

			}else if(_isCanSkill1 && !_isBati && _mainTarget->getGP()<5000){
				this->changeSide(sp);
				this->attack(SKILL1);
				return;
			}else if(_isCanGear03){
				this->useGear(gear03);
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isBati && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {
					this->idle();
					return;
				}
			}else if(abs(sp.x)<128){

				//交战策略
				if(abs(sp.x)> 32 || abs(sp.y)> 32){	
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 && !_isBati){
					this->changeSide(sp);
					this->attack(OUGIS1);
				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill1  && !_isBati && _mainTarget->role()==Role::Flog){
				this->changeSide(sp);
				this->attack(SKILL1);
			}else if(_isCanSkill2 && !_isBati){
				this->changeSide(sp);
				this->attack(SKILL2);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);
			}


		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}




}


void Hero::AI_Asuma(){
	_mainTarget=NULL;
	this->findEnemy2("Hero");


	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}
	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear05);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			
			if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000){
				this->changeSide(sp);
				this->attack(OUGIS1);
				return;
			}else if(_isCanSkill3 && _mainTarget->getGP()<5000){
				//先手无视战斗力
				if((abs(sp.x)> 96 || abs(sp.y)> 32) ){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				this->changeSide(sp);
				this->attack(SKILL3);
				return;
			}else if(_isCanSkill2 && _mainTarget->getGP()<5000){
				//先手无视战斗力
				if((abs(sp.x)> 96 || abs(sp.y)> 32) ){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				this->changeSide(sp);
				this->attack(SKILL2);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else{
					this->idle();
					return;
				}
			}else if(abs(sp.x)<128){

				//交战策略
				if(abs(sp.x)> 96 || abs(sp.y)> 32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if((abs(sp.x)> 32 || abs(sp.y)>32) && !_isCanSkill2 ){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
					&& _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
					){
						this->changeSide(sp);
						this->attack(OUGIS2);
				}else if(_isCanSkill1 && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL1);

				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}


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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill2 && _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL2);

			}else if(_isCanSkill1 && _mainTarget->role()==Role::Flog ){
				this->changeSide(sp);
				this->attack(SKILL1);

			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}

}


void Hero::AI_Kakashi(){
	_mainTarget=NULL;
	this->findEnemy2("Hero");


	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}
	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear05);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}
	
	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};
	
	
	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			
			if(_isCanSkill3){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL3);
				return;
			}else if(_isCanSkill2 && _mainTarget->getGP()<5000){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL2);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {				
					this->idle();
					return;				
				}
			}else if(abs(sp.x)<128){
	
				//交战策略
				if((abs(sp.x)> 64 || abs(sp.y)> 32 ) ){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				
				 if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
					 && _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
					 && _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
					 && !_mainTarget->_isSticking){
						 this->changeSide(sp);						
						 this->attack(OUGIS2);
				 }else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000){
					 this->changeSide(sp);
					 this->attack(OUGIS1);
				 }else if(_isCanSkill1 && _mainTarget->getGP()<5000){
					 this->changeSide(sp);
					 this->attack(SKILL1);
				 }else{
					 this->changeSide(sp);
					 this->attack(NAttack);
				 }
				
				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
	if(!this->findEnemy2("Flog")){
		this->findEnemy2("Tower");
	}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill3 && _mainTarget->role()==Role::Flog){
				this->changeSide(sp);
				this->attack(SKILL3);
				
			}else if(_isCanSkill1 && _mainTarget->role()==Role::Flog ){
				this->changeSide(sp);
				this->attack(SKILL1);

			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}

}



void Hero::AI_Konan(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");


	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear03);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear05);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}

	
	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};
	
	
	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK ){
			
			if(_isCanSkill1 && !_isBati){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL1);
				return;
			}else if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
				&& _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
				&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
				&& !_mainTarget->_isSticking
				&& !_isBati
				){
					if((abs(sp.x)> 48 || abs(sp.y)> 16)){
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}

					this->changeSide(sp);
					this->attack(OUGIS2);
					return;

			 }else if(_mainTarget->getGP()<5000 && !_isVisable && !_isBati
				&& (_isCanSkill3 || _isCanSkill2)  &&
				_mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
				){
				//先手无视战斗力
			
				if((abs(sp.x)> 128 || abs(sp.y)> 16)){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if(abs(sp.x) < 96){
					this->stepBack();
					return;
				}
			
				if(_isCanSkill3){
					this->changeSide(sp);
					this->attack(SKILL3);				
				}else if(_isCanSkill2){
					this->changeSide(sp);
					this->attack(SKILL2);					
				}

				return;
		
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {		
					this->idle();
					return;		
				}
			}else if(abs(sp.x)<128){

				//交战策略
				if((abs(sp.x)> 8 || abs(sp.y)> 8) && _isBati){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if((abs(sp.x)> 48 || abs(sp.y)>32 )){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if(abs(sp.x)<32 && !_isBati){
					this->stepBack();
					return;
				}

				
			    if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 && !_isBati){
					this->changeSide(sp);
					this->attack(OUGIS1);
				}else if(_isCanSkill2 && !_isBati  && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL2);
				}else{
					if(_isCanGear03){
						this->useGear(gear03);
					}
					if(!_isBati){
						this->changeSide(sp);
						this->attack(NAttack);
					}
				
				}
			
				return;
			}
		}
	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	
	if(_mainTarget){

		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}

		if((abs(sp.x)> 8 || abs(sp.y)> 8) && _isBati){
			moveDirection =ccpNormalize(sp);
			this->walk(moveDirection);
			return;
		}else if(abs(sp.x)> 32 || abs(sp.y)>32){
			moveDirection =ccpNormalize(sp);
			this->walk(moveDirection);
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			if(_isCanGear03){
				this->useGear(gear03);
			}
			//技能清兵
			if(_isCanOugis1  && !_isControled && !_isBati 
				&& _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				
				this->attack(OUGIS1);
			}else if(_isCanSkill2 && !_isBati
				&& _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL2);
			}else if(!_isBati){
			this->changeSide(sp);
			this->attack(NAttack);
			}
		}
		return;

	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}



}

void Hero::AI_Deidara(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear00);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear01);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear05);
		}
	}



	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				if(_isCanGear00 && !_isBati){
					this->useGear(gear00);
				};
				return;
			};
		}else{

			if(this->stepBack()){
				return;
			};
		}
	};
	
	
	
	if(_mainTarget && (battleCondiction>=0 || _isCanSkill3 ||  _isCanOugis1 || _isBati)){
	
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){

			if(_isCanOugis2 && !_isControled 
			&& _delegate->_isOugis2Game 
			&& _mainTarget->getGP()<5000  
			&&!_isBati){
				if(abs(sp.x)> 48  || abs(sp.y)> 16){
					if(_isCanGear00){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;

				}else{
					this->changeSide(sp);
					this->attack(OUGIS2);
				}
			}else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 &&!_isBati){
				this->changeSide(sp);
				this->attack(OUGIS1);
				return;
			}else if(_isCanSkill3 && _mainTarget->getGP()<5000  &&!_isBati){
				//先手无视战斗力
				if(abs(sp.x)> 64  || abs(sp.y)> 16){
					if(_isCanGear00){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				this->changeSide(sp);
				this->attack(SKILL3);	
				return;
			}else if(_isCanSkill2  && !_isBati){
				this->changeSide(sp);
				this->attack(SKILL2);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160 ){
					this->stepBack2();
					return;
				}else {
					if(_skillChangeBuffValue){
						if(!_isBati){
							this->changeSide(sp);
							this->attack(NAttack);
						}	
					}
					this->idle();
					return;
				
				}

			}else if(_isCanSkill1 &&!_isBati){
				this->changeSide(sp);
				this->attack(SKILL1);
				return;

			}else if(abs(sp.x)<128){

				//交战策略
				if( (abs(sp.x)> 8  || abs(sp.y)> 8) && _isBati){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if((abs(sp.x)> 32 || abs(sp.y)>32) && !_isCanSkill1 && !_skillChangeBuffValue){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				
					
				if(!_isBati){
					this->changeSide(sp);
					this->attack(NAttack);
				}		
			
				return;
			}
		}

	}
	
	//无玩家情况
	if(battleCondiction>=0){
		_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	}else {
		_mainTarget=NULL;
		this->findEnemy2("Tower");
	}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_mainTarget->role()==Role::Tower &&
				_isCanOugis2 &&
				!_isControled && _delegate->_isOugis2Game 
				 && !_isBati && isBaseDanger){

				this->changeSide(sp);
				this->attack(OUGIS2);

			}else if(_isCanSkill1 && !_isBati && _mainTarget->role()==Role::Tower  ){
				this->changeSide(sp);
				this->attack(SKILL1);

			}else if(_isCanSkill2  &&!_isBati){
				this->changeSide(sp);
				this->attack(SKILL2);
				return;
			}else if(!_isBati){
				this->changeSide(sp);
				this->attack(NAttack);
			}
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		if(_isCanGear00 && !_isBati){
			this->useGear(gear00);
		};
		this->stepOn();
	}

}


void Hero::AI_Ino(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	if(_skillChangeBuffValue){
		return;
	}

	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear05);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear01);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};



	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(this->getMaxHP()- this->getHP()>=3000 && 
		this->getCoin()>=50 && !_isHealling && _isCanItem1 && _isBati){
			this->setItem(Item1);

	}
	

	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK ){

			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game){
				if(abs(sp.x)> 96 || abs(sp.y)> 16){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else{
					this->changeSide(sp);		
					this->attack(OUGIS2);
				}

				return;
			}else if( _mainTarget->getGP()<5000 
				&& (_isCanSkill3 || _isCanSkill2) ){
				//先手无视战斗力

				if(abs(sp.x)> 96  || abs(sp.y)> 16){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				if(_isCanSkill2){
					this->changeSide(sp);
					this->attack(SKILL2);	
				}else if(_isCanSkill3){
					this->changeSide(sp);
					this->attack(SKILL3);	
				}		

				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {
					this->idle();
					return;
				}
			}else if(abs(sp.x)<128){

				//交战策略
			    if(abs(sp.x)> 32  || abs(sp.y)> 32 ){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				if(_isCanOugis1  
					&& !_isControled  
					&& _mainTarget->getGP()<5000){
					this->changeSide(sp);	
					this->attack(OUGIS1);
				}else if(_isCanSkill1){
					this->changeSide(sp);
					this->attack(SKILL1);
				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_mainTarget->role()==Role::Flog && _isCanSkill1 ){
				this->changeSide(sp);
				this->attack(SKILL1);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);
			}
		}
		return;
	}
	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}

}

void Hero::AI_Sai(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear05);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear01);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};
	

	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){

			
			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game &&!_isBati 
			&& _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
			&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
			&& !_mainTarget->_isSticking
			){
				if(abs(sp.x)> 48 || abs(sp.y)> 16){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else{
					this->changeSide(sp);

					this->attack(OUGIS2);
				}
				return;
			}else if(_isCanSkill3 && _mainTarget->getGP()<5000  &&!_isBati){
				//先手无视战斗力
		
				if(abs(sp.x)> 64  || abs(sp.y)> 16){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else{
				
					this->changeSide(sp);
					this->attack(SKILL3);
				
				} 
				return;

			}else if(_isCanSkill2  && _mainTarget->getGP()<5000 &&!_isBati){
				this->changeSide(sp);
				this->attack(SKILL2);
				return;
			}else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 &&!_isBati){

				this->changeSide(sp);
				this->attack(OUGIS1);
				return;

			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {
					if(_skillChangeBuffValue){
						if(!_isBati){
							this->changeSide(sp);
							this->attack(NAttack);
						}	
					}
					this->idle();
					return;
					
				}
	
			}else if(_isCanSkill1 &&!_isBati){
				this->changeSide(sp);
				this->attack(SKILL1);
				return;
			}else if(abs(sp.x) <128){

				//交战策略			
				if( (abs(sp.x)> 8  || abs(sp.y)> 8) && _isBati){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if((abs(sp.x)> 32  || abs(sp.y)> 32 )  && !_isCanSkill1 && !_skillChangeBuffValue){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
			
				if(!_isBati){
					this->changeSide(sp);
					this->attack(NAttack);
				}
				
				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_mainTarget->role()==Role::Flog && _isCanSkill1 && !_isBati){
				this->changeSide(sp);
				this->attack(SKILL1);
			}else if(_isCanSkill2  && _mainTarget->getGP()<5000 &&!_isBati
				 && _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL2);
			}else if(!_isBati){
				this->changeSide(sp);
				this->attack(NAttack);
			}
		}
		return;
	}
	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}

}



void Hero::AI_Sakura(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	
	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear00);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear04);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear08);
		}
	}

	
	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(_isCanGear00){
				this->useGear(gear00);
			};
			if(this->stepBack()){
				return;
			};
		}
	};

	if(_mainTarget && (battleCondiction>=0 || _isCanOugis1 || _isCanOugis2)){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){

			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game && !_skillChangeBuffValue
			&& _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
			&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
			&& !_mainTarget->_isSticking 
			){
				if(abs(sp.x)> 48 || abs(sp.y)> 16){
					if(_isCanGear00){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				this->changeSide(sp);
				this->attack(OUGIS2);
				return;
			}else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 && !_skillChangeBuffValue){
				if(abs(sp.x)> 48 || abs(sp.y)> 16){
					if(_isCanGear00){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				
				this->changeSide(sp);
				this->attack(OUGIS1);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160 ){
					this->stepBack2();
					return;
				}else {	
					this->idle();
					return;
				}
			}else if(this->getHpPercent()<0.9 && _isCanSkill1){
				//先手无视战斗力
				this->attack(SKILL1);
				return;
			}else if(abs(sp.x) <128){

				//交战策略
				if((abs(sp.x)> 64 || abs(sp.y)> 32) && (!_isCanSkill2 ||  _mainTarget->getGP()<5000) ){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if(abs(sp.x)> 32 || abs(sp.y)>32 ){			
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
			
				 if(_isCanSkill2 && !_skillChangeBuffValue  && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL2);		
				}else if(_isCanSkill3 && !_skillChangeBuffValue && this->getHpPercent()>0.5){
					this->changeSide(sp);
					this->attack(SKILL3);
				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}
				return;
			}
			
		}

	}
	//无玩家情况
	if(battleCondiction>=0){
		_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	}else {
		_mainTarget=NULL;
		this->findEnemy2("Tower");
	}

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			
			if(_isCanSkill1 && this->getHpPercent()<0.9){
				this->changeSide(sp);
				this->attack(SKILL1);
			}else if(_mainTarget->role()==Role::Flog && _isCanSkill2 ){
				this->changeSide(sp);
				this->attack(SKILL2);
			}else if(_mainTarget->role()==Role::Tower  && _isCanSkill3 && !_skillChangeBuffValue){
				this->changeSide(sp);
				this->attack(SKILL3);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);
			}

		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		if(_isCanGear00){
			this->useGear(gear00);
		};
		this->stepOn();
	}

	
}

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

void Hero::AI_Tenten(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear00);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear05);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}


	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(_isCanGear00){
				this->useGear(gear00);
			};
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{

			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(_isCanGear00){
				this->useGear(gear00);
			};
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){

			if(_isCanSkill1){
				this->changeSide(sp);
				this->attack(SKILL1);
				return;
			}else if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game && _mainTarget->getGP()<5000 && !_skillChangeBuffValue){
				if(abs(sp.x)> 48  || abs(sp.y)> 16 ){
					if(_isCanGear00){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				this->changeSide(sp);

				this->attack(OUGIS2);
				return;
			}else if(_isCanOugis1  && !_isControled   && !_mainTarget->_isBati && _mainTarget->getGP()<5000 && !_skillChangeBuffValue ){

				if(abs(sp.x)> 48  || abs(sp.y)> 16 ){
					if(_isCanGear00){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				this->changeSide(sp);
				this->attack(OUGIS1);
				
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_skillChangeBuffValue && !_isControled ){
				//保持距离
				if(abs(sp.x)<160 ){
					this->stepBack2();
					return;
				}else {
					
					this->idle();
					return;
					
				}
			}else if(abs(sp.x) <128){

				//交战策略

				if((abs(sp.x)> 96  || abs(sp.y)> 16) && !_skillChangeBuffValue  ) {
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				if(_isCanSkill2 && _mainTarget->getGP()<5000 ){
					this->changeSide(sp);
					this->attack(SKILL2);		
				}else if(_isCanSkill3 && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL3);
				
				}else{				
					
					if ((abs(sp.x)> 32  || abs(sp.y)> 32 )&& !_skillChangeBuffValue ){
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}else{
						this->changeSide(sp);
						this->attack(NAttack);
					}
					
				}

				return;
			}
		}

	}
	//无玩家情况
		_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_mainTarget->role()==Role::Flog && _isCanSkill1){
				this->changeSide(sp);
				this->attack(SKILL1);
			}else if(_mainTarget->role()==Role::Flog && _isCanSkill3){
				this->changeSide(sp);
				this->attack(SKILL3);
			}else if(_mainTarget->role()==Role::Flog && _isCanSkill2 && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL2);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);
			}
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		if(_isCanGear00){
			this->useGear(gear00);
		};
		this->stepOn();
	}


}
void Hero::AI_Itachi(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}
	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear02);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear05);
		}
	}



	if(this->checkRetri()){

		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};

	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK ){

			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game  &&  !_isBati){
				
				if(abs(sp.x)> 48 || abs(sp.y)> 32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				this->changeSide(sp);
				
				this->attack(OUGIS2);

				return;
			}else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000
				&& !_mainTarget->_isBati
				&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
				&& !_mainTarget->_isSticking
				&& !_isBati){

				this->changeSide(sp);
				
				this->attack(OUGIS1);

				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isBati && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}

			}else if(abs(sp.x)<128 || _isBati){

				//交战策略
				 if( abs(sp.x)> 196 || abs(sp.y)> 64){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if((abs(sp.x)> 56 || abs(sp.y)> 32) && !_isBati){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				 }

				if(_isCanSkill3 && !_isBati && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL3);
				}else if(_isCanSkill2 && !_isBati && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL2);
				}else if(_isCanSkill1 && !_isBati && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL1);
				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
	if(!this->findEnemy2("Flog")){
		this->findEnemy2("Tower");
	}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill3 && !_isBati 
				&& _mainTarget->role()==Role::Flog && isBaseDanger){
					this->changeSide(sp);
					this->attack(SKILL3);
			}else if(_isCanSkill2 && !_isBati 
				&& _mainTarget->role()==Role::Flog && isBaseDanger){
					this->changeSide(sp);
					this->attack(SKILL2);
			}else if(_isCanSkill1  && !_isBati 
				&& _mainTarget->role()==Role::Flog ){
					this->changeSide(sp);
					this->attack(SKILL1);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);
			}

		}

		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}



}
void Hero::AI_Jiraiya(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear01);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			
			
			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game ){
				//先手无视战斗力
				this->changeSide(sp);
			
				this->attack(OUGIS2);
				return;
			}else if(_isCanSkill2 && _mainTarget->getGP()<5000){
				this->changeSide(sp);
				this->attack(SKILL2);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}

			}else if(abs(sp.x)<128){

				//交战策略
				if(abs(sp.x)> 32 || abs(sp.y)>32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000){
					this->changeSide(sp);		
					this->attack(OUGIS1);
				}else if(_isCanSkill3  && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL3);
				}else if(_isCanSkill1  && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL1);
				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill3 && _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL3);

			}else if(_isCanSkill1 && _mainTarget->role()==Role::Flog){
				this->changeSide(sp);
				this->attack(SKILL1);

			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}


}
void Hero::AI_SageJiraiya(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear01);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){


			if(_isCanSkill2 && _mainTarget->getGP()<5000){
				this->changeSide(sp);
				this->attack(SKILL2);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {
					this->idle();
					return;
				}
			}else if(abs(sp.x)<128){
				//交战策略	
				if(abs(sp.x)> 96 || abs(sp.y)> 32 ){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if((abs(sp.x)> 32 || abs(sp.y)>32)  && !_isCanOugis2){

					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game  && _mainTarget->getGP()<5000){
					//先手无视战斗力
					this->changeSide(sp);	
					this->attack(OUGIS2);
					return;
				}else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000){

					this->changeSide(sp);
					
					this->attack(OUGIS1);

				}else if(_isCanSkill3  && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL3);

				}else if(_isCanSkill1  && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL1);

				}else{

					if(abs(sp.x)> 32 || abs(sp.y)>32){
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}else{

						this->changeSide(sp);
						this->attack(NAttack);
					}

				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	
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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill3 && _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL3);

			}else if(_isCanSkill1 && _mainTarget->role()==Role::Flog){
				this->changeSide(sp);
				this->attack(SKILL1);

			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}

}

void Hero::AI_Suigetsu(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");



	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear00);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear05);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear01);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				if(_isCanGear00){
					this->useGear(gear00);
				};
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){

			if(_isCanGear00){
				this->useGear(gear00);
			};

			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
	
			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
				&&!_isBati 
				&& _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
				&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
				&& !_mainTarget->_isSticking
				){
					if(abs(sp.x)> 48 || abs(sp.y)> 16){
						if(_isCanGear00){
							this->useGear(gear00);
						};
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}

					this->changeSide(sp);
					this->attack(OUGIS2);
					return;
			}else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 &&!_isBati){
				if(abs(sp.x)> 48 || abs(sp.y)> 16){
					if(_isCanGear00){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				
				this->changeSide(sp);
				this->attack(OUGIS1);
				return;

			}else if(_isCanSkill3 && _mainTarget->getGP()<5000 && !_isBati && !_skillChangeBuffValue){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL3);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}

			}else if(abs(sp.x)<128){

				//交战策略
				if(abs(sp.x)> 96  || abs(sp.y)> 32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if((abs(sp.x)> 32 || abs(sp.y)>32) && !_isBati){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				if(_isCanSkill2 && _mainTarget->getGP()<5000 && !_isBati){
					this->changeSide(sp);
					this->attack(SKILL2);		
				}else if(_isCanSkill1  && _mainTarget->getGP()<5000 &&!_isBati){
					this->changeSide(sp);
					this->attack(SKILL1);
				}else{
					this->changeSide(sp);
					this->attack(NAttack);
					
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_mainTarget->role()==Role::Flog && _isCanSkill1 && !_isBati){
				this->changeSide(sp);
				this->attack(SKILL1);
			}else if(_isCanSkill2  && _mainTarget->getGP()<5000 &&!_isBati
				&& _mainTarget->role()==Role::Flog && isBaseDanger){
					this->changeSide(sp);
					this->attack(SKILL2);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);
			}
		}
		return;
	}
	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}

}

void Hero::AI_Tsunade(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear03);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear02);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear04);
		}
	}


	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK ){
			//先手无视战斗力
			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game && _mainTarget->getGP()<5000  &&!_isBati){

				if(abs(sp.x)> 64  || abs(sp.y)> 16){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;

				}else{
					this->changeSide(sp);
				
					this->attack(OUGIS2);
				}
				return;

			}else if(_isCanSkill3 && _mainTarget->getGP()<5000  &&!_isBati){

				this->changeSide(sp);
				this->attack(SKILL3);
				return;

			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled && !_isBati){
				//保持距离
				if(abs(sp.x)<160 ){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}

			}else if(abs(sp.x)<128){


				//交战策略

				if( abs(sp.x)> 64  || abs(sp.y)> 32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if(abs(sp.x)> 32 || abs(sp.y)>32  && !_isCanSkill2  &&!_isBati && !_isCanOugis1){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				if(_isCanGear03){
					this->useGear(gear03);
				}
				if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 &&!_isBati){
					this->changeSide(sp);		
					this->attack(OUGIS1);
				}else if(_isCanSkill2  &&!_isBati){
					this->changeSide(sp);
					this->attack(SKILL2);
					return;
				}else if(_isCanSkill1 &&!_isBati){
					this->changeSide(sp);
					this->attack(SKILL1);

				}else{	
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			if(_isCanGear03){
				this->useGear(gear03);
			}
			//技能清兵
			if(_mainTarget->role()==Role::Flog && _isCanSkill1 && !_isBati && isBaseDanger ){
				this->changeSide(sp);
				this->attack(SKILL1);

			}else{
				this->changeSide(sp);
				this->attack(NAttack);
			}
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}
}


void Hero::AI_Sasuke(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");


	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear01);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}


	if(this->checkRetri()){

		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			
			if(this->stepBack()){
				return;
			};
		}
	};

	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK ){

			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game  &&  !_isBati){
				this->changeSide(sp);	
				this->attack(OUGIS2);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isBati && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}
			}else if(_isCanSkill3 && !_isTaunt && !_isBati){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL3);
				return;
			}else if(abs(sp.x)<128){

				//交战策略

				if(abs(sp.x)> 96 || abs(sp.y)> 32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if((abs(sp.x)> 32 || abs(sp.y)>32) && !_isCanSkill1 && !_isCanSkill2){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 && !_isBati){			
					if(abs(sp.x)> 32 || abs(sp.y)>32){
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}
					this->changeSide(sp);			
					this->attack(OUGIS1);

				}else if(_isCanSkill2 && !_isBati && _mainTarget->getGP()<5000){
					 this->changeSide(sp);
					 this->attack(SKILL2);

				 }else if(_isCanSkill1 && !_isBati && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL1);

				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill1  && !_isBati && _mainTarget->role()==Role::Flog){
				this->changeSide(sp);
				this->attack(SKILL1);
			}else if(_isCanSkill2  && isBaseDanger && !_isBati && _mainTarget->role()==Role::Flog){
				this->changeSide(sp);
				this->attack(SKILL2);
			}else if(_isCanSkill3  && !_isBati && _mainTarget->role()==Role::Flog){
				this->changeSide(sp);
				this->attack(SKILL3);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);
			}

		}

		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}

}
void Hero::AI_ImmortalSasuke(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");


	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear01);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}


	if(this->checkRetri()){

		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{

			if(this->stepBack()){
				return;
			};
		}
	};

	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){

			if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 && !_isBati){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(OUGIS1);
				return;

			}else if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game  &&  !_isBati){

				if(abs(sp.x)> 48 || abs(sp.y)> 32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}	

				this->changeSide(sp);
				this->attack(OUGIS2);
				return;
			} else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isBati && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}

			}else if(_isCanSkill3 && !_isBati && !_isTaunt){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL3);
				return;
			}else if(abs(sp.x)<128 || _isBati){

				
				//交战策略
				if( abs(sp.x)> 196 || abs(sp.y)> 64){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if((abs(sp.x)> 96 || abs(sp.y)> 32) && !_isBati ){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if((abs(sp.x)> 32 || abs(sp.y)> 32) && !_isCanSkill1 && !_isCanSkill2 ){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				if(_isCanSkill2 && !_isBati && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL2);

				}else if(_isCanSkill1 && !_isBati && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL1);

				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill1   && !_isBati && _mainTarget->role()==Role::Flog){
				this->changeSide(sp);
				this->attack(SKILL1);
			}else if(_isCanSkill2  && isBaseDanger && !_isBati && _mainTarget->role()==Role::Flog){
				this->changeSide(sp);
				this->attack(SKILL2);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);
			}

		}

		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}
}

void Hero::AI_Kankuro(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");


	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}
	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear04);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear08);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 
		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	bool isFound1=false;
	bool isFound2=false;
	bool isFound3=false;

	if(getMonsterArray() && getMonsterArray()->count()>0){
		CCObject* pObject;
		CCARRAY_FOREACH(getMonsterArray(),pObject){
			Monster* mo=(Monster*) pObject;
			if (mo->isCharacter("Saso")){
				isFound3=true;
			};
			if (mo->isCharacter("Sanshouuo")){
				isFound2=true;
			}
			if (mo->isCharacter("Karasu")){
				isFound1=true;
			};
		}
	};
	

	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK ){


			if(_isCanSkill3){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL3);
				return;
			}else if(_isCanSkill1 && ! isFound1 &&!_isControled){
				this->attack(SKILL1);
				return;
			}else if(_isCanOugis1  && !_isControled && ! isFound2){
				this->changeSide(sp);
				this->attack(OUGIS1);
				return;
			}else if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game && ! isFound3 ){
				this->changeSide(sp);	
				this->attack(OUGIS2);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>5000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {
					this->idle();
					return;
				}

			}else if(abs(sp.x)<128){

				
				//交战策略

				if(abs(sp.x)> 32 || abs(sp.y)>32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else{
					this->changeSide(sp);
					this->attack(NAttack);

				}
					
				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanOugis1  && !_isControled && !isFound2){
				this->changeSide(sp);	
				this->attack(OUGIS1);
			}else if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game  && !isFound3){
				this->changeSide(sp);			
				this->attack(OUGIS2);
			}else if(_isCanSkill1 && ! isFound1){
				this->attack(SKILL1);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}

}


void Hero::AI_Chiyo(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");


	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}
	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear07);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear04);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 
		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	bool isFound1=false;

	if(getMonsterArray() && getMonsterArray()->count()>0){
		CCObject* pObject;
		CCARRAY_FOREACH(getMonsterArray(),pObject){
			Monster* mo=(Monster*) pObject;
			if (mo->isCharacter("Parents")){
				isFound1=true;
			};
		}
	};


	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK ){
			if(_isCanOugis1 && !_isControled && !_buffStartTime){
				
				CCObject* pObject;
				int countNum=0;
				CCARRAY_FOREACH(_delegate->_CharacterArray,pObject){
					Hero* tempHero=(Hero*) pObject;
					if(this->group()==tempHero->group()
						&& (tempHero->role()==Role::Player || tempHero->role()==Role::Com)
						&& tempHero->_actionState!=ACTION_STATE_DEAD
						&& !tempHero->isCharacter("Chiyo")
						){
							CCPoint	sp=ccpSub(tempHero->getPosition(),this->getPosition());
							if(abs(sp.x)<=winSize.width/2){
								countNum++;
							}

					}
					
				}
				if(countNum>=1){
					this->attack(OUGIS1);
					return;
				}

			}
			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
				&& _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
				&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
				&& !_mainTarget->_isSticking){

					if(abs(sp.x)> 96 || abs(sp.y)>32){
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}else if(abs(sp.x)<48){
						this->stepBack();
						return;
					}	
				this->changeSide(sp);	
				this->attack(OUGIS2);
				
			}else if(_isCanSkill1 && ! isFound1 &&!_isControled){
				this->attack(SKILL1);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>5000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {
					this->idle();
					return;
				}

			}else if(abs(sp.x)<128){


				//交战策略

				if(abs(sp.x)> 32 || abs(sp.y)>32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else{
					this->changeSide(sp);
					this->attack(NAttack);

				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
	if(!this->findEnemy2("Flog")){
		this->findEnemy2("Tower");
	}


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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill1 && ! isFound1){
				this->attack(SKILL1);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}

}

void Hero::AI_Kakuzu(){

	_mainTarget=NULL;
	if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){

		if(this->getHpPercent()>0.3f && !_isControled && _isCanSkill1){
			CCObject* pObject;
			float distance;
			float curDistance=0 ;
			CCPoint sp;
	
			CCARRAY_FOREACH(this->getDelegate()->_CharacterArray,pObject){
				ActionManager* target=(ActionManager*) pObject;
				if((target->role()==Role::Player ||
					target->role()==Role::Com) &&
					target->getActionState()==ACTION_STATE_DEAD
					){
						distance=ccpDistance(target->getPosition(),this->getPosition());
						sp=ccpSub(target->getPosition(),this->getPosition());

						if(abs(sp.x)<(winSize.width/2)){
							if(target->_isTaunt){
								_mainTarget=target;
						
							}
							if(curDistance && abs(curDistance)>abs(distance)){
								_mainTarget=target;
								curDistance=distance;
							}else if(!curDistance){
								curDistance=distance;
								_mainTarget=target;
							
							};

						}
				}
			}

			if(_mainTarget){
				CCPoint moveDirection;
				if(_mainTarget->_originY){
					sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
				}else{
					sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
				}

				if(abs(sp.x)> 32 || abs(sp.y)>32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else{
					this->changeSide(sp);
					this->attack(SKILL1);		
				}

				return;
			}

		}
	
	}


	this->findEnemy2("Hero");

	
	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear03);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear08);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear04);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 
		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	bool isSummonAble=false;

	if((getMonsterArray() && getMonsterArray()->count()<3 || !_monsterArray ) && hearts>0){
		isSummonAble=true;
	}


	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK ){
	

			if(_isCanSkill3){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL3);
				return;
			}else if(_isCanOugis1  && !_isControled  && isSummonAble ){
				this->attack(OUGIS1);
				return;
			}else if(_isCanSkill2){

				if(abs(sp.x)> 96 || abs(sp.y)> 32 ){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				if(_isCanGear03){
					this->useGear(gear03);
				}

				this->changeSide(sp);
				this->attack(SKILL2);		
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>5000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {
					this->idle();
					return;
				}

			}else if(abs(sp.x)<128){

				
				//交战策略	
				if(abs(sp.x)> 32 || abs(sp.y)> 32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else{

					this->changeSide(sp);
					this->attack(NAttack);
						
				}

				return;
			}
		}

	}
	//无玩家情况
	
	if(!this->findEnemy2("Flog")){
		this->findEnemy2("Tower");
	}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanOugis1  && !_isControled && !isSummonAble){
				this->changeSide(sp);
				this->attack(OUGIS1);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}

}


void Hero::AI_Shikamaru(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");



	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear00);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear07);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear08);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){

			if(this->stepBack2()){
				return;
			};
		}else{
			if(_isCanGear00 ){
				this->useGear(gear00);
			};
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK  ){

			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
				&& _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
				&& !_mainTarget->_isSticking){

					this->changeSide(sp);
					this->attack(OUGIS2);
					return;
			}else if(_isCanSkill2 && _mainTarget->getGP()<5000){
				this->changeSide(sp);
				this->attack(SKILL2);
				return;
			}else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000){

				if(abs(sp.x)> 48 || abs(sp.y)> 32 ){
					if(_isCanGear00 ){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				this->changeSide(sp);
				this->attack(OUGIS1);
				return;

			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}

			}else if(this->getLV()>=2 && _isCanSkill1){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL1);
				return;

			}else if(_isCanSkill3 && _mainTarget->getGP()<5000 && !_mainTarget->_isBati  && !_mainTarget->_isSticking){

				if(abs(sp.x)> 156 || abs(sp.y)> 32 ){
					if(_isCanGear00 ){
						this->useGear(gear00);
					};

					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if( abs(sp.x)<32){
					this->stepBack();
					return;
				}


				this->changeSide(sp);
				this->attack(SKILL3);
				return;

			}else if(abs(sp.x)<128){


				//交战策略
				if(abs(sp.x)> 32 || abs(sp.y)>32 ){

					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
	if(!this->findEnemy2("Flog")){
		this->findEnemy2("Tower");
	}


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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill2 && _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL2);

			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		if(_isCanGear00 ){
			this->useGear(gear00);
		};
		this->stepOn();
	}

}
void Hero::AI_Gaara(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear02);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear05);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game &&!_isBati 
				&& _mainTarget->getGP()<5000 ){		
					this->changeSide(sp);		
					this->attack(OUGIS2);
					return;
			}else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 &&!_isBati){
				this->changeSide(sp);	
				this->attack(OUGIS1);
				return;
			}else if(_isCanSkill3 && _mainTarget->getGP()<5000  &&!_isBati){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL3);
				return;
			}else if(_isCanSkill2  && _mainTarget->getGP()<5000 &&!_isBati){
				this->changeSide(sp);
				this->attack(SKILL2);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled &&!_isBati){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {
					this->idle();
					return;
				}
			}else if(abs(sp.x)< 128){ 

				//交战策略
				if(abs(sp.x)> 32 || abs(sp.y)> 32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				
				if(_isCanSkill1 &&!_isBati){
					 this->changeSide(sp);
					 this->attack(SKILL1);
				 }else{
					this->changeSide(sp);
					this->attack(NAttack);
				}
				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_mainTarget->role()==Role::Flog && _isCanSkill1 && !_isBati){
				this->changeSide(sp);
				this->attack(SKILL1);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);
			}
		}
		return;
	}
	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}
}
void Hero::AI_Tobirama(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");


	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}
	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear01);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear07);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			
			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
				&& _mainTarget->getGP()<5000){
					//先手无视战斗力
					this->changeSide(sp);	
					this->attack(OUGIS2);
			}else if(_isCanSkill1 && !_mainTarget->_isBati &&(
				 _mainTarget->getActionState()==ACTION_STATE_IDLE ||
				_mainTarget->getActionState()==ACTION_STATE_WALK ||
				_mainTarget->getActionState()==ACTION_STATE_ATTACK
				)
				){	
				this->changeSide(sp);
				this->attack(SKILL1);
				return;
			}else if(_isCanSkill2 && _mainTarget->getGP()<5000){

				if((abs(sp.x)> 64 || abs(sp.y)> 16) ){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				this->changeSide(sp);
				this->attack(SKILL2);			
				return;

			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}

			}else if(abs(sp.x)<128){ 

				//交战策略

				if(abs(sp.x)> 48 || abs(sp.y)> 32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000){
					this->changeSide(sp);	
					this->attack(OUGIS1);

				}else if(_isCanSkill3){
					this->changeSide(sp);
					this->attack(SKILL3);
				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
		_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill3 && _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL3);

			}else if(_isCanSkill2 && _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL2);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}

}

void Hero::AI_Orochimaru(){
}

void Hero::AI_Choji(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear01);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear04);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK ){

			if(_isCanSkill1 && !_isBati){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL1);
				return;
			}else if(_isCanSkill3 && _mainTarget->getGP()<5000 && !_isBati){

				if(abs(sp.x)> 96 || abs(sp.y)> 16){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				this->changeSide(sp);
				this->attack(SKILL3);
				return;

			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled && !_isBati){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}

			}else if(abs(sp.x)<128){ 

				//交战策略
				if(abs(sp.x)> 32 || abs(sp.y)>32){				
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 && !_isBati){
					this->changeSide(sp);
					this->attack(OUGIS1);

				}else if(_isCanSkill2 && _mainTarget->getGP()<5000 && !_isBati && _skillUPBuffValue){
					this->changeSide(sp);
					this->attack(SKILL2);
				}else if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
					&& _mainTarget->getGP()<5000 
					&& !_isBati
					){
						this->changeSide(sp);
						this->attack(OUGIS2);

				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	
	//无玩家情况
	if(battleCondiction>=0){
		_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	}else{
		_mainTarget=NULL;
		this->findEnemy2("Tower");
	}

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill2 && !_isBati &&  _mainTarget->role()==Role::Flog){
				this->changeSide(sp);
				this->attack(SKILL2);
			}else if(_isCanOugis1  && !_isControled && isBaseDanger && !_isBati &&  _mainTarget->role()==Role::Flog){
				this->changeSide(sp);
				this->attack(OUGIS1);
			}else if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
				&& _mainTarget->role()==Role::Tower
				&& !_isBati
				){
					this->changeSide(sp);
					this->attack(OUGIS2);

			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}
}


void Hero::AI_Karin(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");


	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}
	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear01);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear07);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK ){

			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
				&& _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
				&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
				&& !_mainTarget->_isSticking 
				){
					if(abs(sp.x)> 96 || abs(sp.y)> 16){

						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}
					this->changeSide(sp);
					this->attack(OUGIS2);
					return;

			}else if(this->getHpPercent()<0.9 && _isCanOugis1 && !_isControled && !_buffStartTime && !_isHealling){
				this->changeSide(sp);	
				this->attack(OUGIS1);
				return;
			}else if(_isCanSkill3 && _mainTarget->getGP()<5000){
				//先手无视战斗力
				if((abs(sp.x)> 128 || abs(sp.y)> 16) ){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				this->changeSide(sp);
				this->attack(SKILL3);
				return;

			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160 ){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;
				}

			}else if(abs(sp.x)<128){ 
				
				//交战策略

				if(abs(sp.x)> 96 || abs(sp.y)> 32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}	
				if(_isCanSkill2){
					this->changeSide(sp);
					this->attack(SKILL2);

				}else if(_isCanSkill1){
					this->changeSide(sp);
					this->attack(SKILL1);
				}else{

					if(abs(sp.x)> 32 || abs(sp.y)>32){
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	
	if(!this->findEnemy2("Flog")){
		this->findEnemy2("Tower");
	}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill1 && _mainTarget->role()==Role::Flog ){
				this->changeSide(sp);
				this->attack(SKILL1);

			}else if(_isCanSkill2 && _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL2);

			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}
}
void Hero::AI_Lee(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");


	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}
	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear07);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if((_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK ) && abs(sp.x)<128  ){
			if(_isCanSkill1 && bamen<5){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL1);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}

			}else if(abs(sp.x)<128){ 

				//交战策略
				if(abs(sp.x)> 48 || abs(sp.y)> 32 ){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 
					&& !_mainTarget->_isBati 
					&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
					&& !_mainTarget->_isSticking
					&& bamen>=1
					){

						this->changeSide(sp);
						this->attack(OUGIS1);
				}else if(_isCanSkill2){
					this->changeSide(sp);
					this->attack(SKILL2);

				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill2 && _mainTarget->role()==Role::Flog ){
				this->changeSide(sp);
				this->attack(SKILL2);

			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}
}

void Hero::AI_RockLee(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");


	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}
	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear07);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			if(_isCanSkill1 && bamen<5){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL1);
				return;
			}else if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
				&& _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
				&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
				&& !_mainTarget->_isSticking
				&& bamen>=3
				){

					this->changeSide(sp);
				
					this->attack(OUGIS2);

			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}

			}else if(abs(sp.x)<128 || bamen>=5){ 

				
				//交战策略

				if(abs(sp.x)> 46 || abs(sp.y)> 32){

					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 
					&& !_mainTarget->_isBati 
					&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
					&& !_mainTarget->_isSticking
					&& bamen>=1
					){

						this->changeSide(sp);

						this->attack(OUGIS1);

				}else if(_isCanSkill2){
					this->changeSide(sp);
					this->attack(SKILL2);

				}else if(_isCanSkill3 && bamen>=3){
					this->changeSide(sp);
					this->attack(SKILL3);

				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
	if(!this->findEnemy2("Flog")){
		this->findEnemy2("Tower");
	}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill2 && _mainTarget->role()==Role::Flog ){
				this->changeSide(sp);
				this->attack(SKILL2);

			}else if(_isCanSkill3 && bamen>=3 && isBaseDanger && _mainTarget->role()==Role::Flog ){
				this->changeSide(sp);
				this->attack(SKILL3);

			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}
}

void Hero::AI_Nagato(){
}

void Hero::AI_Jugo(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear01);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){

			if(_isCanSkill3 && _mainTarget->getGP()<5000  && !_skillChangeBuffValue){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL3);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {
					this->idle();
					return;

				}

			}else if(abs(sp.x)<128){ 

				//交战策略

				if(abs(sp.x)> 96  || abs(sp.y)> 32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
					&& !_skillChangeBuffValue
					&& _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
					&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
					&& !_mainTarget->_isSticking
					){

						if(abs(sp.x)> 32 || abs(sp.y)>32){
							moveDirection =ccpNormalize(sp);
							this->walk(moveDirection);
							return;
						}
						this->changeSide(sp);
						this->attack(OUGIS2);

				}else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000  && !_skillChangeBuffValue){

					if(abs(sp.x)> 32 || abs(sp.y)>32){
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}

					this->changeSide(sp);
				
					this->attack(OUGIS1);

				}else if(_isCanSkill2 && _mainTarget->getGP()<5000  && !_skillChangeBuffValue ){

					this->changeSide(sp);
					this->attack(SKILL2);
					return;

				}else if(_isCanSkill1  && _mainTarget->getGP()<5000  && !_skillChangeBuffValue){
					this->changeSide(sp);
					this->attack(SKILL1);

				}else{

					if(abs(sp.x)> 48 || abs(sp.y)>32){
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}else{

						this->changeSide(sp);
						this->attack(NAttack);
					}
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_mainTarget->role()==Role::Flog && _isCanSkill1 && !_skillChangeBuffValue){
				this->changeSide(sp);
				this->attack(SKILL1);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);
			}
		}
		return;
	}
	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}


}


void Hero::AI_Kisame(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");



	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear00);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear01);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear05);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){

			if(this->stepBack2()){
				return;
			};
		}else{
			if(_isCanGear00 ){
				this->useGear(gear00);
			};
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if((_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK ) && ( abs(sp.x)<128  || _isCanGear00) ){

			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
				&& _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
				&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
				&& !_mainTarget->_isSticking && !_skillChangeBuffValue){

					if(abs(sp.x)> 48|| abs(sp.y)> 32 ){
						if(_isCanGear00 ){
							this->useGear(gear00);
						};
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}

					this->changeSide(sp);

					this->attack(OUGIS2);
					return;
			}else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 && !_skillChangeBuffValue){

				if(abs(sp.x)> 48 || abs(sp.y)> 32 ){
					if(_isCanGear00 ){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				this->changeSide(sp);
				this->attack(OUGIS1);
				return;

			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled && !_skillChangeBuffValue){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}

			}else if(_isCanSkill3 && !_skillChangeBuffValue){
				if(abs(sp.x)> 96 || abs(sp.y)> 32 ){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL3);
				return;
			}else if(_isCanSkill2 && _mainTarget->getGP()<5000 && !_skillChangeBuffValue){

				if(abs(sp.x)> 96 || abs(sp.y)> 32 ){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				this->changeSide(sp);
				this->attack(SKILL2);
				return;

			}else if(abs(sp.x)<128){


				//交战策略
				if(abs(sp.x)> 64 || abs(sp.y)>32 ){

					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				if(_isCanSkill1 && _mainTarget->getGP()<5000  && !_skillChangeBuffValue){
					this->changeSide(sp);
					this->attack(SKILL1);
				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
	if(!this->findEnemy2("Flog")){
		this->findEnemy2("Tower");
	}


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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill2 && _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL2);

			}else if(_isCanSkill1 && _mainTarget->role()==Role::Flog ){
				this->changeSide(sp);
				this->attack(SKILL1);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		if(_isCanGear00 ){
			this->useGear(gear00);
		};
		this->stepOn();
	}
}



void Hero::AI_Hidan(){

	_mainTarget=NULL;
	bool _isFound=false;
	if(getMonsterArray() && getMonsterArray()->count()>0){
		CCObject* pObject;
		CCARRAY_FOREACH(getMonsterArray(),pObject){
			Monster* mo=(Monster*) pObject;
			if (mo->isCharacter("CircleMark")){
				_mainTarget=mo;
				_isFound=true;
				CCPoint moveDirection;
				CCPoint sp;

				if(_mainTarget->_originY){
					sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
				}else{
					sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
				}
				if((abs(sp.x)> 8|| abs(sp.y)> 8)){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else{
					if(_isBati){

						CCObject* pObject;
						CCARRAY_FOREACH(_delegate->_CharacterArray,pObject){
							ActionManager* tempHero=(ActionManager*) pObject;
							if( tempHero->group()!=this->group()
								&& tempHero->getHP()<2000
								&& tempHero->getActionState()!=ACTION_STATE_DEAD
								&& (tempHero->role()==Role::Player ||
								tempHero->role()==Role::Com
								)
								){
									this->attack(NAttack);
									return;
							}
						};

						this->idle();
						return;

					}
				}

			};
		};

	}

	

	if(!_isFound){
		this->findEnemy2("Hero");
	}


	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}
	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear01);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear07);
		}
	}

	if(this->checkRetri() && !_isBati  && !_isFound){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled && !_isBati  && !_isFound){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){

			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game  && !_isBati){
				if(abs(sp.x)> 48 || abs(sp.y)> 32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				if(_isCanSkill1  && !_isBati && !_isFound){
					this->changeSide(sp);
					this->attack(SKILL1);
					return;
				}
				this->changeSide(sp);		
				this->attack(OUGIS2);
				return;
			}else if(_isCanSkill2 && _mainTarget->getGP()<5000 && !_isBati ){
				//先手无视战斗力
				
				if(abs(sp.x)> 56 || abs(sp.y)> 32){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				this->changeSide(sp);
				this->attack(SKILL2);
				return;

			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled && !_isBati && !_isFound){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}

			}else if(abs(sp.x)<128){ 

				//交战策略
				if((abs(sp.x)> 48 || abs(sp.y)> 32) && !_isBati){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				if(_isCanOugis1 && !_isBati  && !_isControled  && _mainTarget->getGP()<5000){
					this->changeSide(sp);	
					this->attack(OUGIS1);
				}else if(_isCanSkill3 && !_isBati){
					this->changeSide(sp);
					this->attack(SKILL3);
				}else{			
					if(!_isBati){
						this->changeSide(sp);
						this->attack(NAttack);
					}else{
						this->idle();
					}		
				}

				return;
			}
		}

	}

	


	//无玩家情况
	if(!_mainTarget && !_isFound && !_isBati){
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	}

	if(_mainTarget && _mainTarget->role()!=Role::Mon  && !_isBati ){

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill3 && _mainTarget->role()==Role::Flog && isBaseDanger && !_isBati){
				this->changeSide(sp);
				this->attack(SKILL3);

			}else if(_isCanSkill2 && _mainTarget->role()==Role::Flog && isBaseDanger && !_isBati){
				this->changeSide(sp);
				this->attack(SKILL2);

			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}
		}
		return;
	}
	if(!_isBati){
		if(_isHealling &&this->getHpPercent()<1){
			if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
				this->idle();
			}
		}else{
			this->stepOn();
		}
	}else{
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}
	
}

void Hero::AI_Tobi(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");



	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear03);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear02);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear05);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			
			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
				&& _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
				&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
				&& !_mainTarget->_isSticking){

					if(abs(sp.x)> 48 || abs(sp.y)> 16 ){

						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}

					this->changeSide(sp);
					this->attack(OUGIS2);
					return;
			}else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000){
				this->changeSide(sp);
				
				this->attack(OUGIS1);
				return;
			}else if(_isCanSkill3 && !_skillChangeBuffValue){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL3);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled && !_skillChangeBuffValue){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}

			}else if(abs(sp.x)<128){ 
				//交战策略

				if(abs(sp.x)> 64 || abs(sp.y)> 32 ){
					
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else if(abs(sp.x)<32 && (_isCanSkill2 && _mainTarget->getHpPercent()<0.5f) ){
					this->stepBack();
					return;
				}
				
				if(_isCanSkill2 && _mainTarget->getGP()<5000 && (_mainTarget->getHpPercent()<0.5f || !_skillChangeBuffValue) ){
					
					this->changeSide(sp);
					this->attack(SKILL2);
					
				}else if(_isCanSkill1 && _mainTarget->getGP()<5000  && (_mainTarget->getHpPercent()<0.5f ||  !_skillChangeBuffValue) ){
					this->changeSide(sp);
					this->attack(SKILL1);
					
				}else{

					if(abs(sp.x)> 32 || abs(sp.y)>32 ){
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}

					if(_isCanGear03){
						this->useGear(gear03);
					}

					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill2 && _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL2);

			}else if(_isCanSkill1 && _mainTarget->role()==Role::Flog){
				this->changeSide(sp);
				this->attack(SKILL1);

			}else{

				if(_isCanGear03){
					this->useGear(gear03);
				}
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		this->stepOn();
	}


}

void Hero::AI_Shino(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear00);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear01);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			
			if(this->stepBack2()){
				return;
			};
		}else{
			if(_isCanGear00 ){
				this->useGear(gear00);
			};
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(_isCanGear00 ){
				this->useGear(gear00);
			};
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK  ){

			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
				&& _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
				&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
				&& !_mainTarget->_isSticking){

					if(abs(sp.x)> 64 || abs(sp.y)> 16 ){
						if(_isCanGear00 ){
							this->useGear(gear00);
						};
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}

					this->changeSide(sp);
					this->attack(OUGIS2);
					return;

			}else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000){

				if(abs(sp.x)> 64 || abs(sp.y)> 16 ){
					if(_isCanGear00 ){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);

					return;
				}

				this->changeSide(sp);

				this->attack(OUGIS1);

			}else if(_isCanSkill2 && _mainTarget->getGP()<5000){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL2);
				return;
			}else if(_isCanSkill3 && _mainTarget->getGP()<5000){

				if(abs(sp.x)> 64  || abs(sp.y)> 16){

					if(_isCanGear00 ){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else{

					this->changeSide(sp);
					this->attack(SKILL3);

				} 
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {
					if(_skillChangeBuffValue){
						this->changeSide(sp);
						this->attack(NAttack);
					}else{
						this->idle();
					}
					
					return;

				}

			}else if(_isCanSkill1){
				this->changeSide(sp);
				this->attack(SKILL1);
				return;
			}else if(abs(sp.x)<128){ 
				if(this->getMaxHP()- this->getHP()>=3000 && 
					this->getCoin()>=50 && !_isHealling && _isCanItem1){
						this->setItem(Item1);					
				}

				//交战策略
				if( (abs(sp.x)> 32 || abs(sp.y)>32 ) && !_skillChangeBuffValue){	
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				
				this->changeSide(sp);
				this->attack(NAttack);		

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill3 && _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL3);

			}else if(_isCanSkill2 && _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL2);

			}else if(_isCanSkill1 && _mainTarget->role()==Role::Flog ){
				this->changeSide(sp);
				this->attack(SKILL1);

			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		if(_isCanGear00 ){
			this->useGear(gear00);
		};
		this->stepOn();
	}

}

void Hero::AI_Hiruzen(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");

	
	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear00);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear02);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear08);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(_isCanGear00){
				this->useGear(gear00);
			};
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(_isCanGear00){
				this->useGear(gear00);
			};
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK ){

			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
				&&!_isBati 
				&& _mainTarget->getHP()<10000
				&& !_mainTarget->_isCanGear06
				){
					if(abs(sp.x)<32){
						this->stepBack();
						return;
					}else if(abs(sp.x)> 96 || abs(sp.y)> 32){
						if(_isCanGear00){
							this->useGear(gear00);
						};
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}

					this->changeSide(sp);
					this->attack(OUGIS2);
					return;
			}else  if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 &&!_isBati){
				if(abs(sp.x)> 48 || abs(sp.y)> 16){
					if(_isCanGear00){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				this->changeSide(sp);
				
				this->attack(OUGIS1);
				return;

			}else if(_isCanSkill3 && _mainTarget->getGP()<5000 && !_isBati && !_skillChangeBuffValue){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL3);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {
					this->idle();
					return;

				}
			}else if(abs(sp.x)<128){ 

				//交战策略
				if(abs(sp.x)> 96  || abs(sp.y)> 32){		
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				
				if(_isCanSkill1  && _mainTarget->getGP()<5000 &&!_isBati){
					if((abs(sp.x)> 32 || abs(sp.y)>32)){
						
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}
					this->changeSide(sp);
					this->attack(SKILL1);
				}else if(_isCanSkill2 && _mainTarget->getGP()<5000 && !_isBati){
					if(abs(sp.x)<32){
						this->stepBack();
						return;
					}
					this->changeSide(sp);
					this->attack(SKILL2);
				}else{

					if(abs(sp.x)> 32 || abs(sp.y)>32){
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}else{

						this->changeSide(sp);
						this->attack(NAttack);
					}
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_mainTarget->role()==Role::Flog && _isCanSkill1 && !_isBati){
				this->changeSide(sp);
				this->attack(SKILL1);
			}else if(_isCanSkill2  && _mainTarget->getGP()<5000 &&!_isBati
				&& _mainTarget->role()==Role::Flog && isBaseDanger){
					this->changeSide(sp);
					this->attack(SKILL2);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);
			}
		}
		return;
	}
	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		if(_isCanGear00){
			this->useGear(gear00);
		};
		this->stepOn();
	}

}

void Hero::AI_Kiba(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");


	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear00);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear04);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear08);
		}
	}


	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(_isCanGear00){
				this->useGear(gear00);
			};
			if(this->stepBack()){
				return;
			};
		}
	};

	if(_mainTarget && (battleCondiction>=0 || _isCanOugis1 || _isCanOugis2)){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK ){

			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game && !_powerUPBuffValue
				&& _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
				&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
				&& !_mainTarget->_isSticking 
				){
					if(abs(sp.x)> 32 || abs(sp.y)>32){
						if(_isCanGear00){
							this->useGear(gear00);
						};
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}

					this->changeSide(sp);
				
					this->attack(OUGIS2);
					return;
			}else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000 && !_powerUPBuffValue){
				this->changeSide(sp);		
				this->attack(OUGIS1);
				return;
			}else if(_isCanSkill1 && _isBati){
				//先手无视战斗力
				this->attack(SKILL1);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160 ){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;
				}

			}else if(abs(sp.x)<128){ 

				//交战策略

				if(abs(sp.x)> 32 || abs(sp.y)>32){
					if(_isCanGear00){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}


				if(_isCanSkill3 && !_powerUPBuffValue && !_isBati  && this->getHpPercent()>0.5){
					this->changeSide(sp);
					this->attack(SKILL3);

				}else{

					this->changeSide(sp);
					this->attack(NAttack);

				}
				return;
			}

		}

	}
	//无玩家情况
	if(battleCondiction>=0){
		_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	}else {
		_mainTarget=NULL;
		this->findEnemy2("Tower");
	}

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){

			if(_isCanSkill1 && _isBati){
				this->attack(SKILL1);
			}else if(_mainTarget->role()==Role::Tower  && _isCanSkill3 && !_powerUPBuffValue && !_isBati ){
				this->changeSide(sp);
				this->attack(SKILL3);
			}else{
				this->changeSide(sp);
				this->attack(NAttack);
			}

		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		if(_isCanGear00){
			this->useGear(gear00);
		};
		this->stepOn();
	}



}


 

void Hero::AI_Minato(){

	_mainTarget=NULL;
	this->findEnemy2("Hero");


	if(_isCanGear06){
		if((this->getActionState()==ACTION_STATE_FLOAT||
			this->getActionState()==ACTION_STATE_AIRHURT ||
			this->getActionState()==ACTION_STATE_HURT ||
			this->getActionState()==ACTION_STATE_KOCKDOWN
			) && this->getHpPercent()<0.5 && !_isBati && !_isWudi){
				this->useGear(gear06);
		}
	}
	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear06);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear05);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}
	if(this->getHpPercent()<0.3f){

		if(_isCanSkill1){

			CCObject* pObject;
			bool isMark=false;
			if(this->getMonsterArray() && this->getMonsterArray()->count()>0){
				CCARRAY_FOREACH(this->getMonsterArray(),pObject){
					Monster* mo=(Monster*) pObject;
					if(mo->isCharacter("HiraishinMark")){
						if(this->group()==Group::Konoha && mo->getPositionX()<this->getPositionX()){
							isMark=true;
						}else if(this->group()==Group::Akatsuki && mo->getPositionX()>this->getPositionX()){
							isMark=true;
						}
					};
				};
			};
			if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
				if(isMark){
					this->attack(SKILL1);
					return;
				};
			};

		};
	}
	

	if(this->checkRetri()){
		

		if(_mainTarget!=NULL){
			if(this->stepBack2()){
				return;
			};
		}else{
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){

			CCObject* pObject;
			bool isMark=false;
			if(this->getMonsterArray() && this->getMonsterArray()->count()>0){
				CCARRAY_FOREACH(this->getMonsterArray(),pObject){
					Monster* mo=(Monster*) pObject;
					if(mo->isCharacter("HiraishinMark")){
						isMark=true;
					}
				}
			};

			if(_isCanSkill1 && !isMark){
				this->changeSide(sp);
				this->attack(SKILL1);
			}else if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
				&& _mainTarget->getGP()<5000){
					if(abs(sp.x)> 64  || abs(sp.y)> 16){
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;

					}
					this->changeSide(sp);
					
					this->attack(OUGIS2);

					return;

			}else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000){
				if(abs(sp.x)> 64  || abs(sp.y)> 16){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;

				}
				this->changeSide(sp);		
				this->attack(OUGIS1);
				return;
			}else if(_isCanSkill2 && _mainTarget->getGP()<5000){
				//先手无视战斗力
				if(abs(sp.y)> 16){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				this->changeSide(sp);
				this->attack(SKILL2);
				return;
			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled && !_skillChangeBuffValue){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}

			}else if(_isCanSkill3){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL3);
				return;
			}else{ 

				//交战策略
				if(abs(sp.x)> 352  || abs(sp.y)> 128){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;

				}else if((abs(sp.x)> 32 || abs(sp.y)>32 ) && this->getnAttackValue()<260){

					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill3 && _mainTarget->role()==Role::Flog ){
				this->changeSide(sp);
				this->attack(SKILL3);

			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		if(_isCanSkill1){

			CCObject* pObject;
			bool isMark=false;
			if(this->getMonsterArray() && this->getMonsterArray()->count()>0){
				CCARRAY_FOREACH(this->getMonsterArray(),pObject){
					Monster* mo=(Monster*) pObject;
					if(mo->isCharacter("HiraishinMark")){
						
						if(this->group()==Group::Konoha && mo->getPositionX()>this->getPositionX()){
							isMark=true;
						}else if(this->group()==Group::Akatsuki && mo->getPositionX()<this->getPositionX()){
							isMark=true;
						}
						
					};
				};
			};
			if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
				if(isMark){
					this->attack(SKILL1);
					return;
				};
			};

		};

		this->stepOn();
	}

}



void Hero::AI_Hinata(){
	_mainTarget=NULL;
	this->findEnemy2("Hero");

	

	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear00);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear01);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear02);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			
			if(this->stepBack2()){
				return;
			};
		}else{

			if(_isCanGear00 ){
				this->useGear(gear00);
			};
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(_isCanGear00 ){
				this->useGear(gear00);
			};
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK ){

			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
				&& _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
				&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
				&& !_mainTarget->_isSticking){

					if(abs(sp.x)> 48 || abs(sp.y)> 32 ){
						if(_isCanGear00 ){
							this->useGear(gear00);
						};
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}

					this->changeSide(sp);

					this->attack(OUGIS2);
					return;
			}else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000){

				if(abs(sp.x)> 48 || abs(sp.y)> 32 ){
					if(_isCanGear00 ){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				this->changeSide(sp);
				this->attack(OUGIS1);
				return;
			}else if(_isCanSkill1){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL1);
				return;
			}else if((_isCanSkill2 || _isCanSkill3 ) && _mainTarget->getGP()<5000  && _skillUPBuffValue){
				if(abs(sp.x)> 48 || abs(sp.y)> 32 ){
					if(_isCanGear00 ){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}
				if(_isCanSkill2){
					this->changeSide(sp);
					this->attack(SKILL2);
					return;
				}
				
				if(_isCanSkill3){
					this->changeSide(sp);
					this->attack(SKILL3);
					return;
				}
	

			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}

			}else if(abs(sp.x)<128){

				

				//交战策略
				if(abs(sp.x)> 32 || abs(sp.y)>32 ){
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill2 && _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL2);

			}else if(_isCanSkill3 && _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL3);

			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		if(_isCanGear00 ){
			this->useGear(gear00);
		};
		this->stepOn();
	}

}

void Hero::AI_Neji(){
	
	_mainTarget=NULL;
	this->findEnemy2("Hero");


	
	//忍具购买
	if(this->getCoin()>=500 && !_isControled && _delegate->_isHardCoreGame){
		if(this->getGearArray()->count()==0){
			this->setGear(gear00);
		}else if(this->getGearArray()->count()==1){
			this->setGear(gear01);
		}else if(this->getGearArray()->count()==2){
			this->setGear(gear07);
		}
	}

	if(this->checkRetri()){
		if(_mainTarget!=NULL){
			
			if(this->stepBack2()){
				return;
			};
		}else{
			if(_isCanGear00 ){
				this->useGear(gear00);
			};
			if(this->stepBack()){
				return;
			};
		}
	};


	// 检测基地
	if(isBaseDanger && this->checkBase() && !_isControled){
		bool needBack=false;
		if(this->group()==Group::Akatsuki){
			if(this->getPositionX()<85*32){
				needBack=true;
			}
		}else{
			if(this->getPositionX()>11*32){
				needBack=true;
			}
		} 

		if(needBack){
			if(this->stepBack2()){
				return;
			};
		}

	}

	if(_mainTarget && _mainTarget->role()!=Role::Flog ){
		CCPoint moveDirection;
		CCPoint sp;

		if(_mainTarget->_originY){
			sp=ccpSub(ccp(_mainTarget->getPositionX(),_mainTarget->_originY),this->getPosition());
		}else{
			sp=ccpSub(_mainTarget->getPosition(),this->getPosition());
		}
		if((_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK ) && ( abs(sp.x)<128  || _isCanGear00) ){

			if(_isCanOugis2 && !_isControled && _delegate->_isOugis2Game 
				&& _mainTarget->getGP()<5000 && !_mainTarget->_isBati 
				&& _mainTarget->getActionState()!=ACTION_STATE_KOCKDOWN
				&& !_mainTarget->_isSticking){

					if(abs(sp.x)> 48|| abs(sp.y)> 32 ){
						if(_isCanGear00 ){
							this->useGear(gear00);
						};
						moveDirection =ccpNormalize(sp);
						this->walk(moveDirection);
						return;
					}

					this->changeSide(sp);

					this->attack(OUGIS2);
					return;
			}else if(_isCanOugis1  && !_isControled  && _mainTarget->getGP()<5000){
				
				if(abs(sp.x)> 48 || abs(sp.y)> 32 ){
					if(_isCanGear00 ){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				this->changeSide(sp);
				this->attack(OUGIS1);
				return;

			}else if(enemyCombatPoint>friendCombatPoint && abs(enemyCombatPoint-friendCombatPoint)>3000 && !_isHealling && !_isControled){
				//保持距离
				if(abs(sp.x)<160){
					this->stepBack2();
					return;
				}else {

					this->idle();
					return;

				}

			}else if(_isCanSkill1){
				//先手无视战斗力
				this->changeSide(sp);
				this->attack(SKILL1);
				return;
			}else if(_isCanSkill3 && _mainTarget->getGP()<5000 && _isBati){

				if(abs(sp.x)> 48 || abs(sp.y)> 32 ){
					if(_isCanGear00 ){
						this->useGear(gear00);
					};
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				this->changeSide(sp);
				this->attack(SKILL3);
				return;

			}else if(abs(sp.x)<128){

				
				//交战策略
				if(abs(sp.x)> 32 || abs(sp.y)>32 ){
					
					moveDirection =ccpNormalize(sp);
					this->walk(moveDirection);
					return;
				}

				if(_isCanSkill2 && _mainTarget->getGP()<5000){
					this->changeSide(sp);
					this->attack(SKILL2);
				}else{
					this->changeSide(sp);
					this->attack(NAttack);
				}

				return;
			}
		}

	}
	//无玩家情况
	_mainTarget=NULL;
		if(!this->findEnemy2("Flog")){
			this->findEnemy2("Tower");
		}
	

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
			return;
		}

		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			//技能清兵
			if(_isCanSkill2 && _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL2);

			}else if(_isCanSkill3 && _mainTarget->role()==Role::Flog && isBaseDanger){
				this->changeSide(sp);
				this->attack(SKILL3);

			}else{
				this->changeSide(sp);
				this->attack(NAttack);

			}	
		}
		return;
	}

	if(_isHealling &&this->getHpPercent()<1){
		if(_actionState==ACTION_STATE_IDLE || _actionState==ACTION_STATE_WALK || _actionState==ACTION_STATE_ATTACK){
			this->idle();
		}
	}else{
		if(_isCanGear00 ){
			this->useGear(gear00);
		};
		this->stepOn();
	}
}

void Hero::AI_Pain(){
}



