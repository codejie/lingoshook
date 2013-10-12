// -*- C++ -*- generated by wxGlade 0.6.3 on Mon Nov 23 15:38:51 2009
#ifndef LINGOSHOOKAPP_H
#define LINGOSHOOKAPP_H
#include <memory>

#include <wx/wx.h>
#include <wx/image.h>
// begin wxGlade: ::dependencies
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/tglbtn.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/statline.h>
#include <wx/cmdline.h>
#include <wx/intl.h>
// end wxGlade

// begin wxGlade: ::extracode
// end wxGlade

#include "LHControls.h"
#include "DBAccess.h"
#include "MemoryDailyObject.h"

class CHookObject;
class CConfigData;
class CDictObject;
class CTagObject;
class CDisplayObject;
class CSpeakObject;
class CFilterShowObject;
class CTrayIconObject;
class CPluginObject;

class LingosHookFrame: public wxFrame {
public:
    enum IndexNoteID { INID_INDEX = 0, INID_FILTER, INID_MEMORY };
    enum ContextNoteID { CNID_RESULT = 0, CNID_HTML, CNID_TAGS, CNID_SETTING, CNID_PLUGINS, CNID_ABOUT, CNID_TRACE, CNID_MAX_PAGES };

    enum ControlID { CIID_PAGE_INDEX = 10000, CIID_PAGE_CONTEXT, CIID_TEXT_INDEX, CIID_LIST_INDEX, CIID_CHECKBOX_AUTOHOOK, CIID_CHECKBOX_USEHOTKEY, CIID_CHECKBOX_STOPRETRIEVE
            , CIID_BUTTON_APPLY, CIID_LIST_TAGMGNT, CIID_BUTTON_TAGSETDEFAULT, CIID_BUTTON_TAGADD, CIID_BUTTON_TAGRENAME, CIID_TREE_RESULT
            , CIID_BUTTON_TAGREMOVE, CIID_BUTTON_HOOK, XIID_BUTTON_FILTER, CIID_TREE_FILTER, CIID_BUTTON_MEMREMOVE
            , CIID_BUTTON_MEMNEXT, CIID_RADIO_MEMLEVEL1, CIID_RADIO_MEMLEVEL2, CIID_RADIO_MEMLEVEL3, CIID_RADIO_MEMLEVEL4
            , CIID_BUTTON_MEMREGEN, CIID_BUTTON_ABOUTHELP, CIID_BUTTON_ABOUTSEND, CIID_BUTTON_ABOUTPOST, CIID_BUTTON_ABOUTOPENSOURCE, CIID_BUTTON_ABOUTDONATE
            , CIID_CHECKBOX_IGNOREDICT, CIID_CHECKBOX_SKIPHTML, CIID_CHECKBOX_SKIPDICT, CIID_TEXT_MEMTYPE, CIID_BUTTON_DEBUG, CIID_CONTROL_HTMLWINDOW
            , CIID_BUTTON_SETDICTCHOICE, CIID_BUTTON_SETLGSBROWSE, CIID_SLIDER_SETDELAY,CIID_PANEL_HTML, CIID_BUTTON_SETDICTSTORECHOICE
			, CIID_BUTTON_SETANALYSISFILTER, CIID_LIST_PLUGINS, CIID_BUTTON_PLUGINSDETAIL, CIID_BUTTON_PLUGINSRUN, CIID_CHECKBOX_SETHTMLOPTIMUM, CIID_BUTTON_SETHTMLOPTIMUM };
    enum FilterMemuID { FMID_BEGIN = 12000, FMID_TAG, FMID_DATE, FMID_COUNTER, FMID_CLOSE, FMID_END, FMID_REMOVEWORDBYTAG, FMID_REMOVEWORDBYSCORE
            , FMID_REMOVEWORDBYDATE, FMID_MOVEALLBYTAG };
    enum IndexMenuID { IMID_BEGIN = 13000, IMID_SPEAK, IMID_COPY, IMID_TAGREMOVE
            , IMID_DELETE, IMID_SETTAGDEFAULT, IMID_TAGCOPY_START, IMID_TAGCOPY_END = IMID_TAGCOPY_START + MAX_NUMBER_TAG
            , IMID_TAGMOVE_START = IMID_TAGCOPY_END + 1, IMID_TAGMOVE_END = IMID_TAGMOVE_START + MAX_NUMBER_TAG
			, IMID_TAGMOVEALL_START = IMID_TAGMOVE_END + 1, IMID_TAGMOVEALL_END = IMID_TAGMOVEALL_START + MAX_NUMBER_TAG };
public:
    // begin wxGlade: LingosHookFrame::ids
    // end wxGlade

    LingosHookFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

    virtual ~LingosHookFrame();

    void SetHookButton(bool checked);
    void SetCloseFlag(bool canclose);
    void ShowHint(const wxString& hint);
private:
    // begin wxGlade: LingosHookFrame::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    friend class CDisplayObject;

    // begin wxGlade: LingosHookFrame::attributes
	wxStaticBox* sizer_13_staticbox;
    wxStaticBox* sizer_18_staticbox;
    wxStaticBox* sizer_16_staticbox;
    wxStaticBox* sizer_44_staticbox;
    wxStaticBox* sizer_15_staticbox;
	wxStaticBox* sizer_27_staticbox;
    CLHComboBox* m_cbWordIndex;
    CLHListBox* m_listIndex;

    wxPanel* notebook_context_panel[CNID_MAX_PAGES];

    wxPanel* notebook_1_pane_1;
    wxButton* m_btnFilter;
    CLHFilterTreeCtrl* m_treeFilter;
    wxPanel* m_noteIndex_pane_2;
    wxStaticText* m_textMemWord;
    wxStaticText* m_textMemScore;
    wxStaticLine* static_line_1;
    wxStaticText* label_5;
    wxStaticText* label_6;
    CLHRadioButton* m_radioMemLevel0;
    CLHRadioButton* m_radioMemLevel1;
    CLHRadioButton* m_radioMemLevel2;
    CLHRadioButton* m_radioMemLevel3;
    wxStaticLine* static_line_2;
    wxButton* m_btnMemRemove;
    wxPanel* panel_5;
    wxButton* m_btnMemNext;
    wxPanel* panel_6;
    CLHTextCtrl* m_textMemType;
    wxPanel* panel_10;
    wxStaticLine* static_line_3;
    wxPanel* panel_7;
    wxButton* m_btnMemRegen;
    wxPanel* panel_8;
    wxPanel* m_noteIndex_pane_3;
    wxNotebook* m_noteIndex;
    wxPanel* window_1_pane_1;
    CLHResultTreeCtrl* m_treeResult;
//    wxPanel* notebook_context_panel[CNID_RESULT];
    CLHHtmlWindow* m_winHTML;
//    wxPanel* notebook_context_panel[CNID_HTML];
    wxStaticText* label_13;
    wxRadioButton* m_rdSetUILAuto;
    wxRadioButton* m_rdSetUILEnglish;
    wxRadioButton* m_rdSetUILChinese;
    wxPanel* panel_12;
    wxCheckBox* m_cbSetAutoHook;
    wxStaticText* label_11;
    wxTextCtrl* m_textSetLgsLocal;
    wxButton* m_btnSetLgsBrowse;

    wxStaticText* label_14;
    wxComboBox* m_listInfLang;	
    wxCheckBox* m_cbSetStopRetrieve;
    wxCheckBox* m_cbSetUseHotkey;
    wxComboBox* m_listSetHotkey;
    wxPanel* panel_13;
    wxCheckBox* m_cbSetAutoSpeak;	

//    wxStaticText* label_1;
//    wxCheckBox* m_checkSetTagSync;
//    wxCheckBox* m_checkSetMemSync;
    wxStaticText* label_12;
    wxSlider* m_sliderSetDelay;
    wxStaticText* m_labelSetDelay;
    //wxStaticText* label_7;
    //wxCheckBox* m_checkSetUseTidy;
    //wxStaticText* label_2;
    //wxCheckBox* m_checkHTMLSave;
    //wxCheckBox* m_checkHTMLLoad;
    wxStaticLine* static_line_6;
    wxCheckBox* m_checkIgnoreDict;
    wxCheckBox* m_checkSkipDict;
    wxCheckBox* m_checkSkipHtml;
    wxButton* m_btnSetDictStoreChoice;
    wxButton* m_btnSetAnalysisFilter;
	wxCheckBox* m_checkHtmlOptimum;
	wxButton* m_btnSetHtmlOptimum;
    wxStaticLine* static_line_5;
    wxStaticText* label_4;
    wxComboBox* m_comboxExpandDict;
    wxStaticText* label_10;
    wxButton* m_btnSetDictChoice;
    wxStaticText* label_9;
    wxComboBox* m_listFavoriteTab;
    wxCheckBox* m_checkTrace;
    wxPanel* panel_1;
    wxPanel* panel_2;
    wxButton* m_btnSetApply;

    wxStaticText* label_3;
    wxTextCtrl* m_textDefTag;
    wxListCtrl* m_listTagMgnt;
    wxButton* m_btnTagDefault;
    wxPanel* panel_3;
    wxButton* m_btnTagAdd;
    wxPanel* panel_14;
    wxButton* m_btnTagRename;
    wxPanel* panel_4;
    wxButton* m_btnTagRemove;
    //wxPanel* m_noteContext_pane_3;
	
    wxListCtrl* m_listPlugins;
    wxStaticLine* static_line_7;
    wxButton* m_btnPluginsDetail;
    wxPanel* panel_11;
    wxButton* m_btnPluginsRun;

    wxStaticText* label_8;
    wxStaticLine* static_line_4;
    wxButton* m_btnAboutHelp;
    wxButton* m_btnAboutSend;
    wxButton* m_btnAboutPost;
    wxButton* m_btnAboutOpenSource;
    wxButton* m_btnAboutDonate;
    wxPanel* panel_9;
	
    wxTextCtrl* m_textDebug;
    wxButton* m_btnDebug;
    wxTextCtrl* m_textTrace;

    wxNotebook* m_noteContext;
    wxPanel* window_1_pane_2;
    wxSplitterWindow* m_splitWindow;
    wxStaticText* m_labelInfo;
    wxToggleButton* m_btnHook;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    virtual void OnWordIndexEnter(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnWordIndexText(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnWordIndexFocus(wxCommandEvent& event);
    virtual void OnIndexDClick(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnIndexSelected(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnIndexDelete(wxCommandEvent& event);
    virtual void OnIndexFindItem(wxCommandEvent& event);
    virtual void OnIndexContextMenu(wxCommandEvent& event);
    virtual void OnIndexFocus(wxCommandEvent& event);
    virtual void OnNoteIndexChanged(wxNotebookEvent &event); // wxGlade: <event_handler>
    virtual void OnCheckSetStopRetrieve(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnCheckSetUseHotkey(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnSetApply(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnSetLgsBrowse(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnListTagMgntDeselect(wxListEvent &event); // wxGlade: <event_handler>
    virtual void OnListTagMgntSelect(wxListEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnTagSetDefault(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnTagAdd(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnTagRemove(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnTagRename(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnNoteContextChanged(wxNotebookEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnHook(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnFilter(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnTreeFilterChange(wxTreeEvent &event); // wxGlade: <event_handler>
    virtual void OnTreeFilterFocus(wxCommandEvent& event);
    virtual void OnMenuFilter(wxCommandEvent& event);
    virtual void OnContextMenuFilter(wxCommandEvent& event);
    virtual void OnBtnMemRemove(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnMemNext(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnRadioMemLevel(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnRadioMemLevelFocus(wxCommandEvent& event);
    virtual void OnBtnMemRegen(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnAboutHelp(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnAboutSend(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnAboutPost(wxCommandEvent &event); // wxGlade: <event_handler>	
    virtual void OnBtnAboutOpenSource(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnAboutDonate(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnMenuIndexSpeak(wxCommandEvent& event);
    virtual void OnMenuIndexCopy(wxCommandEvent& event);
    virtual void OnMenuIndexDelete(wxCommandEvent& event);
    virtual void OnMenuIndexTagCopy(wxCommandEvent& event);
	virtual void OnMenuIndexTagMoveAll(wxCommandEvent& event);
    virtual void OnMenuIndexTagRemove(wxCommandEvent& event);
    virtual void OnMenuSetTagDefault(wxCommandEvent& event);    
    virtual void OnTreeFilterContextMenu(wxCommandEvent& event);
    virtual void OnCheckIgnoreDict(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnCheckSkipHTML(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnCheckSkipDict(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnTreeResultContextMenu(wxCommandEvent& event);
    virtual void OnMemTypeKeyDown(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnMemTypeFocus(wxCommandEvent& event);
    virtual void OnMemTypeText(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnDebug(wxCommandEvent &event); // wxGlade: <event_handler>

    virtual void OnBtnSetDictStoreChoice(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnSetAnalysisFilter(wxCommandEvent &event); // wxGlade: <event_handler>
	virtual void OnBtnSetHtmlOptimum(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnSetDictChoice(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnScrollSetDelayEnd(wxScrollEvent &event); // wxGlade: <event_handler>

    virtual void OnListPluginsDeselected(wxListEvent &event); // wxGlade: <event_handler>
    virtual void OnListPluginsSelected(wxListEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnPluginsDetail(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnBtnPluginsRun(wxCommandEvent &event); // wxGlade: <event_handler>
	
	virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);

    virtual void OnClose(wxCloseEvent& event);
public:
	void HookTextProc(const wxString& text);
	void HookHTMLProc(const wxString& html);
    void HookCDProc(const wxString& str);

private:
    int CreateObjects();
    int InitConfigData();
    int InitLocale();
	int InitObjects();
    int LoadObjects();
    int LoadHomePage();
    int UpdateConfigData(bool retrieve);
    //int MakeContextMenu(const wxString& title, bool isword, int menuid, const wxPoint& pos);
    int MakeWordContextMenu(const wxString& title, const wxPoint& pos);
    int MakeWordContextMenu(const wxString& title, int filtertype, const wxPoint& pos);
    int MakeFilterContextMenu(const wxString& title, int menuid, const wxPoint& pos);
    wxMenu* MakeTagSubMenu(int baseid, int selectedid = -1);

    int CallHook(bool hook);
private:
    int CopyWord(const wxString& word);
    int SpeakWord(const wxString& word);
    int RemoveWord(const wxString& word);
    int RemoveWord(int wordid);
    int CopyToTag(const wxString& word, int tagpos);
    int RemoveFromTag(int wordid, int tagid);
private:
	CDBAccess _objDB;

	std::auto_ptr<CHookObject> _objHook;
    std::auto_ptr<CConfigData> _dataConfig;
	std::auto_ptr<CDictObject> _objDict;
    std::auto_ptr<CTagObject> _objTag;
    std::auto_ptr<CDisplayObject> _objDisplay;
    std::auto_ptr<MemoryDaily::CManageObject> _objMemoryDaily;
    std::auto_ptr<CSpeakObject> _objSpeak;
    std::auto_ptr<CFilterShowObject> _objFilterShow;
    std::auto_ptr<CPluginObject> _objPlugin;

    CTrayIconObject* _objTrayIcon;
private:
    bool _bSysCanClose;
    wxLocale _objLocale;
}; // wxGlade: end class


class MyApp: public wxApp {
public:
    bool OnInit();
protected:
    static const wxCmdLineEntryDesc _cmdLineDesc[3];
};

DECLARE_APP(MyApp)

#endif // LINGOSHOOKAPP_H