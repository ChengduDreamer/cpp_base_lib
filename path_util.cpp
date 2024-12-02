#include "path_util.h"
#include <memory>
#ifdef WIN32
#include <shlwapi.h>
#include <shellapi.h>
#include <shlobj_core.h>
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Shell32.lib")
#endif // WIN32

namespace yk {
#ifdef WIN32
std::wstring GetModulePathW(HMODULE hModule) {
	const int maxPath = 4096;
	std::unique_ptr<wchar_t[]> sz_path = std::make_unique<wchar_t[]>(maxPath);
	std::unique_ptr<wchar_t[]> sz_full_path = std::make_unique<wchar_t[]>(maxPath);
	memset(sz_path.get(), 0, sizeof(wchar_t) * maxPath);
	memset(sz_full_path.get(), 0, sizeof(wchar_t) * maxPath);
	::GetModuleFileNameW(hModule, sz_path.get(), maxPath);
	::GetLongPathNameW(sz_path.get(), sz_full_path.get(), maxPath);
	::PathRemoveFileSpecW(sz_full_path.get());
	return std::wstring(sz_full_path.get());
}


std::wstring GetProgramDataDir(const std::wstring& suffix) {
	std::wstring dir_path;
	wchar_t sz_path[MAX_PATH] = { 0 };
	if (S_OK == SHGetFolderPathW(NULL, CSIDL_COMMON_APPDATA, NULL, 0, sz_path)) {
		char szTempFileName[MAX_PATH];
		// Append product-specific path - this path needs to already exist
		// for GetTempFileName to succeed.
		PathAppendW(sz_path, suffix.c_str());
		dir_path = sz_path;
	}
	else {
		dir_path =  std::wstring(L"C:\\ProgramData") + suffix;
	}
	return dir_path;
}


#endif // WIN32

}