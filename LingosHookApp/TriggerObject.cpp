/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/
#include "wx/wfstream.h"
#include "wx/txtstrm.h"

#include "TriggerObject.h"

class CTriggerObject g_objTrigger;

CTriggerObject::CTriggerObject()
: _dataConfig(NULL)
, _objDict(NULL)
, _objDisplay(NULL)
, _objTag(NULL)
, _objMemoryDaily(NULL)
, _objSpeak(NULL)
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

void CTriggerObject::AttachSpeakObject(CSpeakObject* obj)
{
    _objSpeak = obj;
}

void CTriggerObject::AttachFilterShowObject(CFilterShowObject* obj)
{
    _objFilterShow = obj;
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

    _objTag->AddIndex(wordid, _objTag->DefaultTag());

    if(_dataConfig->m_iDataSyncTag == 1)
    {
         _objFilterShow->AddWord(wordid);
    }

    if(_dataConfig->m_iDataSyncMem == 1)
    {
        _objMemoryDaily->WordInsert(wordid);
    }

    _objDisplay->ShowInfo(wxString::Format(_("Add a new word : %s."), word));
}

void CTriggerObject::OnWordUpdate(int wordid, const wxString& word)
{
    _objTag->AddIndex(wordid, _objTag->DefaultTag());

    if(_dataConfig->m_iDataSyncTag == 1)
    {
        _objFilterShow->UpdateWord(wordid);    
    }

    if(_dataConfig->m_iDataSyncMem == 1)
    {
        _objMemoryDaily->WordRemove(wordid);
        _objMemoryDaily->WordInsert(wordid);
    }
}

void CTriggerObject::OnWordFound(int wordid, const wxString &word)
{
    _objDisplay->ShowWord(wordid, word);
}

void CTriggerObject::OnResultSave(const TWordResultMap &result)
{
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
    _objTag->DeleteWord(wordid);

    if(_dataConfig->m_iDataSyncTag == 1)
    {
        _objFilterShow->RemoveWord(wordid);
    }

    if(_dataConfig->m_iDataSyncMem == 1)
    {
        _objMemoryDaily->WordRemove(wordid);
    }
}

void CTriggerObject::OnWordResultGetOver(int wordid, const TWordData& data)
{
    if(_dataConfig->m_iAutoSpeak == 1)
        _objSpeak->Speak(data.m_strWord);

    _objTag->GetTagByWord(wordid);

    wxFileOutputStream output(wxT("C:\\T.html"));
    wxTextOutputStream ofs(output);
    ofs.WriteString(data.m_strHTML);

    _objDisplay->ShowWordData(data);
}

void CTriggerObject::OnTagLoad(int tagid, const CTagObject::TRecord &record)
{
    _objDisplay->AppendTag(tagid, record);
}

void CTriggerObject::OnTagDefLoad(int tagid, const CTagObject::TRecord &record)
{
    _objDisplay->ShowDefaultTag(tagid, record);

    if(_dataConfig->m_iDataSyncTag == 1)
    {
        _objFilterShow->UpdateTitle(tagid);
    }
}

void CTriggerObject::OnTagInsert(int tagid, const CTagObject::TRecord& record)
{
    _objDisplay->AppendTag(tagid, record);

    if(_dataConfig->m_iDataSyncTag == 1)
    {
        if(_objFilterShow->GetMode() == FilterShow::FM_TAG)            
           _objFilterShow->AddTitle(tagid);  
    }
}

void CTriggerObject::OnTagUpdate(int tagid, const CTagObject::TRecord& record)
{
    _objDisplay->UpdateTag(tagid, record);
}

void CTriggerObject::OnTagRemove(int tagid)
{
    _objDisplay->RemoveTag(tagid);

    if(_dataConfig->m_iDataSyncTag == 1)
    {
        if(_objFilterShow->GetMode() == FilterShow::FM_TAG)            
            _objFilterShow->RemoveTitle(tagid);  
    }    
}

void CTriggerObject::OnTagIndexUpdate(int wordid, int tagid)
{
    if(_dataConfig->m_iDataSyncTag == 1)
    {
        if(_objFilterShow->GetMode() == FilterShow::FM_TAG)
            _objFilterShow->UpdateWord(wordid);
    }
}

void CTriggerObject::OnTagFoundByWord(int wordid)
{
    _objDisplay->ShowTag();
}

void CTriggerObject::OnTagGetByWord(int tagid, int wordid)
{
    _objDisplay->ShowTag(_objTag->GetTitle(tagid));
}

void CTriggerObject::OnTagGetWord(int tagid, int wordid)
{
}

void CTriggerObject::OnSortShow(CLHFilterTreeCtrl::FilterType type)
{
    _objDisplay->SortModeChanged(type);

    if(type == CLHFilterTreeCtrl::FT_TAG)
    {
//        _objTag->GetAll();
        _objFilterShow->SetMode(FilterShow::FM_TAG);
        _objFilterShow->LoadWords();
    }
    else if(type == CLHFilterTreeCtrl::FT_DATE)
    {
        _objFilterShow->SetMode(FilterShow::FM_DATE);
        _objFilterShow->LoadWords();
    }
    else if(type == CLHFilterTreeCtrl::FT_SCORE)
    {
        _objFilterShow->SetMode(FilterShow::FM_SCORE);
        _objFilterShow->LoadWords();
    }
}

void CTriggerObject::OnMemoryDailyLoadOver()
{
     _objMemoryDaily->NextWord();
}

void CTriggerObject::OnMemoryDailyLoadWord(int wordid, const wxString &word, int score)
{
    if(_dataConfig->m_iDataSyncMem == 1)
    {
        _objDisplay->MemoryDailyLoadWord(word, score);
    }
}

void CTriggerObject::OnMemoryDailyPopWord(int wordid, const wxString &word, int score)
{
    _objDisplay->MemoryDailyPopWord(word, score);
}

void CTriggerObject::OnMemoryDailyPushWord(int wordid, const wxString &word, int score)
{
    if(_dataConfig->m_iDataSyncTag == 1)
    {
        if(_objFilterShow->GetMode() == FilterShow::FM_SCORE)
            _objFilterShow->UpdateWord(wordid);
    }
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

