/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __FILTERSHOWOBJECT_H__
#define __FILTERSHOWOBJECT_H__

#include <map>
#include <set>

#include "wx/wx.h"

#include "LHControls.h"
#include "DBAccess.h"

namespace FilterShow
{

enum FilterMode { FM_UNKNOWN = -1, FM_DATE = 0, FM_TAG, FM_SCORE };

class CBase
{
public:
    CBase(FilterMode mode = FM_UNKNOWN);
    virtual ~CBase() {}

    static CBase* GetFilter(FilterMode mode);
    FilterMode GetMode() const { return _eMode; }

    virtual int LoadWords(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree) = 0;
    virtual int AddTitle(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int id) { return -1; }
    virtual int RemoveTitle(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int id) { return -1; }
    virtual int AddWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid) = 0;
    virtual int RemoveWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid) = 0;
    virtual int UpdateWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid) = 0;
protected:
    FilterMode _eMode;
};  

class CDateMode : public CBase
{
protected:
    typedef std::pair<CLHFilterTreeItemData::ItemType, int> TDateType;
    typedef std::map<TDateType, wxTreeItemId> TDateMap;

    static const size_t MAX_DAY     =   6;
    static const size_t MAX_WEEK    =   12;    
public:
    CDateMode();
    virtual ~CDateMode() {}

    virtual int LoadWords(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree);
    virtual int AddWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid);
    virtual int RemoveWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid);
    virtual int UpdateWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid) { return -1; }

protected:
    void PreLoad(CLHFilterTreeCtrl* tree);
    void PostLoad(CLHFilterTreeCtrl* tree);
    int ShowWord(CLHFilterTreeCtrl* tree, int wordid, const wxString& word, const wxDateTime& checkin);
private:
    wxTreeItemId GetItemID(CLHFilterTreeCtrl* tree, const wxDateTime& checkin);
    void UpdateItemText(CLHFilterTreeCtrl *tree, const wxTreeItemId& itemid);
    int GetWordData(CDBAccess::TDatabase& db, int wordid, wxString& word, wxDateTime& date) const;
private:
    TDateMap _mapDate;
};

class CTagMode : public CBase
{
protected:
    typedef std::pair<wxString, wxTreeItemId> TTagValue;
    typedef std::map<int, TTagValue> TTagMap;//tagid + itemid
    typedef std::set<int> TIntSet;
public:
    CTagMode();
    virtual ~CTagMode() {}

    virtual int LoadWords(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree);
    virtual int AddTitle(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int id);
    virtual int RemoveTitle(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int id);

    virtual int AddWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid) { return -1; }
    virtual int RemoveWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid) { return -1; }

    virtual int UpdateWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid);

protected:
    int PreLoad(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree);
    void PostLoad(CLHFilterTreeCtrl* tree);
    void ShowTag(CLHFilterTreeCtrl* tree, const wxTreeItemId& root, int tagid, const wxString& title);
    int ShowWord(CLHFilterTreeCtrl* tree, int tagid, int wordid, const wxString& word);
    void UpdateItemText(CLHFilterTreeCtrl* tree, int tagid, const TTagValue& tag);
private:
    int GetWordData(CDBAccess::TDatabase& db, int wordid, wxString& word, TIntSet& settag) const;
    int GetTagData(CDBAccess::TDatabase& db, int tagid, wxString& title) const;
private:
    TTagMap _mapTag;
};

class CScoreMode : public CBase
{
protected:
    static const size_t MAX_SCORE   =   16;

    typedef std::map<int, wxTreeItemId> TScoreMap;//score + itemid
public:
    CScoreMode();
    virtual ~CScoreMode() {}

    virtual int LoadWords(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree);
    virtual int AddWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid);
    virtual int RemoveWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid);
    virtual int UpdateWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid);

protected:
    void PreLoad(CLHFilterTreeCtrl* tree);
    void PostLoad(CLHFilterTreeCtrl* tree);
    int ShowWord(CLHFilterTreeCtrl* tree, int wordid, const wxString& word, int count);
    void UpdateItemText(CLHFilterTreeCtrl* tree, int score, const wxTreeItemId& itemid);
    wxTreeItemId GetItemID(CLHFilterTreeCtrl* tree, int count);
private:
    int GetWordData(CDBAccess::TDatabase& db, int wordid, wxString& word, int& score) const;
private:
    TScoreMap _mapScore;
};

}

class CFilterShowObject
{
public:
    CFilterShowObject(CDBAccess& db, CLHFilterTreeCtrl* tree);
    virtual ~CFilterShowObject() {}

    int SetMode(FilterShow::FilterMode mode);
    FilterShow::FilterMode GetMode() const;

    int LoadWords();
    int AddTitle(int id);
    int RemoveTitle(int id);

    int AddWord(int wordid);
    int RemoveWord(int wordid);
    int UpdateWord(int wordid);
protected:
    CDBAccess::TDatabase& _db;
    CLHFilterTreeCtrl* _tree;
private:
    std::auto_ptr<FilterShow::CBase> _objFilter;
};

#endif
