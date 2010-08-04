/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include "DictStructure.h"

namespace SpecialDictParser
{

CDictResult::CDictResult(const CParserResult* result)
: _result(NULL)
{
    if(result != NULL)
    {
        _result.reset(result);
    }
}

CDictResult::~CDictResult()
{
}

const CParserResult* CDictResult::Result() const
{
    return _result.get();;
}

void CDictResult::Attach(const CParserResult* result)
{
    _result.reset(result);
}

CDictResult::CDictResult(const CDictResult& right)
{
    if(this != &right)
    {
        _result.reset((const_cast<CDictResult&>(right))._result.release());
    }
}

}

/////////

HtmlDictParser::TDictDefAttr g_stSysDictDefAttr[SIZE_DICTDEFATTR] = 
    {
        {L"E1C27E806399D047822B6650194A3D32", HtmlDictParser::HTMLDATATYPE_1},//Vicon EC
        {L"7AB175CC5F622A44A0DECE976AF22A16", HtmlDictParser::HTMLDATATYPE_1},//Langdao EC
        {L"1E388F323A0BD943B30FC68598772814", HtmlDictParser::HTMLDATATYPE_1},//Fundset DC
        {L"0895DFE8DB67F9409DB285590D870EDD", HtmlDictParser::HTMLDATATYPE_2},//Dict.CN online
        {L"D4722835273E184582F2D24696A738EA", HtmlDictParser::HTMLDATATYPE_1},//Longman Dictionary of Contemporary English
        {L"25D62D261B9B6943BE86B7DCF8F9D255", HtmlDictParser::HTMLDATATYPE_1},//Concise English Synonym &amp; Antonym Dictionary
        {L"74BF7ECCE3A9AC43B5E8902261A19B29", HtmlDictParser::HTMLDATATYPE_1},//Hudong Wiki
        {L"C68363F3F2F8F04997C0552720B30742", HtmlDictParser::HTMLDATATYPE_1},//Google Web Definition
        {L"3A259F0851B07D4AAA30FBFA43686AB3", HtmlDictParser::HTMLDATATYPE_2},//Dr.eye Online
        {L"9455286C1F1BC84985523D2608B6A913", HtmlDictParser::HTMLDATATYPE_3},//Web Translation
    };

