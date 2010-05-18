/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#include <sstream>

#include "TinyHtmlParser.h"

namespace TinyHtmlParser
{

CExceptionObject::CExceptionObject(TinyHtmlParser::ExceptionNumber type, const std::wstring &info)
: _type(type)
, _info(info)
{
}

CExceptionObject::CExceptionObject(const TinyHtmlParser::CExceptionObject &right)
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

    std::wstring a, v;
    std::wstring::size_type pos = attr.find(L"="), start = 0;
    while(pos != std::wstring::npos)
    {
        if(pos == attr.size() - 1)
        {
            THROW_EXCEPTION(EN_ATTRIB_VALUEMISS, L"Attribue analyse failed - attribute string : " << attr);
            return -1;
        }
        a = attr.substr(start, pos - start);
        start = pos + 1;
        if(attr[pos + 1] == L'\"')
        {
            pos = attr.find(L"\"", start + 1);
            if(pos == std::wstring::npos)
            {
                THROW_EXCEPTION(EN_ATTRIB_VALUEBROKEN, L"Attribue analyse failed - attribute string : " << attr);
                return -1;
            }
            v = attr.substr(start, pos - start + 1);
            start = pos + 2;
        }
        else
        {
            pos = attr.find(L" ", start);
            if(pos == std::wstring::npos)
                pos = attr.size();
            v = attr.substr(start, pos - start);
            start = pos + 1;
        }
        if(MakeAttribute(a, v) != 0)
            return -1;

        if(start >= attr.size())
            break;

        pos = attr.find(L"=", start);
    }
    return 0;
}

int CElementObject::MakeAttribute(const std::wstring &attr)
{
    std::wstring::size_type pos = attr.find(L"=");
    if(pos == std::wstring::npos)
        return -1;

    return MakeAttribute(attr.substr(0, pos), attr.substr(pos));
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
    std::wstring::size_type pos = this->value.find(L"&nbsp;");
    while(pos != std::wstring::npos)
    {
        this->value.replace(pos, 6, L" ");
        pos = this->value.find(L"&nbsp;", pos + 1);
    }    

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
    os << "[" << this->level << "]" << "Tag : " << this->tag;
    if(this->type == ET_ELEMENT)
        os << " -- value = " << /*std::wstring*/(this->value);
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
    if(type == DT_TAG || type == DT_SPECIAL)
    {
        title = html.substr(tag.first + 1, tag.second - tag.first - 1);
        std::wstring::size_type pos = title.find(L" ");
        if(pos != std::wstring::npos)
            title = title.substr(0, pos);
    }
    else if(type == DT_END)
    {
        title = html.substr(tag.first + 2, tag.second - tag.first - 2);
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
    os << "\nTag Start = " << this->tag.first << " - End = " << this->tag.second;

    for(TValueVector::const_iterator it = this->value.begin(); it != this->value.end(); ++ it)
    {
        os << "\nValue Start = " << it->first << " - End = " << it->second;
    }
    os << std::endl;
}

void CParserData::Show(std::wostream& os, const std::wstring& html) const
{
    os << "\nType = " << this->type;
    os << "\nTag = " << "[" << this->tag.first << "," << this->tag.second << "]" << html.substr(this->tag.first, this->tag.second - this->tag.first + 1);
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

int CDocumentObject::Load(const std::wstring &str, bool strict)
{
    std::wstring html;
    if(PreProcess(str, html, strict) != 0)
        return -1;
    TNodeQueue que;
    if(PreParser(html, que, strict) != 0)
        return -1;
    if(Parser(html, que, strict) != 0)
        return -1;
    return 0;
}

int CDocumentObject::PreProcess(const std::wstring& str, std::wstring& html, bool strict)
{
    //html = str;
    bool tag = false;
    for(std::wstring::const_iterator it = str.begin(); it != str.end(); ++ it)
    {
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
                if((unsigned char)(*it) == '\r' || (unsigned char)(*it) == '\n')
                    continue;
            }
        }
        html += *it;
    }

    return 0;
}

int CDocumentObject::PreParser(const std::wstring& html, CDocumentObject::TNodeQueue& que, bool strict)
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
                if(PushValueData(/*CParserData::DT_VALUE, */start, pos -1, datastack) != 0)
                    return -1;
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
        if(level < node.first)
        {
            pp = pe;
            ps = NULL;
        }
        else if(level == node.first)
        {
            ps = pe;
        }
        else//>
        {
            ps = pe;
            pp = pe->parent;
            int t = level - node.first;
            while(t > 0)
            {
                ps = ps->parent;
                pp = pp->parent;
                -- t;
            }
        }        
        level = node.first;

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
    
    data.tag.first = pos;

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
                data.type = CParserData::DT_SPECIAL;
            }

            data.tag.second = pos;
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

    //if(prev.type == CParserData::DT_TAG)
    //{
    //    prev.value.push_back(std::make_pair(start, end));

    //    prev.type = CParserData::DT_TAG_VALUE;
    //}
    //else if(prev.type == CParserData::DT_TAG_VALUE)
    //{
    //    prev.value.push_back(std::make_pair(start, end));
    //}
    //else
    //{
    //    THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"Type does match : " << prev.type << " - pos : " << start);
    //    return -1;
    //}

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
        datastack.push(data);
    }
    else if(data.type == CParserData::DT_END)
    {
        if(datastack.size() == 0)
        {
            THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"Miss start-tag before end-tag - pos : " << data.tag.first);
            return -1;
        }

        while(datastack.size() > 0)
        {
            CParserData& tag = datastack.top();

            //std::wstring str = html.substr(tag.tag.first + 1, tag.tag.second - tag.tag.first - 1);
            //std::wstring::size_type pos = str.find(L" ");
            //if(pos != std::wstring::npos)
            //    str = str.substr(0, pos);
            //std::wstring end = html.substr(data.tag.first + 2, data.tag.second - data.tag.first - 2);

            if(tag.title == data.title)//str == end)
            {
                nodeque.push_front(std::make_pair(datastack.size() - 1, datastack.top()));
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
                        if(it->second.type == CParserData::DT_SPECIAL)
                            it->first = level;
                        else
                            break;
                        ++ it;
                    }
                    nodeque.push_front(std::make_pair(level, tag));
                    datastack.pop();
                }
                else
                {
                    THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"do NOT match tag : " << tag.title << L"(Non-Special) and " << data.title);
                    return -1;
                }
            }
        }

        //if(CheckTag(html, datastack.top(), data) != 0)
        //    return -1;
        //
        //nodeque.push_front(std::make_pair(datastack.size() - 1, datastack.top()));
        //datastack.pop();
    }
    //else if(data.type == CParserData::DT_SPECIAL)
    //{
    //    nodeque.push_front(std::make_pair(datastack.size(), data));
    //}
    else
    {
        THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"Wrong tag type : " << data.type << L" - pos : " << data.tag.first);
        return -1;
    }
    return 0;
}

int CDocumentObject::PreParserBroken(const std::wstring& html, TDataStack& datastack, TNodeQueue& nodeque) const
{
    while(datastack.size() > 0)
    {
        CParserData& data = datastack.top();

        nodeque.push_front(std::make_pair(datastack.size() - 1, data));

        datastack.pop(); 
        //if(data.type == CParserData::DT_TAG || data.type == CParserData::DT_TAG_VALUE)
        //{
        //    nodeque.push_front(std::make_pair(datastack.size() - 1, data));

        //    datastack.pop();
        //}
        ////else if(data.type == CParserData::DT_BROKEN)
        ////{
        ////    nodeque.push_front(std::make_pair(datastack.size() - 1, data));

        ////    datastack.pop();        
        ////}
        //else
        //{
        //    return -1;
        //}
    }

    return 0;    
}

//int CDocumentObject::CheckSpecialTag(const std::wstring& html, const CParserData& data) const
//{
//    std::wstring tag = html.substr(data.tag.first + 1, data.tag.second - data.tag.first - 1);
//    std::wstring::size_type pos = tag.find(L" ");
//    if(pos != std::wstring::npos)
//        tag = tag.substr(0, pos);   
//   
//    if(tag == L"IMG")
//        return 0;
//    if(tag == L"PARAM")
//        return 0;
//    if(tag == L"BR")
//        return 0;
//    if(tag == L"HR")
//        return 0;
//    if(tag == L"P")
//        return 0;
//    if(tag == L"META")
//        return 0;
//    if(tag == L"EMBED")
//        return 0;
//
//    return -1;
//}

int CDocumentObject::IsSpecialTag(const CParserData& tag) const
{
    if(tag.title == L"IMG")
        return 0;
    if(tag.title == L"PARAM")
        return 0;
    if(tag.title == L"HR")
        return 0;
    if(tag.title == L"META")
        return 0;
    if(tag.title == L"EMBED")
        return 0;
    if(tag.title == L"P")
        return 0;
    if(tag.title == L"BR")
        return 0;
    return -1;
}

//int CDocumentObject::CheckTag(const std::wstring& html, const CParserData& tag, const CParserData& end) const
//{
//    std::wstring str = html.substr(tag.tag.first + 1, tag.tag.second - tag.tag.first - 1);
//    std::wstring::size_type pos = str.find(L" ");
//    if(pos != std::wstring::npos)
//        str = str.substr(0, pos);
//    
//    if(str != html.substr(end.tag.first + 2, end.tag.second - end.tag.first - 2))
//    {
//        THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"do NOT match tag : " << str << L" and " << html.substr(end.tag.first + 2, end.tag.second - end.tag.first - 2));
//        return -1;
//    }
//   return 0;
//}

CElementObject* CDocumentObject::MakeElement(const std::wstring& html, const CDocumentObject::TNodeData &node, CElementObject *parent, CElementObject *sibling) const
{
    std::auto_ptr<CElementObject> ele(new CElementObject);
    
    ele->level = node.first;

    ele->tag = html.substr(node.second.tag.first + 1, node.second.tag.second - node.second.tag.first - 1);

    if(node.second.value.size() > 0)
    {
        ele->type = ET_ELEMENT;
        for(CParserData::TValueVector::const_iterator it = node.second.value.begin(); it != node.second.value.end(); ++ it)
        {
            ele->value += html.substr(it->first, it->second - it->first + 1);
        }
    }
    else
    {
        ele->type = ET_NODE;
    }

    //if(node.second.type == CParserData::DT_TAG)
    //{
    //    ele->type = ET_NODE;
    //    ele->tag = html.substr(node.second.tag.first + 1, node.second.tag.second - node.second.tag.first - 1);
    //}
    ////else if(node.second.type == CParserData::DT_SPECIAL)
    ////{
    ////    ele->type = ET_TAG;
    ////    ele->tag = html.substr(node.second.tag.first + 1, node.second.tag.second - node.second.tag.first - 1);
    ////}
    //else if(node.second.type == CParserData::DT_TAG_VALUE || node.second.type == CParserData::DT_SPECIAL)
    //{
    //    ele->type = ET_ELEMENT;
    //    ele->tag = html.substr(node.second.tag.first + 1, node.second.tag.second - node.second.tag.first - 1);
    //    
    //    ele->value = L"";
    //    for(CParserData::TValueVector::const_iterator it = node.second.value.begin(); it != node.second.value.end(); ++ it)
    //    {
    //        ele->value += html.substr(it->first, it->second - it->first + 1);
    //    }
    //}
    //else
    //{
    //    THROW_EXCEPTION(EN_DOCUMENT_FORMATERROR, L"Wrong Tag Type : " << node.second.type);
    //    return NULL;
    //}

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

const CElementObject* CDocumentObject::FindFirstElement(const std::wstring &tag)
{
    if(_root == NULL)
        return NULL;

    _findtag = tag;
    while(!_findstack.empty())
        _findstack.pop();
 
    return FindElement(NULL, _root, _findtag, _findstack);
}

const CElementObject* CDocumentObject::FindNextElement()
{
    if(_findstack.empty())
        return NULL;

    return FindElement(NULL, _findstack.top()->child, _findtag, _findstack);
}

const CElementObject* CDocumentObject::FindFirstElement(const CElementObject* element, const std::wstring& tag, TElementStack& tmpstack)
{
    if(element == NULL)
        return NULL;

    while(!tmpstack.empty())
        tmpstack.pop();

    return FindElement(element, element, tag, tmpstack);
}

const CElementObject* CDocumentObject::FindNextElement(const CElementObject* element, const std::wstring& tag, TElementStack& tmpstack)
{
    if(tmpstack.empty())
        return NULL;

    return FindElement(element, tmpstack.top()->child, tag, tmpstack);
}

const CElementObject* CDocumentObject::FindElement(const CElementObject* root, const CElementObject* pe, const std::wstring& tag, TElementStack& stack)
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

const CAttributeObject* CDocumentObject::FindAttribute(const TinyHtmlParser::CElementObject *element, const std::wstring &attr)
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

}