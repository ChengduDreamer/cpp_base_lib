#include "str_util.h"

#include <codecvt>
#include <locale>

namespace yk {

std::string ToUTF8(const std::wstring& src) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(src);
}

std::wstring ToWString(const std::string& src) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(src);
}

}