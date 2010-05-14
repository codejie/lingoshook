/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/
#include <Windows.h>

#include <iostream>

#include "tidy.h"
#include "buffio.h"

#include "HtmlTidyObject.h"

int CHtmlTidyObject::Tidy(const std::wstring &input, std::wstring &output)
{
    int codepage = CP_UTF8;//54936;//CP_UTF8;

    int ret = -1;

    TidyDoc tdoc = tidyCreate();
    if(tidyOptSetBool(tdoc, TidyMark, no) != yes)
        return -1;
    if(tidyOptSetInt(tdoc, TidyDoctypeMode, TidyDoctypeOmit) != yes)
        return -1;
    if(tidyOptSetBool(tdoc, TidyHideComments, yes) != yes)
        return -1;
    if(tidyOptSetInt(tdoc, TidyWrapLen, 0) != yes)
        return -1;
    //if(tidyOptSetBool(tdoc, TidyMakeClean, yes) != yes)//css
    //    return -1;
  
    if(tidyOptSetBool(tdoc, TidyUpperCaseTags, yes) != yes)
        return -1;
    if(tidyOptSetBool(tdoc, TidyHtmlOut, yes) != yes)
        return -1;
    if(tidySetCharEncoding(tdoc, "raw") != 0)
        return -1;
    
    //if(tidyOptSetBool(tdoc, TidyShowWarnings, no) != yes)
    //    return -1;
    //if(tidyOptSetInt(tdoc, TidyShowErrors, 0) != yes)
    //    return -1;

    if(tidyOptSetBool(tdoc, TidyForceOutput, yes) != yes)
        return -1;

    int sz = WideCharToMultiByte(codepage, 0, input.c_str(), input.size(), NULL, 0, NULL, NULL);
    if(sz == -1)
        return -1;
    char* buf = new char[sz + 1];
    sz = WideCharToMultiByte(codepage, 0, input.c_str(), input.size(), buf, sz, NULL, NULL);
    
    if(tidyParseString(tdoc, buf) >= 0)
    {
        //TidyBuffer errbuf = {0};
        //tidySetErrorBuffer( tdoc, &errbuf );
        if(tidyCleanAndRepair(tdoc) >= 0)
        {
            //tidyRunDiagnostics( tdoc ); 

            TidyBuffer outbuf = { 0 };
            if(tidySaveBuffer(tdoc, &outbuf) >= 0)
            {
                //std::cout << "OUTPUT->\n" << outbuf.bp << std::endl;
                int wsz = MultiByteToWideChar(codepage, 0, (const char*)outbuf.bp, outbuf.size, NULL, 0);
                wchar_t* wbuf = new wchar_t[wsz + 1];
                wsz = MultiByteToWideChar(codepage, 0, (const char*)outbuf.bp, outbuf.size, wbuf, wsz);
                wbuf[wsz] = L'\0';
                output = wbuf;
                delete [] wbuf;
                ret = 0;
            }
            tidyBufFree(&outbuf);
        }
        //std::cout << "ERROR->\n" << errbuf.bp << std::endl;
        //tidyBufFree(&errbuf);
    }

    delete [] buf;

    tidyRelease(tdoc);

    return ret;
}