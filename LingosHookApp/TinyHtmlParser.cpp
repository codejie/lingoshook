/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include <sstream>

#include "TinyHtmlParser.h"

namespace TinyHtmlParser
{

CExceptionObject::CExceptionObject(ExceptionNumber type, const std::wstring &info)
: _type(type)
, _info(info)
{
}

CExceptionObject::CExceptionObject(const CExceptionObject &right)
: _type(right._type)
, _info(right._info)
{
}

#define THROW_EXCEPTION(type, info) \
{ \
    std::wostringstream oss; \
    oss << info; \
    throw CExceptionObject(type, oss.str()); \
}
///////////////

void CAttributeObject::Show(std::wostream& os) const
{
    os << "    attr : " << this->attr << " -- value = " << this->value << std::endl;
}

CElementObject::CElementObject()
: type(ET_UNKNOWN)
, level(0)
, parent(NULL)
, child(NULL)
, sibling(NULL)
, attrib(NULL)
{
}

CElementObject::~CElementObject()
{
    FreeAnalyseAttribute();
}

int CElementObject::Analyse()
{
    std::wstring str = tag;

    std::wstring::size_type pos = str.find(L" ");
    if(pos != std::wstring::npos)
    {
        tag = str.substr(0, pos);

        str = str.substr(pos + 1);
        if(AnalyseAttribute(str) != 0)
        {
            return -1;
        }
    }
    if(type == ET_ELEMENT)
    {
        if(AnalyseValue() != 0)
            return -1;
    }
    return 0;
}

int CElementObject::AnalyseAttribute(const std::wstring& attr)
{
    if(attr.size() == 0)
        return 0;
    
    std::wstring::size_type as = 0, ae = 0, vs = 0, ve = 0;
    std::wstring::size_type pos = 0;
    bool v = false;
    bool semicolon = false;
    while(pos < attr.size())
    {
        if(attr[pos] == L'=')
        {
			if(!semicolon)
			{
				ae = pos - 1;
				vs = pos + 1;

				v = true;
			}
        }
        else if(attr[pos] == L' ')
        {
            if(!semicolon)
            {
                if(v)
                {
                    ve = pos - 1;
                    if(MakeAttribute(attr.substr(as, ae - as + 1), attr.substr(vs, ve - vs + 1)) != 0)
                        return -1; 
                }
                else
                {
                    ae = pos - 1;
                    if(MakeAttribute(attr.substr(as, ae - as + 1), L"") != 0)
                        return -1; 
                }
                as = pos + 1;
                vs = pos + 1;
                v = false;
            }
        }
        else if(attr[pos] == L'"')
        {
            semicolon = !semicolon;
        }
        ++ pos;
    }

    if(v)
    {
        ve = attr.size() - 1;
        if(MakeAttribute(attr.substr(as, ae - as + 1), attr.substr(vs, ve - vs + 1)) != 0)
            return -1;
    }
    else
    {
        ae = attr.size() - 1;
        if(MakeAttribute(attr.substr(as, ae - as + 1), L"") != 0)
            return -1;
    }
    return 0;
}

int CElementObject::MakeAttribute(const std::wstring &attr, const std::wstring& value)
{
    std::auto_ptr<CAttributeObject> obj(new CAttributeObject(attr, value));//attr.substr(0, pos), attr.substr(pos)));
    
    if(attrib != NULL)
    {
        CAttributeObject* tmp = attrib;
        while(tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = obj.release();
    }
    else
    {
        attrib = obj.release();
    }
    return 0;
}


void CElementObject::FreeAnalyseAttribute()
{
    CAttributeObject* tmp = attrib;
    while(attrib != NULL)
    {
        tmp = attrib->next;
        delete attrib;
        attrib = tmp;
    }

}

int CElementObject::AnalyseValue()
{
/*
    std::wstring::size_type pos = this->value.find(L"&nbsp;");
    while(pos != std::wstring::npos)
    {
        this->value.replace(pos, 6, L" ");
        pos = this->value.find(L"&nbsp;", pos + 1);
    }
*/
    return 0;
}

const CAttributeObject* CElementObject::FindAttribute(const std::wstring& attr) const
{
    const CAttributeObject* pa = this->attrib;
    while(pa != NULL)
    {
        if(pa->attr == attr)
            return pa;
        pa = pa->next;
    }
    return pa;
}

void CElementObject::Show(std::wostream& os) const
{
    os << "[" << this->level << "-" << this->type << "]" << "Tag:";
    if(this->type != ET_VALUE)
    {
        os << this->tag << " - " << this->start << "," << this->end << " -- value = " << this->value;
    }
    else// if(this->type == ET_VALUE)
    {
        os << "<VALUE> - " << this->start << "," << this->end << " -- value = " << this->value;
    }
    os << std::endl;

    const CAttributeObject* attr = this->attrib;
    while(attr != NULL)
    {
        attr->Show(os);
        attr = attr->next;
    }
    os << std::endl;
}
//

int CParserData::GetTitle(const std::wstring& html)
{
    if(type == DT_TAG || type == DT_SPECIAL || type == DT_END_SPECIAL)
    {
        title = html.substr(range.first + 1, range.second - range.first - 1);
        std::wstring::size_type pos = title.find(L" ");
        if(pos != std::wstring::npos)
            title = title.substr(0, pos);
    }
    else if(type == DT_END)
    {
        title = html.substr(range.first + 2, range.second - range.first - 2);
    }
    else if(type == DT_VALUE)
    {
        title = html.substr(range.first, range.second - range.first + 1);;
    }
    else
    {
        return -1;
    }
    return 0;
}

void CParserData::Show(std::wostream &os) const
{
    os << "\nType = " << this->type;
    os << "\nTag Start = " << this->range.first << " - End = " << this->range.second;

    for(TValueVector::const_iterator it = this->value.begin(); it != this->value.end(); ++ it)
    {
        os << "\nValue Start = " << it->first << " - End = " << it->second;
    }
    os << std::endl;
}

void CParserData::Show(std::wostream& os, const std::wstring& html) const
{
    os << "\nType = " << this->type;
    os << "\nTag = " << "[" << this->range.first << "," << this->range.second << "]" << html.substr(this->range.first, this->range.second - this->range.first + 1);
    for(TValueVector::const_iterator it = this->value.begin(); it != this->value.end(); ++ it)
    {
        os << "\nValue = " << "[" << it->first << "," << it->second << "]" << html.substr(it->first, it->second - it->first + 1);
    }
    os << std::endl;
}
//
CDocumentObject::CDocumentObject()
: _root(NULL)
, _bIsMistake(false)
{
}

CDocumentObject::~CDocumentObject()
{
    if(_root != NULL)
        FreeElement(_root);
}

int CDocumentObject::Load(const std::wstring &str, bool strict, bool valueseparate)
{
    //std::wstring html;
    //if(PreProcess(str, html, strict) != 0)
    //    return -1;
    TNodeQueue que;
    if(PreParser(str, que, strict, valueseparate) != 0)
        return -1;
    if(Parser(str, que, strict) != 0)
        return -1;
    return 0;
}

int CDocumentObject::PreProcess(const std::wstring& str, std::wstring& html, bool strict)
{
    //html = str;
    bool tag = false;
    for(std::wstring::const_iterator it = str.begin(); it != str.end(); ++ it)
    {
        //if((*it) == L'\r' || (*it) == L'\n')
        //    continue;

        if(*it == TAG_LT)
        {
            if(tag == true)
            {
                THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"Double '<'.");
                return -1;
            }
            tag = true;
        }
        else if(*it == TAG_GT)
        {
            if(tag == false)
            {
                THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"Miss '<' before '>'.");
                return -1;
            }
            tag = false;
        }
        else
        {
            if(tag == false)
            {
                //if(isspace((unsigned char)*it) != 0)
                //    continue;
                if((*it) == L'\r' || (*it) == L'\n')
                    continue;
            }
        }
        html += *it;
    }

    return 0;
}

int CDocumentObject::PreParser(const std::wstring& html, CDocumentObject::TNodeQueue& que, bool strict, bool valueseparate)
{
    std::wstring::size_type pos = 0;

    if(html.size() == 0)
    {
        THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"HTML is empty.");
        return -1;
    }
    if(html[pos] != TAG_LT)
    {
        THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"First character of HTML is NOT '<' - pos : " << pos);
        return -1;
    }

    TDataStack datastack;
    CParserData data;
    size_t start = 0;

    while(pos < html.size())
    {
        if(html[pos] == TAG_LT)
        {
            if(pos > start)
            {
                if(valueseparate)
                {
                    if(PushValueData(start, pos -1, datastack, que) != 0)
                       return -1;
                }
                else
                {
                    if(PushValueData(start, pos -1, datastack) != 0)
                       return -1;
                }
            }

            if(PreParserLT(html, pos, data) != 0)
            {
                if(strict)
                {
                    THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"Tags do NOT match each other.");
                    return -1;
                }
                else
                {
                    start = pos;
                    break;
                }
            }

            if(PushTagData(html, data, datastack, que) != 0)
                return -1;
            
            ++ pos;
            start = pos;
        }
        //else if(html[pos] == TAG_GT || html[pos] == TAG_SLASH)
        //{
        //    return -1;
        //}
        else
        {
            ++ pos;
        }
    }

    if(datastack.size() > 0)
    {
        if(strict)
        {
            THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"Tags do NOT match each other.");
            return -1;
        }

        if(pos > start)
        {
            if(PushValueData(/*CParserData::DT_BROKEN, */start, pos, datastack) != 0)
                return -1;
        }

        if(PreParserBroken(html, datastack, que) != 0)
            return -1;
    }

    return 0;
}

int CDocumentObject::Parser(const std::wstring& html, CDocumentObject::TNodeQueue& que, bool strict)
{
    CElementObject *pe = NULL,  *pp = NULL, *ps = NULL;
    size_t level = 0;
    while(que.size()> 0)
    {
        const TNodeData &node = que.front();
        if(level < node.level)
        {
            pp = pe;
            ps = NULL;
        }
        else if(level == node.level)
        {
            ps = pe;
        }
        else//>
        {
            ps = pe;
            pp = pe->parent;
            int t = level - node.level;
            while(t > 0/* && pp != NULL && ps != NULL*/)
            {
                ps = ps->parent;
                pp = pp->parent;
                -- t;
            }
        }        
        level = node.level;

        pe = MakeElement(html, node, pp, ps);

        if(pe == NULL)
            return -1;

        que.pop_front();
    }

    if(pp != NULL)
    {
        while(pp->parent != NULL)
            pp = pp->parent;
        _root = pp;
    }
    else
        _root = pe;

    return 0;
}

int CDocumentObject::PreParserLT(const std::wstring& html, std::wstring::size_type& pos, CParserData& data)
{
    if(pos == html.size() - 1)
    {
        //THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"'<' is the last character.");
        return -1;
    }
    
    data.range.first = pos;

    ++ pos;

    if(html[pos] != TAG_SLASH)
    {
       data.type = CParserData::DT_TAG;
    }
    else
    {
        data.type = CParserData::DT_END;
        ++ pos;
    }

   while(pos < html.size())
    {
        if(html[pos] == TAG_GT)
        {
            if(html[pos - 1] == TAG_SLASH)
            {
                data.type = CParserData::DT_END_SPECIAL;
            }

            data.range.second = pos;
            data.GetTitle(html);
     
            return 0;
        }
        else if(html[pos] == TAG_LT)
        {
            //THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"'<' follows '<'.");
            return -1;
        }

        ++ pos;
    }
    //THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"Miss '>' after '<'");
    return -1;
}

int CDocumentObject::PushValueData(/*CParserData::DataType type, */size_t start, size_t end, TDataStack& datastack) const
{
    if(datastack.size() == 0)
    {
        THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"Miss '<' before got value - pos : " << start);
        return -1;
    }

    CParserData& prev = datastack.top();
    prev.value.push_back(std::make_pair(start, end));

    return 0;
}

int CDocumentObject::PushValueData(size_t start, size_t end, TDataStack& datastack, CDocumentObject::TNodeQueue& nodeque) const
{
    if(datastack.size() == 0)
    {
        THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"Miss '<' before got value - pos : " << start);
        return -1;
    }

    size_t level = datastack.size();
    CParserData vd;
    vd.type = CParserData::DT_VALUE;
    vd.range.first = start;
    vd.range.second = end;
    nodeque.push_front(TNodeData(level, vd.range.first, vd.range.second, vd));

    return 0;
}

int CDocumentObject::PushTagData(const std::wstring& html, CParserData& data, CDocumentObject::TDataStack& datastack, CDocumentObject::TNodeQueue& nodeque) const
{
    //if(CheckSpecialTag(html, data) == 0)
    //{
    //    data.type = CParserData::DT_SPECIAL;
    //}

    if(data.type == CParserData::DT_TAG)
    {
        //if(datastack.size() > 0)
        //{
        //    CParserData& pre = datastack.top();
        //    if(pre.value.size() > 0)
        //    {
        //        size_t level = datastack.size();
        //        CParserData vd;
        //        vd.type = CParserData::DT_VALUE;
        //        vd.range.first = pre.value[0].first - 1;
        //        vd.range.second = pre.value[0].second;
        //        nodeque.push_front(TNodeData(level, vd.range.first, vd.range.second, vd));
        //        pre.value.clear();
        //    }
        //}
        datastack.push(data);
    }
    else if(data.type == CParserData::DT_END)
    {
        if(datastack.size() == 0)
        {
            THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"Miss start-tag before end-tag - pos : " << data.range.first);
            return -1;
        }

        while(datastack.size() > 0)
        {
            CParserData& tag = datastack.top();
            if(tag.title == data.title)//str == end)
            {
                size_t level = datastack.size() - 1;
                //if(nodeque.size() > 0)
                //{
                //    level = nodeque.begin()->level;
                //}
                nodeque.push_front(TNodeData(level, tag.range.first, data.range.second,  datastack.top()));//(std::make_pair(datastack.size() - 1, datastack.top()));
                datastack.pop();
                break;
            }
            else
            {
                if(IsSpecialTag(tag) == 0)
                {
                    tag.type = CParserData::DT_SPECIAL;
                    size_t level = datastack.size() - 1;

                    TNodeQueue::iterator it = nodeque.begin();
                    while(it != nodeque.end())
                    {
                        //if(it->data.type == CParserData::DT_SPECIAL)
                        //    it->level = level;
                        //else
                        //    break;
                        //-- it->level;
                        if(it->level > level)
                            -- it->level;// = level;
                        else
                            break;

                        ++ it;
                    }

                    size_t end = tag.range.second;
                    if(tag.value.size() > 0)
                    {
                        end = tag.value.begin()->second;
                    }
                    //nodeque.push_front(TNodeData(level, tag.range.first, end, tag));//. (std::make_pair(level, tag));
                    nodeque.insert(it, TNodeData(level, tag.range.first, end, tag));
                    datastack.pop();
                }
                else
                {
                    THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"do NOT match tag : " << tag.title << L"(Non-Special) and " << data.title);
                    return -1;
                }
            }
        }
    }
    else if(data.type == CParserData::DT_END_SPECIAL)
    {
        size_t level = datastack.size();
        nodeque.push_front(TNodeData(level, data.range.first, data.range.second, data));//(std::make_pair(datastack.size() - 1, datastack.top()));
    }
    else
    {
        THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"Wrong tag type : " << data.type << L" - pos : " << data.range.first);
        return -1;
    }
    return 0;
}

int CDocumentObject::PreParserBroken(const std::wstring& html, TDataStack& datastack, TNodeQueue& nodeque) const
{
    while(datastack.size() > 0)
    {
        CParserData& data = datastack.top();

        nodeque.push_front(TNodeData(datastack.size() - 1, data.range.first, data.range.second, data));// (std::make_pair(datastack.size() - 1, data));

        datastack.pop(); 
    }

    return 0;    
}

int CDocumentObject::IsSpecialTag(const CParserData& tag) const
{
    if(tag.title == L"IMG")
        return 0;
    if(tag.title == L"PARAM")
        return 0;
    if(tag.title == L"HR")
        return 0;
/*    if(tag.title == L"META")
        return 0;
*/
    if(tag.title == L"EMBED")
        return 0;
    if(tag.title == L"P")
        return 0;
    if(tag.title == L"BR")
        return 0;
    if(tag.title == L"INPUT")
        return 0;
    return -1;
}

CElementObject* CDocumentObject::MakeElement(const std::wstring& html, const CDocumentObject::TNodeData &node, CElementObject *parent, CElementObject *sibling) const
{
    std::auto_ptr<CElementObject> ele(new CElementObject);
    
    ele->level = node.level;

    if(node.data.type == CParserData::DT_END_SPECIAL)
    {
        ele->tag = html.substr(node.data.range.first + 1, node.data.range.second - node.data.range.first - 2);
    }
    else if(node.data.type == CParserData::DT_VALUE)
    {
//        ele->tag = L"<VALUE>";
        ele->value = html.substr(node.data.range.first, node.data.range.second - node.data.range.first + 1);
    }
    else
    {
        ele->tag = html.substr(node.data.range.first + 1, node.data.range.second - node.data.range.first - 1);
    }
    //if(node.data.type != CParserData::DT_END_SPECIAL)
    //    ele->tag = html.substr(node.data.range.first + 1, node.data.range.second - node.data.range.first - 1);
    //else
    //    ele->tag = html.substr(node.data.range.first + 1, node.data.range.second - node.data.range.first - 2);
    ele->start = node.start;
    ele->end = node.end;

    if(node.data.value.size() > 0)
    {
        ele->type = ET_ELEMENT;
        for(CParserData::TValueVector::const_iterator it = node.data.value.begin(); it != node.data.value.end(); ++ it)
        {
            ele->value += html.substr(it->first, it->second - it->first + 1);
        }
    }
    else
    {
        if(node.data.type == CParserData::DT_SPECIAL || node.data.type == CParserData::DT_END_SPECIAL)
            ele->type = ET_TAG;
        else if(node.data.type == CParserData::DT_VALUE)
            ele->type = ET_VALUE;
        else
            ele->type = ET_NODE;
    }

    if(ele->Analyse() != 0)
    {
        return NULL;
    }

    if(parent != NULL)
        parent->child = ele.get();
    ele->parent = parent;
    ele->sibling = sibling;

    return ele.release();
}

void CDocumentObject::Show(std::wostream &os) const
{
    if(_root != NULL)
        ShowElement(os, _root);
}

void CDocumentObject::ShowElement(std::wostream& os, const CElementObject* e) const
{
    const CElementObject* pe = e, *ps = e->sibling;

    pe->Show(os);
    
    pe = pe->child;
    if(pe != NULL)
    {
        ShowElement(os, pe);
    }
    if(ps != NULL)
    {
        ShowElement(os, ps);
    }
}

//void CDocumentObject::Rewrite(std::wofstream& ofs) const 
//{
//    if(_root != NULL)
//    {
//        std::stack<std::wstring> tagstack;
//        RewriteElement(ofs, _root, tagstack);
//    }
//}
//
//void CDocumentObject::RewriteElement(std::wofstream& ofs, const CElementObject* e, std::stack<std::wstring>& tagstack) const
//{
//    const CElementObject* pe = e, *ps = e->sibling;
//
//    //bool hasTagParent = false;
//
//    ofs << TAG_LT << pe->tag;
//    const CAttributeObject* attr = pe->attrib;
//    while(attr != NULL)
//    {
//        ofs << TAG_SPACE << attr->attr;
//        if(!attr->value.empty())
//            ofs << TAG_EQUAL << attr->value;
//        attr = attr->next;
//    }
//    if(pe->type == ET_TAG)
//    {
//        ofs << TAG_SLASH << TAG_GT;
//        //hasTagParent = true;
//    }
//    else
//    {
//        ofs << TAG_GT << pe->value;
//        tagstack.push(pe->tag);
//    }
//    
//    pe = pe->child;
//    if(pe != NULL)
//    {
//        RewriteElement(ofs, pe, tagstack);
//    }
//    else
//    {
//        if(e->type != ET_TAG)
//        {
//            ofs << TAG_LT << TAG_SLASH << tagstack.top() << TAG_GT;
//            tagstack.pop();
//        }
//    }
//
//    if(ps != NULL)
//    {
//        RewriteElement(ofs, ps, tagstack);
//    }
//    else if(e != _root)
//    {
//        ofs << TAG_LT << TAG_SLASH << tagstack.top() << TAG_GT;
//        tagstack.pop();
//    }
//}
//
//void CDocumentObject::Rewrite(wxTextOutputStream& ofs) const 
//{
//    if(_root != NULL)
//    {
//        std::stack<std::wstring> tagstack;
//        RewriteElement(ofs, _root, tagstack);
//    }
//}
//
//void CDocumentObject::RewriteElement(wxTextOutputStream& ofs, const CElementObject* e, std::stack<std::wstring>& tagstack) const
//{
//    const CElementObject* pe = e, *ps = e->sibling;
//
//    //bool hasTagParent = false;
//
//    ofs << TAG_LT << pe->tag;
//    const CAttributeObject* attr = pe->attrib;
//    while(attr != NULL)
//    {
//        ofs << TAG_SPACE << attr->attr;
//        if(!attr->value.empty())
//            ofs << TAG_EQUAL << attr->value;
//        attr = attr->next;
//    }
//    if(pe->type == ET_TAG)
//    {
//        ofs << TAG_SLASH << TAG_GT;
//        //hasTagParent = true;
//    }
//    else
//    {
//        ofs << TAG_GT << pe->value;
//        tagstack.push(pe->tag);
//    }
//    
//    pe = pe->child;
//    if(pe != NULL)
//    {
//        RewriteElement(ofs, pe, tagstack);
//    }
//    else
//    {
//        if(e->type != ET_TAG)
//        {
//            ofs << TAG_LT << TAG_SLASH << tagstack.top() << TAG_GT;
//            tagstack.pop();
//        }
//    }
//
//    if(ps != NULL)
//    {
//        RewriteElement(ofs, ps, tagstack);
//    }
//    else if(e != _root)
//    {
//        ofs << TAG_LT << TAG_SLASH << tagstack.top() << TAG_GT;
//        tagstack.pop();
//    }
//}
//
//
//void CDocumentObject::Rewrite(wxString& os) const 
//{
//    if(_root != NULL)
//    {
//        std::stack<std::wstring> tagstack;
//        RewriteElement(os, _root, tagstack);
//    }
//}
//
//void CDocumentObject::RewriteElement(wxString& os, const CElementObject* e, std::stack<std::wstring>& tagstack) const
//{
//    const CElementObject* pe = e, *ps = e->sibling;
//
//    //bool hasTagParent = false;
//
//    if(pe->type != ET_VALUE)
//    {
//        os << TAG_LT << pe->tag;
//        const CAttributeObject* attr = pe->attrib;
//        while(attr != NULL)
//        {
//            os << TAG_SPACE << attr->attr;
//            if(!attr->value.empty())
//                os << TAG_EQUAL << attr->value;
//            attr = attr->next;
//        }
//        if(pe->type == ET_TAG)
//        {
//            os << TAG_SLASH << TAG_GT;
//            //hasTagParent = true;
//        }
//        else
//        {
//            os << TAG_GT << pe->value;
//            tagstack.push(pe->tag);
//        }
//    }
//    else
//    {
//        os << pe->value;
//    }
//
//    if(pe->type != ET_VALUE)
//    {
//        pe = pe->child;
//        if(pe != NULL)
//        {
//            RewriteElement(os, pe, tagstack);
//        }
//        else
//        {
//            if(e->type != ET_TAG)
//            {
//                os << TAG_LT << TAG_SLASH << tagstack.top() << TAG_GT;
//                tagstack.pop();
//            }
//        }
//    }
//
//    if(ps != NULL)
//    {
//        RewriteElement(os, ps, tagstack);
//    }
//    else if(e != _root)
//    {
//        os << TAG_LT << TAG_SLASH << tagstack.top() << TAG_GT;
//        tagstack.pop();
//    }
//}
//
void CDocumentObject::FreeElement(CElementObject* root)
{
    CElementObject* pe = root->child, *ps = root->sibling;

//    std::cout << "free:" << root->tag << std::endl;

    if(root != NULL)
    {
        delete root;
        root = NULL;
    }

    if(pe != NULL)
    {
        FreeElement(pe);
    }
    if(ps != NULL)
    {
        FreeElement(ps);
    }
}

const CElementObject* CDocumentObject::FindFirstElement(const std::wstring &tag) const
{
    if(_root == NULL)
        return NULL;

    _findtag = tag;
    while(!_findstack.empty())
        _findstack.pop();
 
    return FindElement(NULL, _root, _findtag, _findstack);
}

const CElementObject* CDocumentObject::FindNextElement() const
{
    if(_findstack.empty())
        return NULL;

    return FindElement(NULL, _findstack.top()->child, _findtag, _findstack);
}

const CElementObject* CDocumentObject::FindFirstElement(const CElementObject* element, const std::wstring& tag, TElementStack& tmpstack) const
{
    if(element == NULL)
        return NULL;

    while(!tmpstack.empty())
        tmpstack.pop();

    return FindElement(element, element, tag, tmpstack);
}

const CElementObject* CDocumentObject::FindNextElement(const CElementObject* element, const std::wstring& tag, TElementStack& tmpstack) const
{
    if(tmpstack.empty())
        return NULL;

    return FindElement(element, tmpstack.top()->child, tag, tmpstack);
}

const CElementObject* CDocumentObject::FindElement(const CElementObject* root, const CElementObject* pe, const std::wstring& tag, TElementStack& stack) const
{
    while(pe != NULL)
    {
        stack.push(pe);
        if(pe->tag == tag)
            return pe;
        pe = pe->child;
    }
    
    while(!stack.empty() && stack.top() != root && pe == NULL)
    {
        pe = stack.top()->sibling;  
        stack.pop();
    }

    if(pe == NULL)
        return NULL;

    return FindElement(root, pe, tag, stack);
}

const CAttributeObject* CDocumentObject::FindAttribute(const CElementObject *element, const std::wstring &attr) const
{
    if(element == NULL)
        return NULL;
    
    const CAttributeObject* pa = element->attrib;
    while(pa != NULL)
    {
        if(pa->attr == attr)
            return pa;
        pa = pa->next;
    }
    return pa;
}

//

void CDocumentOutputObject::AddKey(TKeyMap* keymap, KeyType type, const wxString& str)
{
    if(keymap == NULL)
        return;

    TKeyMap::iterator it = keymap->find(type);
    if(it == keymap->end())
    {
        it = keymap->insert(std::make_pair(type, TKeySet())).first;
    }

    it->second.insert(std::wstring(str.begin(), str.end()));
}

void CDocumentOutputObject::RemoveKey(TKeyMap* keymap, KeyType type, const wxString& str)
{
    if(keymap == NULL)
        return;

    TKeyMap::iterator it = keymap->find(type);
    if(it != keymap->end())
    {
        it->second.erase(std::wstring(str.begin(), str.end()));
    }
}

bool CDocumentOutputObject::IsKey(const TKeyMap* keymap, KeyType type, const wxString& str)
{
    return IsKey(keymap, type, std::wstring(str.begin(), str.end()));
}

bool CDocumentOutputObject::IsKey(const TKeyMap* keymap, KeyType type, const std::wstring& str)
{
    if(keymap == NULL)
        return false;

    if(type == KT_TAG || type == KT_ALL_TAG)
    {
        if(keymap->find(KT_ALL_TAG) != keymap->end())
            return true;
    }
    else if(type == KT_VALUE || type == KT_ALL_VALUE)
    {
        if(keymap->find(KT_ALL_VALUE) != keymap->end())
            return true;
    }
    else if(type == KT_ATTRIB || type == KT_ALL_ATTRIB)
    {
        if(keymap->find(KT_ALL_ATTRIB) != keymap->end())
            return true;
    }

    TKeyMap::const_iterator it = keymap->find(type);
    if(it == keymap->end())
        return false;
    if(it->second.find(str) == it->second.end())
        return false;
    return true;
}

int CDocumentOutputObject::Rewrite(const CDocumentObject &doc, wxString &ostr, const CDocumentOutputObject::TKeyMap *exclude)
{
    if(doc.Root() != NULL)
    {
        TTagStack tagstack;
        try
        {
            RewriteElement(ostr, doc.Root(), doc.Root(), tagstack, exclude);
        }
        catch(std::exception& e)
        {
            return -1;
        }
    }
    return 0;
}

int CDocumentOutputObject::Rewrite(const TinyHtmlParser::CDocumentObject &doc, wxString &ostr, const TinyHtmlParser::CElementObject *tag, const TinyHtmlParser::CDocumentOutputObject::TKeyMap *exclude)
{
    if(tag != NULL)
    {
        TTagStack tagstack;
        try
        {
            RewriteElement(ostr,tag, tag, tagstack, exclude);
        }
        catch(std::exception& e)
        {
            return -1;
        }
    }
    return 0;
}

//
//void CDocumentOutputObject::RewriteElement(wxString &ostr, const CElementObject* root, const CElementObject *e, CDocumentOutputObject::TTagStack &tagstack, const CDocumentOutputObject::TKeyMap *exclude)
//{
//    const CElementObject* pe = e, *ps = e->sibling;
//
//    if(pe->type != ET_VALUE)
//    {
//        ostr << CDocumentObject::TAG_LT << pe->tag;
//        const CAttributeObject* attr = pe->attrib;
//        while(attr != NULL)
//        {
//            ostr << CDocumentObject::TAG_SPACE << attr->attr;
//            if(!attr->value.empty())
//                ostr << CDocumentObject::TAG_EQUAL << attr->value;
//            attr = attr->next;
//        }
//        if(pe->type == ET_TAG)
//        {
//            ostr << CDocumentObject::TAG_SLASH << CDocumentObject::TAG_GT;
//        }
//        else
//        {
//            ostr << CDocumentObject::TAG_GT << pe->value;
//            tagstack.push(pe->tag);
//        }
//
//        pe = pe->child;
//        if(pe != NULL)
//        {
//            RewriteElement(ostr, root, pe, tagstack, exclude);
//        }
//        else
//        {
//            if(e->type != ET_TAG)
//            {
//                ostr << CDocumentObject::TAG_LT << CDocumentObject::TAG_SLASH << tagstack.top() << CDocumentObject::TAG_GT;
//                tagstack.pop();
//            }
//        }
//    }
//    else
//    {
//        ostr << pe->value;
//    }
//
//    if(ps != NULL)
//    {
//        RewriteElement(ostr, root, ps, tagstack, exclude);
//    }
//    else if(e != root)
//    {
//            ostr << CDocumentObject::TAG_LT << CDocumentObject::TAG_SLASH << tagstack.top() << CDocumentObject::TAG_GT;
//            tagstack.pop();
//    }
//}

void CDocumentOutputObject::RewriteElement(wxString &ostr, const CElementObject* root, const CElementObject *e, CDocumentOutputObject::TTagStack &tagstack, const CDocumentOutputObject::TKeyMap *exclude)
{
    const CElementObject* pe = e, *ps = e->sibling;

    if(pe->type != ET_VALUE)
    {
        RewriteTag(ostr, pe, tagstack, exclude);

        pe = pe->child;
        if(pe != NULL)
        {
            RewriteElement(ostr, root, pe, tagstack, exclude);
        }
        else
        {
            if(e->type != ET_TAG)
            {
                RewriteTagEnd(ostr, e, tagstack, exclude);
            }
        }
    }
    else
    {
        RewriteValue(ostr, pe, exclude);
    }

	if(e == root)
	{
		return;
	}

    if(ps != NULL)
    {
        RewriteElement(ostr, root, ps, tagstack, exclude);
    }
    else if(e != root)
    {
        RewriteTagEnd(ostr, e, tagstack, exclude);
    }
}

void CDocumentOutputObject::RewriteTag(wxString &ostr, const CElementObject *e, CDocumentOutputObject::TTagStack &tagstack, const CDocumentOutputObject::TKeyMap *exclude)
{
    if(!IsKey(exclude, KT_TAG, e->tag))
    {
        ostr << CDocumentObject::TAG_LT << e->tag;
        RewriteAttrib(ostr, e, exclude);

        if(e->type == ET_TAG)
        {
            ostr << CDocumentObject::TAG_SLASH << CDocumentObject::TAG_GT;
        }
        else
        {
            ostr << CDocumentObject::TAG_GT;// << pe->value;
            RewriteValue(ostr, e, exclude);
        }
    }

    if(e->type != ET_TAG)
    {
        tagstack.push(e->tag);
    }
}

void CDocumentOutputObject::RewriteTagEnd(wxString &ostr, const CElementObject *e, CDocumentOutputObject::TTagStack &tagstack, const CDocumentOutputObject::TKeyMap *exclude)
{
    std::wstring& tag = tagstack.top();
    if(!IsKey(exclude, KT_TAG, tag))
    {
        ostr << CDocumentObject::TAG_LT << CDocumentObject::TAG_SLASH << tag << CDocumentObject::TAG_GT;
    }
    tagstack.pop();
}

void CDocumentOutputObject::RewriteValue(wxString &ostr, const CElementObject *e, const CDocumentOutputObject::TKeyMap *exclude)
{
    if(!IsKey(exclude, KT_VALUE, e->value))
    {
        ostr << e->value;
    }
}

void CDocumentOutputObject::RewriteAttrib(wxString &ostr, const CElementObject *e, const CDocumentOutputObject::TKeyMap *exclude)
{
    const CAttributeObject* attr = e->attrib;
    while(attr != NULL)
    {
        if(!IsKey(exclude, KT_ATTRIB, attr->attr))
        {
            ostr << CDocumentObject::TAG_SPACE << attr->attr;
            if(!attr->value.empty())
                ostr << CDocumentObject::TAG_EQUAL << attr->value;
        }
        attr = attr->next;
    }
}


}