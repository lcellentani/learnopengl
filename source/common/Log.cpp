#include "Log.h"
#include <cstdarg>
#include <memory>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <tchar.h>
#define vsnprintf _vsnprintf
#endif

tinyngine::Logger Log{};

namespace {
	static void outputString(const char* const severityString, const char* const formatString, va_list argumentList) {
		//if (m_messageHandler)
		//{
		//	m_messageHandler->output(static_cast<platform::Messenger::Severity>(severity), formatString, argumentList);
		//}

#if defined(__ANDROID__)
		// Note: There may be issues displaying 64bits values with this function
		// Note: This function will truncate long messages
		__android_log_vprint(messageTypes[severity], "com.powervr.Example", formatString, argumentList);
#else
		static char buffer[4096];
		va_list tempList;
		memset(buffer, 0, sizeof(buffer));
#if (defined _MSC_VER) // Pre VS2013
		tempList = argumentList;
#else
		va_copy(tempList, argumentList);
#endif
		vsnprintf(buffer, 4095, formatString, argumentList);

#if defined(_WIN32) && !defined(_CONSOLE)
		if (IsDebuggerPresent())
		{
			OutputDebugString(severityString);
			OutputDebugString(buffer);
			OutputDebugString("\n");
		}
#else
		printf("%s", messageTypes[severity]);
		vprintf(formatString, tempList);
		printf("\n");
#endif
#endif
	}

	static const char* getSeverityString(tinyngine::Logger::Severity severity) {
		static const char* messageTypes[] = {
			"[VERBOSE]: ",
			"[DEBUG]: ",
			"[INFO]: ",
			"[WARNING]: ",
			"[ERROR]: ",
			"[CRITICAL]: ",
		};

		return messageTypes[static_cast<int>(severity)];
	}
}

namespace tinyngine {

Logger::Logger() {

}

Logger::~Logger() {

}

void Logger::operator()(Logger::Severity severity, const char* const formatString, ...)
{
	va_list argumentList;
	va_start(argumentList, formatString);
	outputString(getSeverityString(severity), formatString, argumentList);
	va_end(argumentList);
}

void Logger::operator()(const char* const formatString, ...)
{
	va_list argumentList;
	va_start(argumentList, formatString);
	outputString(getSeverityString(Logger::Severity::Error), formatString, argumentList);
	va_end(argumentList);
}

} // namespace tinyngine