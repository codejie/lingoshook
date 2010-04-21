#ifndef __DBACCESS_H__
#define __DBACCESS_H__

#include <vector>

#include "wx/wx.h"
#include "wx/arrstr.h"

#include "wx/wxsqlite3.h"

//namespace NG
//{

class CDBAccess
{
private:
    static const wxString KEY;
public:
    typedef wxSQLite3Database TDatabase;
    typedef wxSQLite3Statement TQuery;
    typedef wxSQLite3ResultSet TResult;
    typedef wxSQLite3Exception TException;
public:
    CDBAccess();
    virtual ~CDBAccess();

    int Init(const wxString& dbfile);
    TDatabase& Database() { return _db; }
protected:
    void Close();
private:
    TDatabase _db;
};

//}
//
//class CDBAccess
//{
//public:
//    static const wxString TABLENAME_CONFIG;
//	static const wxString TABLENAME_WORD;
//	static const wxString TABLENAME_DICTIONARY;
//	static const wxString TABLENAME_RESULT;
//
//	static const wxString WORDCLASS_NOUN;
//public:
//	enum WordClass { WC_UNKNOWN = -1, WC_NOUN = 0, WC_ADJECTIVE, WC_VERB, WC_ADVERB, WC_CONJ, WC_INTERJECTION };
//	typedef std::pair<WordClass, wxString> TResultPair;//wordclass + result
//	typedef std::vector<TResultPair> TResultVector;
//	typedef std::pair<wxString, TResultVector> TRecordPair;//dictionary+result vector
//	typedef std::vector<TRecordPair> TRecordVector;
//	typedef struct _RecordData_t
//	{
//		wxString m_strWord;
//		wxString m_strSymbol;
//		TRecordVector m_vctResult;
//		wxString m_strHTML;
//	} TRecordData;
//    typedef std::vector<TRecordData> TRecordDataVector;
//
//    typedef struct _StatsData_t
//    {
//        int m_iCounter;
//        wxDateTime m_dtCheckin;
//        wxDateTime m_dtUpdate;
//    } TStatsData;
//    typedef struct _SearchData_t
//    {
//        TRecordData record;
//        TStatsData stats;
//    } TSearchData;
//public:
//	CDBAccess();
//	virtual ~CDBAccess();
//
//	int Init(const wxString& dbfile);
//
//    int GetConfData(int attr, int& value);
//    int GetConfData(int attr, wxString& value);
//    int SetConfData(int attr, int value);
//    int SetConfData(int attr, const wxString& value);
//	
//	int Insert(const TRecordData& record, bool& insert);
//	int Remove(const wxString& word);
//	int Search(const wxString& word, TSearchData& data);
//
//	int GetAllWords(wxArrayString& vct);
//protected:
//	int InitTables();
//	void Close();
//
//	int InsertWordTable(const TRecordData& record, wxLongLong& id, bool& insert);
//	int InsertDictTable(const wxString& dict, wxLongLong& id);
//	int InsertResultTable(const wxLongLong& wordid, const wxLongLong& dictid, const TResultPair& result);
//    int UpdateWordTable(const wxLongLong& wordid, int counter);
//
////	WordClass GetWordClass(const wxString& type) const;
//
//	int RemoveWordTable(const wxString& word, wxLongLong& id);
//	int RemoveResultTable(const wxLongLong& wordid);
//
//	int SearchWordTable(const wxString& word, TSearchData& data, wxLongLong& id);
//	int SearchResultTable(const wxLongLong& wordid, TRecordData& record);
//	const wxString SearchDictTable(const wxLongLong& dictid);
//protected:
//	wxString _strDBFile;
//
//	wxSQLite3Database _DB;
//};

#endif
