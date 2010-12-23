
#include <Windows.h>

#include <iostream>
#include <string>

#include "buffio.h"
//#include "streamio.h"
#include "tidy.h"

std::string WStringToString(const std::wstring& s)
{
    std::string temp(s.length(), ' ');
    std::copy(s.begin(), s.end(), temp.begin());
    return temp;
}

std::wstring StringToWString(const std::string& s)
{
    std::wstring temp(s.length(),L' ');
    std::copy(s.begin(), s.end(), temp.begin());
    return temp;
}

int main()
{
    //int wsz = 100;
    //wchar_t* wbuf = new wchar_t(wsz + 1);
    //wbuf[wsz] = L'0';
   
    ////memset(wbuf, 0, sizeof(wchar_t)*  (wsz + 1));    
    ////wsz = MultiByteToWideChar(54936, 0, (const char*)output.bp, output.size, wbuf, wsz);
    ////wbuf[0] = L'A';
    ////wbuf[1] = L'B';

    //std::wstring o = wbuf;//, wsz);// = std::wstring(wbuf);// (wsz, L' ');



    const std::wstring str = L"<title>基础英汉词典中文Foo</title><p>中文<!--a class=\"sp add-fav\" href=\"#\" title=\"中文\"></a-->";

    char *buf = new char[100];
    int size = WideCharToMultiByte(CP_ACP/* 54936*/, 0, str.c_str(), -1, buf, 100, NULL, NULL );
    //int size = wcstombs(buf, str.c_str(), 100);

    //std::string s(str.begin(), str.end());//  = (const char*)str.c_str();// WStringToString(str);
    //s.assign(str.begin(), str.end());

    const char* input = buf;//s.c_str(); //"<title>Foo</title><p>Foo!";
    TidyBuffer output = {0};
    TidyBuffer errbuf = {0};
    int rc = -1;
    Bool ok;

    TidyDoc tdoc = tidyCreate();                     // Initialize "document"
    //printf( "Tidying:\t%s\n", input );

    //ok = tidyOptSetBool( tdoc, TidyHtmlOut, no );  // Convert to XHTML
    //ok = tidyOptSetBool(tdoc, TidyUpperCaseTags, yes);
    ////ok = tidyOptSetBool(tdoc, TidyBodyOnly, yes);
    //ok = tidyOptSetBool(tdoc, TidyMark, no);
    ////ok = tidyOptSetBool(tdoc, TidyHideComments, yes);
    //ok = tidyOptSetInt(tdoc, TidyDoctypeMode, TidyDoctypeOmit);
    //ok = tidyOptSetInt(tdoc, TidyWrapLen, 0);
    ////ok = tidyOptSetBool( tdoc, TidyEncloseBodyText, yes );  
    //ok = tidyOptSetBool(tdoc, TidyShowWarnings, no);
    ////ok = tidyOptSetBool(tdoc, TidyOutputBOM, yes);
    //ok = tidyOptSetInt(tdoc, TidyCharEncoding, 0);
    //ok = tidyOptSetBool(tdoc, TidyIndentContent, yes);
    //if ( ok )
    //    rc = tidySetErrorBuffer( tdoc, &errbuf );      // Capture diagnostics
    //if ( rc >= 0 )

//bare:no
//clean:no
//fix-bad-comments:yes
//fix-backslash:yes
//indent:yes
//indent-attributes:no
//indent-spaces:2
//char-encoding:raw
//replace-color:yes
//wrap:80
//wrap-asp:yes
//wrap-jste:yes
//wrap-php:yes
//write-back:yes
    ok = tidyOptSetBool(tdoc, TidyMark, no);
    ok = tidyOptSetInt(tdoc, TidyDoctypeMode, TidyDoctypeOmit);
    ok = tidyOptSetInt(tdoc, TidyWrapLen, 0);

    //ok = tidyOptSetBool(tdoc, TidyMakeBare, no);
    //ok = tidyOptSetBool(tdoc, TidyMakeClean, no);
    ok = tidyOptSetBool(tdoc, TidyFixBackslash, yes);
    //ok = tidyOptSetInt(tdoc, TidyCharEncoding, 0);
    ok = tidyOptSetBool( tdoc, TidyHtmlOut, yes );  // Convert to XHTML
    ok = tidyOptSetBool(tdoc, TidyOutputBOM, yes);
    rc = tidySetCharEncoding(tdoc, "raw");


        rc = tidyParseString( tdoc, input );           // Parse the input
    if ( rc >= 0 )
        rc = tidyCleanAndRepair( tdoc );               // Tidy it up!
    //if ( rc >= 0 )
    //    rc = tidyRunDiagnostics( tdoc );               // Kvetch
    //if ( rc > 1 )                                    // If error, force output.
    //    rc = ( tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1 );
    if ( rc >= 0 )
        rc = tidySaveBuffer( tdoc, &output );          // Pretty Print

    if ( rc >= 0 )
    {
        //if ( rc > 0 )
        //    printf( "\nDiagnostics:\n\n%s", errbuf.bp );
        printf( "\nAnd here is the result:\n\n%s", output.bp );
    }
    else
        printf( "A severe error (%d) occurred.\n", rc );

    int wsz = MultiByteToWideChar(CP_ACP, 0, (const char*)output.bp, -1, NULL, 0);
    wchar_t* wbuf = new wchar_t[wsz + 1];//(200);
//    memset(wbuf, 0, sizeof(wchar_t)*  (wsz + 1));    
    wsz = MultiByteToWideChar(CP_ACP, 0, (const char*)output.bp, -1, wbuf, wsz);
    //wbuf[wsz] = L'\0';

    //setlocale(LC_ALL, "chs");
    std::wstring o = wbuf;//StringToWString((const char*)output.bp);// (const char*)output.bp;// wbuf;// (wsz, L' ');
    //o.append(wbuf, 200);
    
    //out = wbuf;
    //out.assign(wbuf, wsz);//, wsz);// = StringToWString((const char*)output.bp);

    const char * dest = (const char*)output.bp;

    printf("\ndesc-->\n%s\n", dest);

    tidyBufFree( &output );
    tidyBufFree( &errbuf );
    tidyRelease( tdoc );


    //setlocale(LC_ALL, "chs");

    //std::wcout << out << std::endl;

    return rc;    
}