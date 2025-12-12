#include <GL/GL.h>
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
#define WGL_CONTEXT_FLAGS_ARB                     0x2094
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
HGLRC CreateGL(HDC hDC, int major, int minor)
{
    HGLRC hRC_temp = wglCreateContext(hDC);
    if (!hRC_temp) return NULL;

    if (!wglMakeCurrent(hDC, hRC_temp))
    {
        wglDeleteContext(hRC_temp);
        return NULL;
    }
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if (!wglCreateContextAttribsARB)
    {
        wglMakeCurrent(NULL, NULL);
        return hRC_temp; 
    }
    const int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, major,
        WGL_CONTEXT_MINOR_VERSION_ARB, minor,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0 
    };

    HGLRC hRC_final = wglCreateContextAttribsARB(hDC, 0, attribs);
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC_temp);

    return hRC_final;
}
#include <vector>
#include <thread>
#include <random>
#include <format>
struct colorU32 {
    uint32_t r, g, b, a;
    colorU32() : r(0), g(0), b(0), a(0) {}
    colorU32(uint32_t _r, uint32_t _g, uint32_t _b, uint32_t _a) : r(_r), g(_g), b(_b), a(_a) {}
    uint32_t get() const {
        return (((uint32_t)(this->a) << 24) | ((uint32_t)(this->b) << 16) | ((uint32_t)(this->g) << 8) | ((uint32_t)(this->r) << 0));
    }
    ImVec4 getIV4() const {
        colorU32 colorGet = { this->r,this->g,this->b,this->a };
        return ImVec4(
            static_cast<float>(this->r / 255),
            static_cast<float>(this->g / 255),
            static_cast<float>(this->b / 255),
            static_cast<float>(this->a / 255)

        );
    }
    std::string toStringView() const {
        return std::format("U32RGBA({},{},{},{})->0x{}", this->r, this->g, this->b, this->a, colorU32(this->r, this->g, this->b, this->a).get());
    }
   
};
uint32_t ToU32(ImVec4 color) {
    ImVec4 col = { color.x * 255 ,color.y * 255,color.z * 255,color.w * 255 };
    return (((uint32_t)(col.w) << 24) | ((uint32_t)(col.z) << 16) | ((uint32_t)(col.y) << 8) | ((uint32_t)(col.x) << 0));
}
struct GLM {
    std::vector<uint32_t>pixel_buffer;
    int64_t i64BrushSize = 20;
    GLuint textureID;
    int width_texture = 0,height_texture = 0;
    void InitTexture() {
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_texture, height_texture, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, pixel_buffer.data());
    }
    void SetSize(int w, int h) {
        width_texture = w;
        height_texture = h;
        pixel_buffer.resize(w * h); 
        std::fill(pixel_buffer.begin(), pixel_buffer.end(), 255);
    }
    void SetPixel(int x, int y, uint32_t color) {
        if (x >= 0 && x < width_texture && y >= 0 && y < height_texture) {
            pixel_buffer[y * width_texture + x] = color;
        }
    }
    void UpdateTexture() {
        if (textureID == 0) return;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexSubImage2D(
            GL_TEXTURE_2D,           
            0,                      
            0, 0,                   
            width_texture, height_texture,
            GL_RGBA, GL_UNSIGNED_BYTE,
            pixel_buffer.data()
        );
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void fillSqware(int size_x, int size_y, bool isEnableMultiThreads = false) {
        if (!isEnableMultiThreads) {
            for (int x = 0; x < size_x; x++) {
                for (int y = 0; y < size_y; y++) {
                    SetPixel(x, y, colorU32(rand() % 255, rand() % 255, rand() % 255, rand() % 255).get());
                }
            }
        }
        else {
            uint32_t num_threads = std::jthread::hardware_concurrency();
            size_t total_pixels = (size_t)size_x * size_y;
            size_t chunk_size = total_pixels / num_threads;

            std::vector<std::jthread> tCPUThreads;

            for (uint32_t t = 0; t < num_threads; ++t) {

                size_t start_index = t * chunk_size;
                size_t end_index = (t == num_threads - 1) ? total_pixels : (t + 1) * chunk_size;

                tCPUThreads.emplace_back([=] {
                    for (size_t i = start_index; i < end_index; ++i) {

                        uint32_t random_rgba = colorU32(rand() % 255,rand() % 255, rand() % 255,255).get();
                        int x = i % size_x;
                        int y = i / size_x;
                        this->pixel_buffer[i] = random_rgba;
                    }
                    });
            }
        }
        
    }

    void Pen(int draw_x,int draw_y, colorU32 u32Color) {
        int brush_size = 20;
        int half_brush = brush_size / 2;
            for (int x = draw_x - half_brush; x < draw_x + half_brush; x++) {
                for (int y = draw_y - half_brush; y < draw_y + half_brush; y++) {
                    if (x >= 0 && x < width_texture &&
                        y >= 0 && y < height_texture)
                    {
                        SetPixel(x, y, u32Color.get());
                    }
                }
            }
    }
};
