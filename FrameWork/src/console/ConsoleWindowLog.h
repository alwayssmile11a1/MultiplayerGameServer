#ifndef __GUICON_H__

#define __GUICON_H__

#ifdef _DEBUG


#define DebugLog(message) _RPT0(_CRT_WARN, message);

void RedirectIOToConsole();

#endif

#endif
