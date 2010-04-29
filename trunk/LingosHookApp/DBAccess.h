/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __DBACCESS_H__
#define __DBACCESS_H__

#include <vector>

#include "wx/wx.h"
#include "wx/arrstr.h"

#include "wx/wxsqlite3.h"

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


#endif
