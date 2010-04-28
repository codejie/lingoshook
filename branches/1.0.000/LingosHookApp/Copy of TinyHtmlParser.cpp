
#include "TinyHtmlParser.h"

namespace TinyHtmlParser
{

void CAttributeObject::Show(std::ostream& os) const
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
    std::string str = tag;

    std::string::size_type pos = str.find(" ");
    if(pos != std::string::npos)
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

int CElementObject::AnalyseAttribute(const std::string& attr)
{
    if(attr.size() == 0)
        return 0;

    std::string a, v;
    std::string::size_type pos = attr.find("="), start = 0;
    while(pos != std::string::npos)
    {
        a = attr.substr(start, pos - start);
        if(pos == attr.size() - 1)
            return -1;
        start = pos + 1;
        if(attr[pos + 1] == '\"')
        {
            pos = attr.find("\"", start + 1);
            if(pos == std::string::npos)
                return -1;
            v = attr.substr(start, pos - start + 1);
            start = pos + 2;
        }
        else
        {
            pos = attr.find(" ", start);
            if(pos == std::string::npos)
                pos = attr.size();
            v = attr.substr(start, pos - start);
            start = pos + 1;
        }
        if(MakeAttribute(a, v) != 0)
            return -1;

        if(start >= attr.size())
            break;

        pos = attr.find("=", start);
    }
    return 0;
}

int CElementObject::MakeAttribute(const std::string &attr)
{
    std::string::size_type pos = attr.find("=");
    if(pos == std::string::npos)
        return -1;

    return MakeAttribute(attr.substr(0, pos), attr.substr(pos));
}

int CElementObject::MakeAttribute(const std::string &attr, const std::string& value)
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
    std::string::size_type pos = this->value.find("&nbsp;");
    while(pos != std::string::npos)
    {
        this->value.replace(pos, 6, " ");
        pos = this->value.find("&nbsp;", pos + 1);
    }    

    return 0;
}

const CAttributeObject* CElementObject::FindAttribute(const std::string& attr) const
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

void CElementObject::Show(std::ostream& os) const
{
    os << "[" << this->level << "]" << "Tag : " << this->tag;
    if(this->type == ET_ELEMENT)
        os << " -- value = " << this->value;
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

CDocumentObject::CDocumentObject()
: _root(NULL)
{
}

CDocumentObject::~CDocumentObject()
{
    if(_root != NULL)
        FreeElement(_root);
}

int CDocumentObject::Load(const std::string &str)
{
    std::string html;
    if(PreProcess(str, html) != 0)
        return -1;
    TNodeQueue que;
    if(PreParser(html, que) != 0)
        return -1;
    if(Parser(html, que) != 0)
        return -1;
    return 0;
}

int CDocumentObject::PreProcess(const std::string& str, std::string& html)
{
    bool tag = false;
    for(std::string::const_iterator it = str.begin(); it != str.end(); ++ it)
    {
        if(*it == TAG_LT)
        {
            if(tag == true)
                return -1;
            tag = true;
        }
        else if(*it == TAG_GT)
        {
            if(tag == false)
                return -1;
            tag = false;
        }
        else
        {
            if(tag == false)
            {
                if(isspace((unsigned char)*it) != 0)
                    continue;
            }
        }
        html += *it;
    }

    return 0;
}

int CDocumentObject::PreParser(const std::string& html, CDocumentObject::TNodeQueue& que)
{
    std::string::size_type pos = 0;

    if(html.size() == 0)
        return -1;
    if(html[pos] != TAG_LT)
        return -1;

    TDataStack datastack;

    CParserData data;

    while(pos < html.size())
    {
        if(html[pos] == TAG_LT)
        {
            if(pos > data.start)
            {
                data.type = CParserData::DT_VALUE;
                data.end = pos;

//                std::cout << "VALUE - " << html.substr(data.start, data.end - data.start) << std::endl;
                if(PushValueData(data, datastack) != 0)
                    return -1;
            }

            if(PreParserLT(html, pos, data) != 0)
                return -1;
//            std::cout << "TAG - " << html.substr(data.start, data.end - data.start) << std::endl;
            if(PushTagData(html, data, datastack, que) != 0)
                return -1;

            ++ pos;
            data.start = pos;
        }
        //else if(html[pos] == TAG_GT || html[pos] == TAG_SLASH)
        //{
        //    return -1;
        //}
        else
        {
            ++ pos;
        }
//        std::cout << (char)html[pos] << std::endl;
    }

    return 0;
}

int CDocumentObject::Parser(const std::string& html, CDocumentObject::TNodeQueue& que)
{
    CElementObject *pe = NULL,  *pp = NULL, *ps = NULL;
    size_t level = 0;
    while(que.size()> 0)
    {
        const TNodeData &node = que.front();
        if(level < que.front().level)
        {
            pp = pe;
            ps = NULL;
        }
        else if(level == que.front().level)
        {
            ps = pe;
        }
        else//>
        {
            ps = pe;
            pp = pe->parent;
            int t = level - que.front().level;
            while(t > 0)
            {
                ps = ps->parent;
                pp = pp->parent;
                -- t;
            }
        }        
        level = que.front().level;

        pe = MakeElement(html, que.front(), pp, ps);

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

int CDocumentObject::PreParserLT(const std::string& html, std::string::size_type& pos, CParserData& data)
{
    if(pos == html.size() - 1)
        return -1;

    data.start = pos;

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
                data.type = CParserData::DT_DONE;
            }

            data.end = pos;
     
            return 0;
        }
        else if(html[pos] == TAG_LT)
        {
            return -1;
        }

        ++ pos;
    }

    return -1;
}

int CDocumentObject::PushValueData(const TinyHtmlParser::CParserData &data, CDocumentObject::TDataStack &datastack) const
{
    if(datastack.size() == 0)
        return -1;
    datastack.push(data);
    return 0;
}

int CDocumentObject::PushTagData(const std::string& html, const CParserData& data, CDocumentObject::TDataStack& datastack, CDocumentObject::TNodeQueue& nodeque) const
{
    if(data.type == CParserData::DT_TAG)
    {
        if(CheckSpecialTag(html, data) == 0)
        {
            TNodeData node;
            node.tag = data;

            node.level = datastack.size();
            nodeque.push_front(node);
            return 0;
        }

        if(datastack.size() > 0 && datastack.top().type == CParserData::DT_VALUE)
        {
            CParserData data = datastack.top();
            datastack.pop();
            if(datastack.top().type != CParserData::DT_TAG)
                return -1;
            datastack.top().type = CParserData::DT_TAG_VALUE;
            datastack.top().vstart = data.start;
            datastack.top().vend = data.end;
        }

        datastack.push(data);
    }
    else if(data.type == CParserData::DT_END)
    {
        if(datastack.size() == 0)
            return -1;

        TNodeData node;
        if(datastack.top().type == CParserData::DT_TAG || datastack.top().type == CParserData::DT_TAG_VALUE)
        {
            node.tag = datastack.top();
            datastack.pop();
        }
        else if(datastack.top().type == CParserData::DT_VALUE)
        {
            node.value = datastack.top();

//            std::cout << "value - " << html.substr(node.value.start, node.value.end - node.value.start) << std::endl;

            datastack.pop();
            
            if(datastack.size() == 0)
                return -1;

            if(datastack.top().type == CParserData::DT_TAG)
            {
                node.tag = datastack.top();
            }
            else if(datastack.top().type == CParserData::DT_TAG_VALUE)
            {
                node.tag = datastack.top();
            }
            else
            {
                return -1;
            }

            //node.tag = datastack.top();
            //else if(datastack.top().type == CParserData::DT_TAG_VALUE)
            //{
            //    node.tag = datastack.top();
            //}
            datastack.pop();
        }
        else
        {
//            std::cout << "type : " << datastack.top().type << std::endl;
            return -1;
        }

        if(CheckTag(html, node.tag, data) != 0)
            return -1;

        node.level = datastack.size();
        nodeque.push_front(node);
    }
    else if(data.type == CParserData::DT_DONE)
    {
        if(datastack.size() > 0 && datastack.top().type == CParserData::DT_VALUE)
        {
            CParserData data = datastack.top();
            datastack.pop();
            if(datastack.top().type != CParserData::DT_TAG)
                return -1;
            datastack.top().type = CParserData::DT_TAG_VALUE;
            datastack.top().vstart = data.start;
            datastack.top().vend = data.end;
        }

//        datastack.push(data);

        TNodeData node;
        node.tag = data;

        node.level = datastack.size();
        nodeque.push_front(node);
    }
    else
    {
        return -1;
    }
    return 0;
}

int CDocumentObject::CheckSpecialTag(const std::string& html, const CParserData& data) const
{
    std::string tag = html.substr(data.start + 1, data.end - data.start - 1);
    std::string::size_type pos = tag.find(" ");
    if(pos != std::string::npos)
        tag = tag.substr(0, pos);   
   
    if(tag == "IMG")
        return 0;

    return -1;
}

int CDocumentObject::CheckTag(const std::string& html, const CParserData& tag, const CParserData& end) const
{
    std::string str = html.substr(tag.start + 1, tag.end - tag.start - 1);
    std::string::size_type pos = str.find(" ");
    if(pos != std::string::npos)
        str = str.substr(0, pos);
    
    if(str != html.substr(end.start + 2, end.end - end.start - 2))
    {
//        std::cout << "tag : " << str << " -- end : " << html.substr(end.start + 2, end.end - end.start - 2) << std::endl;
        return -1;
    }
   return 0;
}

CElementObject* CDocumentObject::MakeElement(const std::string& html, const CDocumentObject::TNodeData &node, CElementObject *parent, CElementObject *sibling) const
{
    std::auto_ptr<CElementObject> ele(new CElementObject);
    
    ele->level = node.level;

    if(node.tag.type == CParserData::DT_TAG)
    {
        ele->type = ET_NODE;
        ele->tag = html.substr(node.tag.start + 1, node.tag.end - node.tag.start - 1);
    }
    else if(node.tag.type == CParserData::DT_DONE)
    {
        ele->type = ET_TAG;
        ele->tag = html.substr(node.tag.start + 1, node.tag.end - node.tag.start - 2);
    }
    else if(node.tag.type == CParserData::DT_TAG_VALUE)
    {
        ele->tag = ET_NODE;
        ele->tag = html.substr(node.tag.start + 1, node.tag.end - node.tag.start - 1);
    }
    else
        return NULL;

    if(node.value.type == CParserData::DT_VALUE)
    {
        ele->type = ET_ELEMENT;
        if(node.tag.type == CParserData::DT_TAG)
            ele->value = html.substr(node.value.start, node.value.end - node.value.start);
        else
            ele->value = html.substr(node.tag.vstart, node.tag.vend - node.tag.vstart) + "%" + html.substr(node.value.start, node.value.end - node.value.start);
    }

    if(ele->Analyse() != 0)
    {
        return NULL;
    }

    if(parent != NULL)
        parent->child = ele.get();
    ele->parent = parent;
    ele->sibling = sibling;

    //std::cout << "element: tag - " << ele->tag << std::endl;

    return ele.release();
}

void CDocumentObject::Show(std::ostream &os) const
{
    if(_root != NULL)
        ShowElement(os, _root);
}

void CDocumentObject::ShowElement(std::ostream& os, const CElementObject* e) const
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
        free(root);
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

const CElementObject* CDocumentObject::FindFirstElement(const std::string &tag)
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

const CElementObject* CDocumentObject::FindFirstElement(const CElementObject* element, const std::string& tag, TElementStack& tmpstack)
{
    if(element == NULL)
        return NULL;

    while(!tmpstack.empty())
        tmpstack.pop();

    return FindElement(element, element, tag, tmpstack);
}

const CElementObject* CDocumentObject::FindNextElement(const CElementObject* element, const std::string& tag, TElementStack& tmpstack)
{
    if(tmpstack.empty())
        return NULL;

    return FindElement(element, tmpstack.top()->child, tag, tmpstack);
}

const CElementObject* CDocumentObject::FindElement(const CElementObject* root, const CElementObject* pe, const std::string& tag, TElementStack& stack)
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

const CAttributeObject* CDocumentObject::FindAttribute(const TinyHtmlParser::CElementObject *element, const std::string &attr)
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