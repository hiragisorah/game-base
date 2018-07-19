#pragma once

#ifdef GRAPHICS_DLL
#define GRAPHICS_DLL_API __declspec(dllexport)
#else
#define GRAPHICS_DLL_API __declspec(dllimport)
#endif

#include "..\window\window.h"

class GRAPHICS_DLL_API Graphics
{
public:
	Graphics(Window * const window);
	~Graphics(void);

public:
	bool Initalize(void);
	bool Run(void);
	bool Finalize(void);

private:
	class Impl;
	Impl * impl_;
};