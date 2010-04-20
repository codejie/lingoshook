#ifndef __TINYHTMLPARSER_H__
#define __TINYHTMLPARSER_H__

#include <iostream>
#include <string>
#include <queue>
#include <stack>

namespace TinyHtmlParser
{

enum ElementType { ET_UNKNOWN = -1, ET_TAG = 0, ET_NODE, ET_ELEMENT };//0:just a tag, 1:no value, 2:have value

class CAttributeObject
{
public:
    CAttributeObject(const std::string& a, const std::string& v)
    : attr(a), value(v), next(NULL)
    {
    }
    virtual ~CAttributeObject() {}

    void Show(std::ostream& os) const;
public:
    std::string attr;
    std::string value;
    CAttributeObject* next;
};

class CElementObject
{
public:
    CElementObject();
    virtual ~CElementObject();

    virtual int Analyse();

    const CAttributeObject* FindAttribute(const std::string& attr) const;

    void Show(std::ostream& os) const;
protected:
    int AnalyseAttribute(const std::string& attr);
    int MakeAttribute(const std::string& attr);
    int MakeAttribute(const std::string& attr, const std::string& value);
    void FreeAnalyseAttribute();
    int AnalyseValue();
public:
    ElementType type; 
    size_t level;
    CElementObject* parent;
    CElementObject* child;
    CElementObject* sibling;

    CAttributeObject* attrib;
public:
    std::string tag;
    std::string value;
};

class CParserData
{
public:
    enum DataType { DT_UNKNOWN = -1, DT_TAG = 0, DT_VALUE, DT_END, DT_DONE, DT_TAG_VALUE };
public:
    CParserData()
        : type(DT_UNKNOWN)
    {
    }
    virtual ~CParserData() {}

public:
    DataType type;
    size_t start;
    size_t end;
    size_t vstart;
    size_t vend;
};

class CDocumentObject
{
protected:
    static const char TAG_LT        =   '<';
    static const char TAG_GT        =   '>';
    static const char TAG_SLASH     =   '/';
    static const char TAG_BSLASH    =   '\\';
    static const char TAG_AND       =   '&';

    typedef std::vector<CParserData> TDataVector;

    typedef std::stack<CParserData> TDataStack;
    struct TNodeData
    {
        size_t level;
        CParserData tag;
        CParserData value;
//        CParserData end;
    };
    typedef std::deque<TNodeData> TNodeQueue;
public:
    typedef std::stack<const CElementObject*> TElementStack;
public:
    CDocumentObject();
    virtual ~CDocumentObject();

    int Load(const std::string& str);

    const CElementObject* Root() const;

    const CElementObject* FindFirstElement(const std::string& tag);
    const CElementObject* FindNextElement();

    const CElementObject* FindFirstElement(const CElementObject* element, const std::string& tag, TElementStack& tmpstack);
    const CElementObject* FindNextElement(const CElementObject* element, const std::string& tag, TElementStack& tmpstack);

    const CAttributeObject* FindAttribute(const CElementObject* element, const std::string& attr);
    
    void Show(std::ostream& os) const;
protected:
    int PreProcess(const std::string& str, std::string& html);
    int PreParser(const std::string& html, TNodeQueue& vct);
    int Parser(const std::string& html, TNodeQueue& que);
private:
    int PreParserLT(const std::string& html, std::string::size_type& pos, CParserData& data);
    int PushValueData(const CParserData& data, TDataStack& datastack) const;
    int PushTagData(const std::string& html, const CParserData& data, TDataStack& datatstack, TNodeQueue& nodeque) const;
    
    int CheckSpecialTag(const std::string& html, const CParserData& data) const;
    int CheckTag(const std::string& html, const CParserData& tag, const CParserData& end) const;
    CElementObject* MakeElement(const std::string& html, const TNodeData& node, CElementObject* parent, CElementObject* sibling) const;

    void CDocumentObject::ShowElement(std::ostream& os, const CElementObject* e) const;

    void FreeElement(CElementObject* root);

    const CElementObject* FindElement(const CElementObject* root, const CElementObject* pe, const std::string& tag, TElementStack& stack);
private:
    CElementObject* _root;
private:
    std::string _findtag;
    TElementStack _findstack;
};

}

#endif
