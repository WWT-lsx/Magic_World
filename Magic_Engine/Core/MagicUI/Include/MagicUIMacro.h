#ifndef _MAGICUIMACRO_H_
#define _MAGICUIMACRO_H_

#include "windows.h"

#ifdef MAGICUI_EXPORTS

#define DLL_MAGICUI_OUTPUT					__declspec(dllexport)
#define DLL_MAGICUI_OUTPUT_INPUT			__declspec(dllexport)

#else

#define DLL_MAGICUI_OUTPUT
#define DLL_MAGICUI_OUTPUT_INPUT			__declspec(dllimport)

#endif

namespace Magic
{
	typedef int(*CallbackMessage_WIN32)(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
}

#endif