//
// Created by RGAA on 2023-12-17.
//

#ifndef TC_APPLICATION_STRINGEXT_H
#define TC_APPLICATION_STRINGEXT_H

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <locale>
#include <string>
#include <codecvt>
#include <cctype>
#include <iomanip>

#ifdef WIN32
#include <Windows.h>
#endif

namespace tc
{

    class StringExt {
    public:

        // 
        static void Split(const std::string &s,
                          std::vector<std::string> &sv,
                          const char delim = ' ') {
            sv.clear();
            std::istringstream iss(s);
            std::string temp;

            while (std::getline(iss, temp, delim)) {
                sv.emplace_back(std::move(temp));
            }
        }


        static void Split(const std::string& s,
                          std::vector<std::string>& res,
                          const std::string& delimiter) {
            size_t pos_start = 0, pos_end, delim_len = delimiter.length();
            std::string token;

            while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
                token = s.substr (pos_start, pos_end - pos_start);
                pos_start = pos_end + delim_len;
                res.push_back (token);
            }

            res.push_back (s.substr (pos_start));
        }

        static void ToLower(std::string& data) {
            std::transform(data.begin(), data.end(), data.begin(),
                           [](unsigned char c){ return std::tolower(c); });
        }

        static std::string ToLowerCpy(const std::string& data) {
            std::string target = data;
            std::transform(target.begin(), target.end(), target.begin(),
                           [](unsigned char c) -> char { return std::tolower(c); });
            return target;
        }

        static bool StartWith(const std::string& input, const std::string& find) {
            if (input.rfind(find, 0) == 0) {
                return true;
            }
            return false;
        }

        static std::string CopyStr(const std::string& origin) {
            std::string copy;
            copy.resize(origin.size());
            memcpy(copy.data(), origin.data(), origin.size());
            return copy;
        }

        static void Replace(std::string& origin, const std::string& from, const std::string& to) {
            size_t start_pos = 0;
            while ((start_pos = origin.find(from, start_pos)) != std::string::npos) {
                origin.replace(start_pos, from.length(), to);
                start_pos += to.length();
            }
        }

        static inline std::wstring ToWString(const std::string& src) {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            return converter.from_bytes(src);
        }

        static inline std::string ToUTF8(const std::wstring& src) {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            return converter.to_bytes(src);
        }

        static std::string Wstring2utf8string(const std::wstring& str)
        {
            static std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
            return strCnv.to_bytes(str);
        }

        static std::wstring Utf8string2wstring(const std::string& str)
        {
            static std::wstring_convert< std::codecvt_utf8<wchar_t> > strCnv;
            return strCnv.from_bytes(str);
        }
#ifdef WIN32
        static std::string GetErrorStr(HRESULT hr) {
            wchar_t buffer[4096] = { 0 };
            FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, hr,
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           buffer, sizeof(buffer) / sizeof(*buffer), NULL);
            std::wstring res = buffer;
            return Wstring2utf8string(res);
        }

        static std::string StandardizeWinPath(const std::string& path) {
            std::string normalized = path;
            StringExt::Replace(normalized, "\\", "/");
            // D: => D:/
            if (normalized.size() == 2 && normalized[1] == ':') {
                normalized += "/";
            }
            // "D:/video/" => "D:/video"
            if (normalized.size() >= 4 && normalized.back() == '/') {
                normalized.pop_back();
            }
            return normalized;
        }

        static std::string GbkToUtf8(const char* src_str)
        {
            int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
            wchar_t* wstr = new wchar_t[len + 1];
            memset(wstr, 0, len + 1);
            MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
            len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
            char* str = new char[len + 1];
            memset(str, 0, len + 1);
            WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
            std::string strTemp = str;
            if (wstr) delete[] wstr;
            if (str) delete[] str;
            return strTemp;
        }

        static std::string Utf8ToGbk(const char* src_str)
        {
            int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
            wchar_t* wszGBK = new wchar_t[len + 1];
            memset(wszGBK, 0, len * 2 + 2);
            MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
            len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
            char* szGBK = new char[len + 1];
            memset(szGBK, 0, len + 1);
            WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
            std::string strTemp(szGBK);
            if (wszGBK) delete[] wszGBK;
            if (szGBK) delete[] szGBK;
            return strTemp;
        }
#endif
        static std::string FormatSize(uint64_t byte_size) {
            static const char* suffixes[] = { "B", "KB", "MB", "GB" };
            const int numSuffixes = sizeof(suffixes) / sizeof(suffixes[0]);
            int suffixIndex = 0;
            while (byte_size >= 1024 && suffixIndex < numSuffixes - 1) {
                byte_size /= 1024;
                suffixIndex++;
            }
            std::ostringstream stream;
            stream << std::fixed << std::setprecision(2) << byte_size << " " << suffixes[suffixIndex];
            return stream.str();
        }
    };

}

#endif //TC_APPLICATION_STRINGEXT_H
