#ifndef __RESULTPROCOBJECT_H__
#define __RESULTPROCOBJECT_H__

#include <memory>

#include "wx/wx.h"

class LingosHookFrame;

class CDBAccess;
class CDictResultParser;

class CResultProcObject
{
public:
	CResultProcObject(LingosHookFrame* frame, CDBAccess& db);
	virtual ~CResultProcObject();

	void TextProc(const wxString& str);
	void HTMLProc(const wxString& str);
private:
	LingosHookFrame* _objFrame;
	CDBAccess& _objDB;
	std::auto_ptr<CDictResultParser> _objDictParser;
private:
	wxString _strHTML;
};

#endif

