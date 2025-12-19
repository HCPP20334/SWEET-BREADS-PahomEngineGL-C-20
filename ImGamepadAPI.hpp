 
 #include "imgui.h"
 #include "imgui_internal.h"
 struct GamepadAPI{
    float fB_a = 0.0f;
    float fB_b = 0.0f;
    float fB_x = 0.0f;
    float fB_y = 0.0f;
    float fB_rt = 0.0f;
    float fB_rb = 0.0f;
    float fB_lb = 0.0f;
    float fB_lt = 0.0f;
    float fB_du = 0.0f;
    float fB_dd = 0.0f;
    float fB_dl = 0.0f;
    float fB_dr = 0.0f;
    float fB_ls = 0.0f;
    float fB_rs = 0.0f;
    float fLX = 0;
    float fLY = 0;
    float fRX = 0;
    float fRY = 0;
    float fLT = 0;
    float fRT = 0;
    bool b_Start = false;
    bool b_Back = false;
 };
 auto Gamepad = std::make_unique<GamepadAPI>();
 
 
void traceGamepadAPI(){
     ImGuiKeyData* fLS_left = ImGui::GetKeyData(ImGuiKey_GamepadLStickLeft);
 ImGuiKeyData* fLS_Right = ImGui::GetKeyData(ImGuiKey_GamepadLStickRight);
 ImGuiKeyData* fLS_Up = ImGui::GetKeyData(ImGuiKey_GamepadLStickUp);
 ImGuiKeyData* fLS_Down = ImGui::GetKeyData(ImGuiKey_GamepadLStickDown);
 //
 ImGuiKeyData* fRS_left = ImGui::GetKeyData(ImGuiKey_GamepadRStickLeft);
 ImGuiKeyData* fRS_Right = ImGui::GetKeyData(ImGuiKey_GamepadRStickRight);
 ImGuiKeyData* fRS_Up = ImGui::GetKeyData(ImGuiKey_GamepadRStickUp);
 ImGuiKeyData* fRS_Down = ImGui::GetKeyData(ImGuiKey_GamepadRStickDown);
 ImGuiKeyData* fLTv = ImGui::GetKeyData(ImGuiKey_GamepadL2);
 ImGuiKeyData* fRTv = ImGui::GetKeyData(ImGuiKey_GamepadR2);
 Gamepad->fLX = fLS_Right->AnalogValue - fLS_left->AnalogValue;
 Gamepad->fLY = fLS_Down->AnalogValue - fLS_Up->AnalogValue;
 Gamepad->fRY = fRS_Down->AnalogValue - fRS_Up->AnalogValue;
 Gamepad->fRX = fRS_Right->AnalogValue - fRS_left->AnalogValue;
 fRTv->Down ? Gamepad->fRT += 10 : Gamepad->fRT -= 10;
 fLTv->Down ? Gamepad->fLT += 10 : Gamepad->fLT -= 10;

 if (Gamepad->fRT > 255) {
     Gamepad->fRT = 255;
 }
 if (Gamepad->fLT > 255) {
     Gamepad->fLT = 255;
 }
 if (Gamepad->fRT <= 0) {
     Gamepad->fRT = 0;
 }
 if (Gamepad->fLT <= 0) {
     Gamepad->fLT = 0;
 }
 if ( ImGui::IsKeyDown(ImGuiKey_GamepadFaceRight)) { Gamepad->fB_b = Gamepad->fB_b + 0.01f; if (Gamepad->fB_b > 1.000f) { Gamepad->fB_b = 1.000f; } }
 if ( ImGui::IsKeyDown(ImGuiKey_GamepadFaceDown)) { Gamepad->fB_a = Gamepad->fB_a + 0.01f; if (Gamepad->fB_a > 1.000f) { Gamepad->fB_a = 1.000f; } }
 if ( ImGui::IsKeyDown(ImGuiKey_GamepadFaceLeft)) { Gamepad->fB_x = Gamepad->fB_x + 0.01f; if (Gamepad->fB_x > 1.000f) { Gamepad->fB_x = 1.000f; } }
 if ( ImGui::IsKeyDown(ImGuiKey_GamepadFaceUp)) { Gamepad->fB_y = Gamepad->fB_y + 0.01f; if (Gamepad->fB_y > 1.000f) { Gamepad->fB_y = 1.000f; } }
 if ( ImGui::IsKeyDown(ImGuiKey_GamepadR1)) { Gamepad->fB_rs = Gamepad->fB_rs + 0.01f; if (Gamepad->fB_rs > 1.000f) { Gamepad->fB_rs = 1.000f; } }
 if ( ImGui::IsKeyDown(ImGuiKey_GamepadL1)) { Gamepad->fB_ls = Gamepad->fB_ls + 0.01f; if (Gamepad->fB_ls > 1.000f) { Gamepad->fB_ls = 1.000f; } }
 if ( ImGui::IsKeyDown(ImGuiKey_GamepadDpadUp)) { Gamepad->fB_du = Gamepad->fB_du + 0.01f; if (Gamepad->fB_du > 1.000f) { Gamepad->fB_du = 1.000f; } }
 if ( ImGui::IsKeyDown(ImGuiKey_GamepadDpadDown)) { Gamepad->fB_dd = Gamepad->fB_dd + 0.01f; if (Gamepad->fB_dd > 1.000f) { Gamepad->fB_dd = 1.000f; } }
 if ( ImGui::IsKeyDown(ImGuiKey_GamepadDpadLeft)) { Gamepad->fB_dl = Gamepad->fB_dl + 0.01f; if (Gamepad->fB_dl > 1.000f) { Gamepad->fB_dl = 1.000f; } }
 if ( ImGui::IsKeyDown(ImGuiKey_GamepadDpadRight)) { Gamepad->fB_dr = Gamepad->fB_dr + 0.01f; if (Gamepad->fB_dr > 1.000f) { Gamepad->fB_dr = 1.000f; } }
 if ( ImGui::IsKeyDown(ImGuiKey_GamepadStart)) { Gamepad->b_Start = !Gamepad->b_Start; }
 if ( ImGui::IsKeyDown(ImGuiKey_GamepadBack)) { Gamepad->b_Back = !Gamepad->b_Back; }
 //
 if (! ImGui::IsKeyDown(ImGuiKey_GamepadFaceRight)) { Gamepad->fB_b = Gamepad->fB_b - 0.01f; if (Gamepad->fB_b < 0.1f) { Gamepad->fB_b = 0.0f; } }
 if (! ImGui::IsKeyDown(ImGuiKey_GamepadFaceDown)) { Gamepad->fB_a = Gamepad->fB_a - 0.01f; if (Gamepad->fB_a < 0.1f) { Gamepad->fB_a = 0.0f; } }
 if (! ImGui::IsKeyDown(ImGuiKey_GamepadFaceLeft)) { Gamepad->fB_x = Gamepad->fB_x - 0.01f; if (Gamepad->fB_x < 0.1f) { Gamepad->fB_x = 0.0f; } }
 if (! ImGui::IsKeyDown(ImGuiKey_GamepadFaceUp)) { Gamepad->fB_y = Gamepad->fB_y - 0.01f; if (Gamepad->fB_y < 0.1f) { Gamepad->fB_y = 0.0f; } }
 if (! ImGui::IsKeyDown(ImGuiKey_GamepadR1)) { Gamepad->fB_rs = Gamepad->fB_rs - 0.01f; if (Gamepad->fB_rs < 0.1f) { Gamepad->fB_rs = 0.0f; } }
 if (! ImGui::IsKeyDown(ImGuiKey_GamepadL1)) { Gamepad->fB_ls = Gamepad->fB_ls - 0.01f; if (Gamepad->fB_ls < 0.1f) { Gamepad->fB_ls = 0.0f; } }
 if (! ImGui::IsKeyDown(ImGuiKey_GamepadDpadUp)) { Gamepad->fB_du = Gamepad->fB_du - 0.01f; if (Gamepad->fB_du < 0.1f) { Gamepad->fB_du = 0.0f; } }
 if (! ImGui::IsKeyDown(ImGuiKey_GamepadDpadDown)) { Gamepad->fB_dd = Gamepad->fB_dd - 0.01f; if (Gamepad->fB_dd < 0.1f) { Gamepad->fB_dd = 0.0f; } }
 if (! ImGui::IsKeyDown(ImGuiKey_GamepadDpadLeft)) { Gamepad->fB_dl = Gamepad->fB_dl - 0.01f; if (Gamepad->fB_dl < 0.1f) { Gamepad->fB_dl = 0.0f; } }
 if (! ImGui::IsKeyDown(ImGuiKey_GamepadDpadRight)) { Gamepad->fB_dr = Gamepad->fB_dr - 0.01f; if (Gamepad->fB_dr < 0.1f) { Gamepad->fB_dr = 0.0f; } }
}
//  Обработчик Нажатий Геймпада ImGui
//  Параметр u8Key принимает Xinput Макросы кнопок
//   Например :
//   if (getKeyPresed(XINPUT_GAMEPAD_A)) {
//       std::cout << "Нажата Кнопка А\n"
//   }
static std::string sPresedKeyGamepad;
bool getKeyPresed(DWORD u8key) {
    ImGuiKey key;
    switch (u8key) {
    case XINPUT_GAMEPAD_A:
        key = ImGuiKey_GamepadFaceDown;
        sPresedKeyGamepad = "button_A";
        break;
    case XINPUT_GAMEPAD_B:
        key = ImGuiKey_GamepadFaceRight;
        sPresedKeyGamepad = "button_B";
        break;
    case XINPUT_GAMEPAD_X:
        key = ImGuiKey_GamepadFaceLeft;
        sPresedKeyGamepad = "button_X";
        break;
    case XINPUT_GAMEPAD_Y:
        key = ImGuiKey_GamepadFaceUp;
        sPresedKeyGamepad = "button_Y";
        break;
    case XINPUT_GAMEPAD_DPAD_UP:
        key = ImGuiKey_GamepadDpadUp;
        sPresedKeyGamepad = "button_dpad_up";
        break;
    case XINPUT_GAMEPAD_DPAD_DOWN:
        key = ImGuiKey_GamepadDpadDown;
        sPresedKeyGamepad = "button_dpad_down";
        break;
    case XINPUT_GAMEPAD_DPAD_LEFT:
        key = ImGuiKey_GamepadDpadLeft;
        sPresedKeyGamepad = "button_dpad_left";
        break;
    case XINPUT_GAMEPAD_DPAD_RIGHT:
        key = ImGuiKey_GamepadDpadRight;
        sPresedKeyGamepad = "button_dpad_right";
        break;
    case XINPUT_GAMEPAD_LEFT_THUMB:
        key = ImGuiKey_GamepadL2;
        sPresedKeyGamepad = "button_LT";
        break;
    case XINPUT_GAMEPAD_RIGHT_THUMB:
        key = ImGuiKey_GamepadR2;
        sPresedKeyGamepad = "button_RT";
        break;
    case XINPUT_GAMEPAD_LEFT_SHOULDER:
        key = ImGuiKey_GamepadL1;
        sPresedKeyGamepad = "button_LS";
        break;
    case XINPUT_GAMEPAD_RIGHT_SHOULDER:
        key = ImGuiKey_GamepadR1;
        sPresedKeyGamepad = "button_RS";
        break;
    case XINPUT_GAMEPAD_START:
        key = ImGuiKey_GamepadStart;
        sPresedKeyGamepad = "button_START";
        break;
    case XINPUT_GAMEPAD_BACK:
        key = ImGuiKey_GamepadBack;
        sPresedKeyGamepad = "button_BACK";
        break;
    default:
        key = ImGuiKey_GamepadBack;
        sPresedKeyGamepad = "button_BACK";
        break;
    }
    return ImGui::IsKeyDown(key);
}
ImVec4 rgba(ImVec4 col) {
    return { col.x / 255.0f,col.y / 255.0f,col.z / 255.0f,col.w / 255.0f };
}
void buttonItemUI(const char* buttonName,bool state) {
    ImVec4 colorButton = (state ? ImVec4{12, 12, 19, 255} : ImVec4{37, 37, 37, 255});
    ImGui::PushStyleColor(ImGuiCol_Button, rgba(colorButton));
    ImGui::Button(buttonName, { 32,32 });
    ImGui::PopStyleColor();
}
void pos_x(float x) {
    ImGui::SetCursorPosX(x);
}
void toLine() {
    ImGui::SameLine();
}
void setCenterToFrame(ImVec2 size, ImVec2 sizeWindow) {
    ImGui::SetCursorPos({
        (sizeWindow - size) / ImVec2{2,2}
        });
}
void GamepadTestUI() {
    bool bA = false, bB = false, bX = false, bY = false;
    bool bDU = false, bDD = false, bDL = false, bDR = false;
    setCenterToFrame({ 300,160 }, ImGui::GetWindowSize());
    if (ImGui::BeginChild("GamepadTest", { 300,160 }, ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)) {
        pos_x(30); ImGui::Text("GamepadTest");
        pos_x(30); ImGui::Separator();
        pos_x(60);
        buttonItemUI("Y", getKeyPresed(XINPUT_GAMEPAD_Y)); toLine(); pos_x(174); buttonItemUI("U", getKeyPresed(XINPUT_GAMEPAD_DPAD_UP));
        pos_x(30);
        buttonItemUI("X", getKeyPresed(XINPUT_GAMEPAD_X)); toLine(); pos_x(90); buttonItemUI("B", getKeyPresed(XINPUT_GAMEPAD_B)); toLine(); pos_x(140); buttonItemUI("L", getKeyPresed(XINPUT_GAMEPAD_DPAD_LEFT)); toLine(); pos_x(204); buttonItemUI("R", getKeyPresed(XINPUT_GAMEPAD_DPAD_RIGHT));
        pos_x(60);
        buttonItemUI("A", getKeyPresed(XINPUT_GAMEPAD_A));  toLine(); pos_x(174); buttonItemUI("D", getKeyPresed(XINPUT_GAMEPAD_DPAD_DOWN));
        pos_x(30);
        ImGui::TextColored({ 255,200,0,255 }, "Close [ESC]");
        ImGui::EndChild();
    }
}