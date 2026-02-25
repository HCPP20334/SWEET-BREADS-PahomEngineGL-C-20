#define CRT_NO_WARNINGS 1
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define PAHOM_ENGINE_ID (int64_t)0x17
#include <windows.h>
#include "render.hpp"
#include <string>
//#include "JoyStick.h"
#include <iostream>
#include <fstream>
#include <string>
#include "imgui_stdlib.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <wtypes.h>

#include "imgui_internal.h"
#include <filesystem>
#include <intrin.h>
#include <random>
#define MA_NO_SSE2
#define MA_NO_SSE4
#define MA_NO_AVX
#include <Audio/Sound.hpp>
#include <dbghelp.h>
#include "JoyStick.h"
#pragma comment(lib,"Audio.lib")
#pragma comment(lib, "dbghelp.lib")
#include <Psapi.h>
#include <unordered_map>
#include <cstring>
#include <thread>
#include <array>
#include <stdexcept>
#define _CRT_INTERNAL_NONSTDC_NAMES 1
#include <stdio.h>
#include <pdh.h>
#include <dwmapi.h>
#include "ImGamepadAPI.hpp"
#include "tui.hpp"
#include <commdlg.h>
#include <span>
#include "backend.hpp"
#pragma comment(lib,"dwmapi.lib")

//
#if !_HAS_CXX20
#error "PahomEngine требует C++20 или новее. Включи флаг -std=c++20 (g++) или /std:c++20 (MSVC)"
#endif

#pragma comment(lib, "pdh.lib")
#define PE_ARRAYSIZE(_ARR) (static_cast<int>(sizeof(_ARR) / sizeof(*(_ARR)))) 
typedef long double double64_t;
constexpr size_t double64_t_size = sizeof(double64_t);
struct d64Vec2 {
    double64_t x, y;
    constexpr d64Vec2() : x(0.0L), y(0.0L) {}
    constexpr d64Vec2(double64_t _x,double64_t _y) : x(_x), y(_y) {}
    constexpr size_t size() const {
        return sizeof(this->x) + sizeof(this->y);
    }
    static constexpr d64Vec2 P_(d64Vec2 a, d64Vec2 b) { 
        return { a.x + b.x, a.y + b.y };
    }
    static constexpr d64Vec2 M_(d64Vec2 a, d64Vec2 b) { 
        return { a.x - b.x, a.y - b.y }; 
    }
    static  d64Vec2 D_(d64Vec2 a, d64Vec2 b){
        if (!b.x || !b.y) {
            printf("divide by zero!! d64Vec2 b.x || b.y=0\n");
            return { 0,0 };
        }
        else {
            return { a.x / b.x,a.y / b.y };
        }
    }
    static  double64_t DA_(double64_t a, double64_t b) {
        if (!b) {
            printf("divide by zero!! b=0\n");
            return 0;
        }
        else {
            return {a / b};
        }
    }
    static constexpr d64Vec2 S_(double64_t s, d64Vec2 v) {
        return { s * v.x, s * v.y };
    }
    std::string_view to_string(int st) const {
        return std::format("{}", ((!st) ? this->x : this->y));
    }
};
auto d64 = std::make_unique<d64Vec2>();


//
typedef void (APIENTRY* PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (APIENTRY* PFNGLGETBUFFERPARAMETERIVPROC)(GLenum target, GLenum pname, GLint* params);
// typedefs
int64_t fstack(void* reg,std::string param_name) {
    std::cout << param_name << "=" << reinterpret_cast<int64_t>(reg)<<"stack="<< reg;
    return reinterpret_cast<int64_t>(reg);
}
std::string str_stack(void* reg, const std::string& param_name) {
    // Cast void* to const char* (assuming reg points to a null-terminated string)
    const char* str = static_cast<const char*>(reg);

    // Log to console
    std::cout << param_name << "=" << reinterpret_cast<uintptr_t>(reg)
        << " stack=" << reg << std::endl;

    // Return the string content as std::string_view
    return std::string(str);
}
template <typename T>
struct UniVec2 {
    T a, b;
    constexpr UniVec2() : a(), b() {}
    constexpr UniVec2(T _a, T _b) : a(_a), b(_b) {}
    size_t size() const {
        size_t out_size = sizeof(this->a) + sizeof(this->b);
        return out_size;
    }
};
    struct floatV3 {
        float x, y, z;
        constexpr floatV3() : x(0.0f), y(0.0f), z(0.0) {}
        constexpr floatV3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    };
    struct MultiVectors2V2 {
        ImVec2 a, b;
        constexpr MultiVectors2V2() : a({ 0,0 }), b({ 0,0 }) {}
        constexpr MultiVectors2V2(ImVec2 _a, ImVec2 _b) : a(_a), b(_b) {}
        size_t size() const {
            size_t out_size = sizeof(this->a) + sizeof(this->b);
            return out_size;
        }
    };
    struct MultiVectors3V2 {
        ImVec2 a, b, c;
        constexpr MultiVectors3V2() : a({ 0,0 }), b({ 0,0 }), c({ 0,0 }) {}
        constexpr MultiVectors3V2(ImVec2 _a, ImVec2 _b, ImVec2 _c) : a(_a), b(_b), c(_c) {}
        size_t size() const {
            size_t out_size = sizeof(this->a) + sizeof(this->b) + sizeof(this->c);
            return out_size;
        }
    };
    struct MultiVectors4V2 {
        ImVec2 a, b, c, d;
        constexpr MultiVectors4V2() : a({ 0,0 }), b({ 0,0 }), c({ 0,0 }), d({ 0,0 }) {}
        constexpr MultiVectors4V2(ImVec2 _a, ImVec2 _b, ImVec2 _c, ImVec2 _d) : a(_a), b(_b), c(_c), d(_d) {}
        size_t size() const {
            size_t out_size = sizeof(this->a) + sizeof(this->b) + sizeof(this->c) + sizeof(this->d);
            return out_size;
        }
    };

//
namespace ImGui {
    bool SliderU32(const char* label, uint32_t* v, uint32_t v_min = 0, uint32_t v_max = 100, const char* format = "%d", ImGuiSliderFlags flags = 0)
    {
        return SliderScalar(label, ImGuiDataType_U32, v, &v_min, &v_max, format, flags);
    }
    void ImageRotated(ImTextureID user_texture_id, const ImVec2& size, int angle, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
    {
        //IM_ASSERT(angle % 90 == 0);
        ImVec2 _uv0, _uv1, _uv2, _uv3;
        switch (angle % 360)
        {
        case 0:
            Image(user_texture_id, size, uv0, uv1, tint_col, border_col);
            return;
        case 180:
            Image(user_texture_id, size, uv1, uv0, tint_col, border_col);
            return;
        case 90:
            _uv3 = uv0;
            _uv1 = uv1;
            _uv0 = ImVec2(uv1.x, uv0.y);
            _uv2 = ImVec2(uv0.x, uv1.y);
            break;
        case 270:
            _uv1 = uv0;
            _uv3 = uv1;
            _uv0 = ImVec2(uv0.x, uv1.y);
            _uv2 = ImVec2(uv1.x, uv0.y);
            break;
        }
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return;
        ImVec2 _size(size.y, size.x);
        ImRect bb(window->DC.CursorPos, window->DC.CursorPos + _size);
        if (border_col.w > 0.0f)
            bb.Max += ImVec2(2, 2);
        ItemSize(bb);
        if (!ItemAdd(bb, 0))
            return;
        if (border_col.w > 0.0f)
        {
            window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(border_col), 0.0f);
            ImVec2 x0 = bb.Min + ImVec2(1, 1);
            ImVec2 x2 = bb.Max - ImVec2(1, 1);
            ImVec2 x1 = ImVec2(x2.x, x0.y);
            ImVec2 x3 = ImVec2(x0.x, x2.y);
            window->DrawList->AddImageQuad(user_texture_id, x0, x1, x2, x3, _uv0, _uv1, _uv2, _uv3, GetColorU32(tint_col));
        }
        else
        {
            ImVec2 x1 = ImVec2(bb.Max.x, bb.Min.y);
            ImVec2 x3 = ImVec2(bb.Min.x, bb.Max.y);
            window->DrawList->AddImageQuad(user_texture_id, bb.Min, x1, bb.Max, x3, _uv0, _uv1, _uv2, _uv3, GetColorU32(tint_col));
        }
    }
    bool CustomToggle( const char* label, bool* v) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImGui::PushID(label); ImGui::TextColored((*v ? ImVec4(1.0f, 1.0f, 1.0f, 1.00f)  : ImVec4(0.65f, 0.65f, 0.65f, 1.00f)), label);
        ImGui::SameLine();
        float height = ImGui::GetFrameHeight();
        float width = height * 1.55f;
        ImGuiStyle& style = ImGui::GetStyle();

        ImVec2 p = ImGui::GetCursorScreenPos();

        // Colors
        ImU32 color_bg_on = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImU32 color_bg_off = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImU32 color_knob = *v ? ImGui::GetColorU32(ImVec4(0.34f, 150, 0.98f, 1.00f)) :
            ImGui::GetColorU32(ImVec4(0.36f, 0.36f, 0.36f, 1.0f));
        ImU32 glow_color = ImGui::GetColorU32(ImVec4(0.34f, 150, 0.98f, 0.3f)); // ���� ��������
        ImU32 border_color = ImGui::GetColorU32(ImGuiCol_Border);

        ImGui::InvisibleButton(label, ImVec2(width, height));
        bool clicked = ImGui::IsItemClicked();
        if (clicked) *v = !(*v);

        // Background + Border
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), *v ? color_bg_on : color_bg_off, height);

        if (style.FrameBorderSize > 0.0f) {
            draw_list->AddRect(
                p,
                ImVec2(p.x + width, p.y + height),
                border_color,
                height * 0.5f,
                0,
                style.FrameBorderSize
            );
        }

        // Knob parameters
        float base_radius = *v ? 10 : 8;
        ImVec2 knob_pos = *v ?
            ImVec2(p.x + width - height * 0.5f, p.y + height * 0.5f) :
            ImVec2(p.x + height * 0.5f, p.y + height * 0.5f);

        // Glow effect (only when active)
        if (*v) {
            // ������ ��������� ����� ��� ������� ��������
            const int glow_layers = 3;
            for (int i = 0; i < glow_layers; i++) {
                float alpha = 0.3f * (1.0f - (float)i / glow_layers);
                float radius = base_radius + 2.0f * (i + 1);
                draw_list->AddCircle(
                    knob_pos,
                    radius,
                    ImGui::GetColorU32(ImVec4(0.34f, 150, 0.98f, alpha)),
                    0, // �������� (0 = �������������)
                    2.0f // ������� �����
                );
            }
        }

        // Main knob
        draw_list->AddCircleFilled(knob_pos, base_radius, color_knob);

        ImGui::PopID();
        return clicked;
    }
    bool Spinner(const char* label, float radius, int thickness, const ImU32& color) {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ItemSize(bb, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return false;

        // Render
        window->DrawList->PathClear();

        int num_segments = 30;
        int start = abs(ImSin(g.Time * 1.8f) * (num_segments - 5));

        const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
        const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

        const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

        for (int i = 0; i < num_segments; i++) {
            const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
            window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + g.Time * 8) * radius,
                centre.y + ImSin(a + g.Time * 8) * radius));
        }

        window->DrawList->PathStroke(color, false, thickness);
    }
    bool SpinnerBar(const char* label, float progress, float radius = 12.0f, int thickness = 4, ImU32 color = IM_COL32(255, 255, 255, 200))
    {

        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems) return false;
        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size(radius * 2, radius * 2 + style.FramePadding.y * 2);
        const ImRect bb(pos, ImVec2(pos.x + size.x,pos.y + size.y));
        ItemSize(bb, style.FramePadding.y);
        if (!ItemAdd(bb, id)) return false;
        const ImVec2 center = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);
        ImDrawList* draw_list = window->DrawList;
        const float start_angle = -IM_PI / 2.0f;
        const float end_angle = start_angle + IM_PI * 2.0f * progress;
        const float anim_speed = 2.5f;
        const float tail_length = 0.3f;
        float anim_offset = ImSin(g.Time * anim_speed) * tail_length * IM_PI * 2.0f;
        draw_list->PathClear();
        draw_list->PathArcTo(center, radius, start_angle + anim_offset, end_angle + anim_offset, 64);
        draw_list->PathStroke(color, false, thickness);
        if (label && label[0] != '\0') {
            ImVec2 text_size = ImGui::CalcTextSize(label);
            ImVec2 text_pos(
                center.x - text_size.x * 0.5f,
                center.y - text_size.y * 0.5f
            );
            draw_list->AddText(text_pos, IM_COL32(255, 255, 255, 255), label);
        }
        return true;
    }
    bool InputInt64(const char* label, int64_t* v, int64_t step, int64_t step_fast, ImGuiInputTextFlags flags)
    {
        // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
        const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
        return InputScalar(label, ImGuiDataType_S64, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
    }
    bool SliderInt64(const char* label, int64_t* v, int64_t v_min, int64_t v_max, const char* format, ImGuiSliderFlags flags)
    {
        return SliderScalar(label, ImGuiDataType_S64, v, &v_min, &v_max, format, flags);
    }


    bool RotatedButton(const char* label, ImVec2 center, ImVec2 size, float angle_deg,
        ImU32 col_button, ImU32 col_text, ImU32 col_hovered = 0, ImU32 col_active = 0)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        const ImGuiID id = window->GetID(label);
       

        // Поворот вокруг центра
        const float angle_rad = angle_deg * IM_PI / 180.0f;
        const float cos_a = cosf(angle_rad);
        const float sin_a = sinf(angle_rad);

        // Вычисляем 4 вершины квадрата с поворотом
        ImVec2 verts[4];
        ImVec2 corners[4] = {
            ImVec2(-size.x * 0.5f, -size.y * 0.5f),
            ImVec2(size.x * 0.5f, -size.y * 0.5f),
            ImVec2(size.x * 0.5f,  size.y * 0.5f),
            ImVec2(-size.x * 0.5f,  size.y * 0.5f)
        };
        ImVec2 bb_min = verts[0], bb_max = verts[0];
        for (int i = 1; i < 4; ++i) {
            bb_min.x = ImMin(bb_min.x, verts[i].x); bb_min.y = ImMin(bb_min.y, verts[i].y);
            bb_max.x = ImMax(bb_max.x, verts[i].x); bb_max.y = ImMax(bb_max.y, verts[i].y);
        }
        const ImRect bb(bb_min, bb_max);
        for (int i = 0; i < 4; ++i) {
            float x = corners[i].x * cos_a - corners[i].y * sin_a;
            float y = corners[i].x * sin_a + corners[i].y * cos_a;
            verts[i].x = (center.x + ImVec2(x, y).x);
            verts[i].y = (center.y + ImVec2(x, y).y);
        }

        // Проверка взаимодействия
        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, ImGuiButtonFlags_MouseButtonLeft);

        // Цвет в зависимости от состояния
        ImU32 col = col_button;
        if (held && hovered) col = col_active ? col_active : ImGui::GetColorU32(ImGuiCol_ButtonActive);
        else if (hovered)    col = col_hovered ? col_hovered : ImGui::GetColorU32(ImGuiCol_ButtonHovered);

        // Рисуем заливку и обводку
        window->DrawList->AddQuadFilled(verts[0], verts[1], verts[2], verts[3], col);
        window->DrawList->AddQuad(verts[0], verts[1], verts[2], verts[3], IM_COL32(0, 0, 0, 255), 1.5f);

        // Повёрнутый текст
        ImVec2 text_size = ImGui::CalcTextSize(label);
        ImVec2 text_pos = {
            (center.y - text_size.y) * 0.5f, (center.x - text_size.x) * 0.5f
         };

        window->DrawList->PushTextureID(ImGui::GetIO().Fonts->TexID);
        window->DrawList->AddText(ImGui::GetFont(), ImGui::GetFontSize(), text_pos, col_text, label);
        window->DrawList->PopTextureID();

        return pressed;
    }
    void ImLine(float size_x, float thickness,const ImVec4 &col) {
        ImVec2 p_min = ImGui::GetCursorScreenPos();
        ImVec2 p_max = ImVec2(p_min.x + size_x, p_min.y + thickness);
        ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, ImGui::GetColorU32(col));
        ImGui::Dummy(ImVec2(size_x, thickness));
    }
    void widget_log(std::string_view t) {
        std::cout << t;
    }
    void RenderBlur(HWND hwnd) {
        MARGINS margins = { -1 };
        HRESULT hr = DwmExtendFrameIntoClientArea(hwnd, &margins);

        if (FAILED(hr)) { // Используем FAILED() для корректной проверки HRESULT на ошибку
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
            // Выводим код ошибки в шестнадцатеричном формате
            widget_log(std::format(" [PahomEngine](dwm) AeroBlur Error. HRESULT: 0x{:X}\n", (unsigned int)hr));
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        }
        else {
            widget_log(std::format(" [PahomEngine](dwm) AeroBlur setup OK. HRESULT: 0x{:X}\n", (unsigned int)hr));
        }
    }
}
ImVec4 RGBAtoIV4(float r, float g, float b, float a) {
    return ImVec4{
        r / 255,
        g / 255,
        b / 255,
        a / 255
    };
}



//
struct CImage {


    float x[2] = { 250,230};
    float y[2] = { 250,230 };
    int fIctx = 0;
    int fIcty = 0;
     float rot;
     float scale;
    GLuint gl_buffer;
    unsigned char CICharBuffer[256];
    bool CreateImg();
    bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height, unsigned char* imgBuffer);
    ImVec2 ResizeImage(uint64_t fCArrayFloat);
    uint64_t InitCImage(std::string png_file);
    uint32_t pixel_buffer[256];
    std::string getAspectRatio(int x, int y);
    int64_t GetImageSize(int width, int height);
    int64_t GetVRAMSize(GLuint vbo);
    int64_t GetFileSize(const std::string& filename);
    void CreateImage(GLuint tx, float dt, ImVec2 windowSize, ImVec2 ImageSize);
    bool LoadTextureRawData(unsigned char* raw, GLuint* out_texture, int* out_width, int* out_height);
};

void CImage::CreateImage(GLuint tx,float dt,ImVec2 windowSize,ImVec2 ImageSize) {
  
    rot += 30.0f * dt;
    scale = 1.0f + 0.05f * sin(rot * 0.1f);

    // Центрирование по ImageSize
    ImGui::SetCursorPos({
        windowSize.x / 2 - ImageSize.x / 2,
        50
        });

    glPushMatrix();

    // ДИНАМИЧЕСКИЙ ЦЕНТР!
    float centerX = ImageSize.x / 2;
    float centerY = ImageSize.y / 2;

    glTranslatef(centerX, centerY, 0);   // к центру
    glRotatef(rot, 0, 0, 1);             // вращаем
    glScalef(scale, scale, 1);           // пульсируем
    glTranslatef(-centerX, -centerY, 0); // обратно

    ImGui::Image(reinterpret_cast<int64_t>(reinterpret_cast<void*>(tx)), ImageSize);

    glPopMatrix();
}//ImGui::Image(reinterpret_cast<int64_t>(reinterpret_cast<void*>(tx)), ImageSize);
std::string CImage::getAspectRatio(int x, int y) {
    double ratio = (double)x / y;
    struct { double val; std::string name; } list[] = {
        {4.0 / 3.0, "4:3"},
        {16.0 / 9.0, "16:9"},
        {21.0 / 9.0, "21:9"},
        {3.0 / 2.0, "3:2"},
        {1.0, "1:1"},
        {5.0 / 4.0, "5:4"},
        {32.0 / 9.0, "32:9"},
        {8.0 / 5.0, "16:10"}
    };
    for (auto& a : list)
        if (std::fabs(ratio - a.val) < 0.01)
            return a.name;
    return std::to_string(ratio);
}
ImVec2 CImage::ResizeImage(uint64_t fCArrayFloat) {
    CImage::fIctx = CImage::x[fCArrayFloat];
    CImage::fIcty = CImage::y[fCArrayFloat];
    return ImVec2(fIctx, fIcty);
}

int64_t CImage::GetImageSize(int width, int height) {
    // ������ � ����������� = ������ * ������ * 4 (��� RGBA, 4 ����� �� �������)
    return static_cast<int64_t>(width) * height * 4;
}
int64_t CImage::GetFileSize(const std::string& filename) {
    std::ifstream ImageStream(filename, std::ios::binary | std::ios::ate);
    if (!ImageStream.is_open()) {
        std::cerr << "[CImage::GetFileSize] Failed to open file: " << filename << std::endl;
        return 0;
    }
    std::streampos fileSize = ImageStream.tellg();
    ImageStream.close();
    return fileSize;
}
bool CImage::LoadTextureRawData(unsigned char* raw, GLuint* out_texture, int* out_width, int* out_height) {
    int image_width = 0;
    int image_height = 0;
    static int ic = 0;
    if (!raw)
    {
        return false;
    }
    else {

        std::cout << std::dec << image_width << " x " << image_height << std::endl;

        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)raw);
        *out_texture = image_texture;
        *out_width = image_width;
        *out_height = image_height;

        glBindTexture(GL_TEXTURE_2D, 0);

    }
}//
bool CImage::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height, uint8_t* imgBuffer) {
    int image_width = 0;
    int image_height = 0;
    static int ic = 0;
    uint8_t* imageData;
    imageData = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (!imageData)
    {
        return false;
    }
    else {
        imgBuffer = imageData;
        std::cout << std::dec << image_width << " x " << image_height << " " << PE_ARRAYSIZE(imageData) << "  " << PE_ARRAYSIZE(imgBuffer) << std::endl;
       
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        stbi_image_free(imageData);

        *out_texture = image_texture;
        *out_width = image_width;
        *out_height = image_height;

        glBindTexture(GL_TEXTURE_2D, 0);
        
    }
}//
struct KurlikAUDIO {
    std::string audiolist[13] = { "assets/audio/kurlik.wav",
                                 "assets/audio/voda.wav",
                                 "assets/audio/intro.wav",
                                 "assets/audio/pidoras.wav",
                                 "assets/audio/mrrobot.wav",
                                 "assets/audio/pain100_1.wav",
                                 "assets/audio/smex.wav",
                                 "assets/audio/aaa.wav",
                                 "assets/audio/khuli-ty-govnom-to-vymazalsia.wav",
                                 "assets/audio/ponimaesh-chto-ty-poekhavshii.wav",
                                 "assets/audio/sound_game.wav",
                                 "assets/audio/click.wav",
                                 "assets/audio/a.wav"
    };
    std::string audiolist_low[13] = { "assets/audio/low/kurlik.wav",
                                 "assets/audio/low/voda.mp3",
                                 "assets/audio/low/intro.mp3",
                                 "assets/audio/low/pidoras.mp3",
                                 "assets/audio/low/mrrobot.mp3",
                                 "assets/audio/low/pain100_1.mp3",
                                 "assets/audio/low/smex.mp3",
                                 "assets/audio/low/aaa.mp3",
                                 "assets/audio/low/khuli-ty-govnom-to-vymazalsia.mp3",
                                 "assets/audio/low/ponimaesh-chto-ty-poekhavshii.mp3",
                                 "assets/audio/low/sound_game.mp3",
                                 "assets/audio/low/click.mp3",
                                 "assets/audio/low/a.mp3"
    };
    bool bUseLowAudioQuality = false;
    std::vector<std::string> musicFiles;
    bool FilesScanned = false;
    void play(int64_t i);
    void play2(int64_t i);
    void play3(int64_t i);
    void getGain(int64_t idx);
    void vue();
    void VuePlay(int64_t idxd, int64_t idx);
    void getTimeline(int64_t idx);
    void setReplay(bool isReplay);
    void Mute();
    float convertToMinutes();
    int64_t idx = 0;
    float masterVolume = 0.02f;
    float masterVolumeLast = 0.0f;
    float fMusicPlayerVolumeOffset = 0.4f;
    void pause();
    Audio::Sound audioDevice,audioDevice2,audioDevice3;
    int64_t i64CurrentTimeLine = 0;
    bool isDeviceActive = false;
    bool isAudioMuted = false;
    struct gain {
        float min = 0.0f;
        float max = 0.0f;
    };
    std::unique_ptr<gain> audioGain = std::make_unique<gain>();
    struct time {
        float current = 0.0f;
        float max = 0.0f;
        std::string_view formatTime(float seconds) {
            float minutes = floor(seconds / 60);
            float secs    = seconds;
            return (std::to_string(minutes) + ":" + (secs < 10 ? "0" : " ") + std::to_string(secs)).c_str();
    }
    };
    bool audioFileIsOK(std::string file) {
        std::ifstream fileAssets(file);
        if (fileAssets.is_open()) {
            return true;
        }
        else {
            return false;
        }
        fileAssets.close();
    }
    void openFileDialog(std::string& fileName) {
        // common dialog box structure, setting all fields to 0 is important
        OPENFILENAMEA ofn;
        char szFileName[MAX_PATH];

        ZeroMemory(&ofn, sizeof(ofn));
        szFileName[0] = 0;

        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0";
        ofn.lpstrFile = szFileName;
        ofn.nMaxFile = 520;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        GetOpenFileNameA(&ofn);
        fileName = std::string_view((char*)ofn.lpstrFile);
    }
    void ScanFiles(const std::string_view& directory = "musics",const std::string_view& file_extension = ".wav") {
        static int64_t i64FileCount = 0;
        musicFiles.clear(); // Очищаем плейлист
        try {
            // Сканируем директорию
            for (const auto& entry : std::filesystem::recursive_directory_iterator(
                directory, std::filesystem::directory_options::skip_permission_denied)) {
                if (entry.is_regular_file() && entry.path().extension() == file_extension) {
                    musicFiles.push_back(entry.path().string());
                    std::cout << __TIME__ << " Found " << i64FileCount << " File: " << musicFiles[i64FileCount] << std::endl;
                    i64FileCount++;
                }
            }
            FilesScanned = true;
            i64FileCount = 0;
            // Получаем длительности для всех файлов

        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Filesystem Error: " << e.what() << std::endl;
            FilesScanned = false;
        }
    }
    std::unique_ptr<time> audioTime = std::make_unique<time>();
    std::string_view sLastAudioFile[3] = {"0","0","0"};
    void music_player(int64_t i) {
        //  PlaySoundA(file.c_str(), NULL, 1);
        idx = i;
        audioDevice.loadSound(musicFiles[idx]);
        audioDevice.play();
        audioDevice.setVolume(isAudioMuted ? 0 : masterVolume);
        isDeviceActive = audioDevice.isPlaying();
    }
    void Device(int64_t idx_device, bool isPlay) {
        switch (idx_device) {
        case 0:
            if (audioDevice) {
                if (isPlay) {
                    audioDevice.play();
                }
                else {
                    audioDevice.pause();
                }
            }
            break;
        case 1:
            if (audioDevice2) {
                if (isPlay) {
                    audioDevice2.play();
                }
                else {
                    audioDevice2.pause();
                }
            }
            break;
        case 2:
            if (audioDevice3) {
                if (isPlay) {
                    audioDevice3.play();
                }
                else {
                    audioDevice3.pause();
                }
            }
            break;
        }
       

    }
    bool loadFileToWav(std::string_view file, int64_t idx_device) {
        switch (idx_device) {
        case 0:
            if (audioDevice) {
                audioDevice.loadSound(file);
                return true;
            }
            else {
                return false;
            }
            break;
        case 1:
            if (audioDevice2) {
                audioDevice2.loadSound(file);
                return true;
            }
            else {
                return false;
            }
            break;
        case 2:
            if (audioDevice3) {
                audioDevice3.loadSound(file);
                return true;
            }
            else {
                return false;
            }
            break;
        }
    }
};
void KurlikAUDIO::Mute() {
    if (audioDevice) {
        audioDevice.pause();
        audioDevice.setVolume(0);
    }
    if (audioDevice2) {
        audioDevice2.pause();
        audioDevice2.setVolume(0);
    }
    if (audioDevice3) {
        audioDevice3.pause();
        audioDevice3.setVolume(0);
    }
}
// не использвать!! хуета.
void KurlikAUDIO::getGain(int64_t idx) {
    if (isDeviceActive) {
        switch (idx) {
        case 0:
            audioGain->min = audioDevice.getMinGain();
            audioGain->max = audioDevice.getMaxGain();
            break;
        case 1:
            audioGain->min = audioDevice2.getMinGain();
            audioGain->max = audioDevice2.getMaxGain();
            break;
        case 2:
            audioGain->min = audioDevice3.getMinGain();
            audioGain->max = audioDevice3.getMaxGain();
            break;
        default:
            audioGain->min = audioDevice.getMinGain();
            audioGain->max = audioDevice.getMaxGain();
            break;
        }
  }
}
// аккуратно использвать. Проверяйте audioDevice на nullptr
void KurlikAUDIO::vue() {
    if (ImGui::BeginPopup("vue", ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("AudioEngine");
        ImGui::Separator();

        // ������ ��������� ��� ����������� ����
        const char* devices[] = { "ch0", "ch1", "ch2" };
        const char* filelist[] = {"1","2","3","4","5","6","6" ,"6" ,"6" ,"6" ,"6" };
        static int32_t i32idxDevice = 0, i32idxFiles = 0;
        for (int files = 0; files < 10; files++) {
            filelist[files] = audiolist[files].c_str();
        }
        
        // ���������� ���� ��� ������ ����������
        if (ImGui::Combo("Devices", &i32idxDevice, devices, 3)) {
            isDeviceActive = false;
        }
        ImGui::Combo("Files", &i32idxFiles, filelist, 6);
        if (ImGui::SliderFloat("Vol", &masterVolume, 0.03f, 1.0f)) {
            VuePlay(i32idxDevice, i32idxFiles);
        }
        // ��������� gain � timeline
       /* if (audioDevice != nullptr || audioDevice2 != nullptr || audioDevice3 != nullptr) {
            if (isDeviceActive) {
                getGain(i32idxDevice);
                getTimeline(i32idxDevice);
           }
        }*/
        if (ImGui::Button("Play", ImVec2(100, 30))) {
            isDeviceActive = true;
            VuePlay(i32idxDevice, i32idxFiles);

        }
        // ������ ��� ����������� gain
       // ��������� ����������� ��� gain
        float gainValues[2] = { audioGain->min, audioGain->max };
        float maxGainValue = (audioGain->max > 0.0f) ? audioGain->max + 1.0f : 1.0f;
        ImGui::Text("Gain %.2f (dB)", (((audioGain->min) > (audioGain->max)) ? (audioGain->min) : (audioGain->max)));
        ImGui::PlotHistogram("Gain", gainValues, 2, 0, "Gain (dB)", 0.0f, maxGainValue, ImVec2(200, 80));

        // ��������� ���������
        float timelineValue[1] = { audioTime->current };
        float maxTimelineValue = (audioTime->max > 0.0f) ? audioTime->max : 1.0f; // ������ �� ������� ������������
       // ImGui::PlotHistogram("Timeline", timelineValue, 1, 0, "Progress (s)", 0.0f, maxTimelineValue, ImVec2(200, 40));
        ImGui::Text("%.0f:%.0f", audioTime->current / 60 , audioTime->current);
        ImGui::Text("%.0f:%.0f", audioTime->max / 60, audioTime->max);
        ImGui::ProgressBar(audioTime->current / maxTimelineValue, ImVec2(220, 30));
        ImGui::EndPopup();
    }
}
float KurlikAUDIO::convertToMinutes() {
    return audioTime->current / 60;
}
// Бесполезная функция. Не использовать
void KurlikAUDIO::getTimeline(int64_t idx) {
    if (isDeviceActive) {
        switch (idx) {
        case 0:
            audioTime->current = audioDevice.getDurationInSeconds();
            audioTime->max = audioDevice.getMaxDistance();
            break;
        case 1:
            audioTime->current = audioDevice2.getDurationInSeconds();
            audioTime->max = audioDevice2.getMaxDistance();
            break;
        case 2:
            audioTime->current = audioDevice3.getDurationInSeconds();
            audioTime->max = audioDevice3.getMaxDistance();
            break;
        default:
            audioTime->current = audioDevice.getDurationInSeconds();
            audioTime->max = audioDevice.getMaxDistance();
            break;
        }
    }
}
void KurlikAUDIO::VuePlay(int64_t idxd,int64_t idx) {
    switch (idxd)
    {
    case 0:
        audioDevice.loadSound(bUseLowAudioQuality ? audiolist_low[idx] : audiolist[idx]);
        audioDevice.play();
        audioDevice.setVolume(masterVolume);
        isDeviceActive = audioDevice.isPlaying();
        break;
    case 1:
        audioDevice2.loadSound(bUseLowAudioQuality ? audiolist_low[idx] : audiolist[idx]);
        audioDevice2.play();
        audioDevice2.setVolume(masterVolume);
        isDeviceActive = audioDevice2.isPlaying();
        break;
    case 2:
        audioDevice3.loadSound(bUseLowAudioQuality ? audiolist_low[idx] : audiolist[idx]);
        audioDevice3.play();
        audioDevice3.setVolume(masterVolume);
        isDeviceActive = audioDevice3.isPlaying();
        break;
    }
}
void KurlikAUDIO::play(int64_t i) {
  //  PlaySoundA(file.c_str(), NULL, 1);
    idx = i;
    audioDevice.loadSound(bUseLowAudioQuality ? audiolist_low[idx] : audiolist[idx]);
    audioDevice.play();
    audioDevice.setVolume(isAudioMuted ? 0 : masterVolume);
    isDeviceActive = audioDevice.isPlaying();
    sLastAudioFile[0] = audiolist[idx];

   // std::cout << " [PahomEngine->KurlikAudio] audio device 0 :-> " << audiolist[idx] << " | volume_offset_:"<< masterVolume * 100 << "%" << std::endl;
}
void KurlikAUDIO::play2(int64_t i) {
    //  PlaySoundA(file.c_str(), NULL, 1);
    idx = i;
    audioDevice2.loadSound(bUseLowAudioQuality ? audiolist_low[idx] : audiolist[idx]);
    audioDevice2.play();
    audioDevice2.setVolume(isAudioMuted ? 0 : masterVolume);
    isDeviceActive = audioDevice2.isPlaying();
    sLastAudioFile[1] = audiolist[idx];
   // std::cout << " [PahomEngine->KurlikAudio] audio device 1 :-> " << audiolist[idx] << " | volume_offset_:" << masterVolume * 100 << "%" << std::endl;
}
void KurlikAUDIO::play3(int64_t i) {
    //  PlaySoundA(file.c_str(), NULL, 1);
    idx = i;
    audioDevice3.loadSound(bUseLowAudioQuality ? audiolist_low[idx] : audiolist[idx]);
    audioDevice3.play();
    audioDevice3.setVolume(isAudioMuted ? 0 : masterVolume);
    isDeviceActive = audioDevice3.isPlaying();
    sLastAudioFile[2] = audiolist[idx];
    //std::cout << " [PahomEngine->KurlikAudio] audio device 2 :-> " << audiolist[idx] << " | volume_offset_:" << masterVolume * 100 << "%" << std::endl;
}
void KurlikAUDIO::setReplay(bool isReplay) {
    if (isDeviceActive) {
        if (isReplay) {
            audioDevice3.replay();
        }
    }
}
void KurlikAUDIO::pause() {
    audioDevice.pause();
}
struct STRINGSDATA {
    void log(std::string_view text,std::string_view moduleName) {
        std::cout<<"[PahomEngine::"+std::format("%s]",moduleName) << text << std::endl;
   }
    std::string_view PAHOM_ENGINE =
        " ����������     ����    ���    ���   ����   ���    �����   \n"
        " ��      ��   ��   ��   ���    ��� ��    �� �� �� ��  ��   \n"
        " ��     ��    ��   ��   ���������� ��    �� ��  ���   ��   \n"
        " �������      �������   ���    ��� ��    �� ��   �    ��   \n"
        " ��           ��   ��   ���    ���   ����   ��        ��   \n"
        "                                                           \n"
        " ��������     ��    ��  �������  ��  ��    ��   �������    \n"
        " ��           �� �  ��  ��       ��  �� �  ��   ��         \n"
        " ��������     ��  � ��  �� ����  ��  ��  � ��   �������    \n"
        " ��           ��   ���  ��   ��  ��  ��   ���   ��         \n" 
        " ��������     ��    ��  �������  ��  ��    ��   �������    \n"
        "                Engine b0.24  .                            \n";
};
//
// defines

#define LOGO_IMAGE   0
#define BACK_IMAGE   1
#define BREAD_IMAGE  2
#define PAHOM_IMAGE  3
#define PAHOM2_IMAGE 4
#define PANEl_IMAGE  5
//
// 


#define engine_bulid std::wstring(L"0.9.00 (pre-release)");

//
struct ASSETSDATA {
    std::string UltraTextures[12] = {
                                "assets/ultramax/logo.png",   //0
                                "assets/ultramax/back.png",   //1
                                "assets/ultramax/bread.png",  //2
                                "assets/ultramax/pahom.png",  //3
                                "assets/ultramax/pahom2.png",  //4
                                "assets/ultramax/panel.png",//5
                                "assets/ultramax/777.png",//6
                                "assets/ultramax/vilka.png",//7
                                "assets/ultramax/kefir.png",//8
                                "assets/ultramax/gortany.png",
                                "assets/ultramax/cryptypahom.png",
                                "assets/ultramax/PEngine.png"

    };
    std::string asset[12] = { "assets/logo.png",   //0
                             "assets/back.jpg",   //1
                             "assets/bread.png",  //2
                             "assets/pahom.png",  //3
                             "assets/pahom2.png",  //4
                             "assets/panel.png",//5
                             "assets/777.png",//6
                             "assets/vilka.png",//7
                             "assets/kefir.png",//8
                             "assets/gortany.png",
                             "assets/cryptypahom.png",
                             "assets/PEngine.png"
    }; //5
    std::string asset_ultra_low[12] = 
                            { "assets/low/logo.png",   //0
                            "assets/low/back.jpg",   //1
                            "assets/low/bread.png",  //2
                            "assets/low/pahom.png",  //3
                            "assets/low/pahom2.png",  //4
                            "assets/low/panel.png",//5
                            "assets/low/777.png",//6
                            "assets/low/vilka.png",//7
                            "assets/low/kefir.png",//8
                            "assets/low/gortany.png",
                            "assets/low/cryptypahom.png",
                            "assets/low/PEngine.png"
    }; //5
    bool bUseLowTextures = false;
    bool validFiles(std::string_view file) {
        std::ifstream fileAssets(file.data());
        if (fileAssets.is_open()) {
            return true;
        }
        else {
            return false;
        }
        fileAssets.close();
    }
};
struct IMAGEDATA {
    GLuint  TextureArray[256];
    uint8_t* TextureBufferArray[256];
    int TextureX[256];
    int TextureY[256];
    int64_t GetImTexture(int idx) const {
        return reinterpret_cast<int64_t>(reinterpret_cast<void*>(this->TextureArray[idx]));
    }
};
struct KEYMAPDATA {
    int8_t u8FORWARD = 'A';
    int8_t u8BACK = 'D';
    int8_t u8RESET = 'R';
    int8_t u8SPACE = VK_SPACE;
    int8_t u8JUMP = 'W';
    int8_t u8ButtonLeft = 'L';
    int8_t u8ButtonRight = 'R';
    int64_t kbDelay = 0;
    int64_t vMaxDelay = 1;
    int64_t i64BACKGamepad = XINPUT_GAMEPAD_DPAD_LEFT;
    int64_t i64FORWARDGamepad = XINPUT_GAMEPAD_DPAD_RIGHT;
    int64_t i64UPGamepad = XINPUT_GAMEPAD_DPAD_UP;
    int64_t i64AGamepad = XINPUT_GAMEPAD_A;
    int64_t i64START = XINPUT_GAMEPAD_START;
    void reMap(uint8_t key, uint8_t newBind) {
        key = newBind;
    }
    //
 
};
struct dataTime {
    DWORD x, y;
    constexpr dataTime() : x(0L), y(0L) {}
    constexpr dataTime(DWORD _x, DWORD _y) : x(_x), y(_y) {}
};
struct MEMORYDATA {
    int64_t i64MemoryTotal = 0;
    int64_t i64MemoryFree  = 0;
    int64_t i64MemoryUsed  = 0;
    //
    MEMORYSTATUS MemoryPtr;
    void getData(int64_t* mem,int64_t idx_data){
        memset(&MemoryPtr, 0, sizeof(MemoryPtr));
        ::GlobalMemoryStatus(&MemoryPtr);// GlobalMemoryStatusEx fuck you asshole
        switch (idx_data) {
        case 0:
            *mem = (MemoryPtr.dwTotalPhys - MemoryPtr.dwAvailPhys) / 1024 / 1024 / 1024; // ��� �������������� ������
            break;
        case 1:
            *mem = MemoryPtr.dwTotalPhys / 1024 / 1024 / 1024; // ��� ����� ������
            break;
        case 2:
            *mem = MemoryPtr.dwAvailPhys / 1024 / 1024 / 1024; // ��� ��������� ������
            break;
        }
    }
    std::string MemoryInfo() {
        getData(&i64MemoryTotal, 1);
        getData(&i64MemoryFree , 2);
        getData(&i64MemoryUsed , 0);
        std::string mem_str = 
            "Total:   " + std::to_string(i64MemoryTotal) + " GB"
            "\nUsed :   " + std::to_string(i64MemoryUsed)  + " GB"
            "\nFree :   " + std::to_string(i64MemoryFree)  + " GB";
        return mem_str;
    }
    void resetBool(bool* v) {
        if (v == nullptr) {
            std::cout << " PahomEngine->Mem.resetBool()-> value it's nullptr!!" << std::endl;
        }
        else {
            *v = false;
        }
    }
    std::string exec(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;

        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }

        return result;
    }

    void Mem_log(std::string_view text) {
        std::cout << text << std::endl;
    }
    std::string GetProcessName() {
        HANDLE hProcess = GetCurrentProcess();
        char processName[MAX_PATH];
        GetModuleFileNameExA(hProcess, NULL, processName, MAX_PATH);
        return std::format("{}", processName);
    }
    

    
};
auto ptrMemory = std::make_unique<MEMORYDATA>();
struct GameEvent {
    std::string_view TextBufferStr = "";
    ImVec4 col;
    int64_t i64TimerEvent = 0;
    bool isTextHidden = false, bTextureEditor = false;
    int64_t i64WindowSize[2] = { 800 , 600 };
    void TextBuffer() {
        ImVec2 f2TextCenterPos = {
           (i64WindowSize[0] - ImGui::CalcTextSize(TextBufferStr.data()).x) / 2,
           (i64WindowSize[1] - ImGui::CalcTextSize(TextBufferStr.data()).y) / 2,
        };
        ImGui::SetCursorPos(
            f2TextCenterPos
        );
        if (!isTextHidden) {
         
            ImGui::TextColored(col, TextBufferStr.data());
        }
        
    }
    std::string sConsoleBufferString;
    void Text(ImVec4 col, std::string text) {
        TextBufferStr = text;
        setColorText(col);
    }
    void setColorText(ImVec4 col) {
        col = col;
    }
    void clearEvent() {
        TextBufferStr = "";
    }
    float clr(float a) {
        return a / 255.0f;
    }
    void TimerToClear() {
        static int64_t i64t = 0;
        i64t++;
        if (i64t > 100) {
            clearEvent();
            isTextHidden = true;
            i64t = 0;
        }
    }
    void WriteLog(std::string text) {
        sConsoleBufferString += "\n"+text;
    }
    void ConsoleBuffer() {
        ImGui::Text(sConsoleBufferString.c_str());
    }
    float progress = 0.0f; // Прогресс анимации
    bool drawReset = false;
    int64_t linesToDraw = 0;
    void mt_fill(float x_max, float y_max) {
        float lineColor[4] = { clr(0), clr(0), clr(5), clr(255) };
        float lineThickness = 2.0f;
        int64_t lineCount = static_cast<int64_t>(y_max / lineThickness);

        // Анимация
        progress += ImGui::GetIO().DeltaTime * IM_PI;
        if (progress > 1.0f) {
            progress = (drawReset ? 1.0f : 0.0f); 
        }

        linesToDraw = static_cast<int64_t>(lineCount * progress);

        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        for (int64_t ln = 0; ln < linesToDraw; ln++) {
            float y = ln * lineThickness;
            draw_list->AddLine(
                ImVec2(0.0f, y),
                ImVec2(x_max, y),
                ImGui::GetColorU32(ImVec4(lineColor[0], lineColor[1], lineColor[2], lineColor[3])),
                lineThickness
            );
        }
        drawReset = true;
    }
    void mt_clear() {
        progress = 0;
        drawReset = false;
        linesToDraw = 0;
    }
    bool bScreamEvent = false;
    bool bScreamEventBackground = false;
    bool bDemoPlay = false;
    bool bEnableKefir = false;
    bool bKefirFriend = false;
    int32_t i32ReverseImage = -1;
    bool AutoScaleImage = false;
    bool bShowNoiseBackground = false;
};
std::string_view logoPahom =
" %%%%%%  %%%%%  %%%  %%%   %%%   %%%    %%%\n"
" %%   %  %% %%  %%%  %%% %%   %% %% %  % %%\n"
" %%%%%   %%%%%  %%%%%%%% %%   %% %%  %%  %%\n"
" %%      %$ %%  %%%  %%% %%   %% %%  %%  %%\n"
" %%      %   %  %%%  %%%   %%%   %%      %%\n";
void stdoutColored(std::string out, int16_t i16colorText) {
    HANDLE hc = GetStdHandle(STD_OUTPUT_HANDLE);
    std::vector <std::string> array;

    SetConsoleOutputCP(CP_UTF8);
    for (int64_t c = 0; c < out.size(); c++) {
        array.push_back(std::string(1, out[c]));
        if (!array[c].empty()) {
            SetConsoleTextAttribute(hc, i16colorText);
        }
        else {
            SetConsoleTextAttribute(hc, 15);
        }
        std::cout << out[c];
    }
}
struct EXCEPTIONS {
    bool ErrorTextures = false;
    std::string sLastError;
    void* pLastStack = nullptr;
    int64_t i64MemoryUsageProcess = 0;
    void log(std::string t) {
        std::cout << "Exception Error: " << t << std::endl;

    }

    void GetProcessMemoryUsage() {
        PROCESS_MEMORY_COUNTERS pmc;
        pmc.cb = sizeof(pmc);
        GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
        i64MemoryUsageProcess = static_cast<int64_t>(pmc.WorkingSetSize);
    }
    void Write(const std::string& t, void* pErrorSegment) {
        ErrorTextures = true;
        sLastError += t + "\n";
        pLastStack = pErrorSegment;
        log("EXCEPTIONS::Write: " + t + ", StackPtr: " + std::to_string(reinterpret_cast<uintptr_t>(pErrorSegment)));
    }

    static std::wstring GetStackTrace(PEXCEPTION_POINTERS pExInfo) {
        std::wstringstream ss;
        HANDLE process = GetCurrentProcess();
        HANDLE thread = GetCurrentThread();

        // ������������� ��������
        SymInitialize(process, NULL, TRUE); // ��������� ������� ��� ���� �������
        SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME); // �������� ����� � ����� ��� ���������
        STACKFRAME64 frame = { 0 };
        frame.AddrPC.Offset = pExInfo->ContextRecord->Rip;
        frame.AddrPC.Mode = AddrModeFlat;
        frame.AddrFrame.Offset = pExInfo->ContextRecord->Rbp;
        frame.AddrFrame.Mode = AddrModeFlat;
        frame.AddrStack.Offset = pExInfo->ContextRecord->Rsp;
        frame.AddrStack.Mode = AddrModeFlat;

        while (StackWalk64(
            IMAGE_FILE_MACHINE_AMD64,
            process,
            thread,
            &frame,
            pExInfo->ContextRecord,
            NULL,
            SymFunctionTableAccess64,
            SymGetModuleBase64,
            NULL)) {
            DWORD64 address = frame.AddrPC.Offset;
            if (address == 0) break; // ����� �����

            // �������������� ������
            ss << L"[0x" << std::hex << std::setw(16) << std::setfill(L'0') << address << L"] ";

            // ��������� ����� �������
            char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME] = { 0 };
            SYMBOL_INFO* symbol = (SYMBOL_INFO*)symbolBuffer;
            symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
            symbol->MaxNameLen = MAX_SYM_NAME;
            DWORD64 displacement = 0;

            if (SymFromAddr(process, address, &displacement, symbol)) {
                // ����������� char* � wstring
                std::string_view funcName(symbol->Name);
                std::wstring wFuncName(funcName.begin(), funcName.end()); // ������� �������������� (��� ����������� MultiByteToWideChar ��� ��������)
                ss << L"Function: " << wFuncName << L" + 0x" << std::hex << displacement;
            }
            else {
                ss << L"Unknown function";
            }

            // ��������� ����� ����� � ������ ������
            IMAGEHLP_LINE64 line = { 0 };
            line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
            DWORD lineDisplacement = 0;
            if (SymGetLineFromAddr64(process, address, &lineDisplacement, &line)) {
                // ����������� char* � wstring
                std::string_view fileName(line.FileName);
                std::wstring wFileName(fileName.begin(), fileName.end());
                ss << L" at " << wFileName << L":" << std::dec << line.LineNumber;
            }

            // ��������� ����� ������
            IMAGEHLP_MODULE64 module = { 0 };
            module.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
            if (SymGetModuleInfo64(process, address, &module)) {
                // ModuleName � ��� TCHAR*, � Unicode-������� ��� wchar_t*
                ss << L" in module " << module.ModuleName;
            }

            ss << L"\n";
        }

        // ������� ��������
        SymCleanup(process);
        return ss.str();
    }

    static LONG WINAPI CrashHandler(PEXCEPTION_POINTERS pExInfo) {
        std::wstring errorMsg;
        std::wstring errorCode;
        std::string_view errorMsgA;
        std::string_view errorCodeA;
        switch (pExInfo->ExceptionRecord->ExceptionCode) {
        case EXCEPTION_ILLEGAL_INSTRUCTION: // 0xC000001D
            errorCode = L"Iligal Instruction (0xC000001D)";
            errorCodeA = "Iligal Instruction (0xC0000005)";
            break;
        case EXCEPTION_ACCESS_VIOLATION: // 0xC0000005
            errorCode = L"Access Violation (0xC0000005)";
            errorCodeA = "Access Violation (0xC0000005)";
            break;
        case EXCEPTION_STACK_OVERFLOW: // 0xC00000FD
            errorCode = L"Stack Overflow (0xC00000FD)";
            errorCodeA = "Stack Overflow (0xC00000FD)";
            break;
        default:
            errorCode = L"Undefined Exception (0x" + std::wstring(std::to_string(pExInfo->ExceptionRecord->ExceptionCode).begin(), std::to_string(pExInfo->ExceptionRecord->ExceptionCode).end()) + L")";
            errorCodeA = "Undefined Exception (0x" + std::to_string(pExInfo->ExceptionRecord->ExceptionCode) + ")";
            break;
        }
        std::vector<std::string_view> vec;
        std::wstring sEngineInfoW = L"PahomEngine " + engine_bulid;
        std::wstring stackTraceW = GetStackTrace(pExInfo);
        std::wstring wsapp = sEngineInfoW;
        std::wstring titleProject = (wsapp+L"::Exception");
        std::ofstream logFile("crash_log.txt", std::ios::app);
        logFile << "PahomEngine crashed!" << std::endl;
        logFile << "========================" << std::endl;
        logFile << "Crash at:     " << __DATE__ << " " << __TIME__ << "\n";
        logFile << "Exception:    " << errorCodeA << "\n";
        logFile << "Stack Trace:\n" << std::string(stackTraceW.begin(), stackTraceW.end()).c_str() << "\n";
        logFile << "------------------------\n";
        logFile.close();
        std::wstring wlogo(logoPahom.begin(), logoPahom.end());
        std::wstring msg = L"[exception]\n" + std::wstring(errorCode.begin(), errorCode.end()) + L" \n[stack_trace]\n" + std::wstring(stackTraceW.begin(), stackTraceW.end()) +
            L"\nSee crash_log.txt for details.\nPlease send crash_log.txt to @hcppstudio.";
        stdoutColored(std::string(msg.begin(), msg.end()), 0x4F);
        std::ofstream fileLogError("crash_log_message.log");
        fileLogError << std::string(msg.begin(), msg.end()) << std::endl;
        fileLogError.close();
        std::string sCommand = "cmd /c start ./RuntimeException.exe crash_log_message.log";
        WinExec(sCommand.c_str(), 3);


        return EXCEPTION_EXECUTE_HANDLER;
    }

    void BugReport() {
        // ������������� �������� (���� ��� ��� ������)
        SymInitialize(GetCurrentProcess(), NULL, TRUE);
        SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
        SetUnhandledExceptionFilter(CrashHandler);
    }

    void Cleanup() {
        SymCleanup(GetCurrentProcess());
    }
};
struct Diffinity {
    int64_t i64diff[3] = {
        16,24,8
    };
    std::vector<std::string_view >ccDiff = {
        "Легкая",
        "Средняя",
        "Поехавший",
        "Рандомная"
    };
    int64_t i64id = 0;
    bool bRandScoreDiff = false;
    bool bRandDiff = false;
    std::string diffSelected = "";
    struct { int64_t i64buffer; int64_t i64buffer1; } diffArray[3] = {
        {4,6},
        {6,10},
        {12,20},
    };
    void setDiff(int64_t i64idx) {
        if (i64idx >= 0 && i64idx < 3) {
            diffSelected = ccDiff[i64idx];
        }
        else {
            diffSelected = ccDiff[1];
        }
    }
   
};
struct cpu_bench64 {
    std::chrono::steady_clock::time_point timeBenchIn;
    std::chrono::steady_clock::time_point timeBenchOut;
    int64_t i64BenchLastTime = 0;
    std::atomic<int64_t> i64MaxSize = 10000000000;
    std::atomic<int64_t> i64MaxTexturesLoad = 10000;
    std::atomic<int64_t> i64ChunkSize = 0;
    std::atomic<int32_t> i32max_thread = 0;
    std::atomic<int32_t> i32CallFunc = 0;
    std::atomic<int64_t> i64MemorySize = 0;
    std::unique_ptr<CImage>  Textures = std::make_unique<CImage>();
    std::atomic<int64_t> vram = 0;
    std::atomic<int64_t> outtime = 0;
    std::atomic<int64_t> i64sucTx = 0;
    std::atomic<std::chrono::steady_clock::time_point> intime;
    std::atomic<std::chrono::steady_clock::time_point> outime;
    // work not correcty #FIXME
    void gpu_render(int64_t count) {
        GLuint textureBuffer[1000];
        unsigned char textureBufferu8[1000];
        int ximage[1000],yimage[1000];
        intime = std::chrono::high_resolution_clock::now();
        for (int64_t image_render = 0; image_render < count; image_render++) {
            if (Textures->LoadTextureFromFile("assets/pahom.png", &textureBuffer[image_render], &ximage[image_render], &yimage[image_render], &textureBufferu8[image_render])) {
                vram.fetch_add(((ximage[image_render] * yimage[image_render]) * 4), std::memory_order_relaxed);
                i64sucTx += image_render;
            }
        }
        std::cout << " --------------------------------------------------------------" << std::endl;
        std::cout << "loaded " << i64sucTx.load() << " / " << count << " vramLoaded: " << vram.load() / 1024 / 1024 << "MB" << std::endl;
        outime = std::chrono::high_resolution_clock::now();
        outtime.fetch_add(std::chrono::duration_cast<std::chrono::milliseconds>(outime.load() - intime.load()).count(), std::memory_order_relaxed);
        std::cout << "time :" << outtime.load() <<" ms" << std::endl;
    }
    std::string hashFn128(int64_t sz) {
        std::string bff;
        i32CallFunc.fetch_add(1, std::memory_order_relaxed);
        const std::string bff_chr = "qwertyuiop[]asdfghjkl;'zxcvbnm,./12345678890";
        int64_t sd_rd = 0;
        srand(time(0));
        timeBenchIn = std::chrono::high_resolution_clock::now();
        bff.reserve(sz);
        try {
            for (int64_t i = 0; i < sz; ++i) {
                int sd_rd = rand() % bff_chr.size();
                bff.push_back(bff_chr[sd_rd]);  // ← push_back быстрее +=
            }
        }
        catch (const std::bad_alloc& e) {
            MessageBoxA(GetActiveWindow(), ("Ошибка!! Не хватает оперативной памяти! " + std::to_string(sz / 1024 / 1024) + "MB").c_str(), "PahomEngine->bench64", 0);
            bff.clear();
        }
        timeBenchOut = std::chrono::high_resolution_clock::now();
        i64MemorySize.fetch_add((bff.size() * 1) / 1024 / 1024, std::memory_order_relaxed);
        std::cout << i32CallFunc << "->call:" << bff.size() << " time_pool " << i64BenchLastTime << " s" <<" MemUsage:"<< i64MemorySize.load() << " MB" << std::endl;
        i64BenchLastTime += std::chrono::duration_cast<std::chrono::seconds>(timeBenchOut - timeBenchIn).count();
        return "Hash Size:" + std::to_string(bff.size());

    }
    void mt() {
        
        i32max_thread.store(std::jthread::hardware_concurrency(), std::memory_order_relaxed);
        i64ChunkSize.store(i64MaxSize / i32max_thread, std::memory_order_relaxed);
        std::vector<std::jthread> cpu_threads;
        cpu_threads.reserve(i32max_thread.load());
        for (uint32_t i32threads = 0; i32threads < i32max_thread.load(); i32threads++) {
            cpu_threads.emplace_back(&cpu_bench64::hashFn128,this, i64ChunkSize.load());
          
        }
        
       
    }
    // work not correcty #FIXME
    void mathFlow(int64_t sz) {
        d64Vec2 mathD64;
        for (int64_t d = 0; d < sz; d++) {
            mathD64.x = 9.9999999999L;
            mathD64.y = 0.9L;
            d64->DA_(mathD64.x, mathD64.y);

        }
    }
    // work not correcty #FIXME
    void mt_math_flow() {

        i32max_thread.store(std::jthread::hardware_concurrency(), std::memory_order_relaxed);
        i64ChunkSize.store(20000 / i32max_thread, std::memory_order_relaxed);
        std::vector<std::jthread> cpu_threads;
        cpu_threads.reserve(i32max_thread.load());
        for (uint32_t i32threads = 0; i32threads < i32max_thread.load(); i32threads++) {
            cpu_threads.emplace_back(&cpu_bench64::mathFlow, this, i64ChunkSize.load());

        }


    }
    // work not correcty #FIXME
    void mt_gpu() {
        HANDLE hHandleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        i32max_thread.store(std::jthread::hardware_concurrency(), std::memory_order_relaxed);
        i64ChunkSize.store(i64MaxSize / i32max_thread, std::memory_order_relaxed);
        std::vector<std::jthread> cpu_threads;
        cpu_threads.reserve(i32max_thread.load());
        for (uint32_t i32threads = 0; i32threads < i32max_thread.load(); i32threads++) {
            cpu_threads.emplace_back(&cpu_bench64::gpu_render, this, 1000 / i32max_thread.load());
            SetConsoleTextAttribute(hHandleConsole, 35);
            std::cout << " join() -> std::jthread tCPU" << i32threads << std::endl;
            SetConsoleTextAttribute(hHandleConsole, 15);
            
        }
        

    }
};

struct GameUI {
  
    void Message(std::string text, ImVec2 MaxSizeWindow, float step_to_speed, double64_t d64DelayToClear, bool* bCurrentWindowShowFlag, bool bShowCenter = false) {
        
        static ImVec2 sizeSmottly = { 10,10 };
        static double64_t d64DelayToClearIn = 0.0L;
        static ImVec2 posCenter = {};
        if (bShowCenter) {
            posCenter = { (ImGui::GetWindowSize().x - MaxSizeWindow.x) / 2,
                          (100)
            };
        }
        else {
            posCenter = {
                           (0),
                           (30)
            };
        }
        sizeSmottly.x += (step_to_speed * ImGui::GetIO().DeltaTime);
        sizeSmottly.y += (step_to_speed * ImGui::GetIO().DeltaTime);
        if (sizeSmottly.x >= MaxSizeWindow.x) {
            sizeSmottly.x = MaxSizeWindow.x;
            
        }
        if (sizeSmottly.y >= MaxSizeWindow.y) {
            sizeSmottly.y = MaxSizeWindow.y;
        }
        ImGui::SetCursorPos(posCenter);
        if (ImGui::BeginChild("text_message", sizeSmottly, ImGuiChildFlags_FrameStyle)) {
            ImGui::SetCursorPos(ImVec2(
                (MaxSizeWindow.x - ImGui::CalcTextSize(text.c_str()).x) / 2,
                (MaxSizeWindow.y - ImGui::CalcTextSize(text.c_str()).y) / 2
            ));
            ImGui::Text(text.c_str());
            ImGui::EndChild();
        }
        if(bCurrentWindowShowFlag)
        {
            d64DelayToClearIn += 2;
            if (d64DelayToClearIn >= d64DelayToClear) {
                sizeSmottly = { 10,10 };
                d64DelayToClearIn = 0.0L;
                *bCurrentWindowShowFlag = false;
            }
        }
    }
    int32_t i32idButton = 0;
    void clear() {
        i32idButton = 0;
    }
    void SetCountsButton(int32_t c) {
        i32idButton = c;
    }
    // beta
    void PaintIm(ImVec2 pos, float radius, ImVec4 col)
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImU32 color = ImGui::ColorConvertFloat4ToU32(col);
        for(int p = 0; p < pos.x;p++)
        {
            drawList->AddCircleFilled(pos, radius, color, 0); // 0 = default segments (auto)
        }
    }
    bool TextButton(ImVec4 colHovered, ImVec4 colDefault, const std::string& text, bool isCenterX = false, bool active = false) {
        static bool isPresedButton = false, isHoveredButton = false;
        if (isCenterX) {
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(text.c_str()).x) / 2);
        }
        ImGui::TextColored((ImGui::IsItemHovered() || active) ? colHovered : colDefault, text.c_str());
        isHoveredButton = ImGui::IsItemHovered();
        isPresedButton = ImGui::IsItemClicked();
        return isPresedButton;
    }
    ImVec4 conv(ImVec4 col) {
        return ImVec4{
            col.x / 255,
            col.y / 255,
            col.z / 255,
            col.w / 255
        };
    }
    bool CustomButton(ImVec4 colButton = { 155,255,0,255 }, ImVec4 ColBorder = { 35,35,35,255 }, ImVec4 colText = { 255,255,255,255 }, float rounding = 10, ImVec2 padiing = { 0,0 }, const char* label = "Button", ImFont* Font = nullptr , ImVec2 size = { 100,30 }) {
        static ImVec2 FramePadding = padiing; static float FrameRounding = rounding, BorderSize = 2;
        static bool isClickedButton = false;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, FramePadding);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, FrameRounding);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, BorderSize);
        ImGui::PushStyleColor(ImGuiCol_Button, conv(colButton));
        ImGui::PushStyleColor(ImGuiCol_Border, conv(ColBorder));
        ImGui::PushStyleColor(ImGuiCol_Text, conv(colText));
        if (Font) {
            ImGui::PushFont(Font);
            isClickedButton =  ImGui::Button(label, size);
            ImGui::PopFont();
        }
        else {
            std::cout << "[PahomEngine::GameUI] func CustomButton(...,ImFont *Font = nullptr); Font Error!" << std::endl;
            isClickedButton =  ImGui::Button(label, size);
        }
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar(3);
        return isClickedButton;
    }
    void TextAnimated(const std::string& text, int type = 0, int speed = 3) {
        static int t = 0, p = 0; static bool dir = true;
        if (++t >= speed) {
            t = 0;
            if (type == 0) { dir ? p++ : p--; if (p >= (int)text.size()) dir = false; if (p <= 0) dir = true; }
            if (type == 1) p = (p + 1) % (text.size() + 1);
            if (type == 2) p = (p + 1) % 40;
        }
        ImGui::Text("%s", (type == 0 ? text.substr(0, p) :
            type == 1 ? text.substr(0, text.size() - p) :
            std::string(text + ((p < 20) ? "_" : " "))).c_str());
    }
    void SetFont(ImFont* font) {
        ImGui::PushFont(font);
    }
    void EndFont() {
        ImGui::PopFont();
    }
    
};
struct GamepadButtons {
    struct buttons {
        bool bGButtonL = false;
        bool bGButtonR = false;
        bool bGButtonU = false;
        bool bGButtonD = false;
        bool bGButtonSTART = false;
    };
    std::unique_ptr<buttons> GButtons = std::make_unique<buttons>();
    void GamepadButtonRender(std::string gamepad_button_name, ImVec4 col = { 255,255,0,255 },bool v = false) {
        ImGui::BeginGroup();
        ImVec2 canvasSize = ImVec2(32, 32);

        ImVec2 canvasPos = ImGui::GetCursorScreenPos();
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImU32 border_color = ImGui::GetColorU32(ImGuiCol_Border);
        ImGuiStyle& style = ImGui::GetStyle();
        float radius = canvasSize.x * 0.5f;

        // Рассчитываем размер текста
        ImVec2 text_size = ImGui::CalcTextSize(gamepad_button_name.c_str());

        // Вычисляем центр кнопки
        ImVec2 center = ImVec2(canvasPos.x + radius, canvasPos.y + radius);

        // Позиция текста (центрированная)
        ImVec2 text_pos = ImVec2(
            center.x - text_size.x * 0.5f,
            center.y - text_size.y * 0.5f
        );

        // Отрисовка текста через drawList
        if (style.FrameBorderSize > 0.0f) {
            drawList->AddCircle(
                center,
                radius - style.FrameBorderSize * 0.5f,
                border_color,
                0, // Автоматическое количество сегментов
                style.FrameBorderSize
            );
        }

        // Фоновый круг
        drawList->AddCircleFilled(
            center,
            radius - style.FrameBorderSize,
            ImGui::GetColorU32(v ? col : ImVec4(7 / 255, 7 / 255, 7 / 255, 1))
        );
        drawList->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), gamepad_button_name.c_str());

        // Положение стика (предполагаем, что v - нормализованное значение [-1, 1])

        // Перемещаем курсор для следующих элементов ImGui
        ImGui::Dummy(canvasSize);
        ImGui::EndGroup();

    }
    ImVec2 minValue(ImVec2 a, ImVec2 b) {
        static float ax_min = 0.0f;
        static float by_min = 0.0f;
        if (a.x > a.y) {
            ax_min = a.y;
        }
        else {
            ax_min = a.x;
        }
        if (b.x > b.y) {
            by_min = b.y;
        }
        else {
            by_min = b.x;
        }
        return ImVec2(ax_min, by_min);
    }
    ImVec4 RGBAtoIv4(ImVec4 col) {
        return ImVec4{
            col.x / 255,
            col.y / 255,
            col.z / 255,
            col.w / 255
        };
    }
    void KeyButtonRender(std::string gamepad_button_name, ImVec4 col = { 255,255,0,255 }, bool v = false) {
        ImGui::BeginGroup();
        ImVec2 canvasSize = ImVec2(32, 32);

        ImVec2 canvasPos = ImGui::GetCursorScreenPos();
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImU32 border_color = ImGui::GetColorU32(ImGuiCol_Border);
        ImGuiStyle& style = ImGui::GetStyle();
        float radius = 1;

        // Рассчитываем размер текста
        ImVec2 text_size = ImGui::CalcTextSize(gamepad_button_name.c_str());

        // Вычисляем центр кнопки
        ImVec2 center = ImVec2(canvasPos.x + radius, canvasPos.y + radius);

        // Позиция текста (центрированная)
        ImVec2 text_pos = ImVec2(
            center.x - text_size.x * 0.5f,
            center.y - text_size.y * 0.5f
        );

        // Отрисовка текста через drawList
        if (style.FrameBorderSize > 0.0f) {
            drawList->AddCircle(
                center,
                radius - style.FrameBorderSize * 0.5f,
                border_color,
                0, // Автоматическое количество сегментов
                style.FrameBorderSize
            );
        }

        // Фоновый круг
        drawList->AddCircleFilled(
            center,
            radius - style.FrameBorderSize,
            ImGui::GetColorU32(v ? RGBAtoIv4(col) : RGBAtoIv4(ImVec4(12, 34, 133, 255)))
        );
        drawList->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), gamepad_button_name.c_str());
        ImGui::Dummy(canvasSize);
        ImGui::EndGroup();

    }
   
};

struct PahomEngineStruct {
    //
    std::string sBuild = "0.9.00 (pre-release)";
    std::string sBuildGame = "0.9.00 (pre-release)";
    //
    std::wstring sWBuild = L"0.9.00 (pre-release)";
    std::wstring sWBuildGame = L"0.9.00 (pre-release)";
    //
    bool CVsync = true;
    uint64_t fCPoint = 0;
    int64_t fStep = 13;
    std::unique_ptr<CImage> img = std::make_unique<CImage>();
    std::unique_ptr<JoyStickAPI> ptrGamepad1 = std::make_unique<JoyStickAPI>(1);
    std::unique_ptr<cpu_bench64> Bench64ptr = std::make_unique<cpu_bench64>();
    std::unique_ptr<GameUI> UI = std::make_unique<GameUI>();
    std::unique_ptr<GamepadButtons> GamepadUI = std::make_unique<GamepadButtons>();
    std::random_device rd;

    //

    bool bIsRandomEngineUsed = true;

    //
    struct GameSettingsOffsets {

        std::string sSettingsBufferString,
            volume_atr = "volume=",
            cpu_atr = "cpu_delay=";
        std::string controls_atr[2] = {"bind_key_left=","bind_key_right="};
        std::string glversion[2] = { "gl_major=" ,"gl_minor=" };
        int8_t u8BindKeyRight = 'D', u8BindKeyLeft = 'A';
        std::string res[2] = { "width=","height=" };
        int iPresetID = 0;
        int64_t i64WindowSize[2] = { 0,0 };
        float fMasterVolume = 0;
        int64_t i64CPUDelay = 0;
        int minor_gl = 0, major_gl = 0;
        bool bUseCustomRender = false;
        bool bShowNoiseBackground = false;
        bool bFlagVsync = false, bFlagRandomEngine = false;
        bool bKeyLDetected = false, bKeyRDetected = false;
        bool bRenderBlur = false;
        bool bFlagEnableAnimationToImageFadeInOut = false;
        bool bUseLowTextures = false;
        bool bUseLowAudioQuality = false;
        std::string preset_ = "preset=";
        void ParseConfig() {
            std::ifstream PahomEngineSettings("PahomEngine.cfg");
            if (PahomEngineSettings.is_open()) {
                while (std::getline(PahomEngineSettings, sSettingsBufferString)) {
                    if (sSettingsBufferString == "vsync=true") {
                        bFlagVsync = true;
                        //Pengine.log("PESettings:: Применен vsync=true", 1);
                    }
                    if (sSettingsBufferString == "vsync=false") {
                        bFlagVsync = false;
                        //Pengine.log("PESettings:: Применен vsync=false", 1);
                    }
                    if (sSettingsBufferString == "random_engine=true") {
                        bFlagRandomEngine = true;
                       // Pengine.log("PESettings:: Применен random_engine=true", 1);
                    }
                    if (sSettingsBufferString == "random_engine=false") {
                        bFlagRandomEngine = false;
                        //Pengine.log("PESettings:: Применен random_engine=false", 1);
                    }
                    if (sSettingsBufferString.rfind(volume_atr, 0) == 0) {
                        fMasterVolume = stof(sSettingsBufferString.substr(volume_atr.length()));
                    }
                    if (sSettingsBufferString.rfind(cpu_atr, 0) == 0) {
                        i64CPUDelay = stoll(sSettingsBufferString.substr(cpu_atr.length()));
                    }
                    if (sSettingsBufferString.rfind(controls_atr[0], 0) == 0) {
                        bKeyLDetected = true;
                        u8BindKeyLeft = static_cast<int8_t>(stoi(sSettingsBufferString.substr(controls_atr[0].length())));
                    }
                    
                    if (sSettingsBufferString.rfind(controls_atr[1], 0) == 0) {
                        bKeyRDetected = true;
                        u8BindKeyRight = static_cast<int8_t>(stoi(sSettingsBufferString.substr(controls_atr[1].length())));
                    }
                    if (sSettingsBufferString.rfind(res[0], 0) == 0) {
                        i64WindowSize[0] = stoll(sSettingsBufferString.substr(res[0].length()));
                    }
                    if (sSettingsBufferString.rfind(res[1], 0) == 0) {
                        i64WindowSize[1] = stoll(sSettingsBufferString.substr(res[1].length()));
                    }
                    if (sSettingsBufferString.rfind(preset_, 0) == 0) {
                        iPresetID = stoi(sSettingsBufferString.substr(preset_.length()));
                    }
                    if (sSettingsBufferString == "render_blur=true") {
                        bRenderBlur = true;
                    }
                    if (sSettingsBufferString == "render_blur=false") {
                        bRenderBlur = false;
                    }
                    if (sSettingsBufferString == "EnableAnimationToImageFadeInOut=true") {
                        bFlagEnableAnimationToImageFadeInOut = true;
                    }
                    if (sSettingsBufferString == "EnableAnimationToImageFadeInOut=false") {
                        bFlagEnableAnimationToImageFadeInOut = false;
                    }
                    if (sSettingsBufferString == "use_custom_render=true") {
                        bUseCustomRender = true;
                    }
                    if (sSettingsBufferString == "use_custom_render=false") {
                        bUseCustomRender = false;
                    }
                    if (sSettingsBufferString.rfind(glversion[0], 0) == 0) {
                        major_gl = (stoi(sSettingsBufferString.substr(glversion[0].length())));
                        /*if (major_gl < 3) {
                            major_gl = 3;
                            std::cout << " (PahomEngine) gl_major not < 3\n";
                        }*/

                    }
                    if (sSettingsBufferString.rfind(glversion[1], 0) == 0) {
                        minor_gl = (stoi(sSettingsBufferString.substr(glversion[1].length())));
                        /*if (minor_gl < 3) {
                            minor_gl = 3;
                            std::cout << " (PahomEngine) gl_minor not < 3\n";
                        }*/
                    }
                    if (sSettingsBufferString == "noise=true") {
                        bShowNoiseBackground = true;
                        //Pengine.log("PESettings:: Применен vsync=true", 1);
                    }
                    if (sSettingsBufferString == "noise=false") {
                        bShowNoiseBackground = false;
                        //Pengine.log("PESettings:: Применен vsync=false", 1);
                    }
                    //use_low_textures=
                    if (sSettingsBufferString == "use_low_textures=true") {
                        bUseLowTextures = true;
                        //Pengine.log("PESettings:: Применен vsync=true", 1);
                    }
                    if (sSettingsBufferString == "use_low_textures=false") {
                        bUseLowTextures = false;
                        //Pengine.log("PESettings:: Применен vsync=false", 1);
                    }
                    //use_low_textures=
                    if (sSettingsBufferString == "use_low_audio_quality=true") {
                        bUseLowAudioQuality = true;
                        //Pengine.log("PESettings:: Применен vsync=true", 1);
                    }
                    if (sSettingsBufferString == "use_low_audio_quality=false") {
                        bUseLowAudioQuality = false;
                        //Pengine.log("PESettings:: Применен vsync=false", 1);
                    }
                }
            }
            PahomEngineSettings.close();
        }
        void Save(std::string_view string_param) {
            std::ofstream PahomEngineSettings("PahomEngine.cfg");
            PahomEngineSettings << string_param << std::endl;
            PahomEngineSettings.close();
        }
    };
    std::unique_ptr<GameSettingsOffsets> PESettings = std::make_unique<GameSettingsOffsets>();
    struct mathValues {
        std::random_device rd;
        std::mt19937 gen;
        bool bIsRandomEngineUsed = true;
        template <typename Tm>
        Tm minv(Tm a, Tm b) {
            return std::min<Tm>(a, b);
        }
        template <typename Tm>
        Tm maxv(Tm a, Tm b) {
            return std::max<Tm>(a, b);
        }
        template <typename Tm>
        Tm abs(Tm x) {
            return std::abs(x);
        }
        // test func
        template <typename Tm>
        Tm fade_add(Tm *x, Tm interval, Tm max_interval,Tm add_part_count) {
            static Tm a = 0;
            std::cout << std::format("{} interval:{} max_interval:{} add:{}\n", *x, interval, max_interval, add_part_count);
            a += interval;
            if (a > max_interval) {
                *x += add_part_count;
                a = 0;
            }
            return *x;   
        }
        // my find func
       /* template <typename T>
        std::string_view find(T str, T sub)
        {
            std::string_view str_buf = std::to_string(str);
            std::string_view sub_buf = std::to_string(sub);
            std::string_view buf;
            for (int i = 0; (i = str_buf.find(sub_buf, i)) != std::string_view::npos; i = i + sub_buf.size())
            {
                buf += sub_buf;
            }

            return buf;
        }*/
        // find V test func
        bool findV(int64_t v, int64_t find_v) {
            std::string_view strBuffer = std::format("{}", v);
            int32_t sizeBuffer = strBuffer.size();
            for (int32_t cint = 0; cint < sizeBuffer; cint++) {
                if (std::format("{}", strBuffer[cint]) == std::format("{}", find_v)) {
                    return true;
                }
            }  
        }
        // set flag to use random engine e.d random_device to #include <random>
        // in true ||  false
        // - true -- enable random_device and std::mt19997
        // - false -- disable random_device random use C rand();
        void setRandomEngineUsed(bool v) {
            bIsRandomEngineUsed = v;
            //std::cout << " (random_engine) enabled: " << (bIsRandomEngineUsed ? "true\n" : "false\n");
        }
        // random engine func 
        // use: PahomEngine->math->random<type>(max_value,isCached);
        // sample:
        // int64_t i64ValueRandom = PahomEngine->math->random<int64_t>(255,false);
        template <typename Tm>
        Tm random(Tm value_max, bool bIsUseCachedRandom = false) {
            if(bIsRandomEngineUsed)
            {
                if (!bIsUseCachedRandom)
                {

                    std::random_device rd_no_cached;
                    std::mt19937 gen_no_cached(rd_no_cached());
                    
                    if constexpr (std::is_floating_point_v<Tm>) {
                        std::uniform_real_distribution<Tm> dist(0, value_max);
                        return dist(gen_no_cached);
                    }
                    else {
                        std::uniform_int_distribution<Tm> dist(0, value_max);
                        return dist(gen_no_cached);
                    }
                }
                else {
                    gen.seed(rd());
                    if constexpr (std::is_floating_point_v<Tm>) {
                        std::uniform_real_distribution<Tm> dist(0, value_max);
                        return dist(gen);
                    }
                    else {
                        std::uniform_int_distribution<Tm> dist(0, value_max);
                        return dist(gen);
                    }

                }
            }
            else {
                return static_cast<Tm>(rand() % (int)value_max);
            }
        }
        template <typename value>
        bool isValueTrue(value data0, value data_max_value) {
            if (data0 > data_max_value && data0 != data_max_value) {
                return true;
            }
            else {
                return false;
            }
        }
    };
    struct castValues {
        bool bUsedStaticCast = false;
        template <typename Tm>
        Tm cast_all(Tm type) {
            if (std::is_arithmetic_v<Tm>) {
                return bUsedStaticCast ? static_cast<Tm>(type) : (Tm)(type);
            }
            else {
                std::cout << ("cast->cast_all<error>(type)") << std::endl;
            }
        }
        template <typename Tm>
        Tm cast_to_string(Tm type) {
            return std::format("{}", type);
        }
        template <typename In, typename Out>
        Out unicast(In value) {
            if constexpr (std::is_arithmetic_v<In> && std::is_arithmetic_v<Out>) {
                return static_cast<Out>(value);
            }
            else {
                std::string sv;
                if constexpr (std::is_convertible_v<In, std::string>) {
                    sv = value;
                }

                Out result{};
                auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), result);
                if (ec == std::errc::invalid_argument) {
                    throw std::invalid_argument("unicast: not a number");
                }
                else if (ec == std::errc::result_out_of_range) {
                    throw std::out_of_range("unicast: out of range");
                }

                return result;
            }
        }
    };
    struct bufferio {
        
        template <class... Tm>
        void print(const std::format_string<Tm...> _Fmt, Tm&&... _Args) {
          std::cout<< _STD vformat(_Fmt.get(), _STD make_format_args(_Args...));
        }
        /// custom allocator Memory
        template <typename alc>
        using Uptr = std::unique_ptr<alc>;
        template <typename alc>
        Uptr<alc> alloc_ptr() {
            return std::make_unique<alc>();
        }
    };
    struct gpuRenderOGL {
        bool bIsUsedGLCustom = false;
        struct apiVersion {
            int minor = 2;
            int major = 3;
            std::string_view strVersion[4] = {
                "1.0",
                "2.0",
                "3.0",
                "4.0"
            };
            void setVersion(int64_t idx) {
                if (strVersion[idx] == "1.0") {
                    major = 1;
                    minor = 0;
                }
                if (strVersion[idx] == "2.0") {
                    major = 2;
                    minor = 0;
                }
                if (strVersion[idx] == "3.0") {
                    major = 3;
                    minor = 0;
                }
                if (strVersion[idx] == "4.0") {
                    major = 4;
                    minor = 0;
                }
            }
        };

        apiVersion gl_ver;
        
    };
    std::unique_ptr<GLM> ogl = std::make_unique<GLM>();
    std::unique_ptr<bufferio>   cio  = std::make_unique<bufferio>();
    std::unique_ptr<castValues> cast = std::make_unique<castValues>();
    std::unique_ptr<mathValues> math = std::make_unique<mathValues>();
    std::unique_ptr<gpuRenderOGL> Render = std::make_unique<gpuRenderOGL>();
    void StyleLoad();
    void StyleLoadBlur();
    ImVec4 RGBA(float r, float g, float b, float a);
    ImVec4 RGBA(ImVec4 col);
    ImVec4 ToRGBA(std::string hex);
    void setTextCenter(const char* text);
    void setItemCenterX(float x);
    void setItemCenter(ImVec2 Size);
    ImVec2 HwndWSizeA(HWND HwNd);
    // structs
    STRINGSDATA strings;
    ASSETSDATA assets;
    IMAGEDATA ImageData;
    KEYMAPDATA keyMap;
    KurlikAUDIO audio;
    GameEvent Event;
    MEMORYDATA Mem;
    std::unique_ptr<EXCEPTIONS> Exceptions = std::make_unique<EXCEPTIONS>();
    std::unique_ptr<Diffinity> pDiff = std::make_unique<Diffinity>();
    // other value
    ///

    float fDeltaTime = 0;
    float fFrameRate = 0;
    //


    //
    bool bRenderIsEdited = false;
    bool bSettings = false;
    ImVec4 fillColorRGBA;//RGBA(133, 133, 133, 255)
    int64_t i64WindowSize[2] = { 800 , 600 };
    int64_t i64WindowSizeGL[2] = { 800 , 600 };
    int64_t i64ScaleRender = 10;
    std::string sGraphPreset;
    float fPahomPosX = 0;
    float fPahomPosY = 0;
    float fMaxPahomPosX = static_cast<float>(i64WindowSize[0]) - 128.0f;
    float fMaxPahomPosY = static_cast<float>(i64WindowSize[1]) - 128.0f;
    float fMinPahomPosX = 0;
    float fMinPahomPosY = 0;
    float updateInterval = 0;
    float fScoreCount = 0.000f;
    float fBreadPosX = 0;
    float fBreadPosY = 400;
    int64_t i64BreadSize[2] = { 64,64 };
    int64_t i64PahomSize[2] = { 128,128 };
    ImVec2 WindowSize = { cast->cast_all<float>(i64WindowSize[0]),cast->cast_all<float>(i64WindowSize[1]) };
    ImVec2 PahomPos   = { cast->cast_all<float>(fPahomPosX)      ,cast->cast_all<float>(fPahomPosY)       };
    ImVec2 BreadPos   = { cast->cast_all<float>(fBreadPosX)      ,cast->cast_all<float>(fBreadPosY)       };
    ImVec2 BreadSize  = { cast->cast_all<float>(i64BreadSize[0]) ,cast->cast_all<float>(i64BreadSize[1])  };
    ImVec2 PahomSize  = { cast->cast_all<float>(i64PahomSize[0]) ,cast->cast_all<float>(i64PahomSize[1])  };
    bool bLoadingFrame = true; bool bLoadingFrameOK = false;
    bool bControlsIsGamepad = false;
    bool bControlsIsKeyboard = false;
    bool bDebug = true;
    bool bFullscreen = false;
    bool bGameOver = false;
    
    int64_t i64CPUDelay = 2;
    // main flags
    bool bStartGame = false;
    float fStepMove = 6.0f;
    void InputI64(const char* id, int64_t* v);
    void InputFloat(const char* id, float* v);
    void setItemCenterX(float sizeText, float window_size_x);
    void reloadBreadPos();
    int64_t rand64(int64_t in_v);
    float randfloat(float in_v);
    bool CheckColiision();
    bool bStartGameFlag = false;
    bool bIsRevesed = false;
    bool bDebugText = false;
    bool bBoost777 = false;
    bool bKefir = false;
    bool bIsImGuiAGamepadAPIUsed = false;
    bool bColoredConsole = false;
    bool bRenderPaused = false;
    int64_t i64RandBoost = 0;
    int64_t i64ValuesRands[4] = { 50 , 100, 256, 777 };
    HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    bool GetGamepadKey(int64_t iKey, int iMaxDelay);
    void Text(ImVec4 col, std::string text);
    void log(std::string text, int iLogTypeFlags);
    void Tbuffer();
    void logo();
    //
    int iLoadData = 0;
    int iReloadTexturesPresetID = 0;
    std::string sTexturePresetName = "";
    void SetPresetName(int id) {
        std::string str[] = { "Низкие" ,"Высокие" ,"Ультра" };
        sTexturePresetName = str[id];
    }
    void ReloadTextures(int preset_id) {
        int64_t i64Size = 0;

        // clear video memory
        for (int _textures = 0; _textures < std::size(assets.asset); _textures++) {
            log(std::format(" (OGL) clearing.. {}", _textures), 1);
            glDeleteTextures(_textures, ImageData.TextureArray);
        }
        // low settings
        if(preset_id == 0){
            
            log(" (OGL) set preset LOW", 1);
            for (int _textures_ld = 0; _textures_ld < std::size(assets.asset_ultra_low); _textures_ld++) {
                log(std::format(" (OGL) Loading.. {} {}", _textures_ld, (assets.asset_ultra_low[_textures_ld])), 1);
                if (img->LoadTextureFromFile(reinterpret_cast<const char*>(assets.asset_ultra_low[_textures_ld].data()),
                    &ImageData.TextureArray[_textures_ld],
                    &ImageData.TextureX[_textures_ld],
                    &ImageData.TextureY[_textures_ld],
                    ImageData.TextureBufferArray[_textures_ld]);
                    i64Size += (ImageData.TextureX[_textures_ld] * ImageData.TextureY[_textures_ld] * 4)) {
                    log(std::format(" (OGL) loaded_buffer {} MB", (i64Size / 1024) / 1024), 1);
                }
                else {
                    log(std::format(" (OGL) loaded_buffer {} MB error", (i64Size / 1024) / 1024), 1);
                }
                //*data = _textures_ld;
            }

             
        }
        // high settings
        if (preset_id == 1) {
            log(" (OGL) set preset HIGH", 1);
            for (int _textures_ld = 0; _textures_ld < std::size(assets.asset); _textures_ld++) {
                log(std::format(" (OGL) Loading.. {} {}", _textures_ld, (assets.asset[_textures_ld])), 1);
                if (img->LoadTextureFromFile(reinterpret_cast<const char*>(assets.asset[_textures_ld].data()),
                    &ImageData.TextureArray[_textures_ld],
                    &ImageData.TextureX[_textures_ld],
                    &ImageData.TextureY[_textures_ld],
                    ImageData.TextureBufferArray[_textures_ld]);
                    i64Size += (ImageData.TextureX[_textures_ld] * ImageData.TextureY[_textures_ld] * 4)) {
                    log(std::format(" (OGL) loaded_buffer {} MB", (i64Size / 1024) / 1024), 1);
                }
                else {
                    log(std::format(" (OGL) loaded_buffer {} MB error", (i64Size / 1024) / 1024), 1);
                }
                //*data = _textures_ld;
            }


        }
        // ultra settings
        if (preset_id == 2) {
            int64_t i64Size = 0;
            

            for (int _textures_ld = 0; _textures_ld < std::size(assets.UltraTextures); _textures_ld++) {
                log(std::format(" (OGL) (preset: ultra_max:) Loading.. {} {}", _textures_ld, assets.UltraTextures[_textures_ld]), 1);
                if (img->LoadTextureFromFile(reinterpret_cast<const char*>(assets.UltraTextures[_textures_ld].data()),
                    &ImageData.TextureArray[_textures_ld],
                    &ImageData.TextureX[_textures_ld],
                    &ImageData.TextureY[_textures_ld],
                    ImageData.TextureBufferArray[_textures_ld]);
                    i64Size += (ImageData.TextureX[_textures_ld] * ImageData.TextureY[_textures_ld] * 4)) {
                    log(std::format(" (OGL) loaded_buffer {} MB", (i64Size / 1024) / 1024), 1);
                }
                else {
                    log(std::format(" (OGL) loaded_buffer {} MB error", (i64Size / 1024) / 1024), 1);
                }

               // *data = _textures_ld;
            }


        }
    }
   
    void progress_bar(float fragtion);
    bool getPressedKey(int8_t key, bool isTurned = false) {
        return (isTurned ? GetAsyncKeyState(key) : GetKeyState(key) > 0);
    }

    void setTextCenterXY(const char* text);
    int64_t ptrint64_t(GLuint tx) {
        return reinterpret_cast<int64_t>(reinterpret_cast<void*>(tx));
    }
    int32_t SetSizeHWND(HWND hwnd, int32_t x, int32_t y);
    void selectedItem(bool v, float rounding);
    void stdoutColored(std::string out, int16_t i16colorText);
    bool bLogEnabled = false;
    //
    template <typename T> using ArrayBuf = std::vector<T>;
    void clearPos();
    void setTextCenterXYRect(const char* text);
    template <class... Tm>
    void Text(const std::format_string<Tm...> _Fmt, Tm&&... _Args) {
        ImGui::Text(_STD vformat(_Fmt.get(), _STD make_format_args(_Args...)).c_str());
        //_STD vformat_to(_Fmt.get(), _STD make_format_args(_Args...));
    }
    template <class... Tm>
    void TextColored(ImVec4 col,const std::format_string<Tm...> _Fmt, Tm&&... _Args) {
        ImGui::TextColored(col,_STD vformat(_Fmt.get(), _STD make_format_args(_Args...)).c_str());
    }
   
};
void PahomEngineStruct::selectedItem(bool v,float rounding = 20) {
    ImDrawList* dw = ImGui::GetWindowDrawList();
    if (v) {
        dw->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImGui::GetColorU32(RGBA(255, 255, 255, 255)), rounding, 0,3);
    }
}
//add hex to rgba @finich_15
//use : PahomEngine->ToRGBA("#dfddfdff") & PahomEngine->ToRGBA("#dfddfd")
ImVec4 PahomEngineStruct::ToRGBA(std::string hex) { // hex  = #ffffffff
    std::string hex0 = hex.substr(1, 2); // ff
    std::string hex1 = hex.substr(3, 2); // ff
    std::string hex2 = hex.substr(5, 2); // ff
    std::string hex3 = hex.size() == 8 ? hex.substr(7, 2) : "00"; // ff
    return ImVec4{
         (float)(stoi(hex0,nullptr,16)) / 255,
         (float)(stoi(hex1,nullptr,16)) / 255,
         (float)(stoi(hex2,nullptr,16)) / 255,
         (float)(stoi(hex3,nullptr,16)) / 255
    };
}
void PahomEngineStruct::clearPos() {
    fPahomPosX = 0;
    fPahomPosY = 0;
    fBreadPosX = 0;
    fBreadPosY = 0;
}
enum type {
    WARN = 0,
    INFO = 1,
    ERR = 2,
    DEBUG = 3,
};
void  PahomEngineStruct::log(std::string text,int iLogTypeFlags = 1) {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm* local_tm = std::localtime(&t);

    int32_t i32LogColorText = 0;
    bLogEnabled = true;
    std::vector<std::string_view> typeStr = {
        "WARN",
        "INFO",
        "ERR",
        "DEBUG"
    };
    std::string_view typeCurrent;
    typeCurrent = (iLogTypeFlags == type::WARN ? typeStr[0] : typeStr[iLogTypeFlags]);
    typeCurrent = (iLogTypeFlags == type::INFO ? typeStr[1] : typeStr[iLogTypeFlags]);
    typeCurrent = (iLogTypeFlags == type::ERR  ? typeStr[2] : typeStr[iLogTypeFlags]);
    typeCurrent = (iLogTypeFlags == type::DEBUG ? typeStr[3] : typeStr[iLogTypeFlags]);
    ImVec4 Time = {
       static_cast<float>(local_tm->tm_hour),      // 0–23
       static_cast<float>(local_tm->tm_min),       // 0–59
       static_cast<float>(local_tm->tm_sec),       // 0–59
       static_cast<float>(now.time_since_epoch().count() % 10000000 / 10000.0f) // миллисекунды (примерно)
    };
    if (bLogEnabled) {
        static ColorV3 colorBacki32 = { 12,12,19 };
        static ColorV3 colorTexti32 = {0,0,0};
        switch (iLogTypeFlags) {
        case type::WARN: 
            colorTexti32 = { 200,250,0 };
            colorBacki32 = { 12,12,19 };
            i32LogColorText = 15;
            break;
        case type::INFO:
            colorBacki32 = { 32,32,49 };
            colorTexti32 = { 56,106,253 };
            i32LogColorText = 11;
            break;
        case type::ERR:
            colorBacki32 = { 12,12,19 };
            colorTexti32 = { 250,0,140 };
            i32LogColorText = 12;
            break;
        case type::DEBUG:
            colorBacki32 = { 12,12,19 };
            colorTexti32 = { 0,255,100 };
            i32LogColorText = 10;
            break;
        
        }
        //SetConsoleTextAttribute(hConsoleHandle, i32LogColorText);
       // std::cout << std::format(" {}::({}:{}:{}) ", typeCurrent, Time.x, Time.y, Time.z) << " [PahomEngine] " << text << std::endl;
        if(bColoredConsole)
        {
            console.pout(std::format(" {}::({}:{}:{}) [PahomEngine] {}\n ", typeCurrent, Time.x, Time.y, Time.z, text), colorTexti32, colorBacki32, false, false);
        }
        else {
            SetConsoleTextAttribute(hConsoleHandle, i32LogColorText);
            std::cout << std::format(" {}::({}:{}:{}) ", typeCurrent, Time.x, Time.y, Time.z) << " [PahomEngine] " << text << std::endl;
        }
       // SetConsoleTextAttribute(hConsoleHandle, 15);
    }
    else {
        Event.WriteLog(std::format(" {}::({}:{}:{}) {}", typeCurrent, Time.x, Time.y, Time.z, text));
    }
}
void  PahomEngineStruct::Text(ImVec4 col, std::string text) {
    PahomEngineStruct::Event.Text(col,text);
}
void  PahomEngineStruct::Tbuffer() {
    PahomEngineStruct::Event.TextBuffer();
}
bool  PahomEngineStruct::GetGamepadKey(int64_t iKey,int iMaxDelay) {
    int iD = 0;
    iD++;
    if (iD > iMaxDelay) {
        if(!bIsImGuiAGamepadAPIUsed)
        {
            return ptrGamepad1->GetState().Gamepad.wButtons == iKey ? true : false;
        }
       
    }
    if (bIsImGuiAGamepadAPIUsed) {
        if (getKeyPresed(iKey)) {
            return true;
        }
    }
}
int64_t PahomEngineStruct::rand64(int64_t in_v) {
    if (bIsRandomEngineUsed) {
        std::random_device rd; // ������������� ������
        std::mt19937 gen(rd()); // ������������� ����������
        std::uniform_int_distribution<int64_t> dist(2, in_v);//���������� ���� ��� ���������
        return dist(gen); //��������� �����
    }
    else {
        return rand() % in_v;
    }
}
float PahomEngineStruct::randfloat(float in_v) {
    std::random_device rd; // ������������� ������
    std::mt19937 gen(rd()); // ������������� ����������
    std::uniform_real_distribution<float> dist(2, in_v);//���������� ���� ��� ���������
    return dist(gen); //��������� �����
}
void PahomEngineStruct::reloadBreadPos() {
    fBreadPosX = randfloat(fMaxPahomPosX);
}

void PahomEngineStruct::InputI64(const char* id, int64_t* v) {
    ImGui::InputInt64(
        id,
        v,
        1,
        100,
        0
    );
}
void PahomEngineStruct::InputFloat(const char* id, float* v) {
    ImGui::InputFloat(id, v);
}
int32_t PahomEngineStruct::SetSizeHWND(HWND hwnd, int32_t x, int32_t y) {

    HWND hWindow = hwnd;
    RECT r;
    GetWindowRect(hWindow, &r);
    MoveWindow(hWindow, r.left, r.top, x, y, TRUE);//
    return x + y;
}
ImVec2 PahomEngineStruct::HwndWSizeA(HWND HwNd)
{
    RECT fSizeWindow;
    GetWindowRect(HwNd, &fSizeWindow);
    int64_t fWctx = fSizeWindow.right - fSizeWindow.left;
    int64_t fWcty = fSizeWindow.bottom - fSizeWindow.top;
    return ImVec2(
        static_cast<float>(fWctx), static_cast<float>(fWcty)
    );
}
void PahomEngineStruct::setTextCenter(const char* text) {
    float fTextSize = ImGui::CalcTextSize(text).x;
    float fTextCenterPosition = (i64WindowSize[0] - fTextSize) / 2;
    ImGui::SetCursorPosX(fTextCenterPosition);
}
void PahomEngineStruct::setTextCenterXY(const char* text) {
    ImVec2 fTextSize = ImGui::CalcTextSize(text);
    ImGui::SetCursorPos(ImVec2{
        (i64WindowSize[0] - fTextSize.x) / 2,
        (i64WindowSize[1] - fTextSize.y) / 2
        });
}
void PahomEngineStruct::setTextCenterXYRect(const char* text) {
    ImVec2 fTextSize = ImGui::CalcTextSize(text);
    ImGui::SetCursorPos(ImVec2{
        (ImGui::GetItemRectSize().x - fTextSize.x) / 2,
        (ImGui::GetItemRectSize().y - fTextSize.y) / 2
        });
}
bool PahomEngineStruct::CheckColiision() {
    return (
        fPahomPosX < fBreadPosX + i64BreadSize[0] &&
        fPahomPosX + i64PahomSize[0] > fBreadPosX &&
        fPahomPosY < fBreadPosY + i64BreadSize[1] &&
        fPahomPosY + i64PahomSize[1] > fBreadPosY
        );
}
void PahomEngineStruct::setItemCenterX(float sizeText,float window_size_x) {
    float fItemSizeX = sizeText;
    float fItemXCenterPosition = (window_size_x - fItemSizeX) / 2;
    ImGui::SetCursorPosX(fItemXCenterPosition);
}
void PahomEngineStruct::setItemCenterX(float x) {
    float fItemSizeX = x;
    float fItemXCenterPosition = (i64WindowSize[0] - fItemSizeX) / 2;
    ImGui::SetCursorPosX(fItemXCenterPosition);
}
void PahomEngineStruct::setItemCenter(ImVec2 Size) {
    float fItemXCenterPosition = (i64WindowSize[0] - Size.x) / 2;
    float fItemYCenterPosition = (i64WindowSize[1] - Size.y) / 2;
    ImGui::SetCursorPos(ImVec2(
        fItemXCenterPosition,
        fItemYCenterPosition
    ));
}
ImVec4  PahomEngineStruct::RGBA(float r, float g, float b, float a) {
    float floatMaxColorRGBA = 255.0f;
    ImVec4 outRGBA{
        r / floatMaxColorRGBA,
        g / floatMaxColorRGBA,
        b / floatMaxColorRGBA,
        a / floatMaxColorRGBA,
    };
    return outRGBA;
}
ImVec4  PahomEngineStruct::RGBA(ImVec4 col) {
    float floatMaxColorRGBA = 255.0f;
    ImVec4 outRGBA{
        col.x / floatMaxColorRGBA,
        col.y / floatMaxColorRGBA,
        col.z / floatMaxColorRGBA,
        col.w / floatMaxColorRGBA,
    };
    return outRGBA;
}
void PahomEngineStruct::logo() {
    std::string_view PAHOM_ENGINE =
        " ______   ______     __  __     ______     __    __             \n"
        "/\\  == \\ /\\  __ \\   /\\ \\_\\ \\   /\\  __ \\   /\\ \"-./  \\            \n"
        "\\ \\  _-/ \\ \\  __ \\  \\ \\  __ \\  \\ \\ \\/\\ \\  \\ \\ \\-./\\ \\           \n"
        " \\ \\_\\    \\ \\_\\ \\_\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\_\\ \\ \\_\\          \n"
        "  \\/_/     \\/_/\\/_/   \\/_/\\/_/   \\/_____/   \\/_/  \\/_/          \n"
        "                                                                \n"
        " ______     __   __     ______     __     __   __     ______    \n"
        "/\\  ___\\   /\\ \"-.\\ \\   /\\  ___\\   /\\ \\   /\\ \"-.\\ \\   /\\  ___\\   \n"
        "\\ \\  __\\   \\ \\ \\-.  \\  \\ \\ \\__ \\  \\ \\ \\  \\ \\ \\-.  \\  \\ \\  __\\   \n"
        " \\ \\_____\\  \\ \\_\\\\\"\\_\\  \\ \\_____\\  \\ \\_\\  \\ \\_\\\\\"\\_\\  \\ \\_____\\ \n"
        "  \\/_____/   \\/_/ \\/_/   \\/_____/   \\/_/   \\/_/ \\/_/   \\/_____/ \n"
        "                                                                \n";
    SetConsoleOutputCP(CP_UTF8);
    if(bColoredConsole)
    {
        for (int64_t c = 0; c < PAHOM_ENGINE.size(); c++) {
            console.pout(std::format("{}", PAHOM_ENGINE[c]), console.randColor(), ColorV3(19, 19, 22), true, false);
        }
        console.TestColors({ 12,12 });
    }
    else {
        for (int64_t c = 0; c < PAHOM_ENGINE.size(); c++) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), rand() % 15);
            std::cout << (std::format("{}", PAHOM_ENGINE[c]));
        }
       // console.TestColors({ 12,12 });
    }
}
void PahomEngineStruct::stdoutColored(std::string out,int16_t i16colorText) {
    HANDLE hc = GetStdHandle(STD_OUTPUT_HANDLE);
    static int64_t i64ColorCh = 0;
    std::vector <std::string> array;
    
    SetConsoleOutputCP(CP_UTF8);
    for (int64_t c = 0; c < out.size(); c++) {
        array.push_back(std::string(1, out[c]));
        if (!array[c].empty()) {
            SetConsoleTextAttribute(hc, i16colorText);
        }
        else {
            SetConsoleTextAttribute(hc, 15);
            i64ColorCh++;
        }
        std::cout << out[c];
    }
    // ���������� ���� �������
    SetConsoleTextAttribute(hc, 7); // ����������� ���� (����� ����� �� ������ ����)
    std::cout << "Colored:" << i64ColorCh << " LogoSize: " << out.size() << std::endl;
}
void PahomEngineStruct::progress_bar(float fragtion) {
    int64_t idx = static_cast<float>(fragtion) * 10;
    COORD nullcd = { 0,0 };
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hcon, nullcd);
    std::string_view str_array[] = {
        "[----------]",//0
        "[�---------]",//1
        "[��--------]",//2
        "[���-------]",//3
        "[����------]",//4
        "[�����-----]",//5
        "[������----]",//6
        "[�������---]",//7
        "[��������--]",//8
        "[���������-]",//9
        "[����������]",//10
       
    };
    std::cout << str_array[idx] << std::endl;
}
void PahomEngineStruct::StyleLoad() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = RGBA(7, 7, 9, 255);
    colors[ImGuiCol_ChildBg] = ImVec4(0.04f, 0.04f, 0.04f, 0.94f);
    colors[ImGuiCol_PopupBg] = RGBA(5, 5, 7, 255);
    colors[ImGuiCol_Border] = RGBA(35, 35, 55, 255);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = RGBA(5, 5, 5, 255);
    colors[ImGuiCol_FrameBgHovered] = RGBA(5, 5, 5, 255);
    colors[ImGuiCol_FrameBgActive] = RGBA(5, 5, 5, 255);
    colors[ImGuiCol_TitleBg] = ImVec4(0.353f, 0.157f, 1.000f, 1.000f);
    colors[ImGuiCol_TitleBgActive] = RGBA(135, 165, 255, 155);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
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
    colors[ImGuiCol_Separator] = RGBA(35, 35, 55, 255);
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
}
void PahomEngineStruct::StyleLoadBlur() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.TabRounding = 0.0f;
    style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);

    // Window
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);

    // Buttons
    style.Colors[ImGuiCol_Button] = ImVec4(0.35f, 0.38f, 0.47f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.31f, 0.33f, 0.42f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.27f, 0.29f, 0.39f, 1.00f);

    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);

    style.Colors[ImGuiCol_Header] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);

    ImVec4 baseColor = style.Colors[ImGuiCol_MenuBarBg];
    ImVec4 fadedColor = ImVec4(baseColor.x * 0.5f, baseColor.y * 0.5f, baseColor.z * 0.5f, baseColor.w * 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = fadedColor;
    style.Colors[ImGuiCol_PopupBg] = fadedColor;
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0);  // Fully transparent

    // Scrollbar
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    style.ScrollbarSize = 20.0f;

    // Slider
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
    style.GrabMinSize = 20.0f;
}
auto PahomEngine = std::make_unique<PahomEngineStruct>();
namespace PE {
    template <class... Tm>
    void print(const std::format_string<Tm...> _Fmt, Tm&&... _Args) {
        std::cout << _STD vformat(_Fmt.get(), _STD make_format_args(_Args...));
       //_STD vformat_to(_Fmt.get(), _STD make_format_args(_Args...));
    }
    template <class... Tm>
    std::string_view ToString(const std::format_string<Tm...> _Fmt, Tm&&... _Args) {
        return _STD vformat(_Fmt.get(), _STD make_format_args(_Args...));
        //_STD vformat_to(_Fmt.get(), _STD make_format_args(_Args...));
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    template <typename Tm>
    Tm random(Tm value_max, bool bIsCached = false) {
        if (!bIsCached) {
            std::random_device rd;
            std::mt19937 gen(rd());
        }
        if constexpr (std::is_floating_point_v<Tm>) {
            std::uniform_real_distribution<Tm> dist(0, value_max);
            return dist(gen);
        }
        else {
            std::uniform_int_distribution<Tm> dist(0, value_max);
            return dist(gen);
        }
    }
    void arrayOut(std::string_view data_raw) {
        for (int raw_copy = 0; raw_copy < data_raw.size(); raw_copy++) {
            if (data_raw[raw_copy] == '\n') {
                for (int raw_copy_buf = 0; raw_copy_buf < raw_copy; raw_copy_buf++) {
                    std::string out = "";
                    out += data_raw[raw_copy_buf];
                    return ImGui::Text(std::format("{}", out).c_str());
                }
            }
        }
    }
};////////////////////////////////
