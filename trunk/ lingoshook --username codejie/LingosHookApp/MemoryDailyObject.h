#ifndef __MEMORYDAILYOBJECT_H__
#define __MEMORYDAILYOBJECT_H__

#include <map>
#include <list>
#include <vector>

#include "DBAccess.h"

namespace MemoryDaily
{

enum LevelOffset { LO_LEVEL_1 = -1, LO_LEVEL_2 = 0, LO_LEVEL_3 = 1, LO_LEVEL_4 = 2 };

typedef struct _record_t
{
    bool operator > (const struct _record_t& right) const
    {
        return m_iScore > right.m_iScore;
    }
    int m_iID;
    wxString m_strWord;
    int m_iScore;//min.
} TRecord;

typedef std::list<_record_t> TRecordList;
typedef std::map<int, TRecordList> TScoreMap;//score + queue;
typedef std::vector<int> TScoreVector;//score vector

class CManageObject
{
public:
    CManageObject(CDBAccess& db);
    virtual ~CManageObject();

    int Init();
    int Load();
    bool IsBuilt() const { return _bBuilt; }

    int NextWord();
    int NextWord(int offset);
    //int PopWord();
    //int PushWord(int offset);

    int GetScore() const { return _iCacheScore; }
    int GetWordID() const { return _stCacheRec.m_iID; }
    const wxString& GetWord() const { return _stCacheRec.m_strWord; }

    int WordInsert(int wordid, const wxString& word, int counter = 1);
    int WordRemove(int wordid);
protected:
    int InitSeed();
    void Clear();
    int LoadWords();
    int SortWords();
private:
    int InsertRecrod(int id, const wxString& word, int counter, const wxDateTime& checkin, const wxDateTime& update);

    int PushWord(int score, const TRecord& rec, int offset);
    //int PopWord();
    int UpdateScoreVector(int score, bool add);
private:
    CDBAccess::TDatabase& _db;
protected:
    bool _bBuilt;

    TScoreMap _mapScore;
    TScoreVector _vctScore;
private:
    int _iCacheScore;
    TRecord _stCacheRec;
};

}

#endif
