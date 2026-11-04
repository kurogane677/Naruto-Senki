#include "KTools.h"
#include "SelectLayer.h"
#include "NetworkLayer.h"

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

using namespace tinyxml2;
using namespace CocosDenshion;

KTools::KTools(void)
{
	_delegate=NULL;
	_delegate2=NULL;
}


KTools::~KTools(void)
{
}

bool KTools::init(){
	bool bRet=false;
	do{

		bRet=true;

	}while(0);
	
	return bRet;
}
 



bool KTools::readXMLToArray(const char* filePath,CCArray* &array)
{

	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();  
	//doc->LoadFile(filePath);  this's doesn't work for android
	unsigned long nSize;
	const char *data = (const char*)ccGetFileData(filePath, "r", &nSize);
	doc->Parse(data);



	XMLElement *rootEle=doc->RootElement();
	XMLElement *actionEle=rootEle->FirstChildElement();

	
	while (actionEle){
		const char* actionName=actionEle->FirstAttribute()->Value();

		XMLElement *actionNodeEle=actionEle->FirstChildElement();
        CCArray* actionArray=CCArray::create();
		CCArray* actionDate=CCArray::create();
		CCArray* actionFrame=CCArray::create();
		while(actionNodeEle){
			if(strcmp(actionNodeEle->Name(),"frame")){
			    XMLElement *dateEle=actionNodeEle->FirstChildElement();

				while(dateEle){

					const char* nodeKey=dateEle->FirstAttribute()->Value();
					CCString* nodeValue=CCString::create(dateEle->GetText());

					CCDictionary* dateDic=CCDictionary::create();
					dateDic->setObject(nodeValue,nodeKey);

					actionDate->addObject(dateDic);
					dateEle=dateEle->NextSiblingElement();
				}

			}else{
				
				XMLElement *frameEle=actionNodeEle->FirstChildElement();

				while(frameEle){
					const char* nodeKey;
					if(strcmp(frameEle->Name(),"frameName")){
						nodeKey=frameEle->FirstAttribute()->Value();

					}else{
						nodeKey=frameEle->Name();
					}
					
					CCString* nodeValue=CCString::create(frameEle->GetText());

					CCDictionary* frameDic=CCDictionary::create();
					frameDic->setObject(nodeValue,nodeKey);
					actionFrame->addObject(frameDic);

					frameEle=frameEle->NextSiblingElement();
				}
				
			}
			actionNodeEle= actionNodeEle->NextSiblingElement();
		}
		actionArray->addObject(actionDate);
		actionArray->addObject(actionFrame);

		array->addObject(actionArray);
		actionEle=actionEle->NextSiblingElement();
		
	}

	delete data;
	
	return true;

};



int isExisted(void *para,int n_column,char **column_value,char **column_name){
	bool *isExisted_=(bool*)para; 
	*isExisted_=(**column_value)!='0'; 
	return 0;
}


void  KTools::encode(std::string &str,int radomKey){
	
	for(int i=0;i<str.length();i++) {
		int ch = str[i];
		ch+=radomKey;
		str[i]=ch;
	}
	str+=radomKey;
	
}


void  KTools::decode(std::string &str){
	int num=str.length()-1;
	int key=str[num];
	str=str.substr(0,num);

	for(int i=0;i<str.length();i++) {
		int ch = str[i];
		ch-=key;
		str[i]=ch;
	}


}


void KTools::initTableInDB(){

	sqlite3* pDB = NULL;
	char* errorMsg=NULL;
	std::string path=CCFileUtils::sharedFileUtils()->getWritablePath() +"sql.db"; 
	int result=sqlite3_open(path.c_str(),&pDB);  
	if(result!=SQLITE_OK){
		CCLOG("open sql file Failed!");
		return;
	};

	CCString*sql=CCString::create("drop table IF EXISTS Achievement");

	sqlite3_exec(pDB,sql->getCString(),NULL,NULL,&errorMsg);


	sql=CCString::create("create table if not exists CharRecord (name char(20)  primary key ,column1 char(10),column2 char(10),column3 char(10))");
	sqlite3_exec(pDB,sql->getCString(),NULL,NULL,&errorMsg);
	 
	sql=CCString::create("select * from  GameRecord");
	
	sqlite3_exec(pDB,sql->getCString(),NULL,NULL,&errorMsg);
	


	if(errorMsg!=NULL){
		errorMsg=NULL;
		sql=CCString::create("create table if not exists GameRecord (id char(10) primary key,coin char(20),version char(20))");
		sqlite3_exec(pDB,sql->getCString(),NULL,NULL,&errorMsg);
		
		std::string str="n>";
		CCString* coin=CCString::create(str);

		sql=CCString::createWithFormat("insert into GameRecord values(1,'%s','1')",coin->getCString());
		int result=sqlite3_exec(pDB,sql->getCString(),NULL,NULL,&errorMsg);

		if(errorMsg!=NULL){
			
			CCLOG("exec sql %s failed with mgs: %s",sql->getCString(),errorMsg);
			sqlite3_close(pDB);
			return;
		}

	
	}

	
	bool isExisted_;
	
	sql=CCString::create("select count(*) from  CharRecord");
	sqlite3_exec(pDB,sql->getCString(),isExisted,&isExisted_,&errorMsg);
	
	if(!isExisted_){

		char* heroList[]={"Konan","Sakura","Naruto","Sai","Deidara",
			"Kakashi","Itachi","Tenten","Jiraiya","Suigetsu",
			"Tsunade","Tobirama","Neji","Ino","Asuma","Gaara",
			"Karin","Sasuke","Hidan","Choji","Kankuro",
			"Shino","Minato","Tobi","Kakuzu","Hinata",
			"Shikamaru","Chiyo","Kisame",
			"Hiruzen","Kiba","Jugo","Lee","Kurenai"
		};
		int num= sizeof(heroList)/sizeof(char*);

		for(int i=0;i<num;i++){

			std::string name=heroList[i];

			int key=rand()%50+40;
			encode(name,key);
			CCString* nameDB=CCString::create(name);
		

			std::string column1="0";
			key=rand()%60+40;
			encode(column1,key);
			CCString* column1DB=CCString::create(column1);

			std::string column2="0";
			key=rand()%60+40;
			encode(column2,key);
			CCString* column2DB=CCString::create(column2);


			std::string column3="";
			key=rand()%60+40;
			encode(column3,key);
			CCString* column3DB=CCString::create(column3);




			CCString* sql=CCString::createWithFormat("insert into  CharRecord values('%s','%s','%s','%s')",nameDB->getCString(),column1DB->getCString(),column2DB->getCString(),column3DB->getCString());
			sqlite3_exec(pDB,sql->getCString(),NULL,NULL,&errorMsg);

			if(errorMsg!=NULL){
				CCLOG("exec sql %s failed with mgs: %s",sql->getCString(),errorMsg);
				sqlite3_close(pDB);
				return;
			}

		}


	}

	sqlite3_close(pDB);
}


void KTools::initColumeInDB(){

	sqlite3* pDB = NULL;
	char* errorMsg=NULL;
	std::string path=CCFileUtils::sharedFileUtils()->getWritablePath() +"sql.db"; 
	int result=sqlite3_open(path.c_str(),&pDB);  
	if(result!=SQLITE_OK){
		CCLOG("open sql file Failed!");
		return;
	};

	
	char **result2;
	int row=0;
	int column=0;
	
	CCString* sql=CCString::create("select coin from GameRecord");
	sqlite3_get_table(pDB,sql->getCString(),&result2,&row,&column,NULL);
	std::string str2=result2[1];
	decode(str2);

	
	std::string str3="uuuuu<";
	decode(str3);
	if(atoi(str2.c_str())>atoi(str3.c_str())){
		CCString* coin=CCString::create("uuuuu<");
		sql=CCString::createWithFormat("update GameRecord set coin='%s'",coin->getCString());
		sqlite3_exec(pDB,sql->getCString(),NULL,NULL,&errorMsg);
	}

	
	

	
}




void KTools::prepareFileOGG(const char* listName,int type/* =0 */){

	if(CCUserDefault::sharedUserDefault()->getBoolForKey("isPreload")){
		
	}else{
		return;
	}

	//MD5 

	std::string md5Path =  "Audio/list.xml";

	bool isExisted=CCFileUtils::sharedFileUtils()->isFileExist(md5Path);

	if(!isExisted){
	return;
	}

	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();  
	unsigned long nSize;
	const char *pXmlBuffer = (const char*)ccGetFileData(md5Path.c_str(), "r", &nSize);
	doc->Parse(pXmlBuffer);

	XMLElement *rootEle=doc->RootElement();
	XMLElement *fileEle=rootEle->FirstChildElement();

	while (fileEle){
	const char* pathSrc=fileEle->FirstAttribute()->Value();
	std::string soundPath= fileEle->GetText();	 
	if(strcmp(pathSrc,listName)==0){
	if(type==0){
	SimpleAudioEngine::sharedEngine()->preloadEffect(soundPath.c_str());
	}else if(type==1){
	SimpleAudioEngine::sharedEngine()->unloadEffect(soundPath.c_str());
	}
	}

	fileEle=fileEle->NextSiblingElement();	
	}
	return;

}











sqlite3* KTools::prepareTableInDB(){

	sqlite3* pDB = NULL;
	char* errorMsg=NULL;

	std::string path=CCFileUtils::sharedFileUtils()->getWritablePath() +"sql.db"; 
	int result=sqlite3_open(path.c_str(),&pDB);  

	if(result!=SQLITE_OK){
		CCLOG("open sql file Failed!");
		return NULL;
	};

	return pDB;


}
bool KTools::saveToSQLite(const char* table/* ="GameRecord"*/,const char* column/* =NULL */,const char* value/* =NULL */,bool isBuy /*= false */){

	sqlite3* pDB =prepareTableInDB();
	char* errorMsg=NULL;
	if(pDB!=NULL){

		int key=rand()%60+40;
		std::string str=value;


		encode(str,key);
		CCString* coin=CCString::create(str);
		CCString* sql=CCString::createWithFormat("update %s set %s='%s'",table,column,coin->getCString());
		sqlite3_exec(pDB,sql->getCString(),NULL,NULL,&errorMsg);
		sqlite3_close(pDB);

		if(errorMsg!=NULL){
			sqlite3_free(errorMsg);
			return false;
		}
		return true;
	}

	return false;
}




CCString* KTools::readFromSQLite(const char* table/* ="GameRecord" */,const char* column/* =NULL */,const char* value/* =NULL */){

	sqlite3* pDB =prepareTableInDB();


	if(pDB!=NULL){

		char **result;
		CCString* sql=CCString::createWithFormat("select %s from  GameRecord","coin");
		int row=0;
		int column=0;

		std::string str;
		if(sqlite3_get_table(pDB,sql->getCString(),&result,&row,&column,NULL)==SQLITE_OK){
			if(row>0 && result[1]){
				str=result[1];
				decode(str);
			}
			sqlite3_free_table(result);
		}
		sqlite3_close(pDB);
		return CCString::create(str.empty()?"0":str);
	}
	return CCString::create("0");

}

CCString* KTools::readSQLite(const char* table,const char* column,const char* value,const char* targetColumn){

	std::string target;
	sqlite3* pDB =prepareTableInDB();
	if(pDB!=NULL){
		char **result=NULL;
		int row=0;
		int columns=0;
		CCString* sql=CCString::createWithFormat("select %s,%s from %s",column,targetColumn,table);
		if(sqlite3_get_table(pDB,sql->getCString(),&result,&row,&columns,NULL)==SQLITE_OK){
			// result holds a header row, then `row` rows of (key, target)
			for(int r=1;r<=row;r++){
				if(!result[r*2]) continue;
				std::string key=result[r*2];
				decode(key);
				if(key==value){
					if(result[r*2+1]){
						target=result[r*2+1];
						decode(target);
					}
					break;
				}
			}
			sqlite3_free_table(result);
		}
		sqlite3_close(pDB);
	}

	if(strcmp(targetColumn,"column3")==0 || strcmp(targetColumn,"column4")==0){
		return CCString::create(target);
	}
	return CCString::createWithFormat("%d",atoi(target.c_str()));
}



void KTools::saveSQLite(const char* table,const char* relatedColumn,const char* value,const char* targetColumn,const char* targetValue){

	sqlite3* pDB =prepareTableInDB();
	if(pDB==NULL){
		return;
	}

	char **result=NULL;
	int row=0;
	int columns=0;
	// keys are stored encoded, so the row is found by decoding every key
	std::string encodedKey;
	CCString* sql=CCString::createWithFormat("select %s from %s",relatedColumn,table);
	if(sqlite3_get_table(pDB,sql->getCString(),&result,&row,&columns,NULL)==SQLITE_OK){
		for(int r=1;r<=row;r++){
			if(!result[r]) continue;
			std::string key=result[r];
			decode(key);
			if(key==value){
				encodedKey=result[r];
				break;
			}
		}
		sqlite3_free_table(result);
	}

	if(!encodedKey.empty()){
		std::string saveValue=targetValue;
		encode(saveValue,rand()%50+40);
		sql=CCString::createWithFormat("update %s set %s='%s' where %s='%s'",table,targetColumn,saveValue.c_str(),relatedColumn,encodedKey.c_str());
		sqlite3_exec(pDB,sql->getCString(),NULL,NULL,NULL);
	}else{
		// no row yet, e.g. a hero added after this sql.db was created
		std::string key=value;
		encode(key,rand()%50+40);
		std::string saveValue=targetValue;
		encode(saveValue,rand()%50+40);
		sql=CCString::createWithFormat("insert into %s (%s,%s) values('%s','%s')",table,relatedColumn,targetColumn,key.c_str(),saveValue.c_str());
		sqlite3_exec(pDB,sql->getCString(),NULL,NULL,NULL);
	}
	sqlite3_close(pDB);
}




CCTips::CCTips(void)
{
}

CCTips::~CCTips(void)
{
}

bool CCTips::init(const char *tips){
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCSprite::init());
		this->setAnchorPoint(ccp(0.5,0.5));
		CCDictionary *strings = CCDictionary::createWithContentsOfFile("Element/strings.xml");  
		const char *reply = ((CCString*)strings->objectForKey(tips))->m_sString.c_str();  

		CCLabelTTF* tipLabel=CCLabelTTF::create(reply,FONT_TYPE,12);
		this->addChild(tipLabel,5000);
		this->setPosition(ccp(winSize.width/2,50));
		CCFiniteTimeAction *call=CCCallFunc::create(this,callfunc_selector(CCTips::onDestroy));

		CCActionInterval*  mv=CCMoveBy::create(0.2f,ccp(0,12));
		CCActionInterval* fadeOut=CCFadeOut::create(0.2f);
		CCDelayTime* delay=CCDelayTime::create(2.0f);
		CCActionInterval* sp=CCSpawn::create(fadeOut,mv,NULL);

		CCArray* seqArray=CCArray::create();
		seqArray->addObject(sp);
		seqArray->addObject(delay);
		seqArray->addObject(call);


		CCAction* seq=CCSequence::create(seqArray);

		this->runAction(seq);


		bRet=true;

	}while(0);

	return bRet;

}

void CCTips::onDestroy(){
	this->removeAllChildren();
	this->removeFromParentAndCleanup(true);
}



CCTips*  CCTips::create( const char *tips ){
	CCTips* ab=new  CCTips();
	if (ab && ab->init(tips))
	{
		ab->autorelease();
		return ab;
	}
	else
	{
		delete ab;
		return NULL;
	}
}