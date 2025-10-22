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
    bool SliderInt64(const char* label, int64_t* v, int64_t v_min, int64_t v_max, const char* format, ImGuiSliderFlags flags)
    {
        return SliderScalar(label, ImGuiDataType_S64, v, &v_min, &v_max, format, flags);
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
class CImage {
public:

    float x[2] = { 250,230};
    float y[2] = { 250,230 };
    int fIctx = 0;
    int fIcty = 0;
    GLuint gl_buffer;
    unsigned char CICharBuffer[256];
    bool CreateImg();
    bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height, unsigned char* imgBuffer);
    ImVec2 ResizeImage(uint64_t fCArrayFloat);
    uint64_t InitCImage(std::string png_file);
    bool CreateImage(int64_t w, int64_t h);
    std::string getAspectRatio(int x, int y);
    int64_t GetImageSize(int width, int height);
    int64_t GetVRAMSize(GLuint vbo);
    int64_t GetFileSize(const std::string& filename);
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
bool CImage::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height, unsigned char* imgBuffer) {
    int image_width = 0;
    int image_height = 0;
    static int ic = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (!image_data)
    {
        return false;
    }
    else {
        std::cout << "PahomEngine::GL--(debug_gl) buffer: ";
        for (size_t i = 0; i < std::min<size_t>(16, image_width * image_height * 4); ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)image_data[i] << " ";
        }
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);

        *out_texture = image_texture;
        *out_width = image_width;
        *out_height = image_height;

        glBindTexture(GL_TEXTURE_2D, 0);
        
    }
}//
struct KurlikAUDIO {
    std::string audiolist[7] = { "assets/audio/kurlik.wav",
                                 "assets/audio/voda.wav",
                                 "assets/audio/krik.wav",
                                 "assets/audio/pidoras.wav",
                                 "assets/audio/mrrobot.wav",
                                 "assets/audio/a.wav",
                                 "assets/audio/smex.wav"
    };
    void play(int64_t i);
    void play2(int64_t i);
    void play3(int64_t i);
    void getGain(int64_t idx);
    void vue();
    void VuePlay(int64_t idxd, int64_t idx);
    void getTimeline(int64_t idx);
    float convertToMinutes();
    int64_t idx = 0;
    float masterVolume = 0.02f;
    void pause();
    Audio::Sound audioDevice,audioDevice2,audioDevice3;
    int64_t i64CurrentTimeLine = 0;
    bool isDeviceActive = false;
    struct gain {
        float min = 0.0f;
        float max = 0.0f;
    };
    std::unique_ptr<gain> audioGain = std::make_unique<gain>();
    struct time {
        float current = 0.0f;
        float max = 0.0f;
        std::string formatTime(float seconds) {
            float minutes = floor(seconds / 60);
            float secs    = seconds;
            return (std::to_string(minutes) + ":" + (secs < 10 ? "0" : " ") + std::to_string(secs)).c_str();
    }
    };
    std::unique_ptr<time> audioTime = std::make_unique<time>();
};
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
void KurlikAUDIO::vue() {
    if (ImGui::BeginPopup("vue", ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("AudioEngine");
        ImGui::Separator();

        // ������ ��������� ��� ����������� ����
        const char* devices[] = { "ch0", "ch1", "ch2" };
        const char* filelist[] = {"1","2","3","4","5","6"};
        static int32_t i32idxDevice = 0, i32idxFiles = 0;
        for (int files = 0; files < 6; files++) {
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
        audioDevice.loadSound(audiolist[idx]);
        audioDevice.play();
        audioDevice.setVolume(masterVolume);
        isDeviceActive = audioDevice.isPlaying();
        break;
    case 1:
        audioDevice2.loadSound(audiolist[idx]);
        audioDevice2.play();
        audioDevice2.setVolume(masterVolume);
        isDeviceActive = audioDevice2.isPlaying();
        break;
    case 2:
        audioDevice3.loadSound(audiolist[idx]);
        audioDevice3.play();
        audioDevice3.setVolume(masterVolume);
        isDeviceActive = audioDevice3.isPlaying();
        break;
    }
}
void KurlikAUDIO::play(int64_t i) {
  //  PlaySoundA(file.c_str(), NULL, 1);
    idx = i;
    audioDevice.loadSound(audiolist[idx]);
    audioDevice.play();
    audioDevice.setVolume(masterVolume);
    isDeviceActive = audioDevice.isPlaying();
}
void KurlikAUDIO::play2(int64_t i) {
    //  PlaySoundA(file.c_str(), NULL, 1);
    idx = i;
    audioDevice2.loadSound(audiolist[idx]);
    audioDevice2.play();
    audioDevice2.setVolume(masterVolume);
    isDeviceActive = audioDevice2.isPlaying();
}
void KurlikAUDIO::play3(int64_t i) {
    //  PlaySoundA(file.c_str(), NULL, 1);
    idx = i;
    audioDevice3.loadSound(audiolist[idx]);
    audioDevice3.play();
    audioDevice3.setVolume(masterVolume + 0.1f);
    isDeviceActive = audioDevice3.isPlaying();
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


#define engine_bulid std::wstring(L"0.6.98a");

//
struct ASSETSDATA {
    std::string asset[9] = { "assets/logo.png",   //0
                             "assets/back.jpg",   //1
                             "assets/bread.png",  //2
                             "assets/pahom.png",  //3
                             "assets/pahom2.png",  //4
                             "assets/panel.png",
                             "assets/777.png",
                             "assets/vilka.png",
                             "assets/kefir.png"
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
            "Total:   " + std::to_string(i64MemoryTotal) + " GB"
            "\nUsed :   " + std::to_string(i64MemoryUsed)  + " GB"
            "\nFree :   " + std::to_string(i64MemoryFree)  + " GB";
        return mem_str;
    }
};
auto ptrMemory = std::make_unique<MEMORYDATA>();
struct GameEvent {
    std::string TextBufferStr;
    ImVec4 col;
    int64_t i64TimerEvent = 0;
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
        TextBufferStr = text;
        setColorText(col);
    }
    void setColorText(ImVec4 col) {
        col = col;
    }
    void clearEvent() {
        TextBufferStr.clear();
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
  
};
std::string logoPahom =
" %%%%%%  %%%%%  %%%  %%%   %%%   %%%    %%%\n"
" %%   %  %% %%  %%%  %%% %%   %% %% %  % %%\n"
" %%%%%   %%%%%  %%%%%%%% %%   %% %%  %%  %%\n"
" %%      %$ %%  %%%  %%% %%   %% %%  %%  %%\n"
" %%      %   %  %%%  %%%   %%%   %%      %%\n";
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
        i64MemoryUsageProcess=   pmc.WorkingSetSize;
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
                std::string funcName(symbol->Name);
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
                std::string fileName(line.FileName);
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
        std::string errorMsgA;
        std::string errorCodeA;
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
        std::wstring msg = sEngineInfoW + L" crashed\n=======================\nException: " + std::wstring(errorCode.begin(), errorCode.end()) + L"\n------------------- STACK TRACE ---------------\n" + std::wstring(stackTraceW.begin(), stackTraceW.end()) +
            L"\nSee crash_log.txt for details.\nPlease send crash_log.txt to @hcppstudio.";
        msg += L"\n" +  wlogo;
        MessageBox(NULL, msg.c_str(), titleProject.c_str(), MB_OK | MB_ICONERROR);

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
    std::vector<std::string >ccDiff = {
        "Легкая",
        "Средняя",
        "Поехавший"
    };
    int64_t i64id = 0;
    bool bRandScoreDiff = false;
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
struct PahomEngineStruct {
    //
    std::string sBuild = "0.6.98a";
    std::string sBuildGame = "0.6.1d";
    //
    std::wstring sWBuild = L"0.6.98a";
    std::wstring sWBuildGame = L"0.6.1d";
    //
    bool CVsync = true;
    uint64_t fCPoint = 0;
    int64_t fStep = 13;
    std::unique_ptr<CImage> img = std::make_unique<CImage>();
    std::unique_ptr<JoyStickAPI> ptrGamepad1 = std::make_unique<JoyStickAPI>(1);
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
    std::unique_ptr<EXCEPTIONS> Exceptions = std::make_unique<EXCEPTIONS>();
    std::unique_ptr<Diffinity> pDiff = std::make_unique<Diffinity>();
    // other value
    ///

    float fDeltaTime = 0;
    float fFrameRate = 0;

    //
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
    int64_t i64CPUDelay = 2;
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
    bool bBoost777 = false;
    bool bKefir = false;
    int64_t i64RandBoost = 0;
    int64_t i64ValuesRands[4] = { 50 , 100, 256, 777 };
    bool GetGamepadKey(int64_t iKey);
    int64_t i64BreadSize[2] = { 64,64 };
    int64_t i64PahomSize[2] = { 128,128 };
    void Text(ImVec4 col, std::string text);
    void log(std::string text);
    void Tbuffer();
    void logo();
    void progress_bar(float fragtion);
    bool getPressedKey(int8_t key, bool isTurned = false) {
        return (isTurned ? GetKeyState(key) & 0x8000 : GetKeyState(key) > 0);
    }
    void setTextCenterXY(const char* text);
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
void PahomEngineStruct::setTextCenterXY(const char* text) {
    ImVec2 fTextSize = ImGui::CalcTextSize(text);
    ImGui::SetCursorPos(ImVec2{
        (i64WindowSize[0] - fTextSize.x) / 2,
        (i64WindowSize[1] - fTextSize.y) / 2
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
    HANDLE hc = GetStdHandle(STD_OUTPUT_HANDLE);
    static int64_t i64ColorCh = 0;
    std::vector <std::string> array;
    std::string PAHOM_ENGINE =
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
    for (int64_t c = 0; c < PAHOM_ENGINE.size(); c++) {
        array.push_back(std::string(1, PAHOM_ENGINE[c]));
        if (array[c].empty() || array[c] == " ") {
            SetConsoleTextAttribute(hc, 15);
        }
        else {
            SetConsoleTextAttribute(hc, (c > (PAHOM_ENGINE.size() / 2) ? (rand() % 8 ) : 15));
            i64ColorCh++;
        }
        std::cout << PAHOM_ENGINE[c];
    }
    // ���������� ���� �������
    SetConsoleTextAttribute(hc, 7); // ����������� ���� (����� ����� �� ������ ����)
    std::cout << "Colored:" << i64ColorCh << " LogoSize: " << PAHOM_ENGINE.size() << std::endl;
}
void PahomEngineStruct::progress_bar(float fragtion) {
    int64_t idx = static_cast<float>(fragtion) * 10;
    COORD nullcd = { 0,0 };
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hcon, nullcd);
    std::string str_array[] = {
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
    return true;
}
auto PahomEngine = std::make_unique<PahomEngineStruct>();

