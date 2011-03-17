/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __DICTSTRUCTURE_H__
#define __DICTSTRUCTURE_H__

#include <map>
#include <vector>
#include <string>

#include "wx/wx.h"

//#include "HtmlDictParser.h"
//#include "SpecialDictParser.h"

struct TWordData
{
    int m_iWordID;
    int m_iSrcID;
    std::wstring m_strWord;
    int m_iCounter;
    wxDateTime m_dtUpdate;
    wxDateTime m_dtCheckin;
    std::wstring m_strHTML;
};

namespace SpecialDictParser
{

class CParserResult
{
public:
    CParserResult() {}
    virtual ~CParserResult() {}
};

class CDictResult
{
public:
    CDictResult(const CParserResult* result = NULL);
    virtual ~CDictResult();

    CDictResult(const CDictResult& right);

    void Attach(const CParserResult* result);
    const CParserResult* Result() const;
protected:
    std::auto_ptr<const CParserResult> _result;
};

typedef std::map<std::wstring, CDictResult> TDictResultMap;//dictid + result

}


namespace HtmlDictParser
{

struct TDictResult
{
    int m_iDictIndex;

    int m_iDictStart;
    int m_iDictEnd;
};

//typedef std::map<int, TDictResult> TDictResultMap;//order + result
typedef std::vector<TDictResult> TDictResultMap;//order + result

//HTML Data Type
//const int HTMLDATATYPE_0    =   0;
const int HTMLDATATYPE_1    =   0;
const int HTMLDATATYPE_2    =   1;
const int HTMLDATATYPE_3    =   2;

const int MAX_HTMLDATATYPE  =   3;

struct TDictDefAttr
{
    const std::wstring dictid;
    int deftype;
};

}

struct TResult
{
    SpecialDictParser::TDictResultMap m_resultDict;
    HtmlDictParser::TDictResultMap m_resultHtml;
};

typedef std::map<std::wstring, TResult> TResultMap;//word + result

///////////////////
const int SIZE_DICTDEFATTR  =   10;
extern HtmlDictParser::TDictDefAttr g_stSysDictDefAttr[SIZE_DICTDEFATTR];

#endif

