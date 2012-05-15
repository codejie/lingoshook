
#include "HtmlOptimumObject.h"

int CHtmlOptimumObject::Clear(const std::wstring& input, std::wstring& output, const TinyHtmlParser::CDocumentOutputObject::TKeyMap& mapKey)
{
	TinyHtmlParser::CDocumentObject doc;

    try
    {
        if(doc.Load(input, true, true) != 0)
        {
            return -1;
        }
    }
    catch(TinyHtmlParser::CExceptionObject& e)
    {
        return -1;
    }

	wxString ret = wxEmptyString;
	if(TinyHtmlParser::CDocumentOutputObject::Rewrite(doc, ret, &mapKey) != 0)
		return -1;

	output = ret.c_str();
	return 0;
}