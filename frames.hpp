#include "PahomEngine.h"
#include <shellapi.h>
#define CRT_NO_WARNINGS 1
struct GameFrames {
    void ShowEventTray(std::wstring text,HWND hwndGame = nullptr) {
        static int32_t i32TrayNoficationId = 0;
        NOTIFYICONDATA nid = {}; nid.cbSize = sizeof(NOTIFYICONDATA); nid.hWnd = hwndGame;
        static int32_t i32Timer = 0;
        i32Timer++;
        nid.uID = 1; nid.uFlags = NIF_MESSAGE; wcsncpy_s(nid.szInfo, text.c_str(), _TRUNCATE); wcsncpy_s(nid.szInfoTitle, L"Пахом", _TRUNCATE);
        nid.dwInfoFlags = NIF_INFO; Shell_NotifyIcon(NIM_ADD, &nid);
        if (i32Timer >= 5) {
            Shell_NotifyIcon(NIM_DELETE, &nid);
            i32Timer = 0;
        }
    }
    void DrawParticles(
        const ImVec2& emitterPos,      // откуда летят
        float dt,                      // delta time
        int maxParticles = 100,        // макс. частиц
        float spawnRate = 0.02f,       // как часто спавн
        const ImVec4& color = { 1, 0.8f, 0.3f, 1 }, // хлебный цвет
        float sizeMin = 2.0f,          // мин. размер
        float sizeMax = 6.0f,          // макс. размер
        float speed = 300.0f,          // скорость
        float gravity = 980.0f,        // гравитация
        float lifeTime = 1.5f          // сколько живёт
    ) {
        static std::vector<ImVec2> positions;
        static std::vector<ImVec2> velocities;
        static std::vector<float> lives;
        static float spawnTimer = 0.0f;

        auto* draw = ImGui::GetWindowDrawList();

        // === СПАВН ===
        spawnTimer += dt;
        while (spawnTimer >= spawnRate && positions.size() < maxParticles) {
            spawnTimer -= spawnRate;

            ImVec2 vel = ImVec2(
                (rand() % 200 - 100) * 0.01f,
                (rand() % 200 - 150) * 0.01f
            );
            vel.x = ImVec2(vel.x, vel.y).x * speed;
            vel.y = ImVec2(vel.x, vel.y).y * speed;
            positions.push_back(emitterPos);
            velocities.push_back(vel);
            lives.push_back(lifeTime);
        }

        // === ОБНОВЛЕНИЕ ===
        for (size_t i = 0; i < positions.size(); ++i) {
            if (lives[i] <= 0.0f) continue;

            // Физика
            velocities[i].y += gravity * dt;
            positions[i].x = positions[i].x + velocities[i].x * dt;
            positions[i].y = positions[i].y + velocities[i].y * dt;
            // Жизнь
            lives[i] -= dt;
            float alpha = lives[i] / lifeTime;

            // Размер
            float size = sizeMin + (sizeMax - sizeMin) * (rand() % 100) / 100.0f;

            // Цвет
            ImU32 col = IM_COL32(
                (int)(color.x * 255),
                (int)(color.y * 255),
                (int)(color.z * 255),
                (int)(color.w * alpha * 255)
            );

            // Рендер
            ImVec2 screenPos;
            screenPos.x = ImGui::GetCursorScreenPos().x + positions[i].x;
            screenPos.y = ImGui::GetCursorScreenPos().y + positions[i].y;
            draw->AddCircleFilled(screenPos, size, col);
        }

        // === УДАЛЕНИЕ МЁРТВЫХ ===
        for (int i = (int)positions.size() - 1; i >= 0; --i) {
            if (lives[i] <= 0.0f) {
                positions.erase(positions.begin() + i);
                velocities.erase(velocities.begin() + i);
                lives.erase(lives.begin() + i);
            }
        }
    }
    float fDeltaTime;
    void TestStyle(ImFont* Font) {
        if (ImGui::BeginPopup("TestStyle", ImGuiWindowFlags_AlwaysAutoResize)) {
            static ImVec2 FramePadding = { 0,0 }; static float FrameRounding = 0, BorderSize = 0;
            static ImVec4 colorButton = { 0,0,0,1 },colorText = { 0,0,0,1 },colorBorder = { 0,0,0,1 };
            ImGui::PushFont(Font);
            ImGui::SetNextItemWidth(150);ImGui::SliderFloat("FP0:", &FramePadding.x, 0, 100); ImGui::SameLine(); ImGui::SetNextItemWidth(150);ImGui::SliderFloat("FP1:", &FramePadding.y, 0, 100);
            ImGui::SetNextItemWidth(150);ImGui::SliderFloat("FR:", &FrameRounding, 0, 100);
            ImGui::SetNextItemWidth(150);ImGui::SliderFloat("BZ:", &BorderSize, 0, 100);
            ImGui::Separator();
            ImGui::Text("Цвет Кнопки");
            ImGui::SetNextItemWidth(150); ImGui::ColorEdit3("_button", reinterpret_cast<float*>(&colorButton), ImGuiColorEditFlags_DisplayRGB);
            ImGui::Text("Цвет Текста");
            ImGui::SetNextItemWidth(150); ImGui::ColorEdit3("_text", reinterpret_cast<float*>(&colorText), ImGuiColorEditFlags_DisplayRGB);
            ImGui::Text("Цвет Обводки");
            ImGui::SetNextItemWidth(150); ImGui::ColorEdit3("_border", reinterpret_cast<float*>(&colorBorder), ImGuiColorEditFlags_DisplayRGB);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, FramePadding);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, FrameRounding);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, BorderSize);
            ImGui::PushStyleColor(ImGuiCol_Button, colorButton);
            ImGui::PushStyleColor(ImGuiCol_Border, colorBorder);
            ImGui::PushStyleColor(ImGuiCol_Text, colorText);
            ImGui::Button("Button", ImVec2(150, 30));
            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar(3);
            ImGui::PopFont();
            ImGui::Separator();
            ImGui::Text("Текстуры");
            static ImVec2 uv0 = { 0,0 };
            static ImVec2 uv1 = { 1,1 };
            static ImVec4 tinCol = { 1,1,1,1 };
            static ImVec4 borderCol;
            ImGui::SliderFloat("uv0_x", &uv0.x, 0.0f, 1.0f, "%.3f");
            ImGui::SameLine();
            ImGui::SliderFloat("uv0_y", &uv0.y, 0.0f, 1.0f, "%.3f");
            ImGui::SliderFloat("uv1_x", &uv1.x, 0.0f, 1.0f, "%.3f");
            ImGui::SameLine();
            ImGui::SliderFloat("uv1_y", &uv1.y, 0.0f, 1.0f, "%.3f");
            ImGui::ColorPicker4("tinCol", reinterpret_cast<float*>(&tinCol));
            ImGui::SameLine();
            ImGui::ColorPicker4("border", reinterpret_cast<float*>(&borderCol));
            ImGui::Separator();
            ImGui::Text("Текстура Измененная uv0:%.3f:%.3f uv1:%.3f:%.3f", uv0.x, uv0.y, uv1.x, uv1.y);
            ImGui::Image(PahomEngine->ptrint64_t(PahomEngine->ImageData.TextureArray[2]), ImVec2(128, 128), uv0, uv1, tinCol, borderCol);
            ImGui::Text("Текстура оригинальная");
            ImGui::Image(PahomEngine->ptrint64_t(PahomEngine->ImageData.TextureArray[2]), ImVec2(128, 128));
            ImGui::Separator();
            ImGui::Text("Частицы");
            if (ImGui::IsItemHovered()) {
                if (ImGui::BeginTooltip()) {
                    PahomEngine->setTextCenterXY("Это Частицы");
                    ImGui::Text("Это частицы");
                    DrawParticles(ImVec2((ImGui::GetItemRectSize().x - 40) / 2, (ImGui::GetItemRectSize().y - 40) / 2),
                        fDeltaTime,
                        1040,                    // макс. частиц
                        0.04f,                  // частота
                        PahomEngine->RGBA(130, 130, 130, 255), // цвет
                        PahomEngine->randfloat(20), PahomEngine->randfloat(20),             // размер
                        1350.0f,                 // скорость
                        800.0f,                 // гравитация
                        21.8f                    // жизнь
                    );
                    ImGui::EndTooltip();
                }
            }
            ImGui::Separator();
            ImGui::Text("Рисуй");
            PahomEngine->UI->PaintIm(v2MousePos,32, PahomEngine->RGBA(255, 0, 133, 255));
            
            ImGui::EndPopup();
        }
    }
    void logo() {
        static float imageSizeX = (PahomEngine->i64WindowSize[1] == 480 ? 64 : 200);
        static float imageSizeY = (PahomEngine->i64WindowSize[1] == 480 ? 64 : 200);
        static float maxSize = (PahomEngine->i64WindowSize[1] == 480 ? 128 : 256);
        static float scale = 0;
        static int64_t i64Timer = 0;
        static bool revesed = false;
        static bool ba = true, bg = true, bLogoEngineShow = false;
        static int64_t i64DelayToRevesed = 0;
        i64Timer += 1;
        
        if (i64Timer >= 2) {
            if (!revesed)
            {

                if (imageSizeX >= (PahomEngine->i64WindowSize[1] == 480 ? 128 : 256) && imageSizeY >= (PahomEngine->i64WindowSize[1] == 480 ? 128 : 256)) {
                    imageSizeX = maxSize;
                    imageSizeY = maxSize;
                    if (ba) {
                        PahomEngine->audio.play2(4);
                        
                        ba = false;
                    }
                    i64DelayToRevesed += 1;
                    if (i64DelayToRevesed == 100) {
                        bLogoEngineShow = true;
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
            static float fAlphaChannelLogo = 0;
            if (bLogoEngineShow) {
                fAlphaChannelLogo += 100 * ImGui::GetIO().DeltaTime;
                ImVec4 ColorImage = PahomEngine->RGBA(255,255,255,fAlphaChannelLogo);
                ImVec2 PosImage = (ImGui::GetWindowSize() - ImVec2(128, 128)) / ImVec2(2, 2);
                PahomEngine->Text("alpha_channel :{}\nframe_rate: {}\ndelta_time: {}", fAlphaChannelLogo, ImGui::GetIO().Framerate, ImGui::GetIO().DeltaTime);
                ImGui::SetCursorPos(PosImage);
                ImGui::Image(PahomEngine->ptrint64_t(PahomEngine->ImageData.TextureArray[11]), { 128,128 }, { 0,0 }, { 1,1 }, ColorImage);
                if (fAlphaChannelLogo >= 255) {
                    PahomEngine->bStartGame = true;
                }
            }
        }
        //  ImGui::Text(" ImageSize:% .1f: % .1f\nTimer: %i revesed:%s", imageSizeX, imageSizeY, i64Timer, (revesed) ? ":true" : ":false");
        if(!revesed)
        {
            PahomEngine->setItemCenter(ImVec2(imageSizeX, imageSizeY));
            //   std::cout << std::format(" x:{} y:{} reversed: {} scale: {} window_size: {}x{} \n", imageSizeX, imageSizeY, revesed, scale, PahomEngine->i64WindowSize[0], PahomEngine->i64WindowSize[1]);
            ImGui::Image(PahomEngine->ptrint64_t(PahomEngine->ImageData.TextureArray[0]), ImVec2(imageSizeX, imageSizeY));
        }
        ///
        PahomEngine->pDiff->i64id = 1;
        PahomEngine->pDiff->setDiff(1);
        PahomEngine->fStepMove = PahomEngine->pDiff->diffArray[PahomEngine->pDiff->i64id].i64buffer;
        PahomEngine->fStep = PahomEngine->pDiff->diffArray[PahomEngine->pDiff->i64id].i64buffer1;
        //
    }
   
    void randomEvents() {
        if (!PahomEngine->bGameOver) {
            PahomEngine->fBreadPosY += PahomEngine->fStepMove * PahomEngine->fDeltaTime;
            if (PahomEngine->fBreadPosY >= PahomEngine->fMaxPahomPosY) {
                if (!PahomEngine->bKefir) {
                    PahomEngine->fBreadPosX = 0;
                    PahomEngine->fBreadPosY = 0;
                    PahomEngine->reloadBreadPos();
                    PahomEngine->bGameOver = true;

                    PahomEngine->audio.play2(3);
                }
                else {
                    PahomEngine->fBreadPosX = 0;
                    PahomEngine->fBreadPosY = 0;
                    PahomEngine->reloadBreadPos();
                }


            }
            if (PahomEngine->getPressedKey('K', true)) {
                PahomEngine->bKefir = true;
            }
            /// vibration delay
            static bool isVibrated = false;
            static int32_t i32VibrateDelay = 0;
            if (isVibrated) {
                i32VibrateDelay++;
                if (i32VibrateDelay > 10) {
                    PahomEngine->ptrGamepad1->Vibrate(0, 0);
                    isVibrated = false;
                    i32VibrateDelay = 0;
                }
            }

            // check coliision
            if (PahomEngine->CheckColiision()) {
                //waveOutSetVolume(NULL, (10 / 0xFFFF));
                PahomEngine->ptrGamepad1->Vibrate(10000, 10000);
                
                isVibrated = true;
                PahomEngine->audio.play(0);
                PahomEngine->reloadBreadPos();

                PahomEngine->i64RandBoost = PahomEngine->rand64(100);
                if (!PahomEngine->pDiff->bRandScoreDiff)
                {
                    if (!PahomEngine->bKefir)
                    {
                        PahomEngine->fScoreCount += PahomEngine->bBoost777 ? 20 : 1;
                    }
                    else {
                        PahomEngine->bGameOver = true;
                    }
                }
                else {
                    if (!PahomEngine->bKefir)
                    {
                        PahomEngine->fScoreCount += PahomEngine->bBoost777 ? 20 : PahomEngine->rand64(100);
                    }
                    else {
                        PahomEngine->bGameOver = true;
                    }
                }
                PahomEngine->fBreadPosY = PahomEngine->randfloat(30.0f);
                if (PahomEngine->i64RandBoost < 6) {
                    if (!PahomEngine->bKefir)
                    {
                        PahomEngine->bBoost777 = true;
                    }
                    PahomEngine->audio.play3(5);
                }
                if (PahomEngine->i64RandBoost < 4) {
                    PahomEngine->reloadBreadPos();
                    PahomEngine->bKefir = true;
                }
               
                if (PahomEngine->i64RandBoost < 25 && PahomEngine->fScoreCount > PahomEngine->rand64(100)) {
                    PahomEngine->Event.bScreamEvent = true;
                }
            }
        }
    }
    void Controls() {
        static std::string keyPresedStr;
        if (PahomEngine->bControlsIsGamepad) {
            if (PahomEngine->ptrGamepad1->IsConnected()) {
                if (PahomEngine->GetGamepadKey(PahomEngine->keyMap.i64FORWARDGamepad,0)) {//PahomEngine->keyMap.u8FORWARDGamepad
                    PahomEngine->keyMap.kbDelay++;
                    if (PahomEngine->keyMap.kbDelay == PahomEngine->keyMap.vMaxDelay) {
                        PahomEngine->GamepadUI->GButtons->bGButtonR = true;
                        PahomEngine->fPahomPosX += PahomEngine->fStep * PahomEngine->fDeltaTime;
                        PahomEngine->bIsRevesed = true;
                        if (PahomEngine->fPahomPosX >= PahomEngine->fMaxPahomPosX) {
                            PahomEngine->fPahomPosX = PahomEngine->fMaxPahomPosX;
                        }
                        PahomEngine->keyMap.kbDelay = 0;
                    }
                    //keyPresedStr = "FORWARD" + (PahomEngine->keyMap.i64FORWARDGamepad);
                }
                else {
                    PahomEngine->GamepadUI->GButtons->bGButtonR = false;
                }
                if (PahomEngine->GetGamepadKey(PahomEngine->keyMap.i64BACKGamepad, 0)) {//PahomEngine->keyMap.u8BACKGamepad
                    PahomEngine->keyMap.kbDelay++;
                    if (PahomEngine->keyMap.kbDelay == PahomEngine->keyMap.vMaxDelay) {
                        PahomEngine->GamepadUI->GButtons->bGButtonL = true;
                        PahomEngine->bIsRevesed = false;
                        PahomEngine->fPahomPosX -= PahomEngine->fStep * PahomEngine->fDeltaTime;
                        if (PahomEngine->fPahomPosX <= PahomEngine->fMinPahomPosX) {
                            PahomEngine->fPahomPosX = PahomEngine->fMinPahomPosX;
                        }
                        PahomEngine->keyMap.kbDelay = 0;
                    }
                    //keyPresedStr = (const char*)("BACK" + PahomEngine->keyMap.u8BACK);
                }
                else {
                    PahomEngine->GamepadUI->GButtons->bGButtonL = false;
                }
                if (PahomEngine->GetGamepadKey(PahomEngine->keyMap.i64UPGamepad, 0)) {
                    PahomEngine->fPahomPosY = 300.0f;
                }
            }
        }
        if (PahomEngine->GetGamepadKey(PahomEngine->keyMap.i64START, 0)) {
            PahomEngine->GamepadUI->GButtons->bGButtonSTART = true;
        }
        else {
            PahomEngine->GamepadUI->GButtons->bGButtonSTART = false;
        }

        if (PahomEngine->bControlsIsKeyboard) {
            if (GetAsyncKeyState(PahomEngine->keyMap.u8BACK) || GetAsyncKeyState(VK_RIGHT)) {//PahomEngine->keyMap.u8FORWARD
                PahomEngine->keyMap.kbDelay++;
                if (PahomEngine->keyMap.kbDelay == PahomEngine->keyMap.vMaxDelay) {
                    PahomEngine->fPahomPosX += PahomEngine->fStep * PahomEngine->fDeltaTime;
                    PahomEngine->bIsRevesed = true;

                    if (PahomEngine->fPahomPosX >= PahomEngine->fMaxPahomPosX) {
                        PahomEngine->fPahomPosX = PahomEngine->fMaxPahomPosX;
                    }
                    PahomEngine->keyMap.kbDelay = 0;
                }
                //keyPresedStr = "FORWARD" + (PahomEngine->keyMap.u8FORWARD);
            }
            if (GetAsyncKeyState(PahomEngine->keyMap.u8FORWARD) || GetAsyncKeyState(VK_LEFT)) {//PahomEngine->keyMap.u8BACK
                PahomEngine->keyMap.kbDelay++;
                if (PahomEngine->keyMap.kbDelay == PahomEngine->keyMap.vMaxDelay) {
                    PahomEngine->bIsRevesed = false;
                    PahomEngine->fPahomPosX -= PahomEngine->fStep * PahomEngine->fDeltaTime;
                    if (PahomEngine->fPahomPosX <= PahomEngine->fMinPahomPosX) {
                        PahomEngine->fPahomPosX = PahomEngine->fMinPahomPosX;
                    }
                    PahomEngine->keyMap.kbDelay = 0;
                }
                // keyPresedStr = (const char*)("BACK" + PahomEngine->keyMap.u8BACK);
            }
            static bool bJump = false;
            static int32_t i32JumpKeyPresedCount = 0;
            if (bJump) {
                
                i32JumpKeyPresedCount++;
                if (i32JumpKeyPresedCount < 2) {
                    PahomEngine->fPahomPosY -= (100 * PahomEngine->fDeltaTime);
                    if(PahomEngine->bIsRevesed){
                        PahomEngine->fPahomPosX += (50 * PahomEngine->fDeltaTime);
                        if (PahomEngine->fPahomPosX >= PahomEngine->fMaxPahomPosX) {
                            PahomEngine->fPahomPosX = PahomEngine->fMaxPahomPosX;
                        }
                    }
                    if (!PahomEngine->bIsRevesed) {
                        PahomEngine->fPahomPosX -= (50 * PahomEngine->fDeltaTime);
                        if (PahomEngine->fPahomPosX <= PahomEngine->fMinPahomPosX) {
                            PahomEngine->fPahomPosX = PahomEngine->fMinPahomPosX;
                        }
                    }
                    i32JumpKeyPresedCount = 2;
                }
                if (i32JumpKeyPresedCount > 2) {
                    bJump = false;
                    i32JumpKeyPresedCount = 2;
                }
            }
            else {
                PahomEngine->fPahomPosY += PahomEngine->fStep * PahomEngine->fDeltaTime;
                if (PahomEngine->fPahomPosY >= (PahomEngine->i64WindowSize[1] - PahomEngine->i64PahomSize[1] - 27)) {
                    PahomEngine->fPahomPosY = (PahomEngine->i64WindowSize[1] - PahomEngine->i64PahomSize[1] - 27);
                    i32JumpKeyPresedCount = 0;
                }
            }
            if (GetAsyncKeyState(PahomEngine->keyMap.u8JUMP)) {
                PahomEngine->keyMap.kbDelay++;
                if (PahomEngine->keyMap.kbDelay == 1) {
                    bJump = true;
                    PahomEngine->keyMap.kbDelay = 0;
                }
            }
        }
    }
    void ConsoleLog(ImFont* Font) {
        if (ImGui::BeginPopup("#console", ImGuiWindowFlags_AlwaysAutoResize)) {
            if (ImGui::BeginChild("console_f", ImVec2(300, 500), ImGuiChildFlags_FrameStyle)) {
                if (Font) {
                    ImGui::PushFont(Font);
                    PahomEngine->Event.ConsoleBuffer();
                    ImGui::PopFont();
                }
                else {
                    PahomEngine->Event.ConsoleBuffer();
                }
                ImGui::EndChild();
                ImGui::Separator();
                if (ImGui::Button("Close Console", ImVec2(150, 20))) {
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndPopup();
        }
    }
    //
    bool bBreadRain = false;
    //
    void ColoredLogo() {
        static float r_color = 0.0f;
        static float g_color = 0.0f;
        static float b_color = 0.0f;
        static float cl_timer = 0.0f;
        static float cl_stop = 0.0f;
        //


        //
        ImVec4 ColorGreen = ImVec4(100, 171, 101, 255);
        cl_timer += 0.1f;
        if (cl_timer >= 0.3f) {
            cl_stop++;
            if (cl_stop >= 30) {
                r_color = ColorGreen.x;
                g_color = ColorGreen.y;
                b_color = ColorGreen.z;
                cl_stop = 30;
            }
            else {
                r_color = PahomEngine->math->random<float>(255);
                g_color = PahomEngine->math->random<float>(255);
                b_color = PahomEngine->math->random<float>(255);
            }
            cl_timer = 0;
        }
        ImGui::TextColored(PahomEngine->RGBA(r_color, g_color, b_color, 255), "SWEET BREADS)");

    }
    void EventBreadRain() {
        if (bBreadRain) {
            for (int64_t i64BreadCountGen = 0; i64BreadCountGen <= (10); i64BreadCountGen++) {
                
                ImGui::SetCursorPos(ImVec2(PahomEngine->math->random<float>(PahomEngine->fMaxPahomPosX) + PahomEngine->fMaxPahomPosX, PahomEngine->fBreadPosY));
                ImGui::Image(PahomEngine->ptrint64_t(PahomEngine->ImageData.TextureArray[BREAD_IMAGE]), ImVec2(PahomEngine->i64BreadSize[0], PahomEngine->i64BreadSize[1]));
            }
            bBreadRain = false;
        }
    }
    void ScremerEvent() {
        static int64_t i64timerScream = 0;
        static bool bScreamAudio = true;
        if (bScreamAudio)
        {
            PahomEngine->audio.play(7);
            bScreamAudio = false;
        }
        if (PahomEngine->audio.audioDevice != nullptr && PahomEngine->audio.isDeviceActive) {
            PahomEngine->audio.audioDevice.setVolume(0.8f);
        }
        i64timerScream++;
        PahomEngine->i64WindowSize[0] = PahomEngine->HwndWSizeA(GetActiveWindow()).x;
        PahomEngine->i64WindowSize[1] = PahomEngine->HwndWSizeA(GetActiveWindow()).y;
        ImGui::SetWindowFocus();
        SetWindowLong(GetActiveWindow(), GWL_STYLE, GetWindowLong(GetActiveWindow(), GWL_STYLE) & ~WS_CAPTION);
        ShowWindow(GetActiveWindow(), SW_SHOWMAXIMIZED);

        
        PahomEngine->setItemCenter(ImVec2(PahomEngine->i64WindowSize[0], PahomEngine->i64WindowSize[1]));
        ImGui::Image(PahomEngine->ptrint64_t(PahomEngine->ImageData.TextureArray[10]), ImVec2(PahomEngine->i64WindowSize[0], PahomEngine->i64WindowSize[1]));
        PahomEngine->audio.play2(5);
        if (i64timerScream > 100) {
            PahomEngine->Event.bScreamEventBackground = true;
            if (ImGui::BeginChild("Message", ImVec2(300, 100), ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_NoMove)) {
                PahomEngine->Text("Пахом проиграл!!\nТы умер)");
                if (ImGui::Button("Выход")) {
                    exit(0);
                }
                ImGui::EndChild();
            }

        }
    }
    void MuteButton() {
        static int iMuted = 0;
        static bool isButtonHovered = false;
        ImGui::SetCursorPosY(10); ImGui::SetCursorPosX(PahomEngine->i64WindowSize[0] - 120);
        ImGui::TextColored((iMuted || isButtonHovered) ? PahomEngine->RGBA(100, 49, 12, 255) : PahomEngine->RGBA(255, 255, 255, 255), (iMuted ? "Не орёт" : "  Орёт "));
        isButtonHovered = ImGui::IsItemHovered();
        if (ImGui::IsItemClicked()) {
            iMuted++;
            if (iMuted > 1) {
                iMuted = 0;
            }
            if (iMuted) {
                PahomEngine->audio.Mute();
            }
            else {
                PahomEngine->audio.masterVolume = PahomEngine->audio.masterVolumeLast;
                if (PahomEngine->audio.audioDevice) {
                    PahomEngine->audio.audioDevice.setVolume(PahomEngine->audio.masterVolume);
                    PahomEngine->audio.audioDevice.play();
                }
                if (PahomEngine->audio.audioDevice2) {
                    PahomEngine->audio.audioDevice2.setVolume(PahomEngine->audio.masterVolume);
                    if(PahomEngine->audio.sLastAudioFile[1] != PahomEngine->audio.audiolist[3])
                    {
                        PahomEngine->audio.audioDevice2.play();
                    }
                }
                if (PahomEngine->audio.audioDevice3) {
                    PahomEngine->audio.audioDevice3.setVolume(PahomEngine->audio.masterVolume);
                }
            }
            PahomEngine->audio.isAudioMuted = iMuted;
        }
    }
    ImVec2 v2MousePos;
    HWND hCurrentHwnd;
    void Editor() {
        PahomEngine->cast->bUsedStaticCast = true;
        ImGui::Begin("PERFOMANCE");
        static float fps_values[120] = {};
        static int offset = 0;

        float fps = ImGui::GetIO().Framerate;  // ← точнее, чем 1/DeltaTime

        // Записываем
        fps_values[offset] = fps;
        offset = (offset + 1) % 120;

        // Рисуем
        char overlay[32];
        static std::string sLogData;
        static int32_t iLogType = 0;
        sprintf(overlay, "%.0f FPS", fps);
        static int64_t i64CPUUsage = 0;
        ImGui::PlotLines("##fps_plot", fps_values, 120, offset, overlay,
            0.0f, 1000.0f, ImVec2(0, 80));
        ImGui::Checkbox("unlock fps", &PahomEngine->CVsync);
        static bool bCPUUsage = false;
        static int32_t i32CPUUsageSwitch = 0;
        static ULARGE_INTEGER ul_idle_old = { 0 };
        static ULARGE_INTEGER ul_kernel_old = { 0 };
        static ULARGE_INTEGER ul_user_old = { 0 };
        static float fcpu = 0.0f;
        if (ImGui::RadioButton("CPUUsage", bCPUUsage)) {
            i32CPUUsageSwitch = !i32CPUUsageSwitch;
            bCPUUsage = i32CPUUsageSwitch != 0;
        }
       
        PahomEngine->Text("cpu_usage: {} %", fcpu);
        ImGui::SpinnerBar("CPU", static_cast<float>(fcpu / 100), 20, 4,ImGui::GetColorU32(PahomEngine->RGBA(255,0,150,255)));
        if (bCPUUsage) {
           
            ULARGE_INTEGER ul_idle_new, ul_kernel_new, ul_user_new;

            FILETIME ft_idle, ft_kernel, ft_user;
            if (GetSystemTimes(&ft_idle, &ft_kernel, &ft_user)) {
                ul_idle_new.LowPart = ft_idle.dwLowDateTime;
                ul_idle_new.HighPart = ft_idle.dwHighDateTime;
                ul_kernel_new.LowPart = ft_kernel.dwLowDateTime;
                ul_kernel_new.HighPart = ft_kernel.dwHighDateTime;
                ul_user_new.LowPart = ft_user.dwLowDateTime;
                ul_user_new.HighPart = ft_user.dwHighDateTime;

                ULARGE_INTEGER ul_sys = { 0 };
                ul_sys.QuadPart = (ul_kernel_new.QuadPart - ul_kernel_old.QuadPart) +
                    (ul_user_new.QuadPart - ul_user_old.QuadPart);
                ULARGE_INTEGER ul_idle = { 0 };
                ul_idle.QuadPart = ul_idle_new.QuadPart - ul_idle_old.QuadPart;

                int64_t i64CPUUsage = 0;
                if (ul_sys.QuadPart > 0) {
                    i64CPUUsage = 100 - ((ul_idle.QuadPart * 100) / ul_sys.QuadPart);
                }

                fcpu = PahomEngine->cast->cast_all<float>(i64CPUUsage);

                ul_idle_old = ul_idle_new;
                ul_kernel_old = ul_kernel_new;
                ul_user_old = ul_user_new;
            }
        }
        else {
            fcpu = 0;
        }
            ImGui::InputText("LogTest", &sLogData);
        if(ImGui::Button("WARN")) {
            iLogType = 0;
        }
        ImGui::SameLine();
        if (ImGui::Button("INFO")) {
            iLogType = 1;
        }
        ImGui::SameLine();
        if (ImGui::Button("ERR")) {
            iLogType = 2;
        }
        ImGui::SameLine();
        if (ImGui::Button("DEBUG")) {
            iLogType = 3;
        }
        if (ImGui::Button("Send")) {
            PahomEngine->log(sLogData, iLogType);
        }
        static bool bDrawBufferTest = false;
        if (ImGui::Button("draw_test")) {
            bDrawBufferTest = true;

        }
        static std::string mask_rgb = "rgb";
        static std::string colorNameRand = mask_rgb;
        static bool bColorsIsLoaded = false, bRenderSquare = false;
        if (bDrawBufferTest) {
            static  colorU32 colors;
            static uint32_t currentColorU32;
            ImGui::Begin("OGL Draw", NULL,ImGuiWindowFlags_NoMove);
            
            PahomEngine->Text("alloc_texture_size:{} | x:{} y:{} color:{}", PahomEngine->ogl->pixel_buffer.size(), PahomEngine->ogl->width_texture, PahomEngine->ogl->height_texture, colorU32(255, 0, 160, 255).toStringView());
            
            if (ImGui::Button("Fill square")) {
                PahomEngine->ogl->fillSqware(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y, true);
            }
            ImGui::SameLine();
            if (ImGui::Button("render 128x128")) {
                bRenderSquare = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("New Image")) {
                PahomEngine->ogl->SetSize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
                PahomEngine->ogl->InitTexture();
            }
  
                if (ImGui::ColorButton("b0", PahomEngine->RGBA(255, 0, 0, 255), 0, {32,32})) {
                    currentColorU32 = colorU32(255,0,0,255).get();
                    colors = colorU32(255, 0, 0, 255);
;                }
                ImGui::SameLine();
                if (ImGui::ColorButton("b1", PahomEngine->RGBA(255, 0, 160, 255), 0, { 32,32 })) {
                    currentColorU32 = colorU32(255, 0, 160, 255).get();
                    colors = colorU32(255, 0, 160, 255);
                }
                ImGui::SameLine();
                if (ImGui::ColorButton("b2", PahomEngine->RGBA(0, 0, 255, 255), 0, { 32,32 })) {
                    currentColorU32 = colorU32(0, 0, 255, 255).get();
                    colors = colorU32(0, 0, 255, 255);
                }
                ImGui::SameLine();
                if (ImGui::ColorButton("b3", PahomEngine->RGBA(160, 0, 255, 255), 0, { 32,32 })) {
                    currentColorU32 = colorU32(255, 0, 0, 255).get();
                    colors = colorU32(255, 0, 0, 255);
                }
                ImGui::SameLine();
                if (ImGui::ColorButton("b4", PahomEngine->RGBA(0, 255, 0, 255), 0, { 32,32 })) {
                    currentColorU32 = colorU32(0, 255, 0, 255).get();
                    colors = colorU32(0, 255, 0, 255);
                }
                ImGui::SameLine();
                if (ImGui::ColorButton("b5", PahomEngine->RGBA(0, 255, 160, 255), 0, { 32,32 })) {
                    currentColorU32 = colorU32(0, 255, 160, 255).get();
                    colors = colorU32(0, 255, 160, 255);
                }
                ImGui::SliderInt64("brush_size", &PahomEngine->ogl->i64BrushSize,1,100,"%lld",0);
            ImGui::Image(PahomEngine->ptrint64_t(PahomEngine->ogl->textureID), ImGui::GetWindowSize());

            if (ImGui::IsItemHovered()) // Проверяем, наведен ли курсор на текстуру
            {
                ImVec2 canvas_pos = ImGui::GetItemRectMin(); // Позиция текстуры на экране
                ImVec2 mouse_pos = ImGui::GetIO().MousePos;  // Абсолютная позиция мыши

               int draw_x = static_cast<int>(mouse_pos.x - canvas_pos.x);
               int draw_y = static_cast<int>(mouse_pos.y - canvas_pos.y);
                if (bRenderSquare) {
                    for (int x_draw = 0; x_draw < 128; x_draw++) {
                        for (int y_draw = 0; y_draw < 128; y_draw++) {
                            PahomEngine->ogl->SetPixel(x_draw, y_draw, currentColorU32);
                        }
                    }
                    bRenderSquare = false;
                }
              
             
                if (draw_x >= 0 && draw_x < ImGui::GetWindowSize().x && draw_y >= 0 && draw_y < ImGui::GetWindowSize().y) {
                    int brush_size = PahomEngine->ogl->i64BrushSize;
                    int half_brush = brush_size / 2;

                    if (ImGui::IsKeyPressed(ImGuiKey_MouseLeft) || ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                        for (int x = draw_x - half_brush; x < draw_x + half_brush; x++) {
                            for (int y = draw_y - half_brush; y < draw_y + half_brush; y++) {
                                if (x >= 0 && x < PahomEngine->ogl->width_texture &&
                                    y >= 0 && y < PahomEngine->ogl->height_texture)
                                {
                                    PahomEngine->ogl->SetPixel(x, y, currentColorU32);
                                }
                            }
                        }
                    }
                }
            }
     
            
            PahomEngine->ogl->UpdateTexture();
            
            
            ImGui::End();
        }
        ImGui::End();

       
        
    }
    void KefirEvent(int image_id) {
        static float fImageKefirOpacity = 0; static bool isReversed = false;
        static ImVec4 color = { 255,255,255,fImageKefirOpacity };
        if (PahomEngine->PESettings->bFlagEnableAnimationToImageFadeInOut) {
            if (PahomEngine->bBoost777 || !PahomEngine->bBoost777 && !PahomEngine->bKefir) {
                if (!isReversed)
                {
                    fImageKefirOpacity += 200 * ImGui::GetIO().DeltaTime;
                    if (fImageKefirOpacity > 255) { isReversed = true; }
                }
                else {
                    fImageKefirOpacity -= 200 * ImGui::GetIO().DeltaTime;
                    if (fImageKefirOpacity < 0) { isReversed = false; }
                }
                color = { 255,255,255,fImageKefirOpacity };
            }
            else {
                if (!isReversed)
                {
                    fImageKefirOpacity += 200 * ImGui::GetIO().DeltaTime;
                    if (fImageKefirOpacity > 255) { isReversed = true; }
                }
                else {
                    fImageKefirOpacity -= 200 * ImGui::GetIO().DeltaTime;
                    if (fImageKefirOpacity < 0) { isReversed = false; }
                }
                color = { 0,255,130,fImageKefirOpacity };
            }
        }
        else {
            color = { 255,255,255,fImageKefirOpacity };
            fImageKefirOpacity = 255;
        }
        ImGui::Image(PahomEngine->ptrint64_t(PahomEngine->ImageData.TextureArray[image_id]), { 64,64 }, { 0,0 }, { 1, 1 }, PahomEngine->RGBA(color));
    }
    
};
auto Game = std::make_unique<GameFrames>();
//PahomEngine->cio->alloc_ptr<GameFrames, Game>(Game);

////////////////////////////////////////////////////