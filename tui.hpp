#include <iostream>
#include <string>
#include <format>
#include <random>
struct ColorV3u32 {
    uint32_t a, b, c;
    ColorV3u32() : a(0), b(0), c(0) {}
    ColorV3u32(uint32_t _a, uint32_t _b, uint32_t _c) : a(_a), b(_b), c(_c) {}
};
struct ColorV3Logs {
    void pout(const std::string& str, const ColorV3u32& col, const ColorV3u32& colBack, bool isBackground = false, bool isCursorHide = true) {
        if (isBackground) {
            if (isCursorHide)
            {
                std::cout << "\x1b[?25l" << std::endl;
            }
            std::cout << std::format("\x1b[48;2;{};{};{}m\x1b[38;2;{};{};{}m{}\x1b[0m",
                colBack.a, colBack.b, colBack.c,
                col.a, col.b, col.c,
                str);
        }
        else {
            std::cout << std::format("\x1b[38;2;{};{};{}m{}\x1b[0m",
                col.a, col.b, col.c, str);
        }
    }
};
ColorV3Logs cv3;
struct int64Vec2 {
    int64_t x, y;
    constexpr int64Vec2() : x(0L),y(0L){}
    constexpr int64Vec2(int64_t _x, int64_t _y) : x(_x), y(_y) {}
};
struct ColorV3 {
    uint8_t r, g, b;
    constexpr ColorV3() : r(0), g(0), b(0) {}
    constexpr ColorV3(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {}
    std::string getHex() const {
        std::string string_formatted = std::format("{:02X}{:02X}{:02X}", this->r, this->g, this->b);
        return string_formatted;
    }
    ColorV3 setU8Color(uint32_t red, uint32_t green, uint32_t blue){
        ColorV3 rgbU32 = {};
        rgbU32.r = static_cast<uint8_t>(red);
        rgbU32.g = static_cast<uint8_t>(green);
        rgbU32.b = static_cast<uint8_t>(blue);
        return rgbU32;
    }
    
    
};
struct ConsoleText {
    void pout(const std::string& str, const ColorV3& col, const ColorV3& colBack, bool isBackground = false, bool isCursorHide = true) {
        if (isBackground) {
            if (isCursorHide)
            {
                std::cout << "\x1b[?25l" << std::endl;
            }
            std::cout << std::format("\x1b[48;2;{};{};{}m\x1b[38;2;{};{};{}m{}\x1b[0m",
                colBack.r, colBack.g, colBack.b,
                col.r, col.g, col.b,
                str);
        }
        else {
            std::cout << std::format("\x1b[38;2;{};{};{}m{}\x1b[0m",
                col.r, col.g, col.b, str);
        }
    }
    template <class... Tm>
    void pout_fmt(const std::format_string<Tm...> _Fmt, Tm&&... _Args, const ColorV3& col, const ColorV3& colBack, bool isBackground = false) {
        pout(_STD vformat(_Fmt.get(), _STD make_format_args(_Args...)), col, colBack, isBackground);
    }
    std::random_device rd;
    uint8_t rnd(uint8_t rnd_max) {
        std::mt19937 gen(ConsoleText::rd());
        std::uniform_int_distribution<int> dist(0, rnd_max);
        return static_cast<uint8_t>(dist(gen));
    }
    ColorV3 randColor() {
        return ColorV3{ rnd(255),rnd(255),rnd(255)};
    }
    void TestColors(int64Vec2 sizeMax) {
        int nCount = 0;
        for (int cubeX = 0; cubeX < sizeMax.x; cubeX++) {
            for (int cubeY = 0; cubeY < sizeMax.y; cubeY++) {
                nCount++;
                if (nCount >= sizeMax.x) {
                    std::cout << "\n";
                    nCount = 0;
                }
                pout("  ", { 0,0,0 }, randColor(), true,false);
            }
        }
    }
    void GetHEXString(uint8_t r, uint8_t g, uint8_t b) {
        ColorV3 colorRGB = { r,g,b };
        pout(colorRGB.getHex(), { 255,255,255 }, { 0,0,0 }, false);
    }
    ColorV3 SetValIV4(ImVec4 color) {
        ColorV3 outBuffer = {
            static_cast<uint8_t>(color.x * 255),
            static_cast<uint8_t>(color.y * 255),
            static_cast<uint8_t>(color.z * 255)
        };
        return outBuffer;
    }
    template<typename Val>
    ColorV3 setVal(Val r, Val g, Val b) {
        ColorV3 rgbVal = {};
        if (std::is_arithmetic_v<Val> || std::is_floating_point_v<Val>) {
            rgbVal.r = static_cast<Val>(r);
            rgbVal.g = static_cast<Val>(g);
            rgbVal.b = static_cast<Val>(b);
        }
        else {
            pout("ColorV3 Vector Error: Val is Arthmetic && Floating value", { 255,0,120 }, { 0,0,0 }, false, false);
            rgbVal = { 0,0,0 };
        }
        return rgbVal;
    }
};
ConsoleText console;



