/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include "wx/wx.h"

#include "DictLoadChoiceDialog.h"
#include "DictStoreChoiceDialog.h"
#include "HtmlDictParser.h"

namespace HtmlDictParser
{

int CDictInfoObject::Init(CDBAccess::TDatabase &db)
{
    try
    {
        if(!db.TableExists(wxT("HtmlDictResultTable")))
        {
		    const char* dicttable = "CREATE TABLE HtmlDictResultTable (WordID INTEGER,DictIndex INTEGER,DictStart INTEGER,DictEnd INTEGER)";
		    db.ExecuteUpdate(dicttable);
        }

        CDBAccess::TQuery query = db.PrepareStatement(wxT("SELECT DictTable.DictIndex, DictTable.DictID, DictTable.Title, DictConfigTable.LoadParam, DictConfigTable.StoreParam FROM DictTable, DictConfigTable WHERE DictTable.DictIndex = DictConfigTable.DictIndex"));
        CDBAccess::TResult res = query.ExecuteQuery();
        while(res.NextRow())
        {
            int index = res.GetInt(0);

            TDictInfo info;
            info.m_strDictID = res.GetString(1);
            info.m_strTitle = res.GetString(2);
            info.m_stConfig.m_iLoadParam = res.GetInt(3);
            info.m_stConfig.m_iStoreParam = res.GetInt(4);

            _mapDictID.insert(std::make_pair(info.m_strDictID, index));
            _mapDictIndex.insert(std::make_pair(index, info));
        }        
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CDictInfoObject::Insert(int index, const HtmlDictParser::TDictInfo &info)
{
    if(_mapDictIndex.find(index) != _mapDictIndex.end())
        return -1;

    _mapDictID.insert(std::make_pair(info.m_strDictID, index));
    _mapDictIndex.insert(std::make_pair(index, info));

    return 0;
}

int CDictInfoObject::GetDictIndex(const std::wstring &id) const
{
    const TDictIDMap::const_iterator it = _mapDictID.find(id);
    if(it == _mapDictID.end())
        return -1;
    return it->second;
}

int CDictInfoObject::GetDictID(int index, std::wstring& id) const
{
    const TDictIndexMap::const_iterator it = _mapDictIndex.find(index);
    if(it == _mapDictIndex.end())
        return -1;
    id = it->second.m_strDictID;
    return 0;
}

int CDictInfoObject::GetDictStoreParam(int index) const
{
    const TDictIndexMap::const_iterator it = _mapDictIndex.find(index);
    if(it == _mapDictIndex.end())
        return -1;
    return it->second.m_stConfig.m_iStoreParam;
}

//////////////////////////

int CDictKnownAttrObject::Init(CDBAccess::TDatabase& db)
{
    if(!db.TableExists(wxT("HtmlDictKnownAttrTable")))
    {
        try
        {
            const char* table = "CREATE TABLE HtmlDictKnownAttrTable(DictID VARCHAR(64) PRIMARY KEY, DefType INTEGER)";
	        db.ExecuteUpdate(table);

            for(int i = 0; i < SIZE_DICTDEFATTR; ++ i)
            {
                if(InsertDefType(db, g_stSysDictDefAttr[i].dictid, g_stSysDictDefAttr[i].deftype) != 0)
                   return -1;
            }
        }
        catch(const CDBAccess::TException& e)
        {
            return -1;
        }
    }
    return LoadDefType(db);
}

int CDictKnownAttrObject::LoadDefType(CDBAccess::TDatabase& db)
{
    try
    {
        const char* sql = "SELECT DictID, DefType FROM HtmlDictKnownAttrTable";
        CDBAccess::TResult res = db.ExecuteQuery(sql);

        while(res.NextRow())
        {
            _mapDefType.insert(std::make_pair(res.GetString(0), res.GetInt(1)));
        }

    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CDictKnownAttrObject::GetDefType(const std::wstring& dictid) const
{
    TDefTypeMap::const_iterator it = _mapDefType.find(dictid);
    if(it == _mapDefType.end())
        return -1;
    return it->second;
}

int CDictKnownAttrObject::SetDefType(CDBAccess::TDatabase &db, const std::wstring &dictid, int type)
{
    TDefTypeMap::iterator it = _mapDefType.find(dictid);
    if(it == _mapDefType.end())
    {
        if(InsertDefType(db, dictid, type) != 0)
            return -1;

       _mapDefType.insert(std::make_pair(dictid, type));
    }
    else
    {
        if(UpdateDefType(db, dictid, type) != 0)
            return -1;
        it->second = type;
    }
    return 0;
}

int CDictKnownAttrObject::InsertDefType(CDBAccess::TDatabase &db, const std::wstring &dictid, int type) const
{
    try
    {
        CDBAccess::TQuery query = db.PrepareStatement(wxT("INSERT INTO HtmlDictKnownAttrTable (DictID, DefType) VALUES (?, ?)"));
        query.Bind(1, dictid);
        query.Bind(2, type);
        if(query.ExecuteUpdate() == 0)
            return -1;
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }

    return 0;
}

int CDictKnownAttrObject::UpdateDefType(CDBAccess::TDatabase &db, const std::wstring &dictid, int type) const
{
    try
    {
        CDBAccess::TQuery query = db.PrepareStatement(wxT("UPDATE HtmlDictKnownAttrTable SET DefType = ? WHERE DictID = ?"));
        query.Bind(1, type);
        query.Bind(2, dictid);
        if(query.ExecuteUpdate() == 0)
            return -1;
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

}

/////////////////

int CHtmlDictParser::Init(CDBAccess::TDatabase &db)
{
    if(_objDictInfo.Init(db) != 0)
        return -1;
    return _objDictKnownAttr.Init(db);
}

int CHtmlDictParser::ParserHTML(CDBAccess::TDatabase &db, const std::wstring& html, const std::wstring &dictid, const TinyHtmlParser::CDocumentObject &doc, const TinyHtmlParser::CElementObject *dict, TResultMap &result)
{
    HtmlDictParser::TDictResult res;
    res.m_iDictIndex = _objDictInfo.GetDictIndex(dictid);
    if(res.m_iDictIndex == -1)
    {
        res.m_iDictIndex = UpdateDictInfo(db, dictid, html, doc, dict);
        if(res.m_iDictIndex == -1)
            return -1;
    }

    int store = _objDictInfo.GetDictStoreParam(res.m_iDictIndex);
    if(store == -1)
        return -1;
    if((store & 0xFFFF) == 0xFFFF)
        return 0;
    
    res.m_iDictStart = dict->start;
    res.m_iDictEnd = dict->end;

    const TinyHtmlParser::CElementObject* pdiv = dict->child->sibling;

    if(pdiv == NULL)
        return 0;

    int type = (store & 0xFFFF0000) >> 16;
    switch(type)
    {
    case HtmlDictParser::HTMLDATATYPE_1:
        return HtmlDataType1Proc(html, dictid, doc, dict, pdiv, res, result);
    case HtmlDictParser::HTMLDATATYPE_2:
        return HtmlDataType2Proc(html, dictid, doc, dict, pdiv, res, result);
    case HtmlDictParser::HTMLDATATYPE_3:
        return HtmlDataType3Proc(html, dictid, doc, dict, pdiv, res, result);
    default:
        return -1;
    }
    return 0;
}

int CHtmlDictParser::PushResult(const std::wstring& word, const HtmlDictParser::TDictResult& res, TResultMap &result) const
{
    if(word.empty())
        return 0;

    TResultMap::iterator it = result.find(word);
    if(it == result.end())
    {
        it = result.insert(std::make_pair(word, TResult())).first;
        it->second.m_resultHtml.push_back(res);
    }
    else
    {
        for(HtmlDictParser::TDictResultMap::const_iterator i = it->second.m_resultHtml.begin(); i != it->second.m_resultHtml.end(); ++ i)
        {
            if(i->m_iDictIndex == res.m_iDictIndex)
            {
                return 0;
            }
        }
        it->second.m_resultHtml.push_back(res);
    }
    return 0;
}

///
int CHtmlDictParser::UpdateDictInfo(CDBAccess::TDatabase &db, const std::wstring &dictid, const std::wstring &html, const TinyHtmlParser::CDocumentObject &doc, const TinyHtmlParser::CElementObject *dict)
{
    if(dict->child == NULL || dict->child->child == NULL || dict->child->child->child == NULL
        || dict->child->child->child->child == NULL || dict->child->child->child->child == NULL
        || dict->child->child->child->child->child == NULL || dict->child->child->child->child->child->child == NULL 
        || dict->child->child->child->child->child->child->sibling == NULL)
        return -1;

    const TinyHtmlParser::CElementObject* pdiv = dict->child->child->child->child->child->child->sibling;

    int store = _objDictKnownAttr.GetDefType(dictid);
    if(store == -1)
        store = 1;

    return InsertDictInfo(db, dictid, pdiv->value, _objDictInfo.Size(), store << 16);
}

int CHtmlDictParser::InsertDictInfo(CDBAccess::TDatabase &db, const std::wstring &dictid, const std::wstring &title, int load, int store)
{
    try
    {      
        HtmlDictParser::TDictInfo info;
        info.m_strDictID = dictid;
        info.m_strTitle = title;
        info.m_stConfig.m_iLoadParam = load;//_objDictInfo.Size();
        info.m_stConfig.m_iStoreParam = store;//0x00010000;//_objDictInfo.Size();

        CDBAccess::TQuery query = db.PrepareStatement(wxT("INSERT INTO DictTable (DictID, Title) VALUES (?, ?)"));
        query.Bind(1, info.m_strDictID.c_str());
        query.Bind(2, info.m_strTitle.c_str());
        query.ExecuteUpdate();

        int index = db.GetLastRowId().ToLong();

        //Update config table
        query.Reset();
        query = db.PrepareStatement("INSERT INTO DictConfigTable (DictIndex, LoadParam, StoreParam) VALUES(?, ?, ?)");
        query.Bind(1, index);
        query.Bind(2, info.m_stConfig.m_iLoadParam);
        query.Bind(3, info.m_stConfig.m_iStoreParam);
        if(query.ExecuteUpdate() == 0)
            return -1;

        if(_objDictInfo.Insert(index, info) != 0)
            return -1;

        return index;
    }    
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return -1;
}

int CHtmlDictParser::UpdateDictConfig(CDBAccess::TDatabase &db, int dictindex, int loadparam, int storeparam)
{
    try
    {
        CDBAccess::TQuery query = db.PrepareStatement("UPDATE DictConfigTable SET LoadParam = ?, StoreParam = ? WHERE DictIndex = ?");
        query.Bind(1, loadparam);
        query.Bind(2, storeparam);
        query.Bind(3, dictindex);
        if(query.ExecuteUpdate() == 0)
            return -1;
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CHtmlDictParser::SaveResult(CDBAccess::TDatabase &db, int wordid, const HtmlDictParser::TDictResultMap &result)
{
    try
    {
        for(HtmlDictParser::TDictResultMap::const_iterator it = result.begin(); it != result.end(); ++ it)
        {
            CDBAccess::TQuery query = db.PrepareStatement(wxT("INSERT INTO HtmlDictResultTable (WordID, DictIndex, DictStart, DictEnd) VALUES (?, ?, ?, ?)"));
            query.Bind(1, wordid);
            query.Bind(2, it->m_iDictIndex);
            query.Bind(3, it->m_iDictStart);
            query.Bind(4, it->m_iDictEnd);
            if(query.ExecuteUpdate() == 0)
                return -1;
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CHtmlDictParser::GetResult(CDBAccess::TDatabase &db, int wordid, HtmlDictParser::TDictResultMap &result)
{
    try
    {
        CDBAccess::TQuery query = db.PrepareStatement(wxT("SELECT HtmlDictResultTable.DictIndex, DictStart, DictEnd FROM HtmlDictResultTable, DictConfigTable WHERE WordID=? AND (HtmlDictResultTable.DictIndex = DictConfigTable.DictIndex) and DictConfigTable.LoadParam != 65535 ORDER BY DictConfigTable.LoadParam"));
        query.Bind(1, wordid);
        CDBAccess::TResult res = query.ExecuteQuery();
        while(res.NextRow())
        {
            HtmlDictParser::TDictResult r;
            r.m_iDictIndex = res.GetInt(0);
            r.m_iDictStart = res.GetInt(1);
            r.m_iDictEnd = res.GetInt(2);
            result.push_back(r);
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CHtmlDictParser::RemoveResult(CDBAccess::TDatabase &db, int wordid)
{
    try
    {
        CDBAccess::TQuery query = db.PrepareStatement(wxT("DELETE FROM HtmlDictResultTable WHERE WordID=?"));
        query.Bind(1, wordid);
        query.ExecuteUpdate();
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

int CHtmlDictParser::GenHtmlResult(const HtmlDictParser::TDictResultMap &result, const std::wstring &html, std::wstring& output) const
{
    output = L"";
    for(HtmlDictParser::TDictResultMap::const_iterator it = result.begin(); it != result.end(); ++ it)
    {
        output  += html.substr(it->m_iDictStart, it->m_iDictEnd - it->m_iDictStart + 1);
    }
    return 0;
}

void CHtmlDictParser::ShowDictLoadInfo(int usehtmldict, CHtmlDictLoadChoiceDialog &dlg) const
{
    CLHCheckBoxList& ctrl = dlg.ListCtrl();

    for(HtmlDictParser::TDictIndexMap::const_iterator it = _objDictInfo._mapDictIndex.begin(); it != _objDictInfo._mapDictIndex.end(); ++ it)
    {
        if(it->second.m_stConfig.m_iLoadParam != 0xFFFF)
        {
            int i = 0;
            while(i < ctrl.GetItemCount())
            {
                int data = ctrl.GetItemData(i);
                const HtmlDictParser::TDictIndexMap::const_iterator iter = _objDictInfo._mapDictIndex.find(data);
                if(iter == _objDictInfo._mapDictIndex.end())
                    return;

                if(it->second.m_stConfig.m_iLoadParam < iter->second.m_stConfig.m_iLoadParam)
                {
                    ctrl.InsertItem(i, it->second.m_strTitle, -1);
                    ctrl.SetItem(i, 1, it->second.m_strDictID);
                    ctrl.SetItemData(i, it->first);
                    break;
                }            
                ++ i;
            }
            if(i == ctrl.GetItemCount())
            {
                ctrl.InsertItem(i, it->second.m_strTitle, -1);
                ctrl.SetItem(i, 1, it->second.m_strDictID);
                ctrl.SetItemData(i, it->first);
            }

            ctrl.SetChecked(i, true);
        }
        else
        {
            int i = ctrl.InsertItem(ctrl.GetItemCount(), it->second.m_strTitle, -1);
            ctrl.SetItem(i, 1, it->second.m_strDictID);
            ctrl.SetItemData(i, it->first);

            ctrl.SetChecked(i, false);
        }
    }

    dlg.SetUseDictRadio(usehtmldict);
}

int CHtmlDictParser::GetDictLoadInfo(CDBAccess::TDatabase &db, int& usehtmldict, const CHtmlDictLoadChoiceDialog &dlg)
{
    dlg.GetUseDictRadio(usehtmldict);

    if(usehtmldict == 0)
        return 0;

    const CLHCheckBoxList& ctrl = dlg.ListCtrl();

    int i = 0;
    while(i < ctrl.GetItemCount())
    {
        int data = ctrl.GetItemData(i);
        const HtmlDictParser::TDictIndexMap::iterator iter = _objDictInfo._mapDictIndex.find(data);
        if(iter == _objDictInfo._mapDictIndex.end())
            return -1;
        if(ctrl.IsChecked(i) == true)
        {
            iter->second.m_stConfig.m_iLoadParam = i;
        }
        else
        {
            iter->second.m_stConfig.m_iLoadParam = 0xFFFF;
        }
        
        if(UpdateDictConfig(db, iter->first, iter->second.m_stConfig.m_iLoadParam, iter->second.m_stConfig.m_iStoreParam) != 0)
            return -1;        

        ++ i;
    }
    return 0;
}

void CHtmlDictParser::ShowDictStoreInfo(CHtmlDictStoreChoiceDialog &dlg) const
{
    CLHCheckBoxList& ctrl = dlg.ListCtrl();
    
    for(HtmlDictParser::TDictIndexMap::const_iterator it = _objDictInfo._mapDictIndex.begin(); it != _objDictInfo._mapDictIndex.end(); ++ it)
    {
        int type = (it->second.m_stConfig.m_iStoreParam & 0xFFFF0000) >> 16;
        int i = ctrl.InsertItem(ctrl.GetItemCount(), wxString::Format(wxT("%d"), type, -1));
        ctrl.SetItem(i, 1, it->second.m_strTitle);
        ctrl.SetItem(i, 2, it->second.m_strDictID);
        ctrl.SetItemData(i, it->first);

        int deftype = _objDictKnownAttr.GetDefType(it->second.m_strDictID);
        if(deftype != -1 && deftype == type)
        {
            wxFont& font = ctrl.GetItemFont(i);
            font.SetWeight(wxFONTWEIGHT_BOLD);
            ctrl.SetItemFont(i, font);
        }
        
        if((it->second.m_stConfig.m_iStoreParam & 0xFFFF) != 0xFFFF)
        {
            ctrl.SetChecked(i, true);
        }
        else
        {
            ctrl.SetChecked(i, false);
        }
    }
}

int CHtmlDictParser::GetDictStoreInfo(CDBAccess::TDatabase &db, const CHtmlDictStoreChoiceDialog &dlg)
{
    const CLHCheckBoxList& ctrl = dlg.ListCtrl();
    
    int i = 0;
    while(i < ctrl.GetItemCount())
    {
        int data = ctrl.GetItemData(i);
        const HtmlDictParser::TDictIndexMap::iterator iter = _objDictInfo._mapDictIndex.find(data);
        if(iter == _objDictInfo._mapDictIndex.end())
            return -1;

        ctrl.GetItemText(i).ToLong((long*)&(iter->second.m_stConfig.m_iStoreParam));
        iter->second.m_stConfig.m_iStoreParam = (iter->second.m_stConfig.m_iStoreParam) << 16;

        if(ctrl.IsChecked(i) == true)
        {
            iter->second.m_stConfig.m_iStoreParam &= 0xFFFF0000;
        }
        else
        {
            iter->second.m_stConfig.m_iStoreParam |= 0x0000FFFF;
        }
        
        if(UpdateDictConfig(db, iter->first, iter->second.m_stConfig.m_iLoadParam, iter->second.m_stConfig.m_iStoreParam) != 0)
            return -1;          

        ++ i;
    }

    return 0;
}

int CHtmlDictParser::ResetDictStoreInfo(CDBAccess::TDatabase &db, CHtmlDictStoreChoiceDialog &dlg)
{
    for(HtmlDictParser::TDictIndexMap::iterator it = _objDictInfo._mapDictIndex.begin(); it != _objDictInfo._mapDictIndex.end(); ++ it)
    {
        int deftype = _objDictKnownAttr.GetDefType(it->second.m_strDictID);
        if(deftype != -1)
        {
            it->second.m_stConfig.m_iStoreParam = (deftype << 16) | (it->second.m_stConfig.m_iStoreParam & 0x0000FFFF);
        }
    }

    dlg.ListCtrl().DeleteAllItems();

    ShowDictStoreInfo(dlg);

    return 0;
}

void CHtmlDictParser::ShowDictStoreInfoItemContextMenu(const CHtmlDictStoreChoiceDialog &dlg, long item, int menubase, wxMenu* submenu) const
{
    const CLHCheckBoxList& ctrl = dlg.ListCtrl();
    int dictindex = (int)ctrl.GetItemData(item);

    std::wstring dictid;
    if(_objDictInfo.GetDictID(dictindex, dictid) != 0)
        return;

    int type = -1;//_objDictInfo.GetDictStoreParam(dictindex) >> 16;
    ctrl.GetItemText(item).ToLong((long*)&type);
    int deftype = _objDictKnownAttr.GetDefType(dictid);

    for(int i = 0; i < HtmlDictParser::MAX_HTMLDATATYPE; ++ i)
    {
        AppendDictStoreInfoTypeMenu(submenu, menubase + i, dictindex, i, deftype);
    }

    //AppendDictStoreInfoTypeMenu(submenu, menubase, dictindex, HtmlDictParser::HTMLDATATYPE_1, deftype);
    //AppendDictStoreInfoTypeMenu(submenu, menubase + 1, dictindex, HtmlDictParser::HTMLDATATYPE_2, deftype);
    //AppendDictStoreInfoTypeMenu(submenu, menubase + 2, dictindex, HtmlDictParser::HTMLDATATYPE_3, deftype);

    if(type != deftype)
    {
        submenu->AppendSeparator();
        submenu->Append(menubase + HtmlDictParser::MAX_HTMLDATATYPE, wxString::Format(wxT("set %d as default"), type));
    }
}

void CHtmlDictParser::AppendDictStoreInfoTypeMenu(wxMenu* menu, int menuid, int dictindex, int type, int deftype) const
{
    if(deftype == type)
    {
        menu->AppendRadioItem(menuid, wxString::Format(wxT("uses type %d"), type));
    }
    else
    {
        menu->Append(menuid, wxString::Format(wxT("uses type %d"), type));
    }
}

void CHtmlDictParser::RefreshDictStoreInfo(CHtmlDictStoreChoiceDialog &dlg, long item, int type) const
{
    CLHCheckBoxList& ctrl = dlg.ListCtrl();
    int dictindex = (int)ctrl.GetItemData(item);

    std::wstring dictid;
    if(_objDictInfo.GetDictID(dictindex, dictid) != 0)
        return;

    ctrl.SetItemText(item, wxString::Format(wxT("%d"), type));

    int deftype = _objDictKnownAttr.GetDefType(dictid);
    wxFont& font = ctrl.GetItemFont(item);
    font.SetWeight(deftype == type ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL);
    ctrl.SetItemFont(item, font);
}

int CHtmlDictParser::UpdateDictStoreInfoDefType(CDBAccess::TDatabase &db, CHtmlDictStoreChoiceDialog &dlg, long item)
{
    CLHCheckBoxList& ctrl = dlg.ListCtrl();
    int dictindex = (int)ctrl.GetItemData(item);

    std::wstring dictid;
    if(_objDictInfo.GetDictID(dictindex, dictid) != 0)
        return -1;

    int deftype = -1;
    ctrl.GetItemText(item).ToLong((long*)&deftype);

    if(_objDictKnownAttr.SetDefType(db, dictid, deftype) != 0)
        return -1;

    wxFont& font = ctrl.GetItemFont(item);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    ctrl.SetItemFont(item, font);

    return 0;
}

//////////////////////////////////////////////////
int CHtmlDictParser::HtmlDataType1Proc(const std::wstring &html, const std::wstring &dictid, const TinyHtmlParser::CDocumentObject &doc, const TinyHtmlParser::CElementObject *dict, const TinyHtmlParser::CElementObject *pdiv, const HtmlDictParser::TDictResult &res, TResultMap &result) const
{
    const TinyHtmlParser::CElementObject *p = pdiv->child;
    if(p == NULL)
        return -1;
    p = p->child;

    while(p != NULL)
    {
        if(p->child == NULL || p->child->child == NULL || p->child->child->sibling == NULL
            || p->child->child->sibling->child == NULL || p->child->child->sibling->child->child == NULL
            || p->child->child->sibling->child->child->child == NULL)
            return 0;
        std::wstring word = p->child->child->sibling->child->child->child->value;
        if(PushResult(word, res, result) != 0)
            return -1;

        if(p->sibling == NULL || p->sibling->sibling == NULL)
            break;
        p = p->sibling->sibling;
    }
    return 0;
}

int CHtmlDictParser::HtmlDataType2Proc(const std::wstring &html, const std::wstring &dictid, const TinyHtmlParser::CDocumentObject &doc, const TinyHtmlParser::CElementObject *dict, const TinyHtmlParser::CElementObject *pdiv, const HtmlDictParser::TDictResult &res, TResultMap &result) const
{
    const TinyHtmlParser::CElementObject *p = pdiv->child;
    if(p == NULL)
        return -1;
    p = p->child;

    while(p != NULL)
    {
        if(p->child == NULL || p->child->child == NULL || p->child->child->sibling == NULL
            || p->child->child->sibling->child == NULL || p->child->child->sibling->child->child == NULL
            || p->child->child->sibling->child->child->child == NULL || p->child->child->sibling->child->child->child->child == NULL)
            return 0;
        std::wstring word = p->child->child->sibling->child->child->child->child->value;
        if(PushResult(word, res, result) != 0)
            return -1;

        if(p->sibling == NULL || p->sibling->sibling == NULL)
            break;
        p = p->sibling->sibling;
    }
    return 0;
}
int CHtmlDictParser::HtmlDataType3Proc(const std::wstring &html, const std::wstring &dictid, const TinyHtmlParser::CDocumentObject &doc, const TinyHtmlParser::CElementObject *dict, const TinyHtmlParser::CElementObject *pdiv, const HtmlDictParser::TDictResult &res, TResultMap &result) const
{
    const TinyHtmlParser::CElementObject *p = pdiv->child;
    if(p == NULL)
        return -1;
    if(p->child == NULL || p->child->child == NULL || p->child->child->child == NULL)
        return 0;

    p = p->child->child->child;

    while(p != NULL)
    {
        if(p->child == NULL)
            return 0;
        std::wstring word = p->child->value;
        if(PushResult(word, res, result) != 0)
            return -1;

        if(p->sibling == NULL)
            break;
        p = p->sibling;
    }
    return 0;
}

