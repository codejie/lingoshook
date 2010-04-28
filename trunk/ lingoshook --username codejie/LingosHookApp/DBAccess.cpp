/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

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
