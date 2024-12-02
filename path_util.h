#pragma once

#include <string>
#ifdef WIN32
#include <Windows.h>
#endif // WIN32

#ifdef WIN32
#define YK_PATH_SEP L"\\"
#elif
#define YK_PATH_SEP "/"
#endif // WIN32

namespace yk {

#ifdef WIN32
std::wstring GetModulePathW(HMODULE hModule);

/*
* suffix : \\app_name
*/
std::wstring GetProgramDataDir(const std::wstring& suffix);
#endif // WIN32

}