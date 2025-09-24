#include "PahomEngine.h"
#pragma once



struct WGL_WindowData { HDC hDC; };
static int64_t fwop = 0;
// Data
static HGLRC            g_hRC;
static WGL_WindowData   g_MainWindow;
static int              g_Width;
static int              g_Height;
ImVec4 clear_color = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);

// Forward declarations of helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data);
void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data);

//
void ResetDeviceWGL();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//
uint64_t fMX = 0;
uint64_t fMY = 0;
bool b_FatalError = false;
int64_t ptrint64_t(GLuint tx) {
    return (int64_t)(void*)tx;
}
int main(int, char** argv)
{

   // str_stack(&b_FatalError,"b_FatalError");
    ImDrawListSplitter JEApp;
    const wchar_t* WindowTitle = L"SweetBreads(PahomEngineGL)";
    ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_OWNDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"SweetBreads(PahomEngineGL)", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, WindowTitle, WS_OVERLAPPEDWINDOW | WS_EX_TOOLWINDOW | WS_EX_NOPARENTNOTIFY, 100, 80, 800, 600, nullptr, nullptr, wc.hInstance, nullptr);
   ::SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_SIZEBOX);
   wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(103));//
    if (!CreateDeviceWGL(hwnd, &g_MainWindow))
    {
        CleanupDeviceWGL(hwnd, &g_MainWindow);
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }
    wglMakeCurrent(g_MainWindow.hDC, g_hRC);
    //
    ::AnimateWindow(hwnd, 100, AW_BLEND);
    //
    ::UpdateWindow(hwnd);
   
    //
    IMGUI_CHECKVERSION();
    //
    ImGui::CreateContext();
    //
    static     int64_t i64Cidx = 0;
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    int fTextSize = 20;
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_InitForOpenGL(hwnd);
    if (!ImGui_ImplOpenGL3_Init()) {
        PahomEngine->log("[Fatal Error] OGL3 NOT SUPPORT ON YOU GPU");
    }
    else {
        PahomEngine->logo();
        PahomEngine->log("(OGL3)::InitToWIN32 OK!");
      
    }
    io.Fonts->AddFontFromFileTTF("assets/BOUNDED.ttf", 20.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());//
    struct STRGPUGL {
        std::string E_Brand = (const char*)glGetString(GL_VENDOR);
        std::string E_Model = (const char*)glGetString(GL_RENDERER);
        std::string E_GLVer = (const char*)glGetString(GL_VERSION);
    };
    auto sGPU = std::make_unique<STRGPUGL>();
    int CPUInfo[4] = { -1 };
    unsigned   nExIds, i = 0;
    char CPUBrandString[0x40];
    // Get the information associated with each extended ID.
    __cpuid(CPUInfo, 0x80000000);
    nExIds = CPUInfo[0];
    for (i = 0x80000000; i <= nExIds; ++i)
    {
        __cpuid(CPUInfo, i);
        // Interpret CPU brand string
        if (i == 0x80000002)
            memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000003)
            memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
        else if (i == 0x80000004)
            memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    }
    std::string dCPUBrandString = CPUBrandString;
    ///
    // Main loop
    std::string mainText = " SWEETBREADS\n"
                           " Это игра создана по фану от нехуй делать\n"
                           " В выпечке сладкого хлеба участие принимали:\n-------------------\n Umbrella - 150р\n PRi8etA - 120р\n Пук - 40р\n Prosto_cheliik2 - 100р\n--------------------\n Игра поддерживает все геймпады Xinput\n (JoyStickAPI by HCPP) и клавиатуру\nРекомендую зайти в опции и настроить звук\nи другое..\n"
                           " Разработал HCPP   ";
    int64_t i64StrMainTextSize = mainText.size();
   
    bool main_logo = true;
    bool done = false;
    uint64_t a = 0; uint64_t b = 0;
    typedef int64_t* intptr64_t;
    uint64_t err = 0;
    ImFontAtlas* fonts = ImGui::GetIO().Fonts;
    ImFont* font15 = io.Fonts->AddFontFromFileTTF("assets/BOUNDED.ttf", 30.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    ImFont* font20 = io.Fonts->AddFontFromFileTTF("assets/BOUNDED.ttf", 20.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    ImFont* font3 = io.Fonts->AddFontFromFileTTF("assets/BOUNDED.ttf", 15, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    ImFont* font10 = io.Fonts->AddFontFromFileTTF("assets/BOUNDED.ttf", 10, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    ImFont* font50 = io.Fonts->AddFontFromFileTTF("assets/BOUNDED.ttf", 40, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    if (PahomEngine->ptrGamepad1->IsConnected()) {
        std::cout << "[PahomEngine::Controls->JoyStickAPI] Gamepad 0 connected!" << std::endl;
    }
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                exit(0);
            //  done = true;
            if (msg.message == WM_DESTROY)
                exit(0);
        }


        if (done)
            break;
        // Start the Dear ImGui frame//
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
           
            JEApp.ClearFreeMemory();
            PahomEngine->StyleLoad();

        }
        JEApp.ClearFreeMemory();
        static std::string sError;
        if (PahomEngine->CVsync) {Sleep(13);} else { Sleep(0);}
        static bool bLoaderFiles = false;
        static bool ftx = 0.0f;
        static int64_t i64OGL3TxCount = 0, i64Exceptions = 0, i64OGL3Errors = 0, i64OGL3TxTotalSize = 0;
        ImGui::Begin("gameFrame", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        if (ImGui::BeginPopup("ogl3_error", ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("PahomEngine::exception");
            ImGui::Separator();
            ImGui::PushFont(font10);
            ImGui::TextColored(PahomEngine->RGBA(233, 0, 65, 255), "%s",sError.c_str());
            ImGui::PopFont();
            ImGui::EndPopup();
        }
        ImGui::SetWindowSize(ImVec2(PahomEngine->i64WindowSize[0], PahomEngine->i64WindowSize[1]));
        ImGui::SetWindowPos(ImVec2(0, 0));
        if (PahomEngine->bLoadingFrame) {
            b_FatalError = false;
            if (bLoaderFiles) {
                for (int tid = 0; tid <= 5; tid++) {
                    //PahomEngine->progress_bar(ftx);
                    if (PahomEngine->img->LoadTextureFromFile(reinterpret_cast<const char*>(PahomEngine->assets.asset[tid].c_str()),
                        &PahomEngine->ImageData.TextureArray[tid],
                        &PahomEngine->ImageData.TextureX[tid],
                        &PahomEngine->ImageData.TextureY[tid],
                        &PahomEngine->ImageData.TextureBufferArray[tid]))
                    {
                        i64OGL3TxTotalSize += PahomEngine->img->GetImageSize(PahomEngine->assets.asset[tid]);
                        i64OGL3TxCount++;
                        ftx = (float)tid / 5;
                        std::cout << "[PahomEngine::OGL] Load texture:"
                            << PahomEngine->assets.asset[tid]
                            << " id:" << tid << " ptrid(void*)"
                            << PahomEngine->ImageData.TextureArray[tid]
                            << " stack:" << &PahomEngine->ImageData.TextureArray[tid] << std::endl;
                    }
                    else {
                        std::cout << "[PahomEngine::OGL] Load texture:" << PahomEngine->assets.asset[tid] << " error!" << std::endl;
                        i64Exceptions++;
                        sError += "\n Load Texture Error!! \nstack="+ str_stack(&PahomEngine->ImageData.TextureBufferArray[tid],"PahomEngine->ImageData.TextureBufferArray[tid]") + "\nfunc:PahomEngine->img->LoadTextureFromFile(reinterpret_cast<const char*>(PahomEngine->assets.asset[tid].c_str()),\n"
                            " & PahomEngine->ImageData.TextureArray[tid],\n"
                            "  & PahomEngine->ImageData.TextureX[tid],\n"
                            "& PahomEngine->ImageData.TextureY[tid],\n"
                            "& PahomEngine->ImageData.TextureBufferArray[tid])\n";
                        ImGui::OpenPopup("ogl3_error");
                    }
            }
               bLoaderFiles = false;
               PahomEngine->bLoadingFrameOK = true;
            }
            a++;
            if (a > 2) {
                a = 0;
                static float fcl = 0;
                fcl += 25.5f;
                if (fcl < 255) {
                    std::cout << "[PahomEngine::TextAlpha] flc:" << fcl << "b: " << b << std::endl;
                }
                if (fcl == 255) {
                    fcl = 255;
                    b = 255;
                    bLoaderFiles = true;
                   

                }
                PahomEngine->fillColorRGBA = PahomEngine->RGBA(133, 133, 133, fcl);
            }
            PahomEngine->setTextCenter(mainText.c_str());
            ImGui::SetCursorPosY(150);
            ImGui::TextColored(PahomEngine->fillColorRGBA, mainText.c_str());
            if (GetAsyncKeyState(VK_F1)) {
                ImGui::OpenPopup("about");
            }
            if (ImGui::BeginPopup("about", ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::PushFont(font50);
                ImGui::TextColored(PahomEngine->RGBA(200, 0, 70, 255), "PAHOMENGINE");
                ImGui::PopFont();
                ImGui::Separator();
                ImGui::Text("build:  0.25  (amd64)");
                ImGui::Text("Writtein C++20 (MSVC-amd64)");
                ImGui::Text("GAPI: OGL3");
                ImGui::Text("AAPI: MinAudio_0.001");
                ImGui::Text("********* DEBUG ***********");
                ImGui::Text("exceptions        : %d", i64Exceptions);
                ImGui::Text("render_errors     : %d", i64OGL3Errors);
                ImGui::Text("textures_to_loaded: %d/6", i64OGL3TxCount);
                ImGui::Text("vram_used_textures: %d KB", i64OGL3TxTotalSize / 1024);
                ImGui::EndPopup();
            }
            if (!PahomEngine->bLoadingFrameOK) {
                PahomEngine->setTextCenter("Проверка данных");
                ImGui::TextColored(PahomEngine->fillColorRGBA, "Проверка данных\n%i:%i", i64Cidx, a);
                ImGui::SameLine();
                ImGui::Spinner(" ", 12, 2, ImGui::GetColorU32(PahomEngine->RGBA(35, 35, 45, 255)));
                if (i64Cidx == 3) {
                    bLoaderFiles = false;
                    PahomEngine->bLoadingFrameOK = true;
                }
            }
            else {
                static int32_t iFileIdx = 0;
                    
                    if (b == 255) {
                    
                        if (ImGui::BeginPopupModal("settings_engine", &PahomEngine->bSettings,ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar)) {
                            ImGui::TextColored(PahomEngine->RGBA(133, 133, 133, 255), "Опции");
                                ImGui::Separator();
                            ImGui::SetCursorPosX(10);
                            ImGui::TextColored(PahomEngine->RGBA(0, 234, 30, 255), "громкость");
                           
                            ImGui::SameLine();
                            
                           
                            if (ImGui::IsItemClicked()) {
                                ImGui::OpenPopup("volume_custom");
                            }
                            if (ImGui::BeginPopup("volume_custom", ImGuiWindowFlags_AlwaysAutoResize)) {
                                ImGui::SetCursorPosX(10);
                                ImGui::TextColored(PahomEngine->RGBA(0, 234, 30, 255), "громкость");
                                ImGui::SliderFloat("Vol", &PahomEngine->audio.masterVolume, 0.03f, 1.0f);
                                if (PahomEngine->audio.masterVolume == PahomEngine->audio.masterVolume + 0.01) {
                                    PahomEngine->audio.play2(iFileIdx);
                                }
                                ImGui::EndPopup();
                            }
                            ImGui::ProgressBar(PahomEngine->audio.masterVolume, ImVec2(200, 20), (std::to_string(int(PahomEngine->audio.masterVolume * 100)) + "%").c_str());
                           // PahomEngine->progress_bar(PahomEngine->audio.masterVolume);
                            ImGui::SameLine();
                            if (ImGui::Button("+", ImVec2(30, 30))) {
                                PahomEngine->audio.masterVolume += 0.01;
                                if (PahomEngine->audio.masterVolume == 0) {
                                    PahomEngine->audio.masterVolume = 0;
                                }
                                PahomEngine->audio.play2(iFileIdx);
                            }
                            ImGui::SameLine();
                            if (ImGui::Button("-", ImVec2(30, 30))) {
                                PahomEngine->audio.masterVolume -= 0.01;
                                if (PahomEngine->audio.masterVolume == 1) {
                                    PahomEngine->audio.masterVolume = 1;
                                }
                                PahomEngine->audio.play2(iFileIdx);
                            }
                            ImGui::SetCursorPosX(10);
                            ImGui::TextColored(PahomEngine->RGBA(0, 234, 30, 255), "Фуллскрин");
                            ImGui::SameLine();
                            ImGui::Checkbox(".", &PahomEngine->bFullscreen);
                            if (PahomEngine->bFullscreen) {

                                PahomEngine->i64WindowSize[0] = PahomEngine->HwndWSizeA(hwnd).x;
                                PahomEngine->i64WindowSize[1] = PahomEngine->HwndWSizeA(hwnd).y;
                                ShowWindow(hwnd, SW_SHOWMAXIMIZED);

                            }
                            else {
                                ShowWindow(hwnd, SW_SHOWNORMAL);
                                PahomEngine->i64WindowSize[0] = 800;
                                PahomEngine->i64WindowSize[1] = 600;
                            }
                            ImGui::SetCursorPosX(10);
                            ImGui::TextColored(PahomEngine->RGBA(0, 234, 30, 255), "60гц (60 фпс лимит)"); ImGui::SameLine();
                            ImGui::Checkbox("60 фпс", &PahomEngine->CVsync);//PahomEngine->CVsync
                            ImGui::SameLine();
                            
                            ImGui::TextColored(PahomEngine->RGBA(0, 234, 30, 255), "%s",(PahomEngine->CVsync ? "60фпс" : "Неогран.фпс"));
                            ImGui::TextColored(PahomEngine->RGBA(30, 30, 60, 255), "CPU:%s", dCPUBrandString.c_str());
                            ImGui::TextColored(PahomEngine->RGBA(30, 30, 60, 255), "GPU:%s", sGPU->E_Model.c_str());
                            ImGui::TextColored(PahomEngine->RGBA(30, 30, 60, 255), "%s", ptrMemory->MemoryInfo().c_str());
                            ImGui::SetCursorPosX(10);
                            if (ImGui::Button("OK", ImVec2(400, 40))) {
                                PahomEngine->bSettings = false;
                                ImGui::CloseCurrentPopup();
                            }
                            ImGui::EndPopup();
                    }
                    PahomEngine->setItemCenterX(410);
                    if (ImGui::Button("Продолжить (A)", ImVec2(200, 30))) {
                        PahomEngine->bLoadingFrame = false;
                        PahomEngine->strings.log("loading frame false", "Engine");

                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Опции", ImVec2(200, 30))) {
                        PahomEngine->bSettings = true;
                        ImGui::OpenPopup("settings_engine");
                    }
                    PahomEngine->bControlsIsKeyboard = true;
                    PahomEngine->bControlsIsGamepad = true;
                    if (PahomEngine->GetGamepadKey(PahomEngine->keyMap.i64AGamepad)) {
                        PahomEngine->bLoadingFrame      = false;
                        PahomEngine->bControlsIsGamepad = true;
                    }
                    if (GetAsyncKeyState(VK_RETURN)) {
                        PahomEngine->bLoadingFrame = false;
                        PahomEngine->bStartGame    = true;
                    }
               }
            }
        }
        else {
            //
           // Audio::Sound audioDevice2{ "./assets/audio/voda.mp3", Audio::Sound::Type::Stream };
            //
            if(!PahomEngine->bStartGame)
            {
                
                static float imageSizeX = 200;
                static float imageSizeY = 200;
                float maxSize = 256.0f;
                static float scale = 0;
                static int64_t i64Timer = 0;
                static bool revesed = false;
                static bool ba = true,bg = true;
                static int64_t i64DelayToRevesed = 0;
                i64Timer++;

                if (i64Timer >= 2) {
                    if (!revesed)
                    {
                       
                        if (imageSizeX >= 256 && imageSizeY >= 256) {
                            imageSizeX = maxSize;
                            imageSizeY = maxSize;
                            if (ba) {
                                PahomEngine->audio.play2(4);
                                ba = false;
                            }
                            i64DelayToRevesed++;
                            if (i64DelayToRevesed == 100) {
                                revesed = true;
                                i64DelayToRevesed = 0;
                            }
                        }
                        else {
                            scale += 2.0f;
                            imageSizeX = scale;
                            imageSizeY = scale;
                        }
                    }
                    else {
                        scale -= 2.0f;
                        if (imageSizeX == 0 && imageSizeY == 0) {
                            imageSizeX = 0;
                            imageSizeY = 0;
                            PahomEngine->bStartGame = true;
                        }
                        else {
                            imageSizeX = scale;
                            imageSizeY = scale;
                        }
                    }

                }
              //  ImGui::Text(" ImageSize:% .1f: % .1f\nTimer: %i revesed:%s", imageSizeX, imageSizeY, i64Timer, (revesed) ? ":true" : ":false");
                PahomEngine->setItemCenter(ImVec2(imageSizeX, imageSizeY));
                ImGui::Image(ptrint64_t(PahomEngine->ImageData.TextureArray[0]), ImVec2(imageSizeX, imageSizeY));
            }
       
            else {
               
                    if (ImGui::BeginPopup("set_params")) {
                        static int64_t fOldSize[2] = { 800,600 };
                       
                        ImGui::TextColored(PahomEngine->RGBA(133, 133, 133, 255), "Настройки (Параметры)");
                        ImGui::Separator();
                        ImGui::Checkbox("60 фпс", &PahomEngine->CVsync);
                        /*ImGui::TextColored(PahomEngine->RGBA(133, 133, 133, 255), "Размер Игрока:   %.1f", phScale); ImGui::SameLine();
                        ImGui::SetNextItemWidth(150);
                        ImGui::SliderFloat("ph", &phScale,0,100,"%.1f");*/
                        ImGui::TextColored(PahomEngine->RGBA(133, 133, 133, 255), "Скорость движения:   %i", PahomEngine->fStep); ImGui::SameLine();
                        ImGui::SetNextItemWidth(150);
                        PahomEngine->InputI64("sp", &PahomEngine->fStep);
                        ImGui::TextColored(PahomEngine->RGBA(133, 133, 133, 255), "Макс.позиция       %.2f", PahomEngine->fMaxPahomPosX); ImGui::SameLine();
                        ImGui::SetNextItemWidth(150);
                        PahomEngine->InputFloat("mxp", &PahomEngine->fMaxPahomPosX);
                        ImGui::TextColored(PahomEngine->RGBA(133, 133, 133, 255), "Мин.позиция:       %.2f", PahomEngine->fMinPahomPosX); ImGui::SameLine();
                        ImGui::SetNextItemWidth(150);
                        PahomEngine->InputFloat("mnp", &PahomEngine->fMinPahomPosX);
                        ImGui::TextColored(PahomEngine->RGBA(133, 133, 133, 255), "Разрешение окна %i : %i", PahomEngine->i64WindowSize[0], PahomEngine->i64WindowSize[1]);
                        ImGui::TextColored(PahomEngine->RGBA(133, 133, 133, 255), "Позиция Игрока Y:       %.2f", PahomEngine->fPahomPosY); ImGui::SameLine();
                        ImGui::SetNextItemWidth(150);
                        PahomEngine->InputFloat("ppy", &PahomEngine->fPahomPosY);
                        ImGui::TextColored(PahomEngine->RGBA(133, 133, 133, 255), "Разрешение окна %i : %i", PahomEngine->i64WindowSize[0], PahomEngine->i64WindowSize[1]);
                        ImGui::SetNextItemWidth(150);
                        PahomEngine->InputI64("wxsz", &fOldSize[0]);
                        ImGui::SameLine();
                        ImGui::SetNextItemWidth(150);
                        PahomEngine->InputI64("wysz", &fOldSize[1]);
                        
                        ImGui::EndPopup();
                    }
                    static std::string keyPresedStr;
                    if (PahomEngine->bControlsIsGamepad) {
                        if (PahomEngine->ptrGamepad1->IsConnected()) {
                            if (PahomEngine->GetGamepadKey(PahomEngine->keyMap.i64FORWARDGamepad)) {//PahomEngine->keyMap.u8FORWARDGamepad
                                PahomEngine->keyMap.kbDelay++;
                                if (PahomEngine->keyMap.kbDelay == PahomEngine->keyMap.vMaxDelay) {
                                    PahomEngine->fPahomPosX += PahomEngine->fStep;
                                    PahomEngine->bIsRevesed = true;
                                    if (PahomEngine->fPahomPosX >= PahomEngine->fMaxPahomPosX) {
                                        PahomEngine->fPahomPosX = PahomEngine->fMaxPahomPosX;
                                    }
                                    PahomEngine->keyMap.kbDelay = 0;
                                }
                                keyPresedStr = "FORWARD" + (PahomEngine->keyMap.i64FORWARDGamepad);
                            }
                            if (PahomEngine->GetGamepadKey(PahomEngine->keyMap.i64BACKGamepad)) {//PahomEngine->keyMap.u8BACKGamepad
                                PahomEngine->keyMap.kbDelay++;
                                if (PahomEngine->keyMap.kbDelay == PahomEngine->keyMap.vMaxDelay) {
                                    PahomEngine->bIsRevesed = false;
                                    PahomEngine->fPahomPosX -= PahomEngine->fStep;
                                    if (PahomEngine->fPahomPosX <= PahomEngine->fMinPahomPosX) {
                                        PahomEngine->fPahomPosX = PahomEngine->fMinPahomPosX;
                                    }
                                    PahomEngine->keyMap.kbDelay = 0;
                                }
                                keyPresedStr = (const char*)("BACK" + PahomEngine->keyMap.u8BACK);
                            }
                            if (PahomEngine->GetGamepadKey(PahomEngine->keyMap.i64UPGamepad)) {
                                PahomEngine->fPahomPosY = 300.0f;
                            }
                        }
                    }
                    
                   
                    if (PahomEngine->bControlsIsKeyboard) {
                        if (GetAsyncKeyState(PahomEngine->keyMap.u8BACK)) {//PahomEngine->keyMap.u8FORWARD
                            PahomEngine->keyMap.kbDelay++;
                            if (PahomEngine->keyMap.kbDelay == PahomEngine->keyMap.vMaxDelay) {
                                PahomEngine->fPahomPosX += PahomEngine->fStep;
                                PahomEngine->bIsRevesed = true;
                                if (PahomEngine->fPahomPosX >= PahomEngine->fMaxPahomPosX) {
                                    PahomEngine->fPahomPosX = PahomEngine->fMaxPahomPosX;
                                }
                                PahomEngine->keyMap.kbDelay = 0;
                            }
                            keyPresedStr = "FORWARD" + (PahomEngine->keyMap.u8FORWARD);
                        }
                        if (GetAsyncKeyState(PahomEngine->keyMap.u8FORWARD)) {//PahomEngine->keyMap.u8BACK
                            PahomEngine->keyMap.kbDelay++;
                            if (PahomEngine->keyMap.kbDelay == PahomEngine->keyMap.vMaxDelay) {
                                PahomEngine->bIsRevesed = false;
                                PahomEngine->fPahomPosX -= PahomEngine->fStep;
                                if (PahomEngine->fPahomPosX <= PahomEngine->fMinPahomPosX) {
                                    PahomEngine->fPahomPosX = PahomEngine->fMinPahomPosX;
                                }
                                PahomEngine->keyMap.kbDelay = 0;
                            }
                            keyPresedStr = (const char*)("BACK" + PahomEngine->keyMap.u8BACK);
                        }
                    }
                    
                    ImGui::SetCursorPos(ImVec2(0,0));
                    ImGui::Image((int64_t)(void*)PahomEngine->ImageData.TextureArray[1], ImVec2(PahomEngine->i64WindowSize[0], PahomEngine->i64WindowSize[1]));
                    //
                 
                    if (PahomEngine->bStartGameFlag) {
                        ImGui::SetCursorPos(ImVec2{
                            PahomEngine->fBreadPosX,
                            PahomEngine->fBreadPosY
                            });
                        if (!PahomEngine->bGameOver) {
                            PahomEngine->fBreadPosY += PahomEngine->fStepMove;
                            if (PahomEngine->fBreadPosY >= PahomEngine->fMaxPahomPosY) {
                                PahomEngine->fBreadPosX = 0;
                                PahomEngine->fBreadPosY = 0;
                                PahomEngine->reloadBreadPos();
                                PahomEngine->bGameOver = true;

                                PahomEngine->audio.play2(3);
                               

                            }
                            if (PahomEngine->CheckColiision()) {
                                //waveOutSetVolume(NULL, (10 / 0xFFFF));
                                PahomEngine->audio.play(0);
                                PahomEngine->reloadBreadPos();
                                PahomEngine->fScoreCount += 1;
                                PahomEngine->fBreadPosY = 0;
                            }
                        }
                        if (PahomEngine->bGameOver) {
                            ImGui::PushFont(font50);
                            ImGui::SetCursorPos(
                                ImVec2(
                                    (PahomEngine->i64WindowSize[0] - ImGui::CalcTextSize("Пидорас обосрался!!").x) / 2,
                                    (PahomEngine->i64WindowSize[1] - ImGui::CalcTextSize("Пидорас обосрался!!").y - 200) / 2
                                )
                            );
                            PahomEngine->bGameOver = true;
                            ImGui::Text("Пидорас обосрался!!");
                            ImGui::PopFont();
                            
                            ImGui::SetCursorPosX(
                                (PahomEngine->i64WindowSize[0] - 150) / 2
                            );

                            if (ImGui::Button("Заново (A)", ImVec2(150, 30))) {
                                PahomEngine->bGameOver = false;
                                PahomEngine->fScoreCount = 0;
                                PahomEngine->audio.play2(1);
                                ImGui::CloseCurrentPopup();
                            }
                            if (PahomEngine->GetGamepadKey(PahomEngine->keyMap.i64AGamepad)) {

                                PahomEngine->bGameOver = false;
                                PahomEngine->fScoreCount = 0;
                                PahomEngine->audio.play2(1);
                                ImGui::CloseCurrentPopup();
                            }
                        }
                    }
                   
                    if (!PahomEngine->bStartGameFlag) {
                        ImGui::SetCursorPos(
                            ImVec2(
                                (PahomEngine->i64WindowSize[0] - 128) / 2,
                                (PahomEngine->i64WindowSize[1] - 600) / 2
                            )
                        );
                        ImGui::Image((int64_t)(void*)PahomEngine->ImageData.TextureArray[2], ImVec2(128, 128));
                        ImGui::PushFont(font50);
                        ImGui::SetCursorPos(
                            ImVec2(
                                (PahomEngine->i64WindowSize[0] - ImGui::CalcTextSize("SWEET BREADS v 0.15\n(alpha_bulid)").x) / 2,
                                (PahomEngine->i64WindowSize[1] - ImGui::CalcTextSize("SWEET BREADS v 0.15\n(alpha_bulid)").y-200) / 2
                            )
                        );
                        static float r_color = 0.0f;
                        static float g_color = 0.0f;
                        static float b_color = 0.0f;
                        static float cl_timer = 0.0f;
                        static float cl_stop = 0.0f;
                        cl_timer += 0.1f;
                        if (cl_timer >= 0.3f) {
                            cl_stop++;
                            if (cl_stop >= 30) {
                                r_color = float(255);
                                g_color = float(255);
                                b_color = float(255);
                                cl_stop = 30;
                            }
                            else {
                                r_color = PahomEngine->randfloat(255);
                                g_color = PahomEngine->randfloat(255);
                                b_color = PahomEngine->randfloat(255);
                            }
                            cl_timer = 0;
                        }
                        ImGui::TextColored(PahomEngine->RGBA(r_color, g_color, b_color, 255), "SWEET BREADS v 0.15\n(alpha_bulid)");
                       
                        ImGui::PopFont();
                        PahomEngine->setTextCenter("PahomEngine_ogl3_amd64 build 0.20.301a3");
                        ImGui::Text("PahomEngine_ogl3_amd64 build 0.20.301a3");
                        PahomEngine->setTextCenter("Написано на С++20 (MSVC)");
                        ImGui::Text("Написано на С++20 (MSVC)");
                        PahomEngine->setTextCenter("BY HCPP");
                        ImGui::Text("BY HCPP");
                        ImGui::SetCursorPosX(
                          (PahomEngine->i64WindowSize[0] - 250) / 2
                        );
                        if (ImGui::Button("Играть", ImVec2(250, 40))) {
                            PahomEngine->bGameOver = false;
                            PahomEngine->bStartGameFlag = true;
                            PahomEngine->audio.play2(1);
                        }
                        if (PahomEngine->GetGamepadKey(PahomEngine->keyMap.i64AGamepad)) {
                            PahomEngine->bGameOver = false;
                            PahomEngine->bStartGameFlag = true;
                            PahomEngine->audio.play2(1);
                        }
                        ImGui::SetCursorPosX(
                            (PahomEngine->i64WindowSize[0] - 250) / 2
                        );

                        if (ImGui::Button("Донат", ImVec2(250, 40))) {
                            std::system("start https://donationalerts.com/r/hcpp");
                        }
                        ImGui::SetCursorPosX(
                            (PahomEngine->i64WindowSize[0] - 250) / 2
                        );

                        if (ImGui::Button("На главную", ImVec2(250, 40))) {
                            PahomEngine->bLoadingFrame = true;
                            PahomEngine->bStartGame = false;
                        }
                       
                    }
                    else {
                        
                        PahomEngine->fMaxPahomPosX = static_cast<float>(PahomEngine->i64WindowSize[0]) - 128.0f;
                        PahomEngine->fMaxPahomPosY = static_cast<float>(PahomEngine->i64WindowSize[1]) - 128.0f;
                        ImGui::Image((int64_t)(void*)PahomEngine->ImageData.TextureArray[2], ImVec2(64, 64));
                        ImGui::SetCursorPos(ImVec2{
                            PahomEngine->fPahomPosX,
                            PahomEngine->fPahomPosY
                            });

                        PahomEngine->i64PahomSize[0] = 128 * (PahomEngine->bFullscreen ? 2 : 1);
                        PahomEngine->i64PahomSize[1] = 128 * (PahomEngine->bFullscreen ? 2 : 1);
                        PahomEngine->i64BreadSize[0] = 64  * (PahomEngine->bFullscreen ? 2 : 1);
                        PahomEngine->i64BreadSize[1] = 64  * (PahomEngine->bFullscreen ? 2 : 1);
                        ImGui::Image((int64_t)(void*)(PahomEngine->ImageData.TextureArray[!PahomEngine->bIsRevesed ? 4 : 3]), ImVec2(PahomEngine->i64PahomSize[0], PahomEngine->i64PahomSize[1]));
                        ImGui::SetCursorPosY(10); ImGui::SetCursorPosX(10);
                        if (ImGui::Button("/", ImVec2(32, 32))) {
                            ImGui::OpenPopup("set_params");
                        }
                        if (GetAsyncKeyState(PahomEngine->keyMap.u8RESET)) {
                            PahomEngine->fPahomPosX = 0;
                            PahomEngine->fPahomPosY = 0;
                            PahomEngine->fBreadPosX = 0;
                            PahomEngine->fBreadPosY = 0;
                           
                        }
                        if (GetAsyncKeyState(PahomEngine->keyMap.u8SPACE) || PahomEngine->GetGamepadKey(PahomEngine->keyMap.i64START)) {
                            PahomEngine->bStartGameFlag = false;
                        }
                        if (GetAsyncKeyState('V')) {
                            PahomEngine->fScoreCount = 500;
                        }
                        if (GetAsyncKeyState('M')) {
                            PahomEngine->fScoreCount = 1000;
                        }
                        //
                        if (PahomEngine->fScoreCount == 500) {
                            
                            ImGui::PushFont(font50);
                            PahomEngine->Text(PahomEngine->RGBA(255, 255, 255, 255), "Хули ты говном вымазался");
                            ImGui::PopFont();
                        }
                        if (PahomEngine->fScoreCount == 1000) {
                            
                            ImGui::PushFont(font50);
                            PahomEngine->Text(PahomEngine->RGBA(255,255,255,255),"Ты понимаешь что\n ты поехавший?");
                            ImGui::PopFont();
                        }
                        //
                        PahomEngine->Tbuffer();
                        ImGui::SetCursorPosY(30);
                        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(std::to_string(PahomEngine->fScoreCount).c_str()).x) / 2);
                        ImGui::PushFont(font50);
                        ImGui::TextColored(PahomEngine->RGBA(255, 255, 255, 255), "%.0f", PahomEngine->fScoreCount);
                        ImGui::PopFont();
                        ImGui::SetCursorPosY(15); ImGui::SetCursorPosX(50);
                        ImGui::TextColored(PahomEngine->RGBA(255, 255, 255, 255), "FPS: %.0f / фреймов: %i", io.Framerate,ImGui::GetFrameCount());
                        PahomEngine->bDebugText = (GetAsyncKeyState('G') && GetAsyncKeyState('A') && GetAsyncKeyState('Y')) ? true : false;
                        PahomEngine->fPahomPosY = PahomEngine->i64WindowSize[1] - PahomEngine->i64PahomSize[1]-27;
                        ImGui::SetCursorPosY(PahomEngine->i64WindowSize[1]- 60);
                        PahomEngine->setTextCenter("[A]/(L) - Влево , [D]/(R) - Вправо , [SPACE]/(START) - Пауза");
                        ImGui::Text("[A]/(L) - Влево , [D]/(R) - Вправо , [SPACE]/(START) - Пауза");
                        if (PahomEngine->bDebugText) {
                            ImGui::PushFont(font10);
                            ImGui::SetCursorPosY(500);

                            ImGui::Text(" pos:%.1f: %.1f\n controls_type_gamepad %s\n controls_type_keyboard %s\n(PahomEngine_OGL_debug_x64 build 0.05)", PahomEngine->fPahomPosX,
                                PahomEngine->fPahomPosY, (PahomEngine->bControlsIsGamepad) ? "GAMEPAD" : "NONE", (PahomEngine->bControlsIsKeyboard ? "KEYBOARD" : "NONE"));
                            ImGui::Text("bread_pos: %.1f:%.1f chk_cl: %s", PahomEngine->fBreadPosX, PahomEngine->fBreadPosY, (PahomEngine->CheckColiision() ? ":true" : ":false"));
                            ;                    ImGui::PopFont();
                       }
                    }
                
            }
        }
       
        ImGui::End();

    // Rendering
        ImGui::Render();
        glViewport(0, 0, 300, 600);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Present
        ::SwapBuffers(g_MainWindow.hDC);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceWGL(hwnd, &g_MainWindow);
    wglDeleteContext(g_hRC);
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    HDC hDc = ::GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    const int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0)
        return false;
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
        return false;
    ::ReleaseDC(hWnd, hDc);

    data->hDC = ::GetDC(hWnd);
    if (!g_hRC)
        g_hRC = wglCreateContext(data->hDC);
    return true;
}

void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    wglMakeCurrent(nullptr, nullptr);
    ::ReleaseDC(hWnd, data->hDC);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            g_Width = LOWORD(lParam);
            g_Height = HIWORD(lParam);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        exit(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}