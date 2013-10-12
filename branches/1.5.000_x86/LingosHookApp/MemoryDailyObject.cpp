/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include <stdlib.h>

#include "TriggerObject.h"
#include "MemoryDailyObject.h"

namespace MemoryDaily
{

CManageObject::CManageObject(CDBAccess& db)
: _db(db.Database())
, _bBuilt(false)
, _iCacheScore(-1)
{
}

CManageObject::~CManageObject()
{
}

int CManageObject::Init()
{
    return 0;
}

int CManageObject::Load()
{
    Clear();
    if(LoadWords() != 0)
        return -1;
    if(SortWords() != 0)
        return -1;

    if(InitSeed() != 0)
        return -1;

    _bBuilt = true;
    _iCacheScore = -1;

    g_objTrigger.OnMemoryDailyLoadOver();

    return 0;
}

int CManageObject::InitSeed()
{
    ::srand((unsigned int)time(NULL));
    ::rand();
    return 0;
}

int CManageObject::LoadWords()
{
    try
    {
        //if(!_db.TableExists(_("DictTable")))
        //    return -1;

        CDBAccess::TResult res = _db.ExecuteQuery("SELECT WordTable.WordID, WordTable.Word, WordTable.Counter, WordTable.UpdateTime, SrcDataTable.CheckinTime FROM WordTable, SrcDataTable WHERE WordTable.SrcID = SrcDataTable.SrcID");
        if(!res.IsOk())
            return -1;
        while(res.NextRow())
        {
            InsertRecrod(res.GetInt(0), res.GetString(1), res.GetInt(2), res.GetTimestamp(3), res.GetTimestamp(4));
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CManageObject::SortWords()
{
    size_t szmap = _mapScore.size();
    int loop = 0;
    for(TScoreMap::iterator it = _mapScore.begin(); it != _mapScore.end(); ++ it)
    {
        it->second.sort(std::greater<_record_t>());

        size_t szque = it->second.size();
        loop = (szque + szmap) / szmap;
        if(loop > 10)
            loop = 10;
        for(int i = 0; i < loop; ++ i)
        {
            _vctScore.push_back(it->first);
        }
    }
    return 0;
}

void CManageObject::Clear()
{
    _vctScore.clear();
    _mapScore.clear();
}

int CManageObject::InsertRecrod(int id, const wxString &word, int counter, const wxDateTime &checkin, const wxDateTime &update)
{
    wxDateTime now(::time(NULL));
    wxTimeSpan interval = (now - update);//(update - checkin);
    int score = counter;// + interval.GetDays();
    TScoreMap::iterator it = _mapScore.find(score);
    if(it == _mapScore.end())
        it = _mapScore.insert(std::make_pair(score, TRecordList())).first;

    _record_t rec;
    rec.m_iID = id;
    rec.m_strWord = word;
    rec.m_iScore = interval.GetMinutes();
    it->second.push_back(rec);

    g_objTrigger.OnMemoryDailyLoadWord(rec.m_iID, rec.m_strWord, score + rec.m_iScore);

    return 0;
}

int CManageObject::NextWord()
{
    if(_vctScore.size() == 0)
    {
        g_objTrigger.OnMemoryDailyPopWordFail();
        return -1;
    }

    size_t r =  ((double)::rand() / (double)(RAND_MAX + 1)) * _vctScore.size();

    TScoreMap::iterator it = _mapScore.find(_vctScore[r]);
    if(it == _mapScore.end())
    {
        g_objTrigger.OnMemoryDailyPopWordFail();
        return -1;
    }

    _iCacheScore = it->first;
    _stCacheRec = (*it->second.begin());

    it->second.pop_front();

    if(it->second.size() == 0)
    {
        UpdateScoreVector(it->first, false);
        _mapScore.erase(it);
    }

    g_objTrigger.OnMemoryDailyPopWord(_stCacheRec.m_iID, _stCacheRec.m_strWord, _iCacheScore);

    return 0;
}

int CManageObject::NextWord(int offset)
{
    if(_iCacheScore != -1)
    {
        if(PushWord(_iCacheScore, _stCacheRec, offset) != 0)
            return -1;
    }
    return NextWord();
}

int CManageObject::PushWord(int score, const TRecord& rec, int offset)
{
    score += offset;
    if(score < 1)
        score = 1;
    try
    {
        //if(offset == -1)
        //{
        //    CDBAccess::TQuery query = _db.PrepareStatement("UPDATE WordTable SET Counter = ? WHERE ID = ?");
        //    query.Bind(1, score);
        //    query.Bind(2, _stCacheRec.m_iID);
        //    query.ExecuteUpdate();
        //}
        //else 
        //if(offset != 0)
        //{
            CDBAccess::TQuery query = _db.PrepareStatement("UPDATE WordTable SET Counter = ?, UpdateTime = DATETIME('NOW', 'LOCALTIME') WHERE WordID = ?");
            query.Bind(1, score);
            query.Bind(2, _stCacheRec.m_iID);
            query.ExecuteUpdate();
        //}

        TScoreMap::iterator it = _mapScore.find(score);
        if(it == _mapScore.end())
        {
            it = _mapScore.insert(std::make_pair(score, TRecordList())).first;
            it->second.push_back(rec);

            UpdateScoreVector(it->first, true);
        }
        else
        {
            it->second.push_back(rec);
        }

        g_objTrigger.OnMemoryDailyPushWord(rec.m_iID, rec.m_strWord, score);
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CManageObject::UpdateScoreVector(int score, bool add)
{
    if(add == true)
    {
        if(_vctScore.size() > 0)
        {
            TScoreVector::iterator it = _vctScore.begin();
            while(it != _vctScore.end())
            {
                if((*it) > score)
                {
                    _vctScore.insert(it, score);
                    return 0;
                }
                ++ it;
            }            
        }
        _vctScore.push_back(score);
        return 0;
    }
    else
    {
        TScoreVector::iterator it = _vctScore.begin();
        while(it != _vctScore.end())
        {
            if((*it) == score)
            {
                _vctScore.erase(it);
                return 0;
            }
            ++ it;
        }
    }
    return -1;
}

int CManageObject::WordInsert(int wordid)
{
    if(_bBuilt == false)
        return 0;

    try
    {
        CDBAccess::TQuery query = _db.PrepareStatement("SELECT Word, Counter FROM WordTable WHERE WordID = ?");
        query.Bind(1, wordid);
        CDBAccess::TResult res =  query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        if(res.NextRow())
        {
            _record_t rec;
            rec.m_iID = wordid;
            rec.m_strWord = res.GetString(0);
            rec.m_iScore = res.GetInt(1);

            TScoreMap::iterator it = _mapScore.find(rec.m_iScore);
            if(it == _mapScore.end())
            {
                it = _mapScore.insert(std::make_pair(rec.m_iScore, TRecordList())).first;
                it->second.push_back(rec);
                UpdateScoreVector(rec.m_iScore, true);
            }
            else
            {
                it->second.push_back(rec);
            }
        }
        else
        {
            return -1;
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;

}

int CManageObject::WordRemove(int wordid)
{
    if(_bBuilt == false)
        return 0;

    if(wordid == _stCacheRec.m_iID)
    {
        return NextWord();
    }

    TScoreMap::iterator it = _mapScore.begin();
    while(it != _mapScore.end())
    {
        TRecordList::iterator i = it->second.begin();
        while(i != it->second.end())
        {
            if(i->m_iID == wordid)
            {
                it->second.erase(i);
                if(it->second.size() == 0)
                {
                    UpdateScoreVector(it->first, false);
                    _mapScore.erase(it);
                }
                return 0;
            }
            ++ i;
        }
        ++ it;
    }
    return 0;
}

int CManageObject::WordUpdate(int wordid)
{
    if(_bBuilt == false)
        return 0;


    try
    {
        //if(!_db.TableExists(_("DictTable")))
        //    return -1;

        CDBAccess::TResult res = _db.ExecuteQuery("SELECT WordID, Word, Counter, CheckinTime, UpdateTime FROM WordTable");
        if(!res.IsOk())
            return -1;
        while(res.NextRow())
        {
            InsertRecrod(res.GetInt(0), res.GetString(1), res.GetInt(2), res.GetTimestamp(3), res.GetTimestamp(4));
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;

    return -1;
}

}
