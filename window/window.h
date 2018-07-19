#pragma once

#ifdef WINDOW_DLL
#define WINDOW_DLL_API __declspec(dllexport)
#else
#define WINDOW_DLL_API __declspec(dllimport)
#endif

#include <string>

class WINDOW_DLL_API Window
{
public:
	Window(void);
	~Window(void);

public:
	const bool Initalize(const std::string & caption = "Window Title.", const unsigned int & width = 1280U, const unsigned int & height = 720U);
	const bool Run(void);
	const bool Finalize(void);

private:
	const unsigned int & get_width(void);
	const unsigned int & get_height(void);
	void * const get_hwnd(void);

public:
	const std::string & get_caption(void);
	template<class _Type> const _Type get_width(void) { return static_cast<_Type>(this->get_width()); }
	template<class _Type> const _Type get_height(void) { return static_cast<_Type>(this->get_height()); }
	template<class _Type> const _Type get_hwnd(void) { return static_cast<_Type>(this->get_hwnd()); }

private:
	class Impl;
	Impl * impl_;
};