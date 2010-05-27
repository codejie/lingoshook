/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __TRIGGEROBJECT_H__
#define __TRIGGEROBJECT_H__

#include "TinyHtmlParser.h"
#include "ConfigData.h"
#include "DictObject.h"
#include "DisplayObject.h"
#include "TagObject.h"
#include "MemoryDailyObject.h"
#include "SpeakObject.h"
#include "FilterShowObject.h"

class CTriggerObject
{
    enum SortMode { SM_UNKNOWN = 0, SM_TAG = 1, SM_DATE, SM_COUNTER , SM_CLOSE};
public:
    CTriggerObject();
    virtual ~CTriggerObject() {}

    void AttachConfigData(CConfigData* data);
    void AttachDictObject(CDictObject* obj);
    void AttachDisplayObject(CDisplayObject* obj);
    void AttachTagObject(CTagObject* obj);
    void AttachMemoryDailyObject(MemoryDaily::CManageObject* obj);
    void AttachSpeakObject(CSpeakObject* obj);
    void AttachFilterShowObject(CFilterShowObject* obj);
public:
    void OnHTMLTrace(const wxString& html);
    void OnHTMLParserFail(const wxString& html);
    void OnHTMLParserException(const wxString& html, const TinyHtmlParser::CExceptionObject& e);

    void OnWordLoad(int wordid, const wxString& word);
    void OnWordSave(int wordid, const wxString& word);
    void OnWordUpdate(int wordid, const wxString& word);

    //void OnWordDataGet(const TWordData& data);
    void OnWordFound(int wordid, const wxString& word);
    void OnResultSave(const TWordResultMap& result);
    void OnResultSave(int wordid, const CDictParser* dict, const CDictResult& result);
    void OnResultGet(int wordid, const CDictParser* dict, const CDictResult& result);
    void OnWordRemove(int wordid);
    void OnWordResultGetOver(int wordid, const TWordData& data);
    
    void OnTagLoad(int tagid, const CTagObject::TRecord& record);
    void OnTagDefLoad(int tagid, const CTagObject::TRecord& record);
    void OnTagInsert(int tag, const CTagObject::TRecord& record);
    void OnTagUpdate(int tagid, const CTagObject::TRecord& record);
    void OnTagRemove(int tagid);

    void OnTagIndexUpdate(int wordid, int tagid);
     
    void OnTagFoundByWord(int wordid);
    void OnTagGetByWord(int tagid, int wordid);
    void OnTagGet(int tagid, const CTagObject::TRecord& record);
    void OnTagGetWord(int tagid, int wordid);

    void OnSortShow(CLHFilterTreeCtrl::FilterType type);

    void OnMemoryDailyLoadOver();
    void OnMemoryDailyLoadWord(int wordid, const wxString& word, int score);
    void OnMemoryDailyPopWord(int wordid, const wxString& word, int score);
    void OnMemoryDailyPushWord(int wordid, const wxString& word, int score);
    void OnMemoryDailyPopWordFail();

    void OnParserUnknown(const wxString& id, const wxString& title);
    void OnParserInitFail(const wxString& id, const wxString& title);
    void OnParserLoad(int index, const wxString& id, const wxString& title);
protected:
    CConfigData* _dataConfig;
    CDictObject* _objDict;
    CDisplayObject* _objDisplay;
    CTagObject* _objTag;
    MemoryDaily::CManageObject* _objMemoryDaily;
    CSpeakObject* _objSpeak;
    CFilterShowObject* _objFilterShow;
};

extern CTriggerObject g_objTrigger;

#endif
