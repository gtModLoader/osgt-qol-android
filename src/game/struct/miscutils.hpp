#pragma once
#include <sstream>
#include <string>


template <class C> std::string toString(C value)
{
    std::ostringstream o;
    o << value;
    return o.str();
}