
#include "Consts.h"
#include "ResHelper.h"

const wxString ResHelper::MakeHomepageString()
{
	return wxString::Format( \
			wxT("<html>") \
			wxT("<head><STYLE>A:link{TEXT-DECORATION:none;}</STYLE></head>") \
            wxT("<body><div style=\"text-align: center;\"><font style=\"font-size: 30pt;\">LingosHook</font>&nbsp;&nbsp;&nbsp;&nbsp;version&nbsp;") \
			wxT("%s") \
            wxT("</div><br/>") \
			wxT("<ul><font sytle=\"font-size: 28pt;\">") \
            wxT("<li><a href=\"http://www.cppblog.com/codejie/archive/2010/07/23/108996.html\">LingosHook Download Link</a></li>") \
            wxT("<li><a href=\"http://lingoshook.googlecode.com\">LingosHook Source Link</a></li>") \
            wxT("<li><a href=\"http://www.cppblog.com/codejie/category/13873.html\">LingosHook Blog Link</a></li>") \
            wxT("<li><a href=\"http://www.cppblog.com/codejie/archive/2011/03/17/142091.html\">LingosHook Donators Link</a></li>") \
            wxT("<li><a href=\"http://shop64703519.taobao.com\">LingosHook Donation Link</a></li>") \
            wxT("</font></ul>") \
            wxT("<br />") \
            wxT("<div style=\"text-align: right;\"><font style=\"font-size: 14pt;\"><a href=\"mailto:codejie@gmail.com\">codejie</a><br/>") \
			wxT("%s") \
            wxT("</div></font></body></html>") \
			, APP_VERSION, DATE_RELEASE);
}