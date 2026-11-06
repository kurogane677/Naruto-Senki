#pragma once
#include "MyUtils/tinyxml2/tinyxml2.h"
#include "Compat/Compat2x.h"
#include "MyUtils/sqlite3.h"


USING_NS_CC;

class SelectLayer;
class NetworkLayer;
class KTools : public CCObject{
public:
	KTools(void);
	~KTools(void);

	bool init();
	CREATE_FUNC(KTools);


	bool readXMLToArray(const char* filePath,CCArray* &array);
	void	initTableInDB();
	void	initColumeInDB();

	sqlite3* prepareTableInDB();


	
	bool	saveToSQLite(const char* table="GameRecord",const char* column=NULL,const char* value=NULL,bool isBuy=false);
	CCString*  readFromSQLite(const char* table="GameRecord",const char* column=NULL,const char* value=NULL);
	
	void encode(std::string &str,int randomKey);
	void decode(std::string &str);
	


	void prepareFileOGG(const char* listName,int type=0);

	

	CCString*  readSQLite(const char* table,const char* column,const char* value,const char* targetColumn);
	void saveSQLite(const char* table,const char* relatedColumn,const char* value,const char* targetColumn,const char* targetValue);


	// Text from Element/strings.xml; the key itself when it is missing
	static std::string uiText(const char* key);
	// Menu bar text in Arimo Medium: a screen title (top-left) or a description (bottom-left)
	static ax::Label* createBarTitle(const char* key);
	static ax::Label* createBarText(const char* key);
	// Wide screens (#20): stretch a 570px menu background to the screen width, or
	// continue a menu bar past its right end with its plain last column
	static void fitScreenWidth(CCSprite* background);
	static void extendBarToScreen(CCSprite* bar);

	CC_SYNTHESIZE(SelectLayer*,_delegate,Delegate);
	CC_SYNTHESIZE(NetworkLayer*,_delegate2,Delegate2);


};

class CCTips : public CCSprite{
public:
	CCTips(void);
	~CCTips(void);

	virtual bool			init(const char *tips);
	static CCTips*	create(const char *tips);


protected:

	void onDestroy();


};

