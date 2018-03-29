#pragma once

#include <cstdint>
#include <cstdarg>
#include <string>

namespace StringUtils {

std::string CreateFormatted(const char* const format, ...);

std::basic_string<wchar_t> CreateFormatted(const wchar_t* const format, ...);

std::string ToLower(std::string& str);

std::string ToLower(const std::string& str);

bool ReadFileToString(const char* source, std::string& content);

} // namespace StringUtils