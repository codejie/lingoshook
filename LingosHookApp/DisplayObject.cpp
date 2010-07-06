/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include "Consts.h"
#include "LingosHookApp.h"
#include "ExceptionRaisedDialog.h"
#include "ViconDictObject.h"
#include "LangdaoDictObject.h"
#include "FundsetDictObject.h"
#include "DisplayObject.h"

CDisplayObject::CDisplayObject(LingosHookFrame *frame)
: _frame(frame)
{
}

void CDisplayObject::ShowInfo(const wxString &info)
{
    _frame->ShowHint(info);
}

void CDisplayObject::AppendWord(int wordid, const wxString &word)
{
    _frame->m_listIndex->Append(word, (void*)wordid);
}


void CDisplayObject::ShowWord(int wordid, const wxString& word)
{
	_frame->m_treeResult->DeleteAllItems();
    _frame->m_winHTML->LoadBlankPage();

    CLHResultTreeItemData* data = new CLHResultTreeItemData(wordid);
	wxTreeItemId root = _frame->m_treeResult->AddRoot(word, -1, -1, data);
}

void CDisplayObject::ShowWordData(const TWordData &data)
{
    wxTreeItemId root = _frame->m_treeResult->GetRootItem();

    wxTreeItemId stats = _frame->m_treeResult->AppendItem(root, _("Stats."));
    _frame->m_treeResult->AppendItem(stats, wxString::Format(_("Score = %d"), data.m_iCounter));
    _frame->m_treeResult->AppendItem(stats, wxString::Format(_("Update = %04d-%02d-%02d %02d:%02d:%02d"), data.m_dtUpdate.GetYear(), data.m_dtUpdate.GetMonth() + 1, data.m_dtUpdate.GetDay(), data.m_dtUpdate.GetHour(), data.m_dtUpdate.GetMinute(), data.m_dtUpdate.GetSecond()));
    _frame->m_treeResult->AppendItem(stats, wxString::Format(_("Checkin = %04d-%02d-%02d %02d:%02d:%02d"), data.m_dtCheckin.GetYear(), data.m_dtCheckin.GetMonth() + 1, data.m_dtCheckin.GetDay(), data.m_dtCheckin.GetHour(), data.m_dtCheckin.GetMinute(), data.m_dtCheckin.GetSecond()));
    
    _frame->m_treeResult->Expand(root);
}

void CDisplayObject::ShowSpecialDictResult(const SpecialDictParser::CDictParser* dict, const SpecialDictParser::CDictResult &result, bool expand)
{
    wxTreeItemId root = _frame->m_treeResult->GetRootItem();
    //wxTreeItemId item = _frame->m_treeResult->InsertItem(root, _frame->m_treeResult->GetChildrenCount(root, false) - 1, dict->GetTitle());
    wxTreeItemId item = _frame->m_treeResult->AppendItem(root, dict->GetTitle());
    ShowDictResult(item, dict, result, expand);
}

void CDisplayObject::ShowHtmlDictResult(const wxString& html)
{
    _frame->m_winHTML->LoadString(html);
}

void CDisplayObject::RemoveWord(int wordid)
{
	_frame->m_treeResult->DeleteAllItems();
    _frame->m_winHTML->LoadBlankPage();

    _frame->m_listIndex->DeleteItem(wordid);
}


void CDisplayObject::TraceHTML(const wxString& html)
{
    _frame->m_textTrace->AppendText(_("\n----- HTML -----\n") + html);
}

void CDisplayObject::AppendTag(int tagid, const CTagObject::TRecord &record)
{
    long index = _frame->m_listTagMgnt->GetItemCount();
    long l = _frame->m_listTagMgnt->InsertItem(index, record.m_strTitle);
    _frame->m_listTagMgnt->SetItemData(l, tagid);
    _frame->m_listTagMgnt->SetItem(l, 1, wxString::Format(_("%d"), record.m_uiCounter));
    _frame->m_listTagMgnt->SetItem(l, 2, wxString::Format(_("%04d-%02d-%02d %02d:%02d:%02d"), record.m_dtCreateTime.GetYear(), record.m_dtCreateTime.GetMonth() + 1, record.m_dtCreateTime.GetDay(), record.m_dtCreateTime.GetHour(), record.m_dtCreateTime.GetMinute(), record.m_dtCreateTime.GetSecond()));
    _frame->m_listTagMgnt->SetItem(l, 3, record.m_strDesc);
}

void CDisplayObject::ShowDefaultTag(int tagid, const CTagObject::TRecord &record)
{
    _frame->m_textDefTag->SetValue(record.m_strTitle);
}

void CDisplayObject::UpdateTag(int tagid, const CTagObject::TRecord &record)
{
    long index = _frame->m_listTagMgnt->FindItem(-1, tagid);
    if(index != -1)
    {
        _frame->m_listTagMgnt->SetItem(index, 1, wxString::Format(_("%d"), record.m_uiCounter));
    }
}

void CDisplayObject::RemoveTag(int tagid)
{
    long index = _frame->m_listTagMgnt->FindItem(-1, tagid);
    if(index != -1)
    {
        _frame->m_listTagMgnt->DeleteItem(index);
    }
}

void CDisplayObject::ShowTag()
{
    wxTreeItemId root = _frame->m_treeResult->GetRootItem();
    _frame->m_treeResult->AppendItem(root, _("Tag"));
}

void CDisplayObject::ShowTag(const wxString &tag)
{
    wxTreeItemId root = _frame->m_treeResult->GetRootItem();
    wxTreeItemId item = _frame->m_treeResult->GetLastChild(root);
    _frame->m_treeResult->AppendItem(item, tag);
}


void CDisplayObject::SortModeChanged(CLHFilterTreeCtrl::FilterType type)
{
    if(_frame->m_treeFilter->GetFilterType() == type)
        return;
    _frame->m_treeFilter->SetFilterType(type);

    switch(type)
    {
    case CLHFilterTreeCtrl::FT_TAG:
        _frame->m_btnFilter->SetLabel(_("Classification by Tag"));        
        break;
    case CLHFilterTreeCtrl::FT_DATE:
        _frame->m_btnFilter->SetLabel(_("Classification by Date"));
        break;
    case CLHFilterTreeCtrl::FT_SCORE:
        _frame->m_btnFilter->SetLabel(_("Classification by Score"));
        break;
    case CLHFilterTreeCtrl::FT_CLOSE:
        _frame->m_btnFilter->SetLabel(_("Closed"));
        break;
    default:
        _frame->m_btnFilter->SetLabel(_("Unknown"));
    }

    _frame->m_treeFilter->DeleteAllItems();
}

/////
void CDisplayObject::ShowDictResult(wxTreeItemId &item, const SpecialDictParser::CDictParser* dict, const SpecialDictParser::CDictResult &result, bool expand)
{
    if(dict->GetID() == SpecialDictParser::VICON::CECParser::ID)
    {
        ShowViconECDictResult(_frame->m_treeResult, item, dict, result);
    }
    else if(dict->GetID() == SpecialDictParser::LANGDAO::CECParser::ID)
    {
        ShowLangdaoECDictResult(_frame->m_treeResult, item, dict, result);
    }
    else if(dict->GetID() == SpecialDictParser::FUNDSET::CDCParser::ID)
    {
        ShowFundsetDCDictResult(_frame->m_treeResult, item, dict, result);
    }
    if(expand)
        _frame->m_treeResult->ExpandAllChildren(item);
}

void CDisplayObject::ShowSortTagMode()
{
    _frame->m_btnFilter->SetLabel(_("Classification by Tag"));        
}

void CDisplayObject::ShowSortDateMode()
{
    _frame->m_btnFilter->SetLabel(_("Classification by Date"));
}

void CDisplayObject::ShowSortCounterMode()
{
    _frame->m_btnFilter->SetLabel(_("Classification by Score"));
}

void CDisplayObject::ShowSortCloseMode()
{
    _frame->m_btnFilter->SetLabel(_("Closed"));
}

////
void CDisplayObject::ShowViconECDictResult(wxTreeCtrl* tree, wxTreeItemId& item, const SpecialDictParser::CDictParser* dict, const SpecialDictParser::CDictResult& result)
{
    const SpecialDictParser::VICON::CECParser* parser = dynamic_cast<const SpecialDictParser::VICON::CECParser*>(dict);
    const SpecialDictParser::VICON::CECResult* res = dynamic_cast<const SpecialDictParser::VICON::CECResult*>(result.Result());
    tree->AppendItem(item, _("[") + res->m_strSymbol + _("]"));
    for(SpecialDictParser::VICON::CECResult::TRecordVector::const_iterator it = res->m_vctRecord.begin(); it != res->m_vctRecord.end(); ++ it)
    {
        wxString str = parser->WCToStr(it->m_eClass);
        str += _(" ") +  it->m_strResult;
        tree->AppendItem(item, str);
    }
}

void CDisplayObject::ShowLangdaoECDictResult(wxTreeCtrl* tree, wxTreeItemId& item, const SpecialDictParser::CDictParser* dict, const SpecialDictParser::CDictResult& result)
{
    const SpecialDictParser::LANGDAO::CECParser* parser = dynamic_cast<const SpecialDictParser::LANGDAO::CECParser*>(dict);
    const SpecialDictParser::LANGDAO::CECResult* res = dynamic_cast<const SpecialDictParser::LANGDAO::CECResult*>(result.Result());
    tree->AppendItem(item, _("[") + res->m_strSymbol + _("]"));

    for(SpecialDictParser::LANGDAO::CECResult::TResultRecordVector::const_iterator it = res->m_stRecord.m_vctResult.begin(); it != res->m_stRecord.m_vctResult.end(); ++ it)
    {
        wxString str = parser->WCToStr(it->m_eClass);
        str += _(" ") +  it->m_strResult;
        tree->AppendItem(item, str);
    }

    if(res->m_stRecord.m_vctSpecial.size() > 0)
    {
        wxTreeItemId special = tree->AppendItem(item, _("Special Words"));
        for(SpecialDictParser::LANGDAO::CECResult::TSpecialRecordVector::const_iterator it = res->m_stRecord.m_vctSpecial.begin(); it != res->m_stRecord.m_vctSpecial.end(); ++ it)
        {
            tree->AppendItem(special, it->m_strSpecial + _(" ") + it->m_strResult);
        }
    }

    if(res->m_stRecord.m_vctCommonly.size() > 0)
    {
        wxTreeItemId commonly = tree->AppendItem(item, _("Commonly Phrase"));
        for(SpecialDictParser::LANGDAO::CECResult::TCommonlyRecordVector::const_iterator it = res->m_stRecord.m_vctCommonly.begin(); it != res->m_stRecord.m_vctCommonly.end(); ++ it)
        {
            tree->AppendItem(commonly, (*it));
        }
    }
}

void CDisplayObject::ShowFundsetDCDictResult(wxTreeCtrl* tree, wxTreeItemId& item, const SpecialDictParser::CDictParser* dict, const SpecialDictParser::CDictResult& result)
{
    const SpecialDictParser::FUNDSET::CDCParser* parser = dynamic_cast<const SpecialDictParser::FUNDSET::CDCParser*>(dict);
    const SpecialDictParser::FUNDSET::CDCResult* res = dynamic_cast<const SpecialDictParser::FUNDSET::CDCResult*>(result.Result());

    if(!res->m_strKasus.empty())
        tree->AppendItem(item, res->m_strKasus);

    for(SpecialDictParser::FUNDSET::CDCResult::TRecordVector::const_iterator it = res->m_vctRecord.begin(); it != res->m_vctRecord.end(); ++ it)
    {
        tree->AppendItem(item, *it);
    }
}


void CDisplayObject::MemoryDailyLoadWord(const wxString &word, int score)
{
    _frame->m_textMemWord->SetLabel(word);
    _frame->m_textMemScore->SetLabel(wxString::Format(_("Score:%d"), score));
}

void CDisplayObject::MemoryDailyPopWord(const wxString &word, int score)
{
    //_frame->m_radioMemLevel0->SetFocus();

    _frame->m_radioMemLevel0->SetValue(false);
    _frame->m_radioMemLevel1->SetValue(false);
    _frame->m_radioMemLevel2->SetValue(false);
    _frame->m_radioMemLevel3->SetValue(false);

        
    _frame->m_btnMemRemove->Enable(false);
    _frame->m_btnMemNext->Enable(false);

    _frame->m_textMemType->SetValue(wxEmptyString);
    _frame->m_textMemType->SetFocus();

	_frame->m_treeResult->DeleteAllItems();
    _frame->m_winHTML->LoadBlankPage();

    _frame->m_textMemWord->SetLabel(word);
    _frame->m_textMemScore->SetLabel(wxString::Format(_("Score:%d"), score));

}

void CDisplayObject::MemoryDailyPopWordFail()
{
    _frame->m_radioMemLevel0->SetValue(false);
    _frame->m_radioMemLevel1->SetValue(false);
    _frame->m_radioMemLevel2->SetValue(false);
    _frame->m_radioMemLevel3->SetValue(false);

    _frame->m_btnMemRemove->Enable(false);
    _frame->m_btnMemNext->Enable(false);

    _frame->m_textMemWord->SetLabel(_("No Word"));
    _frame->m_textMemScore->SetLabel(_("Score:-1"));
}

void CDisplayObject::ExceptionRaised(const wxString &html, const TinyHtmlParser::CExceptionObject &e)
{
    ShowInfo(_("Catches a EXCEPTION info.."));

    CExceptionRaisedDialog dlg(_frame);
    wxString info = wxString::Format(_("%s v%s - Exception:[%d]%s\n---- HTML ----\n"), APP_TITLE, APP_VERSION, e.Number(), wxString(e.Info().c_str()));//);//, wxConvISO8859_1);
    info += html;

    dlg.SetInfo(info);

    dlg.ShowModal();
}

void CDisplayObject::ParserLoad(int index, const wxString &id, const wxString &title)
{
    CLHComboBoxClientData* data = new CLHComboBoxClientData(index, id);
    _frame->m_comboxExpandDict->Append(title, data);
}

