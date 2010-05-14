/*********************************************************/
// LingosHook by Jie.(codejie@gmail.com), 2010 - 
/*********************************************************/

#ifndef __TINYHTMLPARSER_H__
#define __TINYHTMLPARSER_H__

#include <iostream>
#include <string>
#include <queue>
#include <stack>

namespace TinyHtmlParser
{

enum ExceptionNumber { EN_UNKNOWN = -1, EN_ATTRIB_VALUEMISS = 0, EN_ATTRIB_VALUEBROKEN, 
                    EN_DOCUMENT_FORMATERROR
                    };

class CExceptionObject
{
public:
    CExceptionObject(ExceptionNumber type, const std::wstring& info);
    CExceptionObject(const CExceptionObject& right);
    virtual ~CExceptionObject() {}

    int Number() const { return _type; }
    const std::wstring& Info() const { return _info; }

protected:
    ExceptionNumber _type;
    std::wstring _info;
private:
    CExceptionObject& operator = (const CExceptionObject& right) { return *this; }
};


enum ElementType { ET_UNKNOWN = -1, ET_TAG = 0, ET_NODE, ET_ELEMENT };//0:just a tag, 1:no value, 2:have value

class CAttributeObject
{
public:
    CAttributeObject(const std::wstring& a, const std::wstring& v)
    : attr(a), value(v), next(NULL)
    {
    }
    virtual ~CAttributeObject() {}

    void Show(std::wostream& os) const;
public:
    std::wstring attr;
    std::wstring value;
    CAttributeObject* next;
};

class CElementObject
{
public:
    CElementObject();
    virtual ~CElementObject();

    virtual int Analyse();

    const CAttributeObject* FindAttribute(const std::wstring& attr) const;

    void Show(std::wostream& os) const;
protected:
    int AnalyseAttribute(const std::wstring& attr);
    int MakeAttribute(const std::wstring& attr);
    int MakeAttribute(const std::wstring& attr, const std::wstring& value);
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
    std::wstring tag;
    std::wstring value;
};

class CParserData
{
public:
    enum DataType { DT_UNKNOWN = -1, DT_TAG = 0, /*DT_VALUE,*/ DT_END, DT_SPECIAL, /*DT_TAG_VALUE*//*, DT_BROKEN*/ };
    typedef std::pair<size_t, size_t> TRange;//start + end;
    typedef std::vector<TRange> TValueVector;
public:
    CParserData()
        : type(DT_UNKNOWN)
    {
    }
    virtual ~CParserData() {}

    int GetTitle(const std::wstring& html);

    void Show(std::wostream& os) const;
    void Show(std::wostream& os, const std::wstring& html) const;

public:
    DataType type;
    TRange tag;
    TValueVector value;
    std::wstring title;
};

class CDocumentObject
{
protected:
    static const wchar_t TAG_LT        =   L'<';
    static const wchar_t TAG_GT        =   L'>';
    static const wchar_t TAG_SLASH     =   L'/';
    static const wchar_t TAG_BSLASH    =   L'\\';
    static const wchar_t TAG_AND       =   L'&';

    typedef std::stack<CParserData> TDataStack;
    typedef std::pair<size_t, CParserData> TNodeData;//level + tag;
    typedef std::deque<TNodeData> TNodeQueue;
public:
    typedef std::stack<const CElementObject*> TElementStack;
public:
    CDocumentObject();
    virtual ~CDocumentObject();

    int Load(const std::wstring& str, bool strict = true);

    const CElementObject* Root() const { return _root; }

    const CElementObject* FindFirstElement(const std::wstring& tag);
    const CElementObject* FindNextElement();

    const CElementObject* FindFirstElement(const CElementObject* element, const std::wstring& tag, TElementStack& tmpstack);
    const CElementObject* FindNextElement(const CElementObject* element, const std::wstring& tag, TElementStack& tmpstack);

    const CAttributeObject* FindAttribute(const CElementObject* element, const std::wstring& attr);
    
    bool IsMistake() const { return _bIsMistake; }

    void Show(std::wostream& os) const;
protected:
    int PreProcess(const std::wstring& str, std::wstring& html, bool strict);
    int PreParser(const std::wstring& html, TNodeQueue& que, bool strict);
    int Parser(const std::wstring& html, TNodeQueue& que, bool strict);
private:
    int PreParserLT(const std::wstring& html, std::wstring::size_type& pos, CParserData& data);

    int PushValueData(/*CParserData::DataType type, */size_t start, size_t end, TDataStack& datastack) const;
    int PushTagData(const std::wstring& html, CParserData& data, TDataStack& datastack, TNodeQueue& nodeque) const;
    int PreParserBroken(const std::wstring& html, TDataStack& datastack, TNodeQueue& nodeque) const;
    
    //int CheckSpecialTag(const std::wstring& html, const CParserData& data) const;
    int IsSpecialTag(const CParserData& tag) const;
    int IsLineTag(const CParserData& tag) const;
    //int CheckTag(const std::wstring& html, const CParserData& tag, const CParserData& end) const;
    CElementObject* MakeElement(const std::wstring& html, const TNodeData& node, CElementObject* parent, CElementObject* sibling) const;

    void CDocumentObject::ShowElement(std::wostream& os, const CElementObject* e) const;

    void FreeElement(CElementObject* root);

    const CElementObject* FindElement(const CElementObject* root, const CElementObject* pe, const std::wstring& tag, TElementStack& stack);
private:
    CElementObject* _root;
private:
    std::wstring _findtag;
    TElementStack _findstack;
private:
    bool _bIsMistake;
};

}

#endif
