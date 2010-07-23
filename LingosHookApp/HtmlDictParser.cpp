/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include "wx/wx.h"

#include "DictChoiceDialog.h"
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

}

/////////////////

int CHtmlDictParser::Init(CDBAccess::TDatabase &db)
{
    return _objDictInfo.Init(db);         
}

//int CHtmlDictParser::ParserHTML(const std::wstring &html, HtmlDictParser::TDictResultVector &result)
//{
//    return -1;        
//}

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
    
    res.m_iDictStart = dict->start;
    res.m_iDictEnd = dict->end;

    TResultMap::iterator it = result.end();
    bool find = false;
    TinyHtmlParser::CDocumentObject::TElementStack tmpstack;
    const TinyHtmlParser::CElementObject* pdiv = doc.FindFirstElement(dict, L"DIV", tmpstack);
    while(pdiv != NULL)
    {
        const TinyHtmlParser::CAttributeObject* pstyle = pdiv->FindAttribute(L"style");
        if(pstyle != NULL && pstyle->value == L"\"MARGIN: 0px 0px 5px; COLOR: #808080; LINE-HEIGHT: normal\"")
        {
            if(pdiv->child != NULL && pdiv->child->child != NULL)
            {
                it = result.find(pdiv->child->child->value);
                if(it == result.end())
                {
                    it = result.insert(std::make_pair(pdiv->child->child->value, TResult())).first;
                    it->second.m_resultHtml.push_back(res);
                }
                else
                {
                    find = false;
                    for(HtmlDictParser::TDictResultMap::const_iterator i = it->second.m_resultHtml.begin(); i != it->second.m_resultHtml.end(); ++ i)
                    {
                        if(i->m_iDictIndex == res.m_iDictIndex)
                        {
                            find = true;
                            break;
                        }
                    }
                    if(!find)
                    {
                        it->second.m_resultHtml.push_back(res);
                    }
                }
            }
        }
        pdiv = doc.FindNextElement(dict, L"DIV", tmpstack);
    }

    return 0;
}

///
int CHtmlDictParser::UpdateDictInfo(CDBAccess::TDatabase &db, const std::wstring &dictid, const std::wstring &html, const TinyHtmlParser::CDocumentObject &doc, const TinyHtmlParser::CElementObject *dict)
{
    TinyHtmlParser::CDocumentObject::TElementStack tmpstack;
    const TinyHtmlParser::CElementObject* pdiv = doc.FindFirstElement(dict, L"DIV", tmpstack);
    while(pdiv != NULL)
    {
        const TinyHtmlParser::CAttributeObject* pstyle = pdiv->FindAttribute(L"style");
        if(pstyle != NULL && pstyle->value == L"\"MARGIN: 0px 3px 1px 0px; CURSOR: default\"")
        {
            if(pdiv->child != NULL && pdiv->child->sibling != NULL)
            {
                return InsertDictInfo(db, dictid, pdiv->child->sibling->value);
            }
        }

        pdiv = doc.FindNextElement(dict, L"DIV", tmpstack);
    }
    return -1;
}

int CHtmlDictParser::InsertDictInfo(CDBAccess::TDatabase &db, const std::wstring &dictid, const std::wstring &title)
{
    try
    {      
        HtmlDictParser::TDictInfo info;
        info.m_strDictID = dictid;
        info.m_strTitle = title;
        info.m_stConfig.m_iLoadParam = _objDictInfo.Size();
        info.m_stConfig.m_iStoreParam = _objDictInfo.Size();

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

void CHtmlDictParser::ShowDictInfo(int usehtmldict, CHtmlDictChoiceDialog &dlg) const
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

int CHtmlDictParser::GetDictInfo(CDBAccess::TDatabase &db, int& usehtmldict, const CHtmlDictChoiceDialog &dlg)
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

