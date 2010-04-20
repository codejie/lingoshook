#include <map>

#include "wx/wx.h"

#include "DBAccess.h"

//namespace NG
//{

const wxString CDBAccess::KEY    =   wxEmptyString;

CDBAccess::CDBAccess()
{
}

CDBAccess::~CDBAccess()
{
	Close();
}

int CDBAccess::Init(const wxString &dbfile)
{
	try
	{
		Close();

		_db.Open(dbfile, KEY);
	}
	catch(const TException& e)
	{
		return -1;
	}
	return 0;
}

void CDBAccess::Close()
{
	if(_db.IsOpen())
		_db.Close();
}

//}

////////////////////////////

//const wxString CDBAccess::TABLENAME_CONFIG          =   _("ConfigTable");
//const wxString CDBAccess::TABLENAME_WORD			=	_("WordTable");
//const wxString CDBAccess::TABLENAME_DICTIONARY		=	_("DictionaryTable");
//const wxString CDBAccess::TABLENAME_RESULT			=	_("ResultTable");
//
//const wxString CDBAccess::WORDCLASS_NOUN			=	_("n.");
//
//CDBAccess::CDBAccess()
//{
//}
//
//CDBAccess::~CDBAccess()
//{
//	Close();
//}
//
//int CDBAccess::Init(const wxString &dbfile)
//{
//	try
//	{
//		Close();
//
//		_DB.Open(dbfile);
//		if(InitTables() != 0)
//			return -1;
//	}
//	catch(const wxSQLite3Exception& e)
//	{
//		return -1;
//	}
//	return 0;
//}
//
//int CDBAccess::InitTables()
//{
//	try
//	{
//        const char* conftable = "CREATE TABLE IF NOT EXISTS ConfigTable (Attr INTEGER PRIMARY KEY, IntVal INTEGER, StrVal VARCHAR(255))";
//        _DB.ExecuteUpdate(conftable);
//		const char* wordtable = "CREATE TABLE IF NOT EXISTS WordTable (ID INTEGER PRIMARY KEY AUTOINCREMENT,Word VARCHAR(32) UNIQUE,Symbol VARCHAR(64),HTML TEXT,Counter INTEGER, CheckinTime DATE, UpdateTime DATE)";
//		_DB.ExecuteUpdate(wordtable);
//		const char* dicttable = "CREATE TABLE IF NOT EXISTS DictionaryTable (ID INTEGER PRIMARY KEY AUTOINCREMENT,Title VARCHAR(64),UpdateTime DATE)";
//		_DB.ExecuteUpdate(dicttable);
//		const char* resulttable = "CREATE TABLE IF NOT EXISTS ResultTable (WordID INTEGER,DictID INTEGER,ClassID INTEGER,Result VARCHAR(255))";
//		_DB.ExecuteUpdate(resulttable);
//	}
//	catch(const wxSQLite3Exception& e)
//	{
//		return -1;
//	}	
//
//	return 0;
//}
//
//void CDBAccess::Close()
//{
//	if(_DB.IsOpen())
//		_DB.Close();
//}
//
//int CDBAccess::GetConfData(int attr, int &value)
//{
//    try
//    {
//	    wxSQLite3Statement query = _DB.PrepareStatement("SELECT IntVal FROM ConfigTable WHERE Attr = ?");
//	    query.Bind(1, attr);
//	    wxSQLite3ResultSet res = query.ExecuteQuery();
//	    if(!res.IsOk())
//		    throw wxSQLite3Exception(255, _("SELECT Attr of ConfigTable FAILED."));
//	    if(!res.Eof())
//	    {
//            value = res.GetInt(0);
//	    }
//	    else 
//	    {
//            return -1;
//	    }
//    }
//    catch(const wxSQLite3Exception& e)
//    {
//        return -1;
//    }
//	return 0;
//}
//
//int CDBAccess::GetConfData(int attr, wxString &value)
//{
//    try
//    {
//	    wxSQLite3Statement query = _DB.PrepareStatement("SELECT StrVal FROM ConfigTable WHERE Attr = ?");
//	    query.Bind(1, attr);
//	    wxSQLite3ResultSet res = query.ExecuteQuery();
//	    if(!res.IsOk())
//		    throw wxSQLite3Exception(255, _("SELECT Attr of ConfigTable FAILED."));
//	    if(!res.Eof())
//	    {
//            value = res.GetAsString(0);
//	    }
//	    else 
//	    {
//            return -1;
//	    }
//    }
//    catch(const wxSQLite3Exception& e)
//    {
//        return -1;
//    }
//	return 0;
//}
//
//int CDBAccess::SetConfData(int attr, int value)
//{
//    try
//    {
//	    wxSQLite3Statement query = _DB.PrepareStatement("SELECT IntVal FROM ConfigTable WHERE Attr = ?");
//	    query.Bind(1, attr);
//	    wxSQLite3ResultSet res = query.ExecuteQuery();
//	    if(!res.IsOk())
//		    throw wxSQLite3Exception(255, _("SELECT Attr of ConfigTable FAILED."));
//        if(!res.Eof())
//        {
//            query.Reset();
//            query = _DB.PrepareStatement("UPDATE ConfigTable SET IntVal = ? WHERE Attr = ?");
//            query.Bind(1, value);
//            query.Bind(2, attr);
//            if(query.ExecuteUpdate() == 0)
//                return -1;
//        }
//        else
//        {
//            query.Reset();
//            query = _DB.PrepareStatement("INSERT INTO ConfigTable (Attr, IntVal, StrVal) VALUES(?, ?, NULL)");
//            query.Bind(1, attr);
//            query.Bind(2, value);
//            if(query.ExecuteUpdate() == 0)
//                return -1;
//        }
//    }
//	catch(const wxSQLite3Exception& e)
//	{
//		return -1;
//	}
//    return 0;
//}
//
//int CDBAccess::SetConfData(int attr, const wxString& value)
//{
//    try
//    {
//	    wxSQLite3Statement query = _DB.PrepareStatement("SELECT count(*) FROM ConfigTable WHERE Attr = ?");
//	    query.Bind(1, attr);
//	    wxSQLite3ResultSet res = query.ExecuteQuery();
//	    if(!res.IsOk())
//		    throw wxSQLite3Exception(255, _("SELECT Attr of ConfigTable FAILED."));
//        if(!res.Eof())
//        {
//            query.Reset();
//            query = _DB.PrepareStatement("UPDATE ConfigTable SET StrVal = ? WHERE Attr = ?");
//            query.Bind(1, value);
//            query.Bind(2, attr);
//            if(query.ExecuteUpdate() == 0)
//                return -1;
//        }
//        else
//        {
//            query.Reset();
//            query = _DB.PrepareStatement("INSERT INTO ConfigTable (Attr, IntVal, StrVal) VALUES(?, -1, ?)");
//            query.Bind(1, attr);
//            query.Bind(2, value);
//            if(query.ExecuteUpdate() == 0)
//                return -1;
//        }
//    }
//	catch(const wxSQLite3Exception& e)
//	{
//		return -1;
//	}
//    return 0;
//}
//
//int CDBAccess::Insert(const CDBAccess::TRecordData &record, bool& insert)
//{
//	try
//	{
//		_DB.Begin();
//
//		wxLongLong wordid = 0;
//		if(InsertWordTable(record, wordid, insert) == 0)
//		{
//            if(insert)
//            {
//			    for(TRecordVector::const_iterator it = record.m_vctResult.begin(); it != record.m_vctResult.end(); ++ it)
//			    {
//				    wxLongLong dictid = 0;
//				    InsertDictTable(it->first, dictid);
//				    for(TResultVector::const_iterator i = it->second.begin(); i != it->second.end(); ++ i)
//				    {
//					    InsertResultTable(wordid, dictid, *i);
//				    }
//			    }
//            }
//			_DB.Commit();
//			return 0;
//		}
//		else
//		{
//			_DB.Commit();
//			return -1;
//		}
//	}
//	catch(const wxSQLite3Exception& e)
//	{
//		_DB.Rollback();
//		return -1;
//	}
//}
//
//int CDBAccess::InsertWordTable(const CDBAccess::TRecordData &record, wxLongLong &id, bool& insert)
//{
//	wxSQLite3Statement query = _DB.PrepareStatement("SELECT ROWID, Counter FROM WordTable WHERE Word = ?");
//	query.Bind(1, record.m_strWord);
//	wxSQLite3ResultSet res = query.ExecuteQuery();
//	if(!res.IsOk())
//		throw wxSQLite3Exception(255, _("SELECT ROWID of WordTable FAILED."));
//	if(!res.Eof())
//	{
//        insert = false;
//
//		id = res.GetInt64(0);
//        int counter = res.GetInt(1);
//
//        return UpdateWordTable(id, counter + 1);
//	}
//	else
//	{
//		wxSQLite3Statement query = _DB.PrepareStatement("INSERT INTO WordTable VALUES(NULL, ?, ?, ?, 1, DATETIME('NOW', 'LOCALTIME'), DATETIME('NOW', 'LOCALTIME'))");
//		query.Bind(1, record.m_strWord);
//		query.Bind(2, record.m_strSymbol);
//		query.Bind(3, record.m_strHTML);
//		query.ExecuteUpdate();
//
//		id = _DB.GetLastRowId();
//        insert = true;
//	}
//
//	return 0;
//}
//
//int CDBAccess::UpdateWordTable(const wxLongLong& wordid, int counter)
//{
//    wxSQLite3Statement query = _DB.PrepareStatement("UPDATE WordTable SET Counter = ?, UpdateTime = DATETIME('NOW', 'LOCALTIME') WHERE ROWID = ?");
//    query.Bind(1, counter);
//    query.Bind(2, wordid);
//    query.ExecuteUpdate();
//
//    return 0;
//}
//
//int CDBAccess::InsertDictTable(const wxString& dict, wxLongLong &id)
//{
//	wxSQLite3Statement query = _DB.PrepareStatement("SELECT ROWID FROM DictionaryTable WHERE Title = ?");
//	query.Bind(1, dict);
//	wxSQLite3ResultSet res = query.ExecuteQuery();
//	if(!res.IsOk())
//		throw wxSQLite3Exception(255, _("SELECT ROWID of DictTable FAILED."));
//	if(!res.Eof())
//	{
//		id = res.GetInt64(0);
//	}
//	else
//	{
//		wxSQLite3Statement query = _DB.PrepareStatement("INSERT INTO DictionaryTable VALUES(NULL, ?, DATETIME('NOW', 'LOCALTIME'))");
//		query.Bind(1, dict);
//		query.ExecuteUpdate();
//
//		id = _DB.GetLastRowId();
//	}
//
//	return 0;
//}
//
//int CDBAccess::InsertResultTable(const wxLongLong &wordid, const wxLongLong &dictid, const TResultPair& result)
//{
////	WordClass type = GetWordClass(result.first);
//	
//	wxSQLite3Statement query = _DB.PrepareStatement("INSERT INTO ResultTable VALUES(?, ?, ?, ?)");
//	query.Bind(1, wordid);
//	query.Bind(2, dictid);
//	query.Bind(3, result.first);
//	query.Bind(4, result.second);
//	query.ExecuteUpdate();	
//
//	return 0;
//}
//
////CDBAccess::WordClass CDBAccess::GetWordClass(const wxString &type) const
////{
////	if(type == WORDCLASS_NOUN)
////		return WC_NOUN;
////	else
////		return WC_UNKNOWN;
////}
//
//int CDBAccess::Remove(const wxString &word)
//{
//	try
//	{
//		_DB.Begin();
//		
//		wxLongLong wordid = 0;
//		RemoveWordTable(word, wordid);
//		RemoveResultTable(wordid);
//
//		_DB.Commit();
//	}
//	catch(const wxSQLite3Exception& e)
//	{
//		_DB.Rollback();
//		return -1;
//	}
//	return 0;
//}
//
//int CDBAccess::RemoveWordTable(const wxString &word, wxLongLong &id)
//{
//	wxSQLite3Statement query = _DB.PrepareStatement("SELECT ROWID FROM WordTable WHERE Word = ?");
//	query.Bind(1, word);
//	wxSQLite3ResultSet res = query.ExecuteQuery();
//	if(!res.IsOk())
//		throw wxSQLite3Exception(255, _("SELECT ROWID of WordTable FAILED."));
//	if(!res.Eof())
//	{
//		id = res.GetInt64(0);
//
//		wxSQLite3Statement query = _DB.PrepareStatement("DELETE FROM WordTable WHERE ID = ?");
//		query.Bind(1, id);		
//		query.ExecuteUpdate();
//	}
//	else
//	{
//		throw wxSQLite3Exception(254, _("SELECT WordTable FAILED."));
//	}
//
//	return 0;
//}
//
//int CDBAccess::RemoveResultTable(const wxLongLong &wordid)
//{
//	wxSQLite3Statement query = _DB.PrepareStatement("DELETE FROM ResultTable WHERE WordID = ?");
//	query.Bind(1, wordid);		
//	query.ExecuteUpdate();
//
//	return 0;
//}
//
//int CDBAccess::Search(const wxString& word, TSearchData& data)
//{
//	try
//	{
//		wxLongLong wordid = 0;
//		if(SearchWordTable(word, data, wordid) != 0)
//			return -1;
//        if(SearchResultTable(wordid, data.record) != 0)
//			return -1;
//	}
//	catch(const wxSQLite3Exception& e)
//	{
//		return -1;
//	}
//	return 0;
//}
//
//int CDBAccess::SearchWordTable(const wxString& word, TSearchData& data, wxLongLong &id)
//{
//	wxSQLite3Statement query = _DB.PrepareStatement("SELECT ROWID,Symbol,HTML, Counter, CheckinTime, UpdateTime FROM WordTable WHERE Word = ?");
//	query.Bind(1, word);
//	wxSQLite3ResultSet res = query.ExecuteQuery();
//	if(!res.IsOk())
//		throw wxSQLite3Exception(255, _("SELECT ROWID of WordTable FAILED."));
//	if(!res.Eof())
//	{
//        data.record.m_strWord = word;
//
//		id = res.GetInt64(0);
//		data.record.m_strSymbol = res.GetAsString(1);
//		data.record.m_strHTML = res.GetAsString(2);
//        data.stats.m_iCounter = res.GetInt(3);
//        data.stats.m_dtCheckin = res.GetDateTime(4);
//        data.stats.m_dtUpdate = res.GetDateTime(5);
//	}
//	else 
//	{
//		return -1;
////		throw wxSQLite3Exception(254, _("SELECT WordTable FAILED."));
//	}
//
//	return 0;
//}
//
//int CDBAccess::SearchResultTable(const wxLongLong& wordid, CDBAccess::TRecordData& record)
//{
//	wxSQLite3Statement query = _DB.PrepareStatement("SELECT DictID, ClassID, Result FROM ResultTable WHERE wordid = ?");
//	query.Bind(1, wordid);
//	wxSQLite3ResultSet res = query.ExecuteQuery();
//	if(!res.IsOk())
//		throw wxSQLite3Exception(255, _("SELECT ROWID of ResultTable FAILED."));
//	
//	std::map<wxLongLong, TResultVector> mapResult;
//
//	res.NextRow();
//	while(!res.Eof())
//	{
//		wxLongLong dictid = res.GetInt64(0);
//		std::map<wxLongLong, TResultVector>::iterator it = mapResult.find(dictid);
//		if(it == mapResult.end())
//			it = mapResult.insert(std::make_pair(dictid, TResultVector())).first;
//		it->second.push_back(std::make_pair(WordClass(res.GetInt(1)), res.GetAsString(2)));
//
//		res.NextRow();
//	}
//
//	for(std::map<wxLongLong, TResultVector>::const_iterator it = mapResult.begin(); it != mapResult.end(); ++ it)
//	{
//		wxString dict = SearchDictTable(it->first);
//		record.m_vctResult.push_back(std::make_pair(dict, it->second));
//	}
//
//	return 0;
//}
//
//const wxString CDBAccess::SearchDictTable(const wxLongLong &dictid)
//{
//	wxSQLite3Statement query = _DB.PrepareStatement("SELECT Title FROM DictionaryTable WHERE ID = ?");
//	query.Bind(1, dictid);
//	wxSQLite3ResultSet res = query.ExecuteQuery();
//	if(!res.IsOk())
//		return _("FAILED.");
//	if(res.Eof())
//		return _("Unknown.");
//	return res.GetAsString(0);
//}
//
////////////////////
//int CDBAccess::GetAllWords(wxArrayString& vct)
//{
//	vct.clear();
//
//	wxSQLite3ResultSet res = _DB.ExecuteQuery("SELECT Word FROM WordTable ORDER BY Word");
//	if(!res.IsOk())
//		return -1;
//
//	//res.NextRow();
//	while(res.NextRow())
//	{
//		vct.push_back(res.GetAsString(0));
//
//		//res.NextRow();
//	}
//
//	return 0;
//}