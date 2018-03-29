#include "StringUtils.h"

#include <cctype>
#include <algorithm>
#include <iostream>
#include <fstream>

namespace {

std::string vaFormatString(const char* format, va_list argumentList) {
	bool result = true;

#if defined(_WIN32)
	int newStringSize = _vscprintf(format, argumentList);
#else
	va_list argumentListCopy;

	va_copy(argumentListCopy, argumentList);
	int newStringSize = vsnprintf(0, 0, format, argumentListCopy);
	va_end(argumentListCopy);
#endif

	// Create a new string
	std::string newString;
	newString.resize(static_cast<uint32_t>(newStringSize));

	if (result) {
#ifdef _WIN32
		if (_vsnprintf_s(const_cast<char*>(newString.c_str()), newStringSize + 1, newStringSize, format, argumentList) != newStringSize)
#else
		pvr::int32 n = vsnprintf(const_cast<char*>(newString.data()), newStringSize + 1, format, argumentList);
		if ((n < 0) || (n >= newStringSize + 1))
#endif
		{
			result = false;
		}
	}

	if (!result) {
		newString.resize(0);
	}
	return std::string(newString);
}

std::basic_string<wchar_t> vaFormatString(const wchar_t* const format, va_list argumentList) {
	bool result = true;

#if defined(_WIN32)
	int newStringSize = _vscwprintf(format, argumentList);
#else
	va_list argumentListCopy;

	va_copy(argumentListCopy, argumentList);
	int newStringSize = vswprintf(0, 0, format, argumentListCopy);
	va_end(argumentListCopy);
#endif

	// Create a new string
	std::basic_string<wchar_t> newString;
	newString.resize(static_cast<uint32_t>(newStringSize + 1));

	if (result) {
#ifdef _WIN32
		if (_vsnwprintf_s(const_cast<wchar_t*>(newString.c_str()), newStringSize + 1, newStringSize, format,
			argumentList) != newStringSize)
#else
		if (vswprintf((wchar_t*)newString.c_str(), (size_t)newStringSize + 1, (const wchar_t*)format, argumentList) != newStringSize)
#endif
		{
			result = false;
		}
	}

	if (!result) {
		newString.resize(0);
	}
	return std::basic_string<wchar_t>(newString);
}

}

namespace StringUtils {

std::string CreateFormatted(const char* const format, ...) {
	// Calculate the length of the new string
	va_list argumentList;
	va_start(argumentList, format);
	std::string newString = vaFormatString(format, argumentList);
	va_end(argumentList);

	return newString;
}

std::basic_string<wchar_t> CreateFormatted(const wchar_t* const format, ...) {
	// Calculate the length of the new string
	va_list argumentList;
	va_start(argumentList, format);
	std::basic_string<wchar_t> newString = vaFormatString(format, argumentList);
	va_end(argumentList);
	return newString;
}

std::string ToLower(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) -> unsigned char { return static_cast<unsigned char>(std::tolower(c)); });
	return str;
}

std::string ToLower(const std::string& str) {
	std::string s = str;
	return ToLower(s);
}

bool ReadFileToString(const char* source, std::string& content) {
	bool result = false;
	if (source) {
		std::ifstream stream(source);
		if (stream) {
			stream.seekg(0, std::ios::end);
			content.reserve(static_cast<unsigned int>(stream.tellg()));
			stream.seekg(0, std::ios::beg);

			content.assign((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

			result = true;
		}
	}
	return result;
}

} // namespace StringUtils