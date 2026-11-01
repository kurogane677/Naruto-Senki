#include "Characters.h"
#include "HudLayer.h"
USING_NS_CC;




HPBar::HPBar(void)
{

	hpBar=NULL;
	hpBottom=NULL;
};


HPBar::~HPBar(void)
{

};


bool HPBar::init(const char *szImage){
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCSprite::init());

		char fileName[3]="xx";
		strncpy(fileName,szImage,2);


		hpBar=CCSprite::createWithSpriteFrameName(szImage);
		hpBar->setAnchorPoint(ccp(0,0));		
		this->addChild(hpBar,1);

		if(strcmp(fileName,"hp")==0){
			hpBottom=CCSprite::createWithSpriteFrameName("hp_bottom.png");
			hpBar->setPosition(ccp(15,1));
		}else{ 
			hpBottom=CCSprite::createWithSpriteFrameName("flog_bar_buttom.png");
			hpBar->setPosition(ccp(1,1));
		}
		hpBottom->setAnchorPoint(ccp(0,0));
		this->addChild(hpBottom,-1);

		bRet=true;

	}while(0);

	return bRet;

}

void HPBar::changeBar(const char *szImage){
	CCSpriteFrame *frame=CCSpriteFrameCache::sharedSpriteFrameCache()->getSpriteFrameByName(szImage); 
	hpBar->setSpriteFrame(frame);
}

void HPBar::loseHP(float percent){

	
	if(_delegate->getDelegate()->_isHardCoreGame){

		const char* gardTower;
		if(_delegate->getDelegate()->zhenying>0){
			gardTower="AkatsukiCenter";
		}else{
			gardTower="KonohaCenter";
		}
		
		if(strcmp(this->_delegate->getCharacter()->getCString(),gardTower)==0){
			if( !_delegate->getDelegate()->_isGuardian && percent<=0.8){
				_delegate->getDelegate()->initGard();
			}


		}

		if(_delegate->getDelegate()->_isGuardian){
			ActionManager* _slayer=(ActionManager*) _delegate->_slayer;
			if(_slayer){
				if(strcmp(this->_delegate->getCharacter()->getCString(),gardTower)==0){
					_slayer->isHurtingTower=true;
				}else{
					_slayer->isHurtingTower=false;
				}
			}

		}
		
	}



	if(percent<=0){

		ActionManager* _slayer=(ActionManager*) _delegate->_slayer;
		ActionManager* currentSlayer;


		//杀死青蛙
		if(_delegate->role()==Role::Flog){

			if(_slayer->getSecMaster()  && !_slayer->isCharacter("KageHand")
				 && !_slayer->isCharacter("KageHands")
				){
				if(_slayer->getSecMaster()->getControler()){
					currentSlayer=_slayer->getSecMaster()->getControler();
				}else{
					currentSlayer=_slayer->getSecMaster();
				}
				
			}else if (_slayer->getMaster()){
				if(_slayer->getMaster()->getControler()){
					currentSlayer=_slayer->getMaster()->getControler();
				}else{
					currentSlayer=_slayer->getMaster();
				}
			}else{
				if(_slayer->getControler()){
					currentSlayer=_slayer->getControler();
				}else{
					currentSlayer=_slayer;
				}
			}

			//全额经验


			if(currentSlayer->getLV()!=6){

				if(currentSlayer->isCharacter("Naruto")||
					currentSlayer->isCharacter("SageNaruto")
					){
						currentSlayer->setEXP(currentSlayer->getEXP()+12);
						currentSlayer->changeHPbar();

				}else{
					currentSlayer->setEXP(currentSlayer->getEXP()+10);
					currentSlayer->changeHPbar();
				}
				
			}

			

			
			if(currentSlayer->role()==Role::Player 
				){
					if(_delegate->getMaxHP()==10000){
						_delegate->setCoinDisplay(30);
						_delegate->getDelegate()->setCoin("30");
						_delegate->getDelegate()->getHudLayer()->setEXPLose(0);
					}else if(_delegate->getMaxHP()==5000){
						_delegate->setCoinDisplay(20);
						_delegate->getDelegate()->setCoin("20");
						_delegate->getDelegate()->getHudLayer()->setEXPLose(0);
					}else{
						_delegate->setCoinDisplay(10);
						_delegate->getDelegate()->setCoin("10");
						_delegate->getDelegate()->getHudLayer()->setEXPLose(0);
	
					}
					
			}



		currentSlayer->_flogNum+=1;
		if(_delegate->getMaxHP()==10000){
			currentSlayer->addCoin(30);
		}else if(_delegate->getMaxHP()==5000){
			currentSlayer->addCoin(20);
		}else{
			currentSlayer->addCoin(10);
		}

		}//拆塔
		else if(_delegate->role()==Role::Tower){


			if(_slayer->getSecMaster()  
				&& !_slayer->isCharacter("KageHand")
				 && !_slayer->isCharacter("KageHands")
				&& !_slayer->isCharacter("SmallSlug")){
					if(_slayer->getSecMaster()->getControler()){
						currentSlayer=_slayer->getSecMaster()->getControler();
					}else{
						currentSlayer=_slayer->getSecMaster();
					}
			}else if (_slayer->getMaster()){
				if(_slayer->getMaster()->getControler()){
					currentSlayer=_slayer->getMaster()->getControler();
				}else{
					currentSlayer=_slayer->getMaster();
				}
			}else{
				if(_slayer->getControler()){
					currentSlayer=_slayer->getControler();
				}else{
					currentSlayer=_slayer;
				}
			}

			if((currentSlayer->role()!=Role::Flog)){
				_delegate->getDelegate()->setReport(currentSlayer->getCharacter()->getCString(),"Tower",currentSlayer->getKillNum()) ;
			

				if(currentSlayer->getLV()!=6){
					if(currentSlayer->isCharacter("Naruto")||
						currentSlayer->isCharacter("SageNaruto")
						){
							currentSlayer->setEXP(currentSlayer->getEXP()+625);
							currentSlayer->changeHPbar();

					}else{
						currentSlayer->setEXP(currentSlayer->getEXP()+500);
						currentSlayer->changeHPbar();
					}

					
				}

			
				
				if(currentSlayer->role()==Role::Player){
					 if(_delegate->getDelegate()->_isHardCoreGame){
						 if(this->_delegate->getMaxHP() >40000){
							 _delegate->getDelegate()->setCoin("1000");
							 _delegate->setCoinDisplay(1000);
						 }else{
							 _delegate->getDelegate()->setCoin("500");
							 _delegate->setCoinDisplay(500);
						 }
					
					 }else{
						 _delegate->getDelegate()->setCoin("300");
						 _delegate->setCoinDisplay(300);
					 }
				
					_delegate->getDelegate()->getHudLayer()->setEXPLose(0);
				}

				if(_delegate->getDelegate()->_isHardCoreGame){
					if(this->_delegate->getMaxHP() >40000){
						currentSlayer->addCoin(1000);
					}else{
						currentSlayer->addCoin(500);
					}
				}else{
					currentSlayer->addCoin(300);
				}

			}


			//同队分享
			CCObject* pObject;
			CCARRAY_FOREACH(_delegate->getDelegate()->_CharacterArray,pObject){
				Hero* otherSlayer=(Hero*) pObject;
				if(currentSlayer->group()==otherSlayer->group() &&
					strcmp(currentSlayer->getCharacter()->getCString(),otherSlayer->getCharacter()->getCString())!=0
					){

						
						if(otherSlayer->getLV()!=6){
							otherSlayer->setEXP(otherSlayer->getEXP()+250);
							otherSlayer->changeHPbar();
						}

						


						if (otherSlayer->role()==Role::Player){

							if(_delegate->getDelegate()->_isHardCoreGame){
								if(this->_delegate->getMaxHP() >40000){
									_delegate->getDelegate()->setCoin("850");
									_delegate->setCoinDisplay(850);
								}else{
									_delegate->getDelegate()->setCoin("350");
									_delegate->setCoinDisplay(350);
									
								}
							}else{
								_delegate->getDelegate()->setCoin("150");
								_delegate->setCoinDisplay(150);
							}

							_delegate->getDelegate()->getHudLayer()->setEXPLose(0);
						}
							


						if(_delegate->getDelegate()->_isHardCoreGame){
							if(this->_delegate->getMaxHP() >40000){
								  otherSlayer->addCoin(850);
							}else{
								  otherSlayer->addCoin(350);
							}
						}else{
							  otherSlayer->addCoin(150);
						}

					  
						

				}
			}



		}
	
		else if((_delegate->role()==Role::Player || _delegate->role()==Role::Com)){
				


			if(_delegate->isCharacter("Kakuzu") && _delegate->getDelegate()->_isOugis2Game){
				
				bool reieveAble=false;
				if(_delegate->getCKR2()>=25000 && _delegate->hearts>0){
					if(_delegate->role()==Role::Player){
						if(_delegate->getDelegate()->getHudLayer()->skill5Button){
							reieveAble=true;
						}
					}else if(_delegate->role()==Role::Com){
						reieveAble=true;
					}

				}


				if(reieveAble && _delegate->getActionState()!=ACTION_STATE_O2ATTACK  && !_delegate->_isWudi && _delegate->getActionState()!=ACTION_STATE_DEAD){

					if(_delegate->_isSticking){
						_delegate->_isSticking=false;
					}
					if(_delegate->getActionState()==ACTION_STATE_FLOAT ||
						_delegate->getActionState()==ACTION_STATE_AIRHURT){
							this->setPositionY(_delegate->_originY);
							_delegate->_originY=0;
							_delegate->getDelegate()->reorderChild(_delegate,-_delegate->getPositionY());
					}

					
					if(_delegate->role()==Role::Player){

						_delegate->getDelegate()->getHudLayer()->skill5Button->unLock();
						_delegate->setActionState(ACTION_STATE_IDLE);
						_delegate->getDelegate()->setSkillFinish(true);
						_delegate->getDelegate()->getHudLayer()->skill5Button->click();	
						_delegate->getDelegate()->getHudLayer()->skill5Button->setLock();

					}else{

						if(_delegate->_isCanOugis2){

							
							_delegate->setActionState(ACTION_STATE_IDLE);
							_delegate->attack(OUGIS2);
						}

					}
					_delegate->hearts-=1;

					if(_delegate->_heartEffect){
						CCSpriteFrame *frame=CCSpriteFrameCache::sharedSpriteFrameCache()->getSpriteFrameByName(CCString::createWithFormat("Heart_Effect_%02d.png",_delegate->hearts)->getCString()); 
						_delegate->_heartEffect->setSpriteFrame(frame);
					}

					if(_delegate->hearts<1){
						if(_delegate->role()==Role::Player){
							if(_delegate->getDelegate()->getHudLayer()->skill4Button){
								_delegate->getDelegate()->getHudLayer()->skill4Button->setLock();
							}
						}
					}

					return;
				}

				if( _delegate->getActionState()==ACTION_STATE_O2ATTACK){
					return;
				}


				
			}


				if(_delegate->_isControled){
					_delegate->_isSuicide=true;
					currentSlayer=_delegate->getControler();
				}else if(_slayer->getSecMaster() 
					&& !_slayer->isCharacter("KageHand")
					 && !_slayer->isCharacter("KageHands")
					&& !_slayer->isCharacter("SmallSlug")
					&& !_slayer->isCharacter("FakeItachi")
					){
					if(_slayer->getSecMaster()->getControler()){
						currentSlayer=_slayer->getSecMaster()->getControler();
					}else{
						currentSlayer=_slayer->getSecMaster();
					}
				}else if (_slayer->getMaster()){
					if(_slayer->getMaster()->getControler()){
						currentSlayer=_slayer->getMaster()->getControler();
					}else{
						currentSlayer=_slayer->getMaster();
					}
				}else{
					if(_slayer->getControler()){
						currentSlayer=_slayer->getControler();
					}else{
						currentSlayer=_slayer;
					}
				}

				if(currentSlayer->isCharacter("Kakuzu") ){
					if(currentSlayer->hearts<=4){
						currentSlayer->hearts+=1;	

						if(currentSlayer->_heartEffect){
							CCSpriteFrame *frame=CCSpriteFrameCache::sharedSpriteFrameCache()->getSpriteFrameByName(CCString::createWithFormat("Heart_Effect_%02d.png",currentSlayer->hearts)->getCString()); 
							currentSlayer->_heartEffect->setSpriteFrame(frame);
						}

						if(currentSlayer->role()==Role::Player){
							if(currentSlayer->getMonsterArray() && currentSlayer->getMonsterArray()->count()<3 && currentSlayer->getLV()>=2){
								if(_delegate->getDelegate()->getHudLayer()->skill4Button){
									_delegate->getDelegate()->getHudLayer()->skill4Button->unLock();
								}
							}										
						}

					}

				}

				if(currentSlayer->role()!=Role::Flog){

					int realKillNum=currentSlayer->getKillNum()+1;
					currentSlayer->setKillNum(realKillNum);
					_delegate->getDelegate()->setReport(currentSlayer->getCharacter()->getCString(),_delegate->getCharacter()->getCString(),currentSlayer->getKillNum());
				
					
					//记分牌
					const char* currentTeam;
					if(_delegate->getDelegate()->zhenying>0){
						 currentTeam="Konoha";
					}else{
						 currentTeam="Akatsuki";
					}
					
					if(strcmp(currentSlayer->getGroup()->getCString(),currentTeam)==0){
						int teamKills=atoi(_delegate->getDelegate()->getHudLayer()->KonoLabel->getString().data())+1;
						_delegate->getDelegate()->getHudLayer()->KonoLabel->setString(CCString::createWithFormat("%d",teamKills)->getCString());
					}else{
						int teamKills=atoi(_delegate->getDelegate()->getHudLayer()->AkaLabel->getString().data())+1;
						_delegate->getDelegate()->getHudLayer()->AkaLabel->setString(CCString::createWithFormat("%d",teamKills)->getCString());
					}
					//
					if(!currentSlayer->isCharacter("Han") &&
						!currentSlayer->isCharacter("Roshi")
						){
							int newValue=atoi(_delegate->getDelegate()->getTotalKills()->getCString())+1;
							_delegate->getDelegate()->setTotalKills(CCString::createWithFormat("%d",newValue));
					}

					

					
					if( currentSlayer->getLV()!=6){

						if(currentSlayer->isCharacter("Naruto")||
							currentSlayer->isCharacter("SageNaruto")
							){
								currentSlayer->setEXP(currentSlayer->getEXP()+125);
								currentSlayer->changeHPbar();

						}else{

							currentSlayer->setEXP(currentSlayer->getEXP()+100);
							currentSlayer->changeHPbar();
						}

					}
				
				

				
					if (currentSlayer->role()==Role::Player){
						
						if(_delegate->isCharacter("Han")||
							_delegate->isCharacter("Roshi")
							){
								_delegate->getDelegate()->setCoin("1000");
								_delegate->setCoinDisplay(1000);
						}else{
							if(_delegate->getDelegate()->_isHardCoreGame){
								_delegate->getDelegate()->setCoin(CCString::createWithFormat("%d",50+(_delegate->getLV()-1)*10)->getCString());
								_delegate->setCoinDisplay(50+(_delegate->getLV()-1)*10);
							}else{
								_delegate->getDelegate()->setCoin(CCString::createWithFormat("%d",50)->getCString());
								_delegate->setCoinDisplay(50);
							}


						}

						_delegate->getDelegate()->getHudLayer()->setEXPLose(0);
						const char* kl=_delegate->getDelegate()->getHudLayer()->killLabel->getString().data();
						int kills=atoi(kl)+1;
						_delegate->getDelegate()->getHudLayer()->killLabel->setString(CCString::createWithFormat("%d",kills)->getCString());
					}



					if(_delegate->isCharacter("Han")||
					_delegate->isCharacter("Roshi")
					){
					currentSlayer->addCoin(1000);
					}else{
						if(_delegate->getDelegate()->_isHardCoreGame){
						currentSlayer->addCoin(50+(_delegate->getLV()-1)*10);
						}else{
						currentSlayer->addCoin(50);
						
						}
				
					}


					

				}

				//同队分享
				CCObject* pObject;
				CCARRAY_FOREACH(_delegate->getDelegate()->_CharacterArray,pObject){
					Hero* otherSlayer=(Hero*) pObject;
					if(currentSlayer->group()==otherSlayer->group() &&
						strcmp(currentSlayer->getCharacter()->getCString(),otherSlayer->getCharacter()->getCString())!=0
						){

						
						if(otherSlayer->getLV()!=6){
							otherSlayer->setEXP(otherSlayer->getEXP()+25);
							otherSlayer->changeHPbar();
						}

						
						if (otherSlayer->role()==Role::Player){
							if(_delegate->getDelegate()->_isHardCoreGame){
								if(_delegate->isCharacter("Han")||
									_delegate->isCharacter("Roshi")
									){
										_delegate->getDelegate()->setCoin(CCString::createWithFormat("%d",850)->getCString());
										_delegate->setCoinDisplay(850);
								}else{
									_delegate->getDelegate()->setCoin(CCString::createWithFormat("%d",25+(_delegate->getLV()-1)*10)->getCString());
									_delegate->setCoinDisplay(25+(_delegate->getLV()-1)*10);
								}
							
							}else{
								_delegate->getDelegate()->setCoin(CCString::createWithFormat("%d",25)->getCString());
								_delegate->setCoinDisplay(25);
							}
							_delegate->getDelegate()->getHudLayer()->setEXPLose(0);
						}
						if(_delegate->getDelegate()->_isHardCoreGame){		
							if(_delegate->isCharacter("Han")||
								_delegate->isCharacter("Roshi")
								){
									otherSlayer->addCoin(850);
							}else{
								otherSlayer->addCoin(25+(_delegate->getLV()-1)*10);		
							}

						}else{
							otherSlayer->addCoin(25);
						}
						

					}
				}

				

		}

		_delegate->dead();

	}else{
		CCActionInterval* s=CCScaleTo::create(0.2f,percent,1);
		hpBar->runAction(s);
		if(_delegate->role()==Role::Player){
			_delegate->getDelegate()->setHPLose(percent);
		}
	}

}


HPBar* HPBar::create( const char *szImage ){
	HPBar* hb=new HPBar();
	if (hb && hb->init(szImage))
	{
		hb->autorelease();
		return hb;
	}
	else
	{
		delete hb;
		return NULL;
	}
}
