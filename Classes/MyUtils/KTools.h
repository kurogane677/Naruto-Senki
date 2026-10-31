#pragma once
#include "MyUtils/tinyxml2/tinyxml2.h"
#include "Compat/Compat2x.h"
#include "MyUtils/CMD5Checksum.h"
#include "MyUtils/sqlite3.h"


USING_NS_CC;

class CMD5Checksum;
class SelectLayer;
class NetworkLayer;
class KTools : public CCObject{
public:
	KTools(void);
	~KTools(void);

	bool init();
	CREATE_FUNC(KTools);

	void prepareFileSHA();

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


	CC_SYNTHESIZE(SelectLayer*,_delegate,Delegate);
	CC_SYNTHESIZE(NetworkLayer*,_delegate2,Delegate2);


	std::string encodeData(std::string data);
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

