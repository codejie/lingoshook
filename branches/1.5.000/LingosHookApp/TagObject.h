/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __TAGOBJECT_H__
#define __TAGOBJECT_H__

#include <map>

#include "DBAccess.h"

//class CDBAccess;

class CTagObject
{
public:
    static const wxString DEFAULT_SYSTEM_TAG;
public:
    struct TRecord
    {
        wxString m_strTitle;
        wxDateTime m_dtCreateTime;
        wxString m_strDesc;
        int m_uiCounter;
    };
    typedef std::map<int, TRecord> TRecordMap;//id + record

    typedef std::vector<int> TWordIDVector;
    typedef std::vector<int> TTagIDVector;
public:
    CTagObject(CDBAccess& db);
    virtual ~CTagObject();

    int Init();
    int Load();

    int InsertTag(const wxString& title, const wxString& desc);
    int RemoveTag(int id);
    int RenameTag(int tagid, const wxString& title, const wxString& desc);

    int UpdateDefaultTag(int id);

    int AddIndex(int wordid, int tagid);
    int DeleteIndex(int wordid, int tagid);
    int DeleteWord(int wordid);

    int GetWordByTag(int tagid, TWordIDVector& vctwordid);
    int GetTagByWord(int wordid, TTagIDVector& vcttagid);

    int GetWordByTag(int tagid);
    int GetTagByWord(int wordid);

    int DefaultTag() const;
    int SysDefTag() const;

    const wxString GetTitle(int tagid) const;
    const wxString GetDescription(int tagid) const;

    //void GetAll();

    int ShowTagSubMenu(int menubase, wxMenu*& submenu) const;
protected:
    int IsIndexExist(int wordid, int tagid);
    int IsOnlySysDefTag(int wordid);
    int IsNoTag(int wordid);
    int InsertIndex(int wordid, int tagid);
    int RemoveIndex(int wordid, int tagid);
protected:
    CDBAccess::TDatabase& _db;
private:
    TRecordMap _mapRecord;
    int _iDefaultTag;
    int _iSysDefTag;
};


#endif
