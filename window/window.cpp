#include "window.h"

#define WIN32_LEAD_AND_MEAN
#include <Windows.h>

namespace
{
	static std::string caption_ = "Window Title";
	static unsigned int width_ = 1280U;
	static unsigned int height_ = 720U;
	static HWND hwnd_ = nullptr;
	static HINSTANCE hinstance_ = nullptr;

	static LRESULT __stdcall WndProc(HWND hwnd, UINT i_msg, WPARAM w_param, LPARAM l_param)
	{
		switch (i_msg)
		{
		case WM_DESTROY: PostQuitMessage(0); break;
		case WM_ACTIVATEAPP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP: break;
		default:
			return DefWindowProc(hwnd, i_msg, w_param, l_param);
		}
		return 0;
	}
}

bool Window::Initalize(const std::string && caption, const unsigned int && width, const unsigned int && height)
{
	caption_ = caption;
	width_ = width;
	height_ = height;

	{// �n���h���擾
		hinstance_ = GetModuleHandleA(nullptr);
	}

	{// �E�B���h�E�N���X�̓o�^
		WNDCLASSEX  wc;
		memset(&wc, 0, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.hInstance = hinstance_;
		wc.hIcon = LoadIconA(nullptr, IDI_APPLICATION);
		wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
		wc.lpszClassName = get_caption().c_str();
		wc.hIconSm = LoadIconA(nullptr, IDI_APPLICATION);
		RegisterClassExA(&wc);
	}

	{// �E�B���h�E�̑傫���̍Ē����A�����z�u�v�Z
		RECT rc;
		rc.left = 0;
		rc.top = 0;
		rc.right = get_width<long>();
		rc.bottom = get_height<long>();

		AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, true, 0);

		int w = rc.right - rc.left;
		int h = rc.bottom - rc.top;

		RECT rc_desk;
		GetWindowRect(GetDesktopWindow(), &rc_desk);

		int x = rc_desk.right / 2 - w / 2;
		int y = rc_desk.bottom / 2 - h / 2;

		{// �E�B���h�E�̐����A�\��
			hwnd_ = CreateWindowExA(0, get_caption().c_str(), get_caption().c_str(), WS_OVERLAPPEDWINDOW,
				x, y, w, h, 0, 0, hinstance_, 0);

			ShowWindow(hwnd_, SW_SHOW);
		}
	}

	return true;
}

bool Window::Run(void)
{
	return MessageLoop();
}

bool Window::Finalize(void)
{
	return true;
}

bool Window::MessageLoop(void)
{
	MSG msg = { 0 };
	memset(&msg, 0, sizeof(msg));

	while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
		if (msg.message == WM_QUIT) return false;
	}
	return true;
}

const std::string & Window::get_caption(void)
{
	return caption_;
}

const unsigned int & Window::get_width(void)
{
	return width_;
}

const unsigned int & Window::get_height(void)
{
	return height_;
}

void * Window::get_hwnd(void)
{
	return hwnd_;
}