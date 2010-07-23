
#include "DictStructure.h"

namespace SpecialDictParser
{

CDictResult::CDictResult(const CParserResult* result)
: _result(NULL)
{
    if(result != NULL)
    {
        _result.reset(result);
    }
}

CDictResult::~CDictResult()
{
}

const CParserResult* CDictResult::Result() const
{
    return _result.get();;
}

void CDictResult::Attach(const CParserResult* result)
{
    _result.reset(result);
}

CDictResult::CDictResult(const CDictResult& right)
{
    if(this != &right)
    {
        _result.reset((const_cast<CDictResult&>(right))._result.release());
    }
}

}
