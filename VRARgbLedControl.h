#ifndef VRA_RGB_LED_CONTROL_H
#define VRA_RGB_LED_CONTROL_H

#include "BLEService.h"

// #include "VRASettings.h"
// #include "VRAStorage.h"
#include "IntervalEvent.h"
#include "neopixel.h"

#define CHAR_LEN_SET_LED 5
#define CHAR_LEN_LED_MODE 9

class VRARgbLedControl : public BLEService
{
public:
    enum class Characteristics: uint16_t
    {
        SetLed = 0xFF00,
        LedMode = 0xFF01
    };

    enum class LedMode{
        Off,
        On,
        Blink,
        Ramp,
        Sine
    };

    VRARgbLedControl(UUID *p_uuid, EventQueue *p_eq, StateChain *p_stateChain, PinName ledPin=NC, int length=0);

    void init();
    void initCharacteristics();
    void pastBleInit();
    void setColorRGBA(int ledIndex, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void ledMode(uint8_t ledIndex, int color, uint8_t mode, uint16_t interval, uint8_t repeat);
private:
    void onStateOff();
    void onStateStandby();
    void onStateOn();

    void setLedWriteCb();
    void ledModeWriteCb();

    EventQueue *eq;
    // VRASettings *settings;
    // VRAStorage *storage;
    NeoPixel *neoPixel;
    BLECharacteristic* setLedCharacteristic;
    BLECharacteristic* ledModeCharacteristic;
    int *eventIds;

    // static constexpr uint8_t sineLut{

    // };
};

#endif //VRA_RGB_LED_CONTROL_H