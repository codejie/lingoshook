/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/
#ifndef __HTMLOPTIMUMOBJECT_H__
#define __HTMLOPTIMUMOBJECT_H__

#include <string>

#include "TinyHtmlParser.h"

class CHtmlOptimumObject
{
public:
	CHtmlOptimumObject();
	virtual ~CHtmlOptimumObject();

	static int Clear(const std::wstring& input, std::wstring& output, const TinyHtmlParser::CDocumentOutputObject::TKeyMap& mapKey);

};


#endif
