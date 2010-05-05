#include <iostream>
#include <string>
#include <fstream>

#include "TinyHtmlParser.h"

using namespace TinyHtmlParser;

int Test()
{
    //std::wstring str = L"<hh></hh><HTML>\n<DOC DFDK=\"DD\"/><BODY> 1 2 3 </BODY><abc>  <def></def> </abc></HTML><TEST>567</TEST>";
//    std::string str = \
//"<TABLE onselectstart=\"return true\" id=dict_head_E1C27E806399D047822B6650194A3D32 cellSpacing=0 cellPadding=0 border=0>"
//"<TBODY>"
//"<TR>"
//"<TD style=\"BORDER-RIGHT: #92b0dd>"
//"<DIV onmouseup=> </DIV>"
//"<TD style=\"BORDER-border=0> </TD>"
//"<TD style=\"BORDER->中文</TD>"
//"<DIV style=\"OVERFL border=0></DIV></TD></TR></TBODY></TABLE>";

//    std::wstring str = \
//L"<DIV id=main dir=ltr style=\"FONT-SIZE: 9pt; FONT-FAMILY: Tahoma, Arial; HEIGHT: 100%\">"
//L"<DIV id=main_info style=\"DISPLAY: none\">&nbsp;</DIV>"
//L"<DIV id=main_wnd>"
//L"<DIV id=lingoes_dictarea></DIV>"
//L"<DIV id=dict_E1C27E806399D047822B6650194A3D32 style=\"PADDING-RIGHT: 10px; PADDING-LEFT: 10px; FONT-SIZE: 10.5pt; PADDING-BOTTOM: 0px; WIDTH: 100%; LINE-HEIGHT: 1.2em; PADDING-TOP: 10px; FONT-FAMILY: 'Tahoma'\" groupid=\"2\" dictid=\"E1C27E806399D047822B6650194A3D32\">"
//L"<TABLE onselectstart=\"return true\" id=dict_head_E1C27E806399D047822B6650194A3D32 cellSpacing=0 cellPadding=0 border=0>"
//L"<TBODY>"
//L"<TR>"
//L"<TD style=\"BORDER-RIGHT: #92b0dd 1px solid; BORDER-TOP: #92b0dd 1px solid; FONT-SIZE: 9pt; BACKGROUND: #cfddf0; BORDER-LEFT: #92b0dd 1px solid; COLOR: #000080; LINE-HEIGHT: 1em; BORDER-BOTTOM: #92b0dd 1px solid; FONT-FAMILY: ''\" noWrap>"
//L"<DIV onmouseup=\"this.className='btn2_mouse_up'\" class=btn2_mouse_out onmousedown=\"this.className='btn2_mouse_down'\" id=dict_title_E1C27E806399D047822B6650194A3D32 onmouseover=\"this.className='btn2_mouse_over'\" title=\"Dictionary Menu\" style=\"MARGIN: 0px 3px 1px 0px\" onclick=\"window.navigate('app://dictmenu/E1C27E806399D047822B6650194A3D32-2')\" onmouseout=\"this.className='btn2_mouse_out'\"><IMG height=16 hspace=3 src=\"file:///D:/Profiles/grp436/Local%20Settings/Application%20Data/Lingoes/Translator/temp/dict/E1C27E806399D047822B6650194A3D32/icon.png\" width=16 align=absMiddle border=0><SPAN style=\"PADDING-RIGHT: 4px; PADDING-LEFT: 2px; PADDING-BOTTOM: 0px; PADDING-TOP: 0px\">Vicon English-Chinese(S) Dictionary</SPAN><IMG height=4 src=\"file:///C:/Program%20Files/Lingoes/Translator2/dict/image/menu.png\" width=7 align=absMiddle border=0> </DIV></TD>"
//L"<TD style=\"BORDER-BOTTOM: #92b0dd 1px solid\"><IMG style=\"DISPLAY: none\" height=8 hspace=5 src=\"file:///C:/Program%20Files/Lingoes/Translator2/dict/image/sst.png\" width=11 align=absMiddle border=0> </TD>"
//L"<TD style=\"BORDER-BOTTOM: #92b0dd 1px solid\" align=right width=\"100%\">"
//L"<DIV style=\"OVERFLOW: hidden; WIDTH: 10px; CURSOR: hand; MARGIN-RIGHT: 2px; HEIGHT: 10px\"><IMG onmouseup=\"this.style.marginLeft = '-10px'\" onmousedown=\"this.style.marginLeft = '-10px'\" id=dict_show_E1C27E806399D047822B6650194A3D32 onmouseover=\"this.style.marginLeft = '-10px'\" title=\"Minimize Result\" style=\"MARGIN-TOP: 0px; MARGIN-LEFT: 0px\" onclick=\"window.navigate('app://hidemeaning/E1C27E806399D047822B6650194A3D32-2') ; this.style.marginTop = (parseInt(this.style.marginTop) == 0) ? '-10px' : '0px' ;\" onmouseout=\"this.style.marginLeft = '0px'\" height=20 src=\"file:///C:/Program%20Files/Lingoes/Translator2/dict/image/expand.png\" width=20 border=0></DIV></TD></TR></TBODY></TABLE>"
//L"<DIV id=dict_body_E1C27E806399D047822B6650194A3D32>"
//L"<DIV id=dict_gls_E1C27E806399D047822B6650194A3D32>"
//L"<DIV style=\"MARGIN: 5px 0px\">"
//L"<DIV style=\"WIDTH: 100%\">"
//L"<DIV style=\"FLOAT: left; LINE-HEIGHT: normal\"><IMG height=11 src=\"file:///C:/Program%20Files/Lingoes/Translator2/dict/image/entry_p.png\" width=10 align=absMiddle border=0>&nbsp;</DIV>"
//L"<DIV style=\"OVERFLOW-X: hidden; WIDTH: 100%\">"
//L"<DIV style=\"MARGIN: 0px 0px 5px; COLOR: #808080; LINE-HEIGHT: normal\"><SPAN style=\"FONT-SIZE: 10.5pt; COLOR: #000000; LINE-HEIGHT: normal\"><B>what</B></SPAN> &nbsp;<SPAN style=\"FONT-SIZE: 10.5pt; LINE-HEIGHT: normal; FONT-FAMILY: 'Lingoes Unicode'\">[<FONT color=#009900>hwɑt ,hw?t /w?t</FONT>]</SPAN></DIV>"
//L"<DIV style=\"MARGIN: 0px 0px 5px\">"
//L"<DIV style=\"MARGIN: 4px 0px\"><FONT color=#c00000>adj.</FONT> &nbsp;什么</DIV></DIV>"
//L"<DIV style=\"MARGIN: 0px 0px 5px\">"
//L"<DIV style=\"MARGIN: 4px 0px\"><FONT color=#c00000>adv.</FONT> &nbsp;到什么程度, 在哪一方面</DIV></DIV>"
//L"<DIV style=\"MARGIN: 0px 0px 5px\">"
//L"<DIV style=\"MARGIN: 4px 0px\"><FONT color=#c00000>interj.</FONT> &nbsp;怎么</DIV></DIV></DIV></DIV></DIV></DIV></DIV></DIV></DIV></DIV>";

    //std::wstring str = \
    //    L"<1 hello><1-1>[<1-1-1/>]</1-1><1-2/><1-3/></1><2/><3><3-1><3-1-1/></3-1></3>";

//    std::wstring str = \
//L"<DIV id=dict_body_0D9A782C04A3704F8668D0093676117B>"
//L"<DIV id=dict_gls_0D9A782C04A3704F8668D0093676117B>"
//L"<DIV style=\"MARGIN: 5px 0px\">"
//L"<DIV style=\"WIDTH: 100%\">"
//L"<DIV style=\"FLOAT: left; LINE-HEIGHT: normal\"><IMG height=11 src=\"file:///C:/Program%20Files/Lingoes/Translator2/dict/image/entry_p.png\" width=10 align=absMiddle border=0>&nbsp;</DIV>"
//L"<DIV style=\"OVERFLOW-X: hidden; WIDTH: 100%\">"
//L"<DIV style=\"MARGIN: 0px 0px 5px; COLOR: #808080; LINE-HEIGHT: normal\"><SPAN style=\"FONT-SIZE: 10.5pt; COLOR: #000000; LINE-HEIGHT: normal\"><B>镶嵌</B></SPAN></DIV>"
//L"<DIV style=\"MARGIN: 0px 0px 5px\">"
//L"<DIV style=\"MARGIN: 4px 0px\"><FONT color=#c00000>v.</FONT> &nbsp;<FONT color=#009900>(<I>Trad</I><I>, Pinyin</I>=xiang1 qian4)</FONT> inlay, implant, decorate, multiply, drive in, set in something, entrench, hatch, insert, mount, mosaic, embed</DIV></DIV></DIV></DIV></DIV></DIV></DIV>";

//    std::wstring str = \
//L"<DIV style=\"MARGIN: 0px 0px 5px\">"
//L"<DIV style=\"MARGIN: 4px 0px\"><FONT color=#c00000>v.</FONT> &nbsp;<FONT color=#009900>(<I>Trad</I>555<I>, Pinyin</I>=xiang1 qian4)</FONT> inlay, implant, decorate, multiply, drive in, set in something, entrench, hatch, insert, mount, mosaic, embed</DIV></DIV>";

//    std::wstring str = \
//L"<DIV id=main dir=ltr style=\"FONT-SIZE: 9pt; FONT-FAMILY: Tahoma, Arial; HEIGHT: 100%; BACKGROUND-COLOR: #ffffff\">"
//L"<DIV id=lingoes_dictarea></DIV>"
//L"<DIV id=dict_E1C27E806399D047822B6650194A3D32 style=\"PADDING-RIGHT: 10px; PADDING-LEFT: 10px; FONT-SIZE: 10.5pt; PADDING-BOTTOM: 0px; WIDTH: 100%; LINE-HEIGHT: 1.2em; PADDING-TOP: 10px; FONT-FAMILY: 'Tahoma'\" groupid=\"4\" dictid=\"E1C27E806399D047822B6650194A3D32\">"
//L"<TABLE onselectstart=\"return true\" id=dict_head_E1C27E806399D047822B6650194A3D32 cellSpacing=0 cellPadding=0 border=0>"
//L"<TBODY>"
//L"<TR>"
//L"<TD style=\"BORDER-RIGHT: #92b0dd 1px solid; BORDER-TOP: #92b0dd 1px solid; FONT-SIZE: 9pt; BACKGROUND: #cfddf0; BORDER-LEFT: #92b0dd 1px solid; COLOR: #000080; LINE-HEIGHT: 1em; BORDER-BOTTOM: #92b0dd 1px solid; FONT-FAMILY: ''\" noWrap>"
//L"<DIV class=btn2_mouse_out id=dict_title_E1C27E806399D047822B6650194A3D32 style=\"MARGIN: 0px 3px 1px 0px; CURSOR: default\"><IMG height=16 hspace=3 src=\"file:///D:/Profiles/grp436/Local%20Settings/Application%20Data/Lingoes/Translator/temp/dict/E1C27E806399D047822B6650194A3D32/icon.png\" width=16 align=absMiddle border=0><SPAN style=\"PADDING-RIGHT: 4px; PADDING-LEFT: 2px; PADDING-BOTTOM: 0px; PADDING-TOP: 0px\">Vicon English-Chinese(S) Dictionary</SPAN> </DIV></TD>"
//L"<TD style=\"BORDER-BOTTOM: #92b0dd 1px solid\"><IMG style=\"DISPLAY: none\" height=8 hspace=5 src=\"file:///C:/Program%20Files/Lingoes/Translator2/dict/image/sst.png\" width=11 align=absMiddle border=0> </TD>"
//L"<TD style=\"BORDER-BOTTOM: #92b0dd 1px solid\" align=right width=\"100%\">"
//L"<DIV style=\"OVERFLOW: hidden; WIDTH: 10px; CURSOR: hand; MARGIN-RIGHT: 2px; HEIGHT: 10px\"><IMG onmouseup=\"this.style.marginLeft = '-10px'\" onmousedown=\"this.style.marginLeft = '-10px'\" id=dict_show_E1C27E806399D047822B6650194A3D32 onmouseover=\"this.style.marginLeft = '-10px'\" title=\"Minimize Result\" style=\"MARGIN-TOP: 0px; MARGIN-LEFT: 0px\" onclick=\"window.navigate('app://hidemeaning/E1C27E806399D047822B6650194A3D32-4') ; this.style.marginTop = (parseInt(this.style.marginTop) == 0) ? '-10px' : '0px' ;\" onmouseout=\"this.style.marginLeft = '0px'\" height=20 src=\"file:///C:/Program%20Files/Lingoes/Translator2/dict/image/expand.png\" width=20 border=0></DIV></TD></TR></TBODY></TABLE>"
//L"<DIV id=dict_body_E1C27E806399D047822B6650194A3D32>"
//L"<DIV id=dict_gls_E1C27E806399D047822B6650194A3D32>"
//L"<DIV style=\"MARGIN: 5px 0px\">"
//L"<DIV style=\"WIDTH: 100%\">"
//L"<DIV style=\"FLOAT: left; LINE-HEIGHT: normal\"><IMG height=11 src=\"file:///C:/Program%20Files/Lingoes/Translator2/dict/image/entry_p.png\" width=10 align=absMiddle border=0>&nbsp;</DIV>"
//L"<DIV style=\"OVERFLOW-X: hidden; WIDTH: 100%\">"
//L"<DIV style=\"MARGIN: 0px 0px 5px; COLOR: #808080; LINE-HEIGHT: normal\"><SPAN style=\"FONT-SIZE: 10.5pt; COLOR: #000000; LINE-HEIGHT: normal\"><B>HTML (HyperText Markup Language)</B></SPAN> &nbsp;</DIV>"
//L"<DIV style=\"MARGIN: 0px 0px 5px\">"
//L"<DIV style=\"MARGIN: 4px 0px\">html, 超文字标记语言, 为因特网书写网页的语言 (计算机用语)</DIV></DIV></DIV></DIV></DIV></DIV></DIV></DIV>"
//L"<DIV style=\"FONT-SIZE: 0px; LINE-HEIGHT: 0em; HEIGHT: 10px\"></DIV></DIV>";

//    std::wstring str = \
//L"<DIV style=\"MARGIN: 0px 0px 5px\"><DIV style=\"MARGIN: 4px 0px\">超文字标记语言, 为因特网书写网页的语言 (计算机用语)</DIV></DIV>";

    //std::wstring str = L"<HTML>=====<SPARATOR>s-value</SPARATOR>++++++<BODY A=\"AA\">{123}<BR>[456]</BODY><IMG>*****</H";

    std::wstring tmp, str;
    std::wifstream ifs("C:\\test.html");
    while(!ifs.eof())
    {
        std::getline(ifs, tmp);
        str += tmp;
    }
    //ifs >> str;
    ifs.close();

    CDocumentObject doc;
    try
    {
        doc.Load(str, false);
    }
    catch(const CExceptionObject& e)
    {
        return -1;
    }

//    setlocale(LC_ALL, "chs");
    doc.Show(std::wcout);

    std::cout << "\n----------------" << std::endl;

//    const CElementObject* pe = doc.FindFirstElement(L"DIV");
//    
//    while(pe != NULL)
//    {
//        const CAttributeObject* pa = doc.FindAttribute(pe, L"style");
//        if(pa != NULL)
//        {
//            if(pa->value == L"\"MARGIN: 4px 0px\"")
//                pa->Show(std::wcout);
////            pe->Show(std::wcout);
//        }
//        //pe->Show(std::cout);
//
//        //CDocumentObject::TElementStack tmp;
//
//        //const CElementObject* p = doc.FindFirstElement(pe, "DIV", tmp);
//        //while(p != NULL)
//        //{
//        //    p->Show(std::cout);
//        //    p = doc.FindNextElement(pe, "DIV", tmp);
//        //}
//        pe = doc.FindNextElement();
//    }

    return 0;
}


int main()
{
	Test();
	return 0;
}