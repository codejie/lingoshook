/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/
#ifndef __HTMLTIDYOBJECT_H__
#define __HTMLTIDYOBJECT_H__

#include <string>

class CHtmlTidyObject
{
public:
    CHtmlTidyObject() {}
    virtual ~CHtmlTidyObject() {}

    static int Tidy(const std::wstring& input, std::wstring& output);
};

#endif
