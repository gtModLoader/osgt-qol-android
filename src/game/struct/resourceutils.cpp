#include "resourceutils.hpp"

void StringReplace(const std::string& what, const std::string& with, std::string& in)
{
	size_t pos = 0;
	size_t whatLen = what.length();
	size_t withLen = with.length();
	while ((pos = in.find(what, pos)) != std::string::npos)
	{
		in.replace(pos, whatLen, with);
		pos += withLen;
	}
}