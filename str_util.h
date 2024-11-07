#pragma once
#include <string>

namespace yk {

std::string ToUTF8(const std::wstring& src);

std::wstring ToWString(const std::string& src);

}