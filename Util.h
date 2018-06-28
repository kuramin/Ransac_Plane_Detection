#pragma once

#include <algorithm>
#include <vector>

namespace util
{

	class Util 
	{

	public:
		static std::vector<std::string> Split(const std::string &s, const std::string &delim, const bool keep_empty = true)
		{
			std::vector<std::string> result;
			if (delim.empty()) {
				result.push_back(s);
				return result;
			}
			std::string::const_iterator substart = s.begin(), subend;
			while (true) {
				subend = search(substart, s.end(), delim.begin(), delim.end());
				std::string temp(substart, subend);
				if (keep_empty || !temp.empty()) {
					result.push_back(temp);
				}
				if (subend == s.end()) {
					break;
				}
				substart = subend + delim.size();
			}
			return result;
		}
	};
}