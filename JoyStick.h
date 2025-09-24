#pragma comment (lib , "xinput.lib")
#include <stdint.h>
#include <XInput.h>
struct battery {
    uint8_t type;
    uint8_t level;
    constexpr battery() : type(0), level(0) {}
    constexpr battery(uint8_t _type, uint8_t _level) : type(_type),level(_level) {}
};

class JoyStickAPI//-
{
private:
    XINPUT_STATE _controllerState;
    XINPUT_BATTERY_INFORMATION _batteryState;
    int _controllerNum;
    int64_t BatteryLevel;
    //int64_t fBatteryLevel;
public:
    JoyStickAPI(int64_t playerNumber);
    XINPUT_STATE GetState();
    XINPUT_BATTERY_INFORMATION BatteryState();
    bool IsConnected();
    void Vibrate(int64_t leftVal = 0, int64_t rightVal = 0);
    int64_t BatLevel();
    int64_t GetCountControllers(int64_t i);
    battery *xBattery = new battery;
};
int64_t JoyStickAPI::GetCountControllers(int64_t i) {
    return _controllerNum;
}
JoyStickAPI::JoyStickAPI(int64_t playerNumber)
{
    // Set the Controller Number
    _controllerNum = playerNumber - 1;
}
XINPUT_STATE JoyStickAPI::GetState()
{
    // Zeroise the state
    ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

    // Get the state
    XInputGetState(_controllerNum, &_controllerState);

    return _controllerState;
}
bool JoyStickAPI::IsConnected()
{
    // Zeroise the state
    ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

    // Get the state
    DWORD Result = XInputGetState(_controllerNum, &_controllerState);

    if(Result == ERROR_SUCCESS)
    {
        return true;
    }
    else
    {
        return false;
    }
}
int64_t JoyStickAPI::BatLevel()
{
    // Create a Vibraton State
    _XINPUT_BATTERY_INFORMATION Battery;

    // Zeroise the Vibration
    ZeroMemory(&Battery, sizeof(_XINPUT_BATTERY_INFORMATION));

    // Set the Vibration Values
    // Vibrate the controller
    XInputGetBatteryInformation(_controllerNum,BATTERY_DEVTYPE_GAMEPAD,&Battery);
    xBattery->type =  Battery.BatteryType;
    xBattery->level = Battery.BatteryLevel;
    return 0;
}
void JoyStickAPI::Vibrate(int64_t  leftVal, int64_t rightVal)
{
    // Create a Vibraton State
    XINPUT_VIBRATION Vibration;

    // Zeroise the Vibration
    ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

    // Set the Vibration Values
    Vibration.wLeftMotorSpeed = leftVal;
    Vibration.wRightMotorSpeed = rightVal;

    // Vibrate the controller
    XInputSetState(_controllerNum, &Vibration);
}
