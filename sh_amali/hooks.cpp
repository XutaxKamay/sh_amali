#include "pch.h"
#include <hooks.h>

EndScene_t OriginalEndScene = nullptr;
std::condition_variable g_bRenderingEvent;

bool g_bRendering = false;

static bool bDrawMenu = true;

HRESULT __fastcall new_EndScene(ptr_t thisptr,
                                ptr_t dx,
                                LPDIRECT3DDEVICE9 pDevice)
{
	g_bRendering = true;

    static bool init = false;
    if (!init)
    {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        ImGui_ImplWin32_Init(FindWindowA(NULL, "Shaiya"));
        ImGui_ImplDX9_Init(pDevice);
        init = true;
    }


    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (bDrawMenu)
    {
        drawMenu();
    }

    ImGui::EndFrame();
    ImGui::Render();

    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    auto ret = OriginalEndScene(thisptr, pDevice);

    if (g_bWantExit)
    {
        g_bRenderingEvent.notify_one();
        g_bWantExit = false;
    }

	g_bRendering = false;

    return ret;
}

WNDPROC oWndProc;

LRESULT __stdcall WndProc(const HWND hWnd,
                          UINT uMsg,
                          WPARAM wParam,
                          LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
        case WM_SIZE:
            if (wParam != SIZE_MINIMIZED)
            {
                ImGui_ImplDX9_InvalidateDeviceObjects()
                ImGui_ImplDX9_CreateDeviceObjects();
            }
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
