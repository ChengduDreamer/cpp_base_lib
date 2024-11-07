#include "win32_helper.h"

#ifdef WIN32
#include "str_util.h"

namespace yk {

std::string GetErrorStr(HRESULT hr) {
	wchar_t buffer[4096] = { 0 };
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		buffer, sizeof(buffer) / sizeof(*buffer), NULL);
	std::wstring res = buffer;
	return ToUTF8(res);
}

}

#endif // WIN32