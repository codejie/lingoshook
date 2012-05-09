/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/
#ifndef __HTMLCLEAROBJECT_H__
#define __HTMLCLEAROBJECT_H__

class CHtmlClearObject
{
public:
	CHtmlClearObject();
	virtual ~CHtmlClearObject();

	int UpdateDatabase(DictObject *objDict);

	static int Clear(const std::wstring& input, std::wstring& output);
private:
	int LoadSrcData(int& wordid, int& srcid);
	int CheckSrcID(int srcid);
};


#endif
