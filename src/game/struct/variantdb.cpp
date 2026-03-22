#include "game/struct/variantdb.hpp"
#include "game/signatures.hpp"

void VariantDB::Print() { real::VariantDBPrint(this); }

FunctionObject* VariantDB::GetFunction(const std::string& rhs) { return real::VariantDBGetFunction(this, rhs); }
FunctionObject* VariantDB::GetFunctionIfExists(const std::string& rhs)
{
    return real::VariantDBGetFunctionIfExists(this, rhs);
}
void VariantDB::CallFunctionIfExists(const std::string& rhs, VariantList* pVList)
{
    real::VariantDBCallFunctionIfExists(this, rhs, pVList);
}
Variant* VariantDB::GetVarWithDefault(const std::string& key, const Variant& vDefault)
{
    return real::VariantDBGetVarWithDefault(this, key, vDefault);
}
Variant* VariantDB::GetVarIfExists(const std::string& rhs) { return real::VariantDBGetVarIfExists(this, rhs); }
Variant* VariantDB::GetVar(const std::string& rhs) { return real::VariantDBGetVar(this, rhs); }
void VariantDB::DeleteVar(const std::string& rhs) { real::VariantDBDeleteVar(this, rhs); }
void VariantDB::DeleteAll() { real::VariantDBDeleteAll(this); }