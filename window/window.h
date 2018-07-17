#pragma once

#ifdef WINDOW_DLL
#define WINDOW_DLL_API __declspec(dllexport)
#else
#define WINDOW_DLL_API __declspec(dllimport)
#endif

#include <string>

class WINDOW_DLL_API Window
{
private:
	Window(void) {};

public:
	static bool Initalize(const std::string && caption = "Window Title.", const unsigned int && width = 1280U, const unsigned int && height = 720U);
	static bool Run(void);
	static bool Finalize(void);

private:
	static bool MessageLoop(void);

private:
	static const unsigned int & get_width(void);
	static const unsigned int & get_height(void);
	static void * get_hwnd(void);

public:
	static const std::string & get_caption(void);
	template<class _Type> static const _Type get_width(void) { return static_cast<_Type>(get_width()); }
	template<class _Type> static const _Type get_height(void) { return static_cast<_Type>(get_height()); }
	template<class _Type> static const _Type get_hwnd(void) { return static_cast<_Type>(get_hwnd()); }
};