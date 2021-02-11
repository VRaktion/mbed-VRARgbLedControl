#include "VRARgbLedControl.h"    
    
VRARgbLedControl::VRARgbLedControl(
    UUID *p_uuid, EventQueue *p_eq, StateChain *p_stateChain, PinName ledPin, int length):
    BLEService("ledCtrl", p_uuid, p_eq, p_stateChain),
    eq(p_eq),
    neoPixel(new NeoPixel(ledPin, length))
{
    // this->intervalEvents = malloc(lenght * sizeof(IntervalEvent));
    // for(int i = 0; i<length; i++){
    //     this->intervalEvents[i] = new IntervalEvent(this->eq, 10, );
    // }
    this->eventIds = (int*) malloc(length * sizeof(int));
}

void VRARgbLedControl::init(){
    printf("[ledCtrl] init\r\n");
}

void VRARgbLedControl::initCharacteristics(){
    printf("[ledCtrl] init Characteristics\r\n");
    this->setLedCharacteristic = new BLECharacteristic(
            (uint16_t) VRARgbLedControl::Characteristics::SetLed,
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ 
            | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE,
            CHAR_LEN_SET_LED//size
        );
    this->addCharacteristic(
        this->setLedCharacteristic
        );
    this->setLedCharacteristic->setWriteCallback(
        new Callback<void(void)>(this, &VRARgbLedControl::setLedWriteCb));

    this->ledModeCharacteristic = new BLECharacteristic(
            (uint16_t) VRARgbLedControl::Characteristics::LedMode,
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ 
            | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE,
            CHAR_LEN_LED_MODE//size: 1b index 4b color 1b mode 2b interval 1b repeat
        );
    this->addCharacteristic(
        this->ledModeCharacteristic
        );
    this->ledModeCharacteristic->setWriteCallback(
        new Callback<void(void)>(this, &VRARgbLedControl::ledModeWriteCb));

}

void VRARgbLedControl::pastBleInit(){
    printf("[ledCtrl] pastBleInit\r\n");
}

void VRARgbLedControl::onStateOff(){
    printf("[ledCtrl] off\r\n");
}

void VRARgbLedControl::onStateStandby(){
    printf("[ledCtrl] standby\r\n");
}

void VRARgbLedControl::onStateOn(){
    printf("[ledCtrl] on\r\n");
}

void VRARgbLedControl::setLedWriteCb(){
    BLE &ble = BLE::Instance(BLE::DEFAULT_INSTANCE);
    uint8_t values[CHAR_LEN_SET_LED];
    uint16_t length = CHAR_LEN_SET_LED;
    ble_error_t err = ble.gattServer().read(
            this->setLedCharacteristic->getValueHandle(), values, &length);
    printf("new setLed %d r: %d g: %d b: %d\r\n", values[0], values[1], values[2], values[3]);
    this->eq->call(callback(this, &VRARgbLedControl::setColorRGBA), 
        values[0], values[1], values[2], values[3], values[4]);
}

//1b index 3b color 1b mode 2b interval 1b repeat
void VRARgbLedControl::ledModeWriteCb(){
        BLE &ble = BLE::Instance(BLE::DEFAULT_INSTANCE);
    uint8_t values[CHAR_LEN_LED_MODE];
    uint16_t length = CHAR_LEN_LED_MODE;
    ble_error_t err = ble.gattServer().read(
            this->setLedCharacteristic->getValueHandle(), values, &length);
    // this->ledMode();
}

void VRARgbLedControl::setColorRGBA(int ledIndex, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    r=r*a/255;
    g=g*a/255;
    b=b*a/255;
    this->neoPixel->setColor(ledIndex, r << 16 | g << 8 | b);
    this->neoPixel->show();
}

void VRARgbLedControl::ledMode(uint8_t ledIndex, int color, uint8_t mode, uint16_t interval, uint8_t repeat){

}