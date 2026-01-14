#pragma once
#define CRT_NO_WARNINGS 1
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include "imgui_internal.h"
#include "imgui_stdlib.h"
#include "render.hpp"
#include <string>
#define max_color_float float(255.0f)
struct screen
{
    int w, h;
    screen() : w(0), h(0) {}
    screen(int w, int h) : w(w), h(h) {}
};
ImVec4 RGBA(float r, float g, float b, float a) {
    return ImVec4{
        r / max_color_float,
        g / max_color_float,
        b / max_color_float,
        a / max_color_float
    };
}
//namespace ImGui {
//    bool InputInt64(const char* label, int64_t* v, int64_t step, int64_t step_fast, ImGuiInputTextFlags flags)
//    {
//        // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
//        const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
//        return InputScalar(label, ImGuiDataType_S64, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
//    }
//}

struct window {
    HWND hwnd;
    struct WGL_WindowData { HDC hDC; };
    bool bIsMultiThread = false;
    bool render = true;
    LPCWSTR wIcon;
    int64_t fwop = 0;
    HGLRC g_hRC;
    WGL_WindowData g_MainWindow;
    int g_Width;
    int g_Height;
    int gl_minor = 3;
    int gl_major = 3;
    bool bUseCustomRender = false;
    ImVec4 clear_color = ImVec4(0.01f, 0.01f, 0.02f, 0.00f);
    std::wstring WindowTitle = L"SWEET BREADS (PahomEngineGL)";
    ImGuiIO io;
    void initIO(ImGuiIO* io_ptr) {
        *io_ptr = ImGui::GetIO();
    }
    // Вспомогательные функции
    void SetResizeWindow(int32_t x, int32_t y) {
        RECT r;
        GetWindowRect(hwnd, &r);
        MoveWindow(hwnd, r.left, r.top, x, y, TRUE);//
    }
    ImVec2 GetWindowSize(HWND HwNd)
    {
        RECT fSizeWindow;
        GetWindowRect(HwNd, &fSizeWindow);
        int64_t fWctx = fSizeWindow.right - fSizeWindow.left;
        int64_t fWcty = fSizeWindow.bottom - fSizeWindow.top;
        return ImVec2(
            static_cast<float>(fWctx), static_cast<float>(fWcty)
        );
    }
    void SetDarkStyle() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = RGBA(56, 62, 74, 255);
        colors[ImGuiCol_ChildBg] = ImVec4(0.04f, 0.04f, 0.04f, 0.94f);
        colors[ImGuiCol_PopupBg] = RGBA(15, 15, 17, 255);
        colors[ImGuiCol_Border] = RGBA(144, 159, 210, 255);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = RGBA(21, 22, 29, 255);
        colors[ImGuiCol_FrameBgHovered] = RGBA(5, 5, 5, 255);
        colors[ImGuiCol_FrameBgActive] = RGBA(5, 5, 5, 255);
        colors[ImGuiCol_TitleBg] = RGBA(135, 165, 255, 155);
        colors[ImGuiCol_TitleBgActive] = RGBA(135, 165, 255, 155);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = RGBA(56, 62, 74, 255);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.50f, 0.60f, 1.0f, 1.0f);
        colors[ImGuiCol_SliderGrab] = RGBA(35, 35, 55, 255);
        colors[ImGuiCol_SliderGrabActive] = RGBA(35, 35, 55, 255);
        colors[ImGuiCol_Button] = RGBA(7, 7, 9, 255);
        colors[ImGuiCol_ButtonHovered] = RGBA(35, 35, 55, 255);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_Header] = RGBA(5, 5, 5, 255);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.15f, 0.15f, 0.15f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
        colors[ImGuiCol_Separator] = RGBA(144, 159, 210, 255);
        colors[ImGuiCol_SeparatorHovered] = RGBA(35, 35, 55, 255);
        colors[ImGuiCol_SeparatorActive] = RGBA(35, 35, 55, 255);
        colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.13f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.26f, 0.98f, 0.50f);
        colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.20f, 0.58f, 0.73f);
        //colors[ImGuiCol_TabSelected] = ImVec4(0.29f, 0.20f, 0.68f, 1.00f);
      ///  colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        //colors[ImGuiCol_TabDimmed] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
      //  colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
        //colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
        colors[ImGuiCol_PlotLines] = RGBA(0, 235, 147, 255);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = RGBA(35, 35, 55, 255);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 150);
        //colors[ImGuiCol_TextLink] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        // colors[ImGuiCol_NavCursor] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
        style.FrameBorderSize = 1;
        style.ScrollbarSize = 4;

    }
    bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data) {
        HDC hDc = ::GetDC(hWnd);
        PIXELFORMATDESCRIPTOR pfd = { 0 };
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1; // Исправлено с 3
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cAlphaBits = 8;
        pfd.cColorBits = 32;
        const int pf = ::ChoosePixelFormat(hDc, &pfd);
        if (pf == 0) return false;
        if (::SetPixelFormat(hDc, pf, &pfd) == FALSE) return false;
        ::ReleaseDC(hWnd, hDc);

        data->hDC = ::GetDC(hWnd);
        if (!g_hRC)
            g_hRC = bUseCustomRender ? CreateGL(data->hDC, gl_major, gl_minor) : wglCreateContext(data->hDC); // Используем стандартный или твой CreateGL
        return true;
    }
    void EnableCustomRender(bool v, int minor = 3, int major = 3) {
        bUseCustomRender = v;
        gl_major = major;
        gl_minor = minor;
    }
    void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data) {
        wglMakeCurrent(nullptr, nullptr);
        ::ReleaseDC(hWnd, data->hDC);
    }
    void setTitleText(std::wstring text) {
        WindowTitle = text;
    }
    void SetSize(int width, int height) {
        g_Width = width;
        g_Height = height;
    }
    void getMessage() {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT || msg.message == WM_DESTROY)
            {
                render = false;
                std::cout << " (PahomEngine) render stop" << std::endl;
            }
        }
    }
    void LoadResIcon(int id) {
        wIcon = MAKEINTRESOURCEW(id);
    }
    void createWindow(LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)) {
        ImGui_ImplWin32_EnableDpiAwareness();
        WNDCLASSEXW wc = { sizeof(wc), CS_OWNDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"PahomEngine", nullptr };
        wc.hIcon = LoadIconW(wc.hInstance, wIcon);
        ::RegisterClassExW(&wc);
        hwnd = ::CreateWindowW(wc.lpszClassName, WindowTitle.data(), WS_OVERLAPPEDWINDOW, 100, 80, g_Width,g_Height, nullptr, nullptr, wc.hInstance, nullptr);
        ImGui_ImplWin32_EnableDpiAwareness();
        ::SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_SIZEBOX);
        if (!CreateDeviceWGL(hwnd, &g_MainWindow)) {
            exit(1);
        }
        ::ShowWindow(hwnd, SW_SHOW);
        ::UpdateWindow(hwnd);
    }

    void initDeviceGL() {
        wglMakeCurrent(g_MainWindow.hDC, g_hRC);
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplWin32_InitForOpenGL(hwnd);
        ImGui_ImplOpenGL3_Init();
    }

    void NewFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void viewport() {
        glViewport(0, 0, g_Width,g_Height);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ::SwapBuffers(g_MainWindow.hDC);
    }

    void shutdownDevice() {
        std::cout << " (PahomEngine) shutDown Device (WIN32) (OpenGL)" << std::endl;
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        CleanupDeviceWGL(hwnd, &g_MainWindow);
        if (g_hRC) wglDeleteContext(g_hRC);
        ::DestroyWindow(hwnd);
    }
  //////////////////////////////////
};