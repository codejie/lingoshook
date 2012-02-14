/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __DISPLAYOBJECT_H__
#define __DISPLAYOBJECT_H__

#include "wx/wx.h"

#include "LHControls.h"
#include "TinyHtmlParser.h"
#include "DictStructure.h"//Object.h"
#include "SpecialDictParser.h"
#include "TagObject.h"
#include "ActivityObject.h"

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
    void ShowWord(int wordid, const wxString& word);
    void ShowWordData(const TWordData& data);
    void ShowSpecialDictResult(const SpecialDictParser::CDictParser* dict, const SpecialDictParser::CDictResult& result, bool expand);
    void ShowHtmlDictResult(const wxString& html);
    void RemoveWord(int wordid);

    void AppendTag(int tagid, const CTagObject::TRecord& record);
    void ShowDefaultTag(int tagid, const CTagObject::TRecord& record);
    void UpdateTag(int tagid, const CTagObject::TRecord& record);
    void RemoveTag(int tagid);
    void ShowTag();
    void ShowTag(const wxString& tag);

    void SortModeChanged(CLHFilterTreeCtrl::FilterType type);

    void MemoryDailyLoadWord(const wxString& word, int score);
    void MemoryDailyPopWord(const wxString& word, int score);
    void MemoryDailyPopWordFail();

    void ExceptionRaised(const wxString& html, const TinyHtmlParser::CExceptionObject& e);

    void ParserLoad(int index, const wxString& id, const wxString& title);

    void AppendPluginsData(int index, const ActivityObject::PropertyData& data, bool imcompatible = false);
protected:
    void ShowDictResult(wxTreeItemId& item, const SpecialDictParser::CDictParser* dict, const SpecialDictParser::CDictResult& result, bool expand);
    void ShowSortTagMode();
    void ShowSortDateMode();
    void ShowSortCounterMode();
    void ShowSortCloseMode();
private:
    void ShowViconECDictResult(wxTreeCtrl* tree, wxTreeItemId& item, const SpecialDictParser::CDictParser* dict, const SpecialDictParser::CDictResult& result);
    void ShowLangdaoECDictResult(wxTreeCtrl* tree, wxTreeItemId& item, const SpecialDictParser::CDictParser* dict, const SpecialDictParser::CDictResult& result);
    void ShowFundsetDCDictResult(wxTreeCtrl* tree, wxTreeItemId& item, const SpecialDictParser::CDictParser* dict, const SpecialDictParser::CDictResult& result);
protected:
    LingosHookFrame* _frame;
};

#endif
