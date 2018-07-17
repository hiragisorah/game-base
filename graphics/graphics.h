#pragma once

#ifdef GRAPHICS_DLL
#define GRAPHICS_DLL_API __declspec(dllexport)
#else
#define GRAPHICS_DLL_API __declspec(dllimport)
#endif

class GRAPHICS_DLL_API Graphics
{
private:
	Graphics(void) {};

public:
	static bool Initalize(void);
	static bool Run(void);
	static bool Finalize(void);
};