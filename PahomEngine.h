#define CRT_NO_WARNINGS 1
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define PAHOM_ENGINE_ID (int64_t)0x17
#include <windows.h>
#include <GL/GL.h>
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
#include <Audio/Sound.hpp>
#include "JoyStick.h"
#pragma comment(lib,"Audio.lib")


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

    // Return the string content as std::string
    return std::string(str);
}
//
namespace ImGui {
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
        ImU32 color_knob = *v ? ImGui::GetColorU32(ImVec4(0.34f, 0.06f, 0.98f, 1.00f)) :
            ImGui::GetColorU32(ImVec4(0.36f, 0.36f, 0.36f, 1.0f));
        ImU32 glow_color = ImGui::GetColorU32(ImVec4(0.34f, 0.06f, 0.98f, 0.3f)); // ���� ��������
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
                    ImGui::GetColorU32(ImVec4(0.34f, 0.06f, 0.98f, alpha)),
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
    bool InputInt64(const char* label, int64_t* v, int64_t step, int64_t step_fast, ImGuiInputTextFlags flags)
    {
        // Hexadecimal input provided as a convenience but the flag name is awkward. Typically you'd use InputText() to parse your own data, if you want to handle prefixes.
        const char* format = (flags & ImGuiInputTextFlags_CharsHexadecimal) ? "%08X" : "%d";
        return InputScalar(label, ImGuiDataType_S64, (void*)v, (void*)(step > 0 ? &step : NULL), (void*)(step_fast > 0 ? &step_fast : NULL), format, flags);
    }
}
ImVec4 RGBAtoIV4(int r, int g, int b, int a) {
    float newr = r / 255;
    float newg = g / 255;
    float newb = b / 255;
    float newa = a;
    return ImVec4(newr, newg, newb, newa);
}
class CImage {
public:

    float x[2] = { 250,230};
    float y[2] = { 250,230 };
    int fIctx = 0;
    int fIcty = 0;
    GLuint gl_buffer;
    unsigned char CICharBuffer;
    bool CreateImg();
    bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height, unsigned char* imgBuffer);
    ImVec2 ResizeImage(uint64_t fCArrayFloat);
    uint64_t InitCImage(std::string png_file);
    bool CreateImage(int64_t w, int64_t h);
    std::string getAspectRatio(int x, int y);
    int64_t GetImageSize(std::string filename);
};
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
bool CImage::CreateImage(int64_t w, int64_t h) {
    ImGui::Image((int64_t)(void*)CImage::gl_buffer, ImVec2(w, h));
    return true;
}
uint64_t CImage::InitCImage(std::string png_file) {
    LoadTextureFromFile((png_file).c_str(), &gl_buffer, &fIctx, &fIcty, &CICharBuffer);
    return 3;
}
int64_t CImage::GetImageSize(std::string filename) {
    std::ifstream ImageStream(filename, std::ios::binary | std::ios::ate);
    std::streampos fileSize = ImageStream.tellg();
    return fileSize;
}
bool CImage::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height, unsigned char* imgBuffer) {
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (!image_data)
    {
        return false;
    }
    else {
        *imgBuffer = *image_data;
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);

        *out_texture = image_texture;
        *out_width = image_width;
        *out_height = image_height;

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}//
struct KurlikAUDIO {
    std::string audiolist[5] = { "assets/audio/kurlik.wav",
                                 "assets/audio/voda.wav",
                                 "assets/audio/krik.wav",
                                 "assets/audio/aaa.wav",
                                 "assets/audio/mrrobot.wav"};
    void play(int64_t i);
    void play2(int64_t i);
    int64_t idx = 0;
    float masterVolume = 0.02f;
    void pause();
    Audio::Sound audioDevice,audioDevice2;
   
    
};
void KurlikAUDIO::play(int64_t i) {
  //  PlaySoundA(file.c_str(), NULL, 1);
    idx = i;
    audioDevice.loadSound(audiolist[idx]);
    audioDevice.play();
    audioDevice.setVolume(masterVolume);
}
void KurlikAUDIO::play2(int64_t i) {
    //  PlaySoundA(file.c_str(), NULL, 1);
    idx = i;
    audioDevice2.loadSound(audiolist[idx]);
    audioDevice2.play();
    audioDevice2.setVolume(masterVolume);
}
void KurlikAUDIO::pause() {
    audioDevice.pause();
}
struct STRINGSDATA {
    void log(std::string text,std::string moduleName) {
        std::cout<<"[PahomEngine::"+std::format("%s]",moduleName) << text << std::endl;
   }
    std::string PAHOM_ENGINE =
        " ����������     ����    ���    ���   ����   ���    �����   \n"
        " ��      ��   ��   ��   ���    ��� ��    �� �� �� ��  ��   \n"
        " ��     ��    ��   ��   ���������� ��    �� ��  ���   ��   \n"
        " �������      �������   ���    ��� ��    �� ��   �    ��   \n"
        " ��           ��   ��   ���    ���   ����   ��        ��   \n"
        "                Engine b0.23.                            \n";
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
struct ASSETSDATA {
    std::string asset[6] = { "assets/logo.png",   //0
                             "assets/back.jpg",   //1
                             "assets/bread.png",  //2
                             "assets/pahom.png",  //3
                             "assets/pahom2.png",  //4
                             "assets/panel.png"
    }; //5


    bool validFiles(std::string file) {
        std::ifstream fileAssets(file);
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
    uint8_t TextureBufferArray[256];
    int TextureX[256];
    int TextureY[256];
};
struct KEYMAPDATA {
    int8_t u8FORWARD = 'A';
    int8_t u8BACK = 'D';
    int8_t u8RESET = 'R';
    int8_t u8SPACE = VK_SPACE;
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
struct MEMORYDATA {
    int64_t i64MemoryTotal = 0;
    int64_t i64MemoryFree  = 0;
    int64_t i64MemoryUsed  = 0;
    //
    MEMORYSTATUS MemoryPtr;
    void getData(int64_t* mem,int64_t idx_data){
        memset(&MemoryPtr, 0, sizeof(MemoryPtr));
        ::GlobalMemoryStatus(&MemoryPtr);
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
            "\nTotal:   " + std::to_string(i64MemoryTotal) + " GB"
            "\nUsed :   " + std::to_string(i64MemoryUsed)  + " GB"
            "\nFree :   " + std::to_string(i64MemoryFree)  + " GB";
        return mem_str;
    }
};
auto ptrMemory = std::make_unique<MEMORYDATA>();
struct GameEvent {
    std::string TextBufferStr;
    ImVec4 col;
    bool isTextHidden = false;
    int64_t i64WindowSize[2] = { 800 , 600 };
    void TextBuffer() {
        ImVec2 f2TextCenterPos = {
           (i64WindowSize[0] - ImGui::CalcTextSize(TextBufferStr.c_str()).x) / 2,
           (i64WindowSize[1] - ImGui::CalcTextSize(TextBufferStr.c_str()).y) / 2,
        };
        ImGui::SetCursorPos(
            f2TextCenterPos
        );
        if (!isTextHidden) {
         
            ImGui::TextColored(col, TextBufferStr.c_str());
        }
        
    }
    void Text(ImVec4 col, std::string text) {
        TextBufferStr += "\n" + text;
        setColorText(col);
    }
    void setColorText(ImVec4 col) {
        col = col;
    }
    void clearEvent() {
        TextBufferStr.clear();
    }
  
};
struct PahomEngineStruct {
    bool CVsync = true;
    uint64_t fCPoint = 0;
    int64_t fStep = 13;
    CImage* img = new CImage;
    bool StyleLoad();
    ImVec4 RGBA(float r, float g, float b, float a);
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
    // other value
    bool bSettings = false;
    ImVec4 fillColorRGBA;//RGBA(133, 133, 133, 255)
    int64_t i64WindowSize[2] = { 800 , 600 };
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
    bool bLoadingFrame = true; bool bLoadingFrameOK = false;
    bool bControlsIsGamepad = false;
    bool bControlsIsKeyboard = false;
    bool bDebug = true;
    bool bFullscreen = false;
    bool bGameOver = false;
    // main flags
    bool bStartGame = false;
    float fStepMove = 6.0f;
    void InputI64(const char* id, int64_t* v);
    void InputFloat(const char* id, float* v);
    void reloadBreadPos();
    int64_t rand64(int64_t in_v);
    float randfloat(float in_v);
    bool CheckColiision();
    bool bStartGameFlag = false;
    bool bIsRevesed = false;
    bool bDebugText = false;
    bool GetGamepadKey(int64_t iKey);
    JoyStickAPI* ptrGamepad1 = new JoyStickAPI(1);
    int64_t i64BreadSize[2] = { 64,64 };
    int64_t i64PahomSize[2] = { 128,128 };
    void Text(ImVec4 col, std::string text);
    void log(std::string text);
    void Tbuffer();
    void logo();
    void progress_bar(float fragtion);
};
void  PahomEngineStruct::log(std::string text) {
    std::cout << " [PahomEngine::log] " << text << std::endl;
}
void  PahomEngineStruct::Text(ImVec4 col, std::string text) {
    PahomEngineStruct::Event.Text(col,text);
}
void  PahomEngineStruct::Tbuffer() {
    PahomEngineStruct::Event.TextBuffer();
}
bool  PahomEngineStruct::GetGamepadKey(int64_t iKey) {
   return ptrGamepad1->GetState().Gamepad.wButtons == iKey ? true : false;
}
int64_t PahomEngineStruct::rand64(int64_t in_v) {
    std::random_device rd; // ������������� ������
    std::mt19937 gen(rd()); // ������������� ����������
    std::uniform_int_distribution<int64_t> dist(2, in_v);//���������� ���� ��� ���������
    return dist(gen); //��������� �����
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
bool PahomEngineStruct::CheckColiision() {
    return (
        fPahomPosX < fBreadPosX + i64BreadSize[0] &&
        fPahomPosX + i64PahomSize[0] > fBreadPosX &&
        fPahomPosY < fBreadPosY + i64BreadSize[1] &&
        fPahomPosY + i64PahomSize[1] > fBreadPosY
        );
    /*return (
        playerBox.x < breadBox.x + breadBox.width &&
        playerBox.x + playerBox.width > breadBox.x &&
        playerBox.y < breadBox.y + breadBox.height &&
        playerBox.y + playerBox.height > breadBox.y
        );*/
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
void PahomEngineStruct::logo() {
    std::string PAHOM_ENGINE =
        " ����������     ����    ���    ���   ����   ���    �����   \n"
        " ��      ��   ��   ��   ���    ��� ��    �� �� �� ��  ��   \n"
        " ��     ��    ��   ��   ���������� ��    �� ��  ���   ��   \n"
        " �������      �������   ���    ��� ��    �� ��   �    ��   \n"
        " ��           ��   ��   ���    ���   ����   ��        ��   \n"
        "                Engine b0.23.                            \n";
    std::cout << PAHOM_ENGINE << std::endl;
}
void PahomEngineStruct::progress_bar(float fragtion) {
    int64_t idx = static_cast<float>(fragtion) * 10;
    COORD nullcd = { 0,0 };
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hcon, nullcd);
    std::string str_array[18] = {
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
bool PahomEngineStruct::StyleLoad() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = RGBA(7,7,9,255);
    colors[ImGuiCol_ChildBg] = ImVec4(0.04f, 0.04f, 0.04f, 0.94f);
    colors[ImGuiCol_PopupBg] = RGBA(5, 5, 7, 255);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 1.0f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.04f, 0.04f, 0.04f, 0.94f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.48f, 0.26f, 0.98f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.37f, 0.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.353f, 0.157f, 1.000f, 1.000f);
    colors[ImGuiCol_TitleBgActive] = RGBAtoIV4(135, 165, 255, 155);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.50f, 0.60f, 1.0f, 1.0f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.41f, 0.00f, 1.00f, 0.40f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.48f, 0.26f, 0.98f, 0.52f);
    colors[ImGuiCol_Button] = RGBA(7, 7, 9, 255);
    colors[ImGuiCol_ButtonHovered] = RGBA(35, 35, 55, 255);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_Header] = RGBA(5, 5, 5, 255);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.15f, 0.15f, 0.15f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
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
    colors[ImGuiCol_PlotLines] = RGBA(5, 5, 7, 255);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = RGBA(35, 35, 55, 255);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    //colors[ImGuiCol_TextLink] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    // colors[ImGuiCol_NavCursor] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    style.FrameBorderSize = 1;
    return 0;
}
PahomEngineStruct* PahomEngine = new PahomEngineStruct;

