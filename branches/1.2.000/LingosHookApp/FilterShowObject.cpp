/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/
#include "ConfigData.h"
#include "FilterShowObject.h"


namespace FilterShow
{

CBase::CBase(FilterMode mode)
: _eMode(mode)
{
}

CBase* CBase::GetFilter(FilterShow::FilterMode mode)
{
    switch(mode)
    {
    case FM_DATE:
        return new CDateMode();
    case FM_TAG:
        return new CTagMode();
    case FM_SCORE:
        return new CScoreMode();
    default:
        return NULL;
    }
}

//
CDateMode::CDateMode()
: CBase(FM_DATE)
{
}

int CDateMode::LoadWords(CDBAccess::TDatabase &db, CLHFilterTreeCtrl* tree)
{
    PreLoad(tree);    

    try
    {
        CDBAccess::TQuery query = db.PrepareStatement("SELECT WordTable.WordID, WordTable.Word, SrcDataTable.CheckinTime FROM WordTable, SrcDataTable WHERE WordTable.SrcID = SrcDataTable.SrcID");
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        while(res.NextRow())
        {
            if(ShowWord(tree, res.GetInt(0), res.GetString(1), res.GetDateTime(2)) != 0)
                return -1;
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }

    PostLoad(tree);

    return 0;
}

void CDateMode::PreLoad(CLHFilterTreeCtrl *tree)
{
    _mapDate.clear();
    tree->DeleteAllItems();

    wxTreeItemId root = tree->AddRoot(_("DateMode"));

    //Today
    CLHFilterTreeItemData* data = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_DATE_DAY, 0);
    wxTreeItemId id = tree->AppendItem(root, _("Today"), -1, -1, data);
    _mapDate.insert(std::make_pair(std::make_pair(CLHFilterTreeItemData::IT_DATE_DAY, 0), id));
    //Yesterday
    data = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_DATE_DAY, 1);
    id = tree->AppendItem(root, _("Yesterday"), -1, -1, data);
    _mapDate.insert(std::make_pair(std::make_pair(CLHFilterTreeItemData::IT_DATE_DAY, 1), id));

    //Day before yesterday
    data = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_DATE_DAY, 2);
    id = tree->AppendItem(root, _("Day before Yesterday"), -1, -1, data);
    _mapDate.insert(std::make_pair(std::make_pair(CLHFilterTreeItemData::IT_DATE_DAY, 2), id));

    //Days
    for(int i = 3; i < MAX_DAY + 1; ++ i)
    {
        data = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_DATE_DAY, i);
        id = tree->AppendItem(root, wxString::Format(_("%d Days Ago"), i), -1, -1, data);
        _mapDate.insert(std::make_pair(std::make_pair(CLHFilterTreeItemData::IT_DATE_DAY, i), id));
    }
    //Weeks
    for(int i = 1; i < MAX_WEEK; ++ i)
    {
        data = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_DATE_WEEK, i);
        id = tree->AppendItem(root, wxString::Format(_("%d Weeks Ago"), i), -1, -1, data);
        _mapDate.insert(std::make_pair(std::make_pair(CLHFilterTreeItemData::IT_DATE_WEEK, i), id));
    }

    data = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_DATE_WEEK, MAX_WEEK);
    id = tree->AppendItem(root, wxString::Format(_("More Than %d Weeks Ago"), MAX_WEEK - 1), -1, -1, data);
    _mapDate.insert(std::make_pair(std::make_pair(CLHFilterTreeItemData::IT_DATE_WEEK, MAX_WEEK), id));

    ////Months
    //for(int i = i; i < 6; ++ i)
    //{
    //    data = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_DATE_MONTH, i);
    //    id = tree->AppendItem(root, wxString::Format(_("%d Months Ago"), i), -1, -1, data);
    //    _mapDate.insert(std::make_pair(std::make_pair(CLHFilterTreeItemData::IT_DATE_MONTH, i), id));
    //}
}

void CDateMode::PostLoad(CLHFilterTreeCtrl* tree)
{
    for(TDateMap::const_iterator it = _mapDate.begin(); it != _mapDate.end(); ++ it)
    {
        UpdateItemText(tree, it->second);
    }
}

int CDateMode::ShowWord(CLHFilterTreeCtrl *tree, int wordid, const wxString &word, const wxDateTime &checkin)
{
    wxTreeItemId did = GetItemID(tree, checkin);
    if(!did.IsOk())
        return -1;

    CLHFilterTreeItemData* data = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_WORD, wordid);
    tree->AppendItem(did, word, -1, -1, data);

    return 0;
}

wxTreeItemId CDateMode::GetItemID(CLHFilterTreeCtrl *tree, const wxDateTime &checkin)
{
    wxTreeItemId root = tree->GetRootItem();
    if(!root.IsOk())
        return NULL;

    wxTimeSpan interval = wxDateTime::Now() - checkin;
    if(interval.GetDays() < MAX_DAY + 1)
    {//Day
        TDateMap::const_iterator it = _mapDate.find(std::make_pair(CLHFilterTreeItemData::IT_DATE_DAY, interval.GetDays()));
        if(it == _mapDate.end())
            return NULL;
        return it->second;
    }
    else 
    {
        int w = interval.GetWeeks();
        if(w > MAX_WEEK)
            w = MAX_WEEK;
        TDateMap::const_iterator it = _mapDate.find(std::make_pair(CLHFilterTreeItemData::IT_DATE_WEEK, w));
        if(it == _mapDate.end())
            return NULL;
        return it->second;
    }
    return NULL;
}

void CDateMode::UpdateItemText(CLHFilterTreeCtrl *tree, const wxTreeItemId& itemid)
{
    const CLHFilterTreeItemData* td = (const CLHFilterTreeItemData*)tree->GetItemData(itemid);
    CLHFilterTreeItemData::ItemType type = (CLHFilterTreeItemData::ItemType)td->Type();
    int val = td->ID();

    size_t count = tree->GetChildrenCount(itemid);
    switch(type)
    {
    case CLHFilterTreeItemData::IT_DATE_DAY:
        {
            if(val == 0)
            {
                tree->SetItemText(itemid, wxString::Format(_("Today [%d]"), count));
            }
            else if(val == 1)
            {
                tree->SetItemText(itemid, wxString::Format(_("Yesterday [%d]"), count));
            }
            else if(val == 2)
            {
                tree->SetItemText(itemid, wxString::Format(_("Day before Yesterday [%d]"), count));
            }
            else
            {
                tree->SetItemText(itemid, wxString::Format(_("%d Days Ago [%d]"), val, count));
            }
        }
        break;
    case CLHFilterTreeItemData::IT_DATE_WEEK:
        {
            if(val != MAX_WEEK)
                tree->SetItemText(itemid, wxString::Format(_("%d Weeks Ago [%d]"), val, count));
            else
                tree->SetItemText(itemid, wxString::Format(_("More Than %d Weeks Ago [%d]"), val - 1, count));
        }
        break;
    default:
        tree->SetItemText(itemid, wxString::Format(_("%d Unknown Ago [%d]"), val, count));
    }
}

int CDateMode::AddWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid)
{
    wxString word;
    wxDateTime date;
    if(GetWordData(db, wordid, word, date) != 0)
        return -1;

    wxTreeItemId did = GetItemID(tree, date);
    if(!did.IsOk())
        return -1;

    CLHFilterTreeItemData* data = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_WORD, wordid);
    tree->AppendItem(did, word, -1, -1, data);

    UpdateItemText(tree, did);

    return 0;
}

int CDateMode::RemoveWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid)
{
    for(TDateMap::const_iterator it = _mapDate.begin(); it != _mapDate.end(); ++ it)
    {
        wxTreeItemIdValue cookie;
        wxTreeItemId itemid = tree->GetFirstChild(it->second, cookie);
        while(itemid.IsOk())
        {
            const CLHFilterTreeItemData* cd = (const CLHFilterTreeItemData*)tree->GetItemData(itemid);
            if(cd->ID() == wordid)
            {
                tree->Delete(itemid);
                UpdateItemText(tree, it->second);

                return 0;
            }
            itemid = tree->GetNextChild(it->second, cookie);
        }
    }

    return 0;
}

int CDateMode::GetWordData(CDBAccess::TDatabase& db, int wordid, wxString &word, wxDateTime &date) const
{
    try
    {
        CDBAccess::TQuery query = db.PrepareStatement("SELECT WordTable.WordID, WordTable.Word, SrcDataTable.CheckinTime FROM WordTable, SrcDataTable WHERE WordID = ? AND WordTable.SrcID = SrcDataTable.SrcID");
        query.Bind(1, wordid);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        
        word = res.GetString(0);
        date = res.GetDateTime(1);
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

/////
CTagMode::CTagMode()
: CBase(FM_TAG)
, _iDefaultTag(-1)
{
}

int CTagMode::LoadWords(CDBAccess::TDatabase &db, CLHFilterTreeCtrl *tree)
{
    if(PreLoad(db, tree) != 0)
        return -1;

    try
    {
        CDBAccess::TQuery query = db.PrepareStatement("SELECT TagIndexTable.WordID, TagIndexTable.TagID, WordTable.Word FROM WordTable, TagIndexTable WHERE WordTable.WordID = TagIndexTable.WordID");
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        while(res.NextRow())
        {
            if(ShowWord(tree, res.GetInt(1), res.GetInt(0), res.GetString(2)) != 0)
                return -1;
        }       

    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }

    PostLoad(tree);

    return 0;
}

int CTagMode::PreLoad(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree)
{
    _mapTag.clear();
    tree->DeleteAllItems();

    wxTreeItemId root = tree->AddRoot(_("Tags"));
    
    try
    {
        CDBAccess::TQuery query = db.PrepareStatement("SELECT IntVal FROM ConfigTable WHERE Attr = ?");
        query.Bind(1, CConfigData::CA_DEFAULTTAG);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        if(!res.Eof())
        {
            _iDefaultTag = res.GetInt(0);
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

    try
    {
        CDBAccess::TQuery query = db.PrepareStatement("SELECT TagID, Title FROM TagTable");
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        while(res.NextRow())
        {
            ShowTag(tree, root, res.GetInt(0), res.GetString(1));
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }

    return 0;
}

void CTagMode::PostLoad(CLHFilterTreeCtrl* tree)
{
    for(TTagMap::const_iterator it = _mapTag.begin(); it != _mapTag.end(); ++ it)
    {
        UpdateItemText(tree, it->first, it->second);
    }
}

void CTagMode::ShowTag(CLHFilterTreeCtrl *tree, const wxTreeItemId& root, int tagid, const wxString &title)
{
    CLHFilterTreeItemData* data = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_TAG, tagid);
    wxTreeItemId id = tree->AppendItem(root, title, -1, -1, data);
    _mapTag.insert(std::make_pair(tagid, std::make_pair(title, id)));
    if(tagid == _iDefaultTag)
    {
        tree->SetItemBold(id, true);
    }
}

int CTagMode::ShowWord(CLHFilterTreeCtrl *tree, int tagid, int wordid, const wxString &word)
{
    TTagMap::const_iterator it = _mapTag.find(tagid);
    if(it == _mapTag.end())
        return -1;

    CLHFilterTreeItemData* data = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_WORD, wordid, tagid);
    tree->AppendItem(it->second.second, word, -1, -1, data);

    return 0;
}

void CTagMode::UpdateItemText(CLHFilterTreeCtrl *tree, int tagid, const TTagValue& tag)
{
    size_t count = tree->GetChildrenCount(tag.second);
    
    tree->SetItemText(tag.second, wxString::Format(_("%s [%d]"), tag.first, count));

    //g_objTrigger.OnTagUpdateCount(tagid, count);
}

int CTagMode::AddTitle(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int id)
{
    TTagMap::const_iterator it = _mapTag.find(id);
    if(it != _mapTag.end())
        return -1;

    wxString title;
    if(GetTagData(db, id, title) != 0)
        return -1;
    
    wxTreeItemId root = tree->GetRootItem();
    if(!root.IsOk())
        return -1;
    ShowTag(tree, root, id, title);
    return 0;
}

int CTagMode::RemoveTitle(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int id)
{
    return LoadWords(db, tree);
}

int CTagMode::UpdateTitle(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int id)
{
    TTagMap::const_iterator it = _mapTag.find(_iDefaultTag);
    if(it == _mapTag.end())
        return -1;

    tree->SetItemBold(it->second.second, false);

    try
    {
        CDBAccess::TQuery query = db.PrepareStatement("SELECT IntVal FROM ConfigTable WHERE Attr = ?");
        query.Bind(1, CConfigData::CA_DEFAULTTAG);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        if(!res.Eof())
        {
            _iDefaultTag = res.GetInt(0);
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

    it = _mapTag.find(_iDefaultTag);
    if(it == _mapTag.end())
        return -1;

    tree->SetItemBold(it->second.second, true);

    return 0;
}

//int CTagMode::AddWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid)
//{
//    wxString word;
//    TIntSet settag;
//
//    if(GetWordData(db, wordid, word, settag) != 0)
//        return -1;
//
//    for(TIntSet::const_iterator it = settag.begin(); it != settag.end(); ++ it)
//    {
//        TTagMap::const_iterator i = _mapTag.find((*it));
//        if(i == _mapTag.end())
//            return -1;
//
//        CLHFilterTreeItemData* cd = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_WORD, wordid, (*it));
//        tree->AppendItem(i->second.second, word, -1, -1, cd);
//
//        UpdateItemText(tree, (*it), i->second);
//    }
//
//    return 0; 
//}

//int CTagMode::RemoveWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl *tree, int wordid)
//{
//    for(TTagMap::const_iterator::const_iterator it = _mapTag.begin(); it != _mapTag.end(); ++ it)
//    {
//        wxTreeItemIdValue cookie;
//        wxTreeItemId itemid = tree->GetFirstChild(it->second.second, cookie);
//        while(itemid.IsOk())
//        {
//            const CLHFilterTreeItemData* cd = (const CLHFilterTreeItemData*)tree->GetItemData(itemid);
//            if(cd->ID() == wordid)
//            {
//                tree->Delete(itemid);
//                UpdateItemText(tree, it->first, it->second);
//
//                break;
//            }
//            itemid = tree->GetNextChild(it->second.second, cookie);
//        }
//    }
//    return 0;
//}

int CTagMode::UpdateWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl *tree, int wordid)
{
    wxString word;
    TIntSet settag;

    if(GetWordData(db, wordid, word, settag) != 0)
    {
        settag.clear();
    }

    bool find = false;
    for(TTagMap::const_iterator it = _mapTag.begin(); it != _mapTag.end(); ++ it)
    {
        find = false;
        wxTreeItemIdValue cookie;
        wxTreeItemId itemid = tree->GetFirstChild(it->second.second, cookie);
        while(itemid.IsOk())
        {
            const CLHFilterTreeItemData* cd = (const CLHFilterTreeItemData*)tree->GetItemData(itemid);
            if(cd->ID() == wordid)
            {
                find = true;

                if(settag.find(it->first) == settag.end())
                {
                    tree->Delete(itemid);
                    UpdateItemText(tree, it->first, it->second);
                }
                break;
            }
            itemid = tree->GetNextChild(it->second.second, cookie);
        }
        if(find != true)
        {
            if(settag.find(it->first) != settag.end())
            {
                CLHFilterTreeItemData* cd = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_WORD, wordid, it->first);
                tree->AppendItem(it->second.second, word, -1, -1, cd);

                UpdateItemText(tree, it->first, it->second);
            }
        }
    }

    return 0;
}

int CTagMode::GetWordData(CDBAccess::TDatabase& db, int wordid, wxString &word, CTagMode::TIntSet& settag) const
{
    try
    {
        CDBAccess::TQuery query = db.PrepareStatement("SELECT Word FROM WordTable WHERE WordID = ?");
        query.Bind(1, wordid);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        
        word = res.GetString(0);
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }

    try
    {
        CDBAccess::TQuery query = db.PrepareStatement("SELECT TagID FROM TagIndexTable WHERE WordID = ? ORDER BY WordID DESC");
        query.Bind(1, wordid);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;

        while(res.NextRow())
        {
            settag.insert(res.GetInt(0));
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }

    return 0;
}

int CTagMode::GetTagData(CDBAccess::TDatabase &db, int tagid, wxString &title) const
{
    try
    {
        CDBAccess::TQuery query = db.PrepareStatement("SELECT Title FROM TagTable WHERE TagID = ?");
        query.Bind(1, tagid);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        
        title = res.GetString(0);
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

/////
CScoreMode::CScoreMode()
: CBase(FM_SCORE)
{
}

int CScoreMode::LoadWords(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree)
{
    PreLoad(tree);    

    try
    {
        CDBAccess::TQuery query = db.PrepareStatement("SELECT WordID, Word, Counter FROM WordTable");
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        while(res.NextRow())
        {
            if(ShowWord(tree, res.GetInt(0), res.GetString(1), res.GetInt(2)) != 0)
                return -1;
        }
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }

    PostLoad(tree);

    return 0;    
}

void CScoreMode::PreLoad(CLHFilterTreeCtrl* tree)
{
    _mapScore.clear();
    tree->DeleteAllItems();

    wxTreeItemId root = tree->AddRoot(_("ScoreMode"));

    for(int i = 1; i < MAX_SCORE; ++ i)
    {
        CLHFilterTreeItemData* data = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_SCORE, i);
        wxTreeItemId id = tree->AppendItem(root, wxString::Format(_("Score : %d"), i), -1, -1, data);
        _mapScore.insert(std::make_pair(i, id));
    }

    CLHFilterTreeItemData* data = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_SCORE, MAX_SCORE);
    wxTreeItemId id = tree->AppendItem(root, wxString::Format(_("Score More Than : %d"), MAX_SCORE - 1), -1, -1, data);
    _mapScore.insert(std::make_pair(MAX_SCORE, id));
}

void CScoreMode::PostLoad(CLHFilterTreeCtrl* tree)
{
    for(TScoreMap::const_iterator it = _mapScore.begin(); it != _mapScore.end(); ++ it)
    {
        UpdateItemText(tree, it->first, it->second);
    }
}

int CScoreMode::ShowWord(CLHFilterTreeCtrl *tree, int wordid, const wxString &word, int count)
{
    wxTreeItemId itemid = GetItemID(tree, count);
    if(!itemid.IsOk())
        return -1;

    CLHFilterTreeItemData* data = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_WORD, wordid);
    tree->AppendItem(itemid, word, -1, -1, data);

    return 0;
}

wxTreeItemId CScoreMode::GetItemID(CLHFilterTreeCtrl *tree, int count)
{
    TScoreMap::const_iterator it = _mapScore.find(count);
    if(it == _mapScore.end())
        it = _mapScore.find(MAX_SCORE);
    return it->second;
}

void CScoreMode::UpdateItemText(CLHFilterTreeCtrl* tree, int score, const wxTreeItemId& itemid)
{
    size_t count = tree->GetChildrenCount(itemid);

    if(score != MAX_SCORE)
        tree->SetItemText(itemid, wxString::Format(_("Score : %d [%d]"), score, count));
    else
        tree->SetItemText(itemid, wxString::Format(_("Score More Than : %d [%d]"), score, count));
}

int CScoreMode::AddWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid)
{
    wxString word;
    int score;
    if(GetWordData(db, wordid, word, score) != 0)
        return -1;

    wxTreeItemId itemid = GetItemID(tree, score);
    if(!itemid.IsOk())
        return -1;

    CLHFilterTreeItemData* cd = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_WORD, wordid);
    tree->AppendItem(itemid, word, -1, -1, cd);
    
    UpdateItemText(tree, score, itemid);

    return 0;
}

int CScoreMode::RemoveWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl* tree, int wordid)
{
    for(TScoreMap::const_iterator it = _mapScore.begin(); it != _mapScore.end(); ++ it)
    {
        if(it->second.IsOk())
        {
            wxTreeItemIdValue cookie;
            wxTreeItemId id = tree->GetFirstChild(it->second, cookie);
            while(id.IsOk())
            {
                const CLHFilterTreeItemData* cd = (const CLHFilterTreeItemData*)tree->GetItemData(id);
                if(cd->ID() == wordid)
                {
                    tree->Delete(id);
                    UpdateItemText(tree, it->first, it->second);
                    return 0;
                }
                id = tree->GetNextChild(it->second, cookie);
            }
        }
    }

    return 0;
}

int CScoreMode::UpdateWord(CDBAccess::TDatabase& db, CLHFilterTreeCtrl *tree, int wordid)
{
    wxString word;
    int score;
    if(GetWordData(db, wordid, word, score) != 0)
        return -1;

    bool find = false;

    for(int i = -2; i <= 2; ++ i)
    {
        if(score + i < 1 || i == 0)
            continue;
        wxTreeItemId itemid = GetItemID(tree, score + i);
        if(itemid.IsOk())
        {
            wxTreeItemIdValue cookie;
            wxTreeItemId id = tree->GetFirstChild(itemid, cookie);
            while(id.IsOk())
            {
                const CLHFilterTreeItemData* cd = (const CLHFilterTreeItemData*)tree->GetItemData(id);
                if(cd->ID() == wordid)
                {
                    find = true;

                    tree->Delete(id);
                    UpdateItemText(tree, score + i, itemid);
                    break;
                }
                id = tree->GetNextChild(itemid, cookie);
            }
        }
        if(find == true)
            break;
    }    

    wxTreeItemId itemid = GetItemID(tree, score);
    if(!itemid.IsOk())
        return -1;

    CLHFilterTreeItemData* cd = new CLHFilterTreeItemData(CLHFilterTreeItemData::IT_WORD, wordid);
    tree->AppendItem(itemid, word, -1, -1, cd);
    
    UpdateItemText(tree, score, itemid);

    return 0;

    if(RemoveWord(db, tree, wordid) != 0)
        return -1;

    return AddWord(db, tree, wordid);
}

int CScoreMode::GetWordData(CDBAccess::TDatabase &db, int wordid, wxString &word, int &score) const
{
    try
    {
        CDBAccess::TQuery query = db.PrepareStatement("SELECT Word, Counter, CheckinTime FROM WordTable WHERE WordID = ?");
        query.Bind(1, wordid);
        CDBAccess::TResult res = query.ExecuteQuery();
        if(!res.IsOk())
            return -1;
        if(res.Eof())
            return -1;
        
        word = res.GetString(0);
        score = res.GetInt(1);
    }
    catch(const CDBAccess::TException& e)
    {
        return -1;
    }
    return 0;
}

}


/////
CFilterShowObject::CFilterShowObject(CDBAccess &db, CLHFilterTreeCtrl* tree)
: _db(db.Database())
, _tree(tree)
, _objFilter(NULL)
{
}

int CFilterShowObject::SetMode(FilterShow::FilterMode mode)
{
    _objFilter.reset(FilterShow::CBase::GetFilter(mode));
    if(_objFilter.get() == NULL)
        return -1;
    return 0;
}

FilterShow::FilterMode CFilterShowObject::GetMode() const
{
    if(_objFilter.get() == NULL)
        return FilterShow::FM_UNKNOWN;
    return _objFilter->GetMode();
}

int CFilterShowObject::LoadWords()
{
    if(_objFilter.get() == NULL || _tree == NULL)
        return -1;

    return _objFilter->LoadWords(_db, _tree);
}

int CFilterShowObject::AddTitle(int id)
{
    if(_objFilter.get() == NULL)
        return -1;
    return _objFilter->AddTitle(_db, _tree, id);
}

int CFilterShowObject::RemoveTitle(int id)
{
    if(_objFilter.get() == NULL)
        return -1;
    return _objFilter->RemoveTitle(_db, _tree, id);
}

int CFilterShowObject::UpdateTitle(int id)
{
    if(_objFilter.get() == NULL)
        return -1;
    return _objFilter->UpdateTitle(_db, _tree, id);
}

int CFilterShowObject::AddWord(int wordid)
{
    if(_objFilter.get() == NULL)
        return -1;
    return _objFilter->AddWord(_db, _tree, wordid);
}

int CFilterShowObject::RemoveWord(int wordid)
{
    if(_objFilter.get() == NULL)
        return -1;
    return _objFilter->RemoveWord(_db, _tree, wordid);
}

int CFilterShowObject::UpdateWord(int wordid)
{
    if(_objFilter.get() == NULL)
        return -1;
    return _objFilter->UpdateWord(_db, _tree, wordid);
}



