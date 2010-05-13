#include <iostream>
#include <string>


#include "buffio.h"
#include "tidy.h"



int Tidy()
{
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

    if(tidyOptSetBool(tdoc, TidyUpperCaseTags, yes) != yes)
        return -1;

    if(tidyOptSetBool(tdoc, TidyHtmlOut, yes) != yes)
        return -1;
    if(tidySetCharEncoding(tdoc, "gb18030") != 0)
        return -1;
    
//    if(tidyOptSetBool(tdoc, TidyShowWarnings, no) != yes)
//        return -1;
//    if(tidyOptSetInt(tdoc, TidyShowErrors, 0) != yes)
//        return -1;
    if(tidyOptSetBool(tdoc, TidyForceOutput, yes) != yes)
        return -1;
    
    if(tidyParseFile(tdoc, "c:\\test3.html") >= 0)
    {
        TidyBuffer errbuf = {0};
        tidySetErrorBuffer( tdoc, &errbuf );      // Capture diagnostics
        if(tidyCleanAndRepair(tdoc) >= 0)
        {
            tidyRunDiagnostics( tdoc );               // Kvetch

            TidyBuffer outbuf = { 0 };
            if(tidySaveBuffer(tdoc, &outbuf) >= 0)
            {
                std::cout << "OUTPUT--->\n" << outbuf.bp << std::endl;
                std::string s = (const char*)outbuf.bp;
                std::wstring wstr(s.begin(), s.end());
                std::wcout << wstr << std::endl;

                //int wsz = MultiByteToWideChar(codepage, 0, (const char*)outbuf.bp, outbuf.size, NULL, 0);
                //wchar_t* wbuf = new wchar_t[wsz + 1];
                //wsz = MultiByteToWideChar(codepage, 0, (const char*)outbuf.bp, outbuf.size, wbuf, wsz);
                //output = wbuf;
                //delete [] wbuf;
                //ret = 0;
            }
            tidyBufFree(&outbuf);
        }
        std::cout << "ERROR->\n" << errbuf.bp << std::endl;
        tidyBufFree(&errbuf);
    }

//    delete [] buf;

    tidyRelease(tdoc);

    return ret;
}

int main()
{
    Tidy();

    return 0;
}