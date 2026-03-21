#include "game/struct/variantdb.hpp"
#include "game/signatures.hpp"

void VariantDB::Print() { real::VariantDBPrint(this); }

FunctionObject* VariantDB::GetFunction(const std::string& rhs)
{
    return real::VariantDBGetFunction(this, UbiString(rhs));
}
FunctionObject* VariantDB::GetFunctionIfExists(const std::string& rhs)
{
    return real::VariantDBGetFunctionIfExists(this, UbiString(rhs));
}
void VariantDB::CallFunctionIfExists(const std::string& rhs, VariantList* pVList)
{
    real::VariantDBCallFunctionIfExists(this, UbiString(rhs), pVList);
}
Variant* VariantDB::GetVarIfExists(const std::string& rhs)
{
    return real::VariantDBGetVarIfExists(this, UbiString(rhs));
}
Variant* VariantDB::GetVar(const std::string& rhs) { return real::VariantDBGetVar(this, UbiString(rhs)); }
void VariantDB::DeleteVar(const std::string& rhs) { real::VariantDBDeleteVar(this, UbiString(rhs)); }
void VariantDB::DeleteAll() { real::VariantDBDeleteAll(this); }