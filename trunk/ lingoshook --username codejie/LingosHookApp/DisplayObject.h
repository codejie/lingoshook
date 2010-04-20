#ifndef __DISPLAYOBJECT_H__
#define __DISPLAYOBJECT_H__

#include "wx/wx.h"

#include "LHControls.h"
#include "TinyHtmlParser.h"
#include "DictObject.h"
#include "TagObject.h"

class wxTreeItemId;
class LingosHookFrame;

class CDisplayObject
{
public:
    CDisplayObject(LingosHookFrame* frame);
    virtual ~CDisplayObject() {}

    void ShowInfo(const wxString& info);
    void TraceHTML(const wxString& html);

    void AppendWord(int wordid, const wxString& word);
    void AppendResult(const TWordResultMap& result);
    void ShowWord(int wordid, const wxString& word);
    void ShowWordData(const TWordData& data);
    void ShowResult(const CDictParser* dict, const CDictResult& result, bool expand);
    void RemoveWord(int wordid);
    //void RemoveWord(const wxString& word);

    void AppendTag(int tagid, const CTagObject::TRecord& record);
    void ShowDefaultTag(int tagid, const CTagObject::TRecord& record);
    void UpdateTag(int tagid, const CTagObject::TRecord& record);
    void RemoveTag(int tagid);
    void ShowTag();
    void ShowTag(const wxString& tag);

    void SortTagAppend(int tagid, const CTagObject::TRecord& record);
    void SortTagAppendWordData(int tagid, const TWordData& data);
    void SortTagRemove(int tagid);
    int SortTagUpdate(int tagid, const CTagObject::TRecord& record);
    void SortTagIndexInsert(int tagid, const CTagObject::TRecord& record, const TWordData& data);
    void SortTagIndexRemove(int tagid, const CTagObject::TRecord& record, int wordid);
    void SortModeChanged(CLHFilterTreeCtrl::FilterType type);

    void MemoryDailyLoadWord(const wxString& word, int score);
    void MemoryDailyPopWord(const wxString& word, int score);
    void MemoryDailyPopWordFail();

    void ExceptionRaised(const wxString& html, const TinyHtmlParser::CExceptionObject& e);

    void ParserLoad(int index, const wxString& id, const wxString& title);
protected:
    void ShowDictResult(wxTreeItemId& item, const CDictParser* dict, const CDictResult& result, bool expand);
    void ShowSortTagMode();
    void ShowSortDateMode();
    void ShowSortCounterMode();
    void ShowSortCloseMode();
private:
    void ShowViconECDictResult(wxTreeCtrl* tree, wxTreeItemId& item, const CDictParser* dict, const CDictResult& result);
    void ShowLangdaoECDictResult(wxTreeCtrl* tree, wxTreeItemId& item, const CDictParser* dict, const CDictResult& result);

    int SortTagFindTagItemID(int tagid, wxTreeItemId& item);
    int SortTagFindWordItemID(wxTreeItemId& tagitem, int wordid, wxTreeItemId& item);
protected:
    LingosHookFrame* _frame;
};

#endif
