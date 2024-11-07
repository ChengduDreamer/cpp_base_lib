#pragma once

#ifdef WIN32

#include <Windows.h>
#include <string>

namespace yk {

std::string GetErrorStr(HRESULT hr);

}



#endif // WIN32
