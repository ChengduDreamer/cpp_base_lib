#include "path_util.h"
#include <memory>
#ifdef WIN32
#include <shlwapi.h>
#include <shellapi.h>
#pragma comment(lib, "Shlwapi.lib")
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
#endif // WIN32

}