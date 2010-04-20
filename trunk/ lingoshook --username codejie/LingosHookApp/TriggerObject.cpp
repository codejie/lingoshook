

#include "TriggerObject.h"

class CTriggerObject g_objTrigger;

CTriggerObject::CTriggerObject()
: _dataConfig(NULL)
, _objDict(NULL)
, _objDisplay(NULL)
, _objTag(NULL)
, _objMemoryDaily(NULL)
//, _iSortShowMode(SM_UNKNOWN)
{
}

void CTriggerObject::AttachConfigData(CConfigData* data)
{
    _dataConfig = data;
}

void CTriggerObject::AttachDictObject(CDictObject* obj)
{
    _objDict = obj;
}

void CTriggerObject::AttachDisplayObject(CDisplayObject* obj)
{
    _objDisplay = obj;
}

void CTriggerObject::AttachTagObject(CTagObject* obj)
{
    _objTag = obj;
}

void CTriggerObject::AttachMemoryDailyObject(MemoryDaily::CManageObject* obj)
{
    _objMemoryDaily = obj;
}

void CTriggerObject::OnHTMLTrace(const wxString& html)
{
    _objDisplay->TraceHTML(html);
}

void CTriggerObject::OnHTMLParserFail(const wxString& html)
{
    _objDisplay->ShowInfo(_("HTML Parser Failed."));
}

void CTriggerObject::OnHTMLParserException(const wxString& html, const TinyHtmlParser::CExceptionObject& e)
{
    _objDisplay->ExceptionRaised(html, e);
}

void CTriggerObject::OnWordLoad(int wordid, const wxString &word)
{
    _objDisplay->AppendWord(wordid, word);
}

void CTriggerObject::OnWordSave(int wordid, const wxString& word)
{
    _objDisplay->AppendWord(wordid, word);
    _objTag->InsertIndex(wordid, _objTag->DefaultTag());

    if(_dataConfig->m_iDataSyncMem == 1)
    {
        _objMemoryDaily->WordInsert(wordid, word);
    }

    _objDisplay->ShowInfo(wxString::Format(_("Add a new word : %s."), word));
}

void CTriggerObject::OnWordUpdate(int wordid, const wxString& word, int counter)
{
    if(_objTag->IsIndexExist(wordid, _objTag->DefaultTag()) != 0)
        _objTag->InsertIndex(wordid, _objTag->DefaultTag());

    if(_dataConfig->m_iDataSyncMem == 1)
    {
        _objMemoryDaily->WordRemove(wordid);
        _objMemoryDaily->WordInsert(wordid, word, counter);
    }
}

void CTriggerObject::OnWordDataGet(const TWordData &data)
{
//    _objDisplay->ShowWordData(data);
}

void CTriggerObject::OnWordFound(int wordid, const wxString &word)
{
    _objDisplay->ShowWord(wordid, word);
}

void CTriggerObject::OnResultSave(const TWordResultMap &result)
{
    _objDisplay->AppendResult(result);
}

void CTriggerObject::OnResultSave(int wordid, const CDictParser *dict, const CDictResult &result)
{
}

void CTriggerObject::OnResultGet(int wordid, const CDictParser *dict, const CDictResult &result)
{
    _objDisplay->ShowResult(dict, result, (_dataConfig->m_iExpandDict == -1 || _dataConfig->m_iExpandDict == dict->GetIndex()));
}

void CTriggerObject::OnWordRemove(int wordid)
{
    _objDisplay->RemoveWord(wordid);
    _objTag->RemoveIndex(wordid);

    if(_dataConfig->m_iDataSyncMem == 1)
    {
        _objMemoryDaily->WordRemove(wordid);
    }
}

//void CTriggerObject::OnWordRemove(const wxString& word)
//{
//    _objDisplay->RemoveWord(word);
//}

void CTriggerObject::OnWordResultGetOver(int wordid, const TWordData& data)
{
    _objTag->GetTagByWord(wordid);
    _objDisplay->ShowWordData(data);        
}

void CTriggerObject::OnTagLoad(int tagid, const CTagObject::TRecord &record)
{
    _objDisplay->AppendTag(tagid, record);
}

void CTriggerObject::OnTagDefLoad(int tagid, const CTagObject::TRecord &record)
{
    _objDisplay->ShowDefaultTag(tagid, record);
}

void CTriggerObject::OnTagInsert(int tagid, const CTagObject::TRecord& record)
{
    _objDisplay->AppendTag(tagid, record);
    _objDisplay->SortTagAppend(tagid, record);

    //if(_iSortShowMode == SM_TAG)
    //{
    //    _objDisplay->SortTagAppend(tagid, record);
    //}
}

void CTriggerObject::OnTagUpdate(int tagid, const CTagObject::TRecord& record)
{
    //_objDisplay->UpdateTag(tagid, record);
    if(_objDisplay->SortTagUpdate(tagid, record) == 0)
    {
        _objTag->GetWordByTag(tagid);
    }

    //if(_iSortShowMode == SM_TAG)
    //{
    //    _objDisplay->SortTagUpdate(tagid, record);
    //    _objTag->GetWordByTag(tagid);
    //}
}

void CTriggerObject::OnTagRemove(int tagid)
{
    _objDisplay->RemoveTag(tagid);
    _objDisplay->SortTagRemove(tagid);

    //if(_iSortShowMode == SM_TAG)
    //{
    //    _objDisplay->SortTagRemove(tagid);
    //}
}

void CTriggerObject::OnTagIndexInsert(int tagid, int wordid, const CTagObject::TRecord &record)
{
    if(_dataConfig->m_iDataSyncTag == 1)
    {
        _objDisplay->UpdateTag(tagid, record);

        TWordData data;
        if(_objDict->GetWordData(wordid, data) == 0)
        {
            _objDisplay->SortTagIndexInsert(tagid, record, data);
        }
    }

    //_objDisplay->UpdateTag(tagid, record);

    //if(_dataConfig->m_iDataSyncTag == 1)
    //{
    //    if(_iSortShowMode == SM_TAG)
    //    {
    //        TWordData data;
    //        if(_objDict->GetWordData(wordid, data) == 0)
    //        {
    //            _objDisplay->SortTagIndexInsert(tagid, record, data);
    //        }
    //    }
    //}
}

void CTriggerObject::OnTagIndexRemove(int tagid, int wordid, const CTagObject::TRecord &record)
{
    if(_dataConfig->m_iDataSyncTag == 1)
    {
        _objDisplay->UpdateTag(tagid, record);
        _objDisplay->SortTagIndexRemove(tagid, record, wordid);
    }

    //_objDisplay->UpdateTag(tagid, record);

    //if(_dataConfig->m_iDataSyncTag == 1)
    //{
    //    if(_iSortShowMode == SM_TAG)
    //    {
    //        _objDisplay->SortTagIndexRemove(tagid, record, wordid);
    //    }
    //}
}

void CTriggerObject::OnTagFoundByWord(int wordid)
{
    _objDisplay->ShowTag();
}

void CTriggerObject::OnTagGetByWord(int tagid, int wordid)
{
    _objDisplay->ShowTag(_objTag->GetTitle(tagid));
}

void CTriggerObject::OnTagGet(int tagid, const CTagObject::TRecord &record)
{
    _objDisplay->SortTagAppend(tagid, record);
}

void CTriggerObject::OnTagGetWord(int tagid, int wordid)
{
    TWordData data;
    if(_objDict->GetWordData(wordid, data) == 0)
    {
        _objDisplay->SortTagAppendWordData(tagid, data);
    }
}

void CTriggerObject::OnSortShow(CLHFilterTreeCtrl::FilterType type)
{
    _objDisplay->SortModeChanged(type);

    if(type == CLHFilterTreeCtrl::FT_TAG)
    {
        _objTag->GetAll();
    }

    //if(mode == _iSortShowMode)
    //    return;

    //_objDisplay->ShowSortTree(mode);

    //switch(mode)
    //{
    //case SM_TAG://tag
    //    _objTag->GetAll();
    //    break;
    //case SM_DATE://date
    //    break;
    //case SM_COUNTER://counter
    //    break;
    //case SM_CLOSE://
    //    break;
    //default:
    //    return;
    //}    

    //_iSortShowMode = mode;
}

void CTriggerObject::OnMemoryDailyLoadOver()
{
 //   _objDisplay->MemoryDailyPopWordFail();
    _objMemoryDaily->NextWord();
}

void CTriggerObject::OnMemoryDailyLoadWord(int id, const wxString &word, int score)
{
    if(_dataConfig->m_iDataSyncMem == 1)
        _objDisplay->MemoryDailyLoadWord(word, score);
}

void CTriggerObject::OnMemoryDailyPopWord(int id, const wxString &word, int score)
{
    _objDisplay->MemoryDailyPopWord(word, score);
}

void CTriggerObject::OnMemoryDailyPushWord(int id, const wxString &word, int score)
{
}

void CTriggerObject::OnMemoryDailyPopWordFail()
{
    _objDisplay->MemoryDailyPopWordFail();
}

void CTriggerObject::OnParserUnknown(const wxString &id, const wxString &title)
{
    _objDisplay->ShowInfo(wxString::Format(_("Unknown Dictionary Parser : %s"), title));
}

void CTriggerObject::OnParserInitFail(const wxString &id, const wxString &title)
{
    _objDisplay->ShowInfo(wxString::Format(_("Init Dictionary Parser Failed - Title : %s"), title));
}

void CTriggerObject::OnParserLoad(int index, const wxString &id, const wxString &title)
{
    _objDisplay->ShowInfo(wxString::Format(_("Load Dictionary Parser : %s"), title));
    _objDisplay->ParserLoad(index, id, title);
}

