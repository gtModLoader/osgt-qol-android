#include "variant.hpp"
#include "game/signatures.hpp"

void Variant::Set(std::string const& var)
{
    real::VariantSetStr(this, var);
}