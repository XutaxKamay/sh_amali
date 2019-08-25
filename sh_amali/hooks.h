#pragma once

using namespace memory_helper;

extern bool g_bWantExit, g_bRendering;

HRESULT __fastcall new_EndScene(ptr_t thisptr,
                                ptr_t dx,
                                LPDIRECT3DDEVICE9 pDevice);

using EndScene_t = HRESULT(__thiscall*)(ptr_t, LPDIRECT3DDEVICE9);

extern EndScene_t OriginalEndScene;

extern std::condition_variable g_bRenderingEvent;


LRESULT __stdcall WndProc(const HWND hWnd,
                          UINT uMsg,
                          WPARAM wParam,
                          LPARAM lParam);

extern WNDPROC oWndProc;
