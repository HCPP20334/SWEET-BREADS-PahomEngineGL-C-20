#include "PahomEngine.h"
#define CRT_NO_WARNINGS 1
struct GameFrames {
    void SetDiffinity() {
        int64_t i64diffImageId = 0;
        static std::string sInfo;
        if (ImGui::BeginPopupModal("Сложность", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            PahomEngine->pDiff->setDiff(PahomEngine->pDiff->i64id);
            switch (PahomEngine->pDiff->i64id)
            {
            case 0:
                i64diffImageId = 6;
                sInfo = "Медленная скорость";

                break;
            case 1:
                i64diffImageId = 2;
                sInfo = "Чуть быстрее скорость";
                break;
            case 2:
                i64diffImageId = 7;
                sInfo = "Поехавшая скорость\nС такой скорость братишка чистит вилкой";
                break;
            }
            if (ImGui::Button("<", ImVec2(64, 64))) {
                PahomEngine->pDiff->i64id--;
                if (PahomEngine->pDiff->i64id < 0) {
                    PahomEngine->pDiff->i64id = 0;
                }
                PahomEngine->pDiff->setDiff(PahomEngine->pDiff->i64id);
            }

            ImGui::SameLine();
            ImGui::Image(
                PahomEngine->ptrint64_t(PahomEngine->ImageData.TextureArray[i64diffImageId]),
                ImVec2(128, 128)
            );
            ImGui::SameLine();

            // Кнопка ">" (увеличение сложности)
            if (ImGui::Button(">", ImVec2(64, 64))) {
                PahomEngine->pDiff->i64id++;
                if (PahomEngine->pDiff->i64id > 2) {
                    PahomEngine->pDiff->i64id = 2;
                }
                PahomEngine->pDiff->setDiff(PahomEngine->pDiff->i64id);
            }
            ImGui::CustomToggle("Рандомный счет", &PahomEngine->pDiff->bRandScoreDiff);
            ImGui::CustomToggle("Нарастающая сложность", &PahomEngine->pDiff->bRandDiff);
            ImGui::CustomToggle("DemoPlay", &PahomEngine->Event.bDemoPlay);

            ImGui::Text("%s", PahomEngine->pDiff->diffSelected.c_str());
            ImGui::TextColored(RGBAtoIV4(30, 30, 45, 255), "%s", sInfo.c_str());
    

            if (ImGui::Button("OK", ImVec2(150, 30))) {
                if (PahomEngine->pDiff->bRandDiff) {
                    PahomEngine->log("->Diffinity:: set diff random");
                    PahomEngine->pDiff->setDiff(3);
                    PahomEngine->fStepMove = PahomEngine->randfloat(PahomEngine->pDiff->diffArray[PahomEngine->rand64(2)].i64buffer);
                    PahomEngine->fStep = PahomEngine->randfloat(PahomEngine->pDiff->diffArray[PahomEngine->rand64(2)].i64buffer1);
                }
                else {
                    if (!PahomEngine->bFullscreen)
                    {
                        PahomEngine->pDiff->setDiff(PahomEngine->pDiff->i64id);
                        PahomEngine->fStepMove = PahomEngine->pDiff->diffArray[PahomEngine->pDiff->i64id].i64buffer;
                        PahomEngine->fStep = PahomEngine->pDiff->diffArray[PahomEngine->pDiff->i64id].i64buffer1;
                    }
                    else {
                        PahomEngine->pDiff->setDiff(PahomEngine->pDiff->i64id);
                        PahomEngine->fStepMove = PahomEngine->pDiff->diffArray[PahomEngine->pDiff->i64id].i64buffer;
                        PahomEngine->fStep = PahomEngine->pDiff->diffArray[PahomEngine->pDiff->i64id].i64buffer1 + 20;
                    }
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
    void logo() {
        static float imageSizeX = 200;
        static float imageSizeY = 200;
        float maxSize = 256.0f;
        static float scale = 0;
        static int64_t i64Timer = 0;
        static bool revesed = false;
        static bool ba = true, bg = true;
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
        ImGui::Image(PahomEngine->ptrint64_t(PahomEngine->ImageData.TextureArray[0]), ImVec2(imageSizeX, imageSizeY));
        ///
        PahomEngine->pDiff->i64id = 1;
        PahomEngine->pDiff->setDiff(1);
        PahomEngine->fStepMove = PahomEngine->pDiff->diffArray[PahomEngine->pDiff->i64id].i64buffer;
        PahomEngine->fStep = PahomEngine->pDiff->diffArray[PahomEngine->pDiff->i64id].i64buffer1;
        //
    }
    void debug_offset() {
        if (ImGui::BeginPopup("set_params")) {
            static int64_t fOldSize[2] = { 800,600 };
            ImGui::TextColored(PahomEngine->RGBA(133, 133, 133, 255), "Настройки (Параметры)");
            ImGui::Separator();
            ImGui::Checkbox("60 фпс", &PahomEngine->CVsync);
            ImGui::Checkbox("Отладка", &PahomEngine->bDebugText);
            ImGui::Checkbox("PahomEngine->Event.bScreamEvent", &PahomEngine->Event.bScreamEvent);
            ImGui::Checkbox("PahomEngine->Event.bScreamEventBackground", &PahomEngine->Event.bScreamEventBackground);
            ImGui::Checkbox("PahomEngine->bKefir", &PahomEngine->bKefir);
            ImGui::Separator();
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
                if (PahomEngine->i64RandBoost < 1) {
                    PahomEngine->bBoost777 = true;
                    PahomEngine->audio.play3(5);
                }
                if (PahomEngine->i64RandBoost < 3) {
                    PahomEngine->bKefir = true;
                    // PahomEngine->audio.play3(7);
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
                if (PahomEngine->GetGamepadKey(PahomEngine->keyMap.i64FORWARDGamepad)) {//PahomEngine->keyMap.u8FORWARDGamepad
                    PahomEngine->keyMap.kbDelay++;
                    if (PahomEngine->keyMap.kbDelay == PahomEngine->keyMap.vMaxDelay) {
                        PahomEngine->fPahomPosX += PahomEngine->fStep * PahomEngine->fDeltaTime;
                        PahomEngine->bIsRevesed = true;
                        if (PahomEngine->fPahomPosX >= PahomEngine->fMaxPahomPosX) {
                            PahomEngine->fPahomPosX = PahomEngine->fMaxPahomPosX;
                        }
                        PahomEngine->keyMap.kbDelay = 0;
                    }
                    //keyPresedStr = "FORWARD" + (PahomEngine->keyMap.i64FORWARDGamepad);
                }
                if (PahomEngine->GetGamepadKey(PahomEngine->keyMap.i64BACKGamepad)) {//PahomEngine->keyMap.u8BACKGamepad
                    PahomEngine->keyMap.kbDelay++;
                    if (PahomEngine->keyMap.kbDelay == PahomEngine->keyMap.vMaxDelay) {
                        PahomEngine->bIsRevesed = false;
                        PahomEngine->fPahomPosX -= PahomEngine->fStep * PahomEngine->fDeltaTime;
                        if (PahomEngine->fPahomPosX <= PahomEngine->fMinPahomPosX) {
                            PahomEngine->fPahomPosX = PahomEngine->fMinPahomPosX;
                        }
                        PahomEngine->keyMap.kbDelay = 0;
                    }
                    //keyPresedStr = (const char*)("BACK" + PahomEngine->keyMap.u8BACK);
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
                    PahomEngine->fPahomPosX += PahomEngine->fStep * PahomEngine->fDeltaTime;
                    PahomEngine->bIsRevesed = true;
                    if (PahomEngine->fPahomPosX >= PahomEngine->fMaxPahomPosX) {
                        PahomEngine->fPahomPosX = PahomEngine->fMaxPahomPosX;
                    }
                    PahomEngine->keyMap.kbDelay = 0;
                }
                //keyPresedStr = "FORWARD" + (PahomEngine->keyMap.u8FORWARD);
            }
            if (GetAsyncKeyState(PahomEngine->keyMap.u8FORWARD)) {//PahomEngine->keyMap.u8BACK
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
        }
    }
};
auto Game = std::make_unique<GameFrames>();