#ifndef XY_TRACE_H
#define XY_TRACE_H

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

enum TraceLevel
{
	TraceNone = 0, // no trace
	TraceError = 10, // only trace error
	TraceWarn = 20, // trace warning & error
	TraceInfo = 30, // some extra info
	TraceDebug = 40, // debugging info
	TraceDetail = 50 // detailed debugging info
};

extern void SetTraceFilePrefix(LPCTSTR strFilePrefix);
extern void SetTraceLevel(const int nLevel);
extern void WriteTrace(const int nLevel, LPCTSTR strFormat, ...);

#endif // XY_TRACE_H
