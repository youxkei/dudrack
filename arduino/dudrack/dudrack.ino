#define USBCON

#include <hidboot.h>
#include <hiduniversal.h>
#include <usbhub.h>
#include <Keyboard.h>
#include <Mouse.h>

#define LEN 8

#define MODIFIER_CTRL_LEFT 0x1
#define MODIFIER_SHIFT_LEFT 0x2

#define KEY_CAPS_LOCK 0x39
#define KEY_HENKAN 0x8A
#define KEY_MUHENKAN 0x8B
#define KEY_HIRAGANA 0x88

#define KEY_1 0x1E
#define KEY_2 0x1F
#define KEY_3 0x20
#define KEY_4 0x21
#define KEY_5 0x22
#define KEY_6 0x23
#define KEY_7 0x24
#define KEY_8 0x25
#define KEY_9 0x26
#define KEY_0 0x27
#define KEY_MINUS 0x2D
#define KEY_HAT 0x2E
#define KEY_YEN 0x89
#define KEY_Q 0x14
#define KEY_W 0x1A
#define KEY_E 0x8
#define KEY_R 0x15
#define KEY_T 0x17
#define KEY_Y 0x1C
#define KEY_U 0x18
#define KEY_I 0xC
#define KEY_O 0x12
#define KEY_P 0x13
#define KEY_AT 0x2F
#define KEY_BRACKET_OPEN 0x30
#define KEY_A 0x4
#define KEY_S 0x16
#define KEY_D 0x7
#define KEY_F 0x9
#define KEY_G 0xA
#define KEY_H 0xB
#define KEY_J 0xD
#define KEY_K 0xE
#define KEY_L 0xF
#define KEY_SEMICOLON 0x33
#define KEY_COLON 0x34
#define KEY_BRACKET_CLOSE 0x32
#define KEY_Z 0x1D
#define KEY_X 0x1B
#define KEY_C 0x6
#define KEY_V 0x19
#define KEY_B 0x5
#define KEY_N 0x11
#define KEY_M 0x10
#define KEY_COMMA 0x36
#define KEY_PERIOD 0x37
#define KEY_SLASH 0x38
#define KEY_BACKSLASH 0x87

#define KEY_ESCAPE 0x29
#define KEY_ENTER 0x28
#define KEY_TAB 0x2B
#define KEY_BACKSPACE 0x2A
#define KEY_DELETE 0x4C
#define KEY_HAN_ZEN 0x35

#define KEY_UP 0x52
#define KEY_DOWN 0x51
#define KEY_LEFT 0x50
#define KEY_RIGHT 0x4F
#define KEY_HOME 0x4A
#define KEY_END 0x4D

#define KEY_COUNT 0x100

#define OUTPUT_TO_SERIAL 1

uint8_t neutralTable[KEY_COUNT];
void initNeutralTable(){
    for (int i = 0; i < KEY_COUNT; ++i){
        neutralTable[i] = i;
    }

    neutralTable[KEY_Q        ] = KEY_COLON;
    neutralTable[KEY_W        ] = KEY_COMMA;
    neutralTable[KEY_E        ] = KEY_PERIOD;
    neutralTable[KEY_R        ] = KEY_P;
    neutralTable[KEY_T        ] = KEY_Y;
    neutralTable[KEY_Y        ] = KEY_F;
    neutralTable[KEY_U        ] = KEY_G;
    neutralTable[KEY_I        ] = KEY_C;
    neutralTable[KEY_O        ] = KEY_R;
    neutralTable[KEY_P        ] = KEY_L;
    neutralTable[KEY_AT       ] = KEY_SLASH;
    neutralTable[KEY_S        ] = KEY_O;
    neutralTable[KEY_D        ] = KEY_E;
    neutralTable[KEY_F        ] = KEY_U;
    neutralTable[KEY_G        ] = KEY_I;
    neutralTable[KEY_H        ] = KEY_D;
    neutralTable[KEY_J        ] = KEY_H;
    neutralTable[KEY_K        ] = KEY_T;
    neutralTable[KEY_L        ] = KEY_N;
    neutralTable[KEY_SEMICOLON] = KEY_S;
    neutralTable[KEY_COLON    ] = KEY_MINUS;
    neutralTable[KEY_Z        ] = KEY_SEMICOLON;
    neutralTable[KEY_X        ] = KEY_Q;
    neutralTable[KEY_C        ] = KEY_J;
    neutralTable[KEY_V        ] = KEY_K;
    neutralTable[KEY_B        ] = KEY_X;
    neutralTable[KEY_N        ] = KEY_B;
    neutralTable[KEY_M        ] = KEY_M;
    neutralTable[KEY_COMMA    ] = KEY_W;
    neutralTable[KEY_PERIOD   ] = KEY_V;
    neutralTable[KEY_SLASH    ] = KEY_Z;
}

uint8_t henkanTable[KEY_COUNT];
void initHenkanTable(){
    for (int i = 0; i < KEY_COUNT; ++i){
        henkanTable[i] = i;
    }

    henkanTable[KEY_Q        ] = KEY_1;
    henkanTable[KEY_W        ] = KEY_2;
    henkanTable[KEY_E        ] = KEY_3;
    henkanTable[KEY_R        ] = KEY_4;
    henkanTable[KEY_T        ] = KEY_5;
    henkanTable[KEY_Y        ] = KEY_6;
    henkanTable[KEY_U        ] = KEY_7;
    henkanTable[KEY_I        ] = KEY_8;
    henkanTable[KEY_O        ] = KEY_9;
    henkanTable[KEY_P        ] = KEY_0;
    henkanTable[KEY_AT       ] = KEY_SLASH;
    henkanTable[KEY_A        ] = KEY_TAB;
    henkanTable[KEY_S        ] = KEY_ESCAPE;
    henkanTable[KEY_D        ] = KEY_ENTER;
    henkanTable[KEY_F        ] = KEY_BACKSPACE;
    henkanTable[KEY_G        ] = KEY_DELETE;
    henkanTable[KEY_H        ] = KEY_HOME;
    henkanTable[KEY_J        ] = KEY_LEFT;
    henkanTable[KEY_K        ] = KEY_UP;
    henkanTable[KEY_L        ] = KEY_RIGHT;
    henkanTable[KEY_SEMICOLON] = KEY_END;
    henkanTable[KEY_COLON    ] = KEY_MINUS;
    henkanTable[KEY_Z        ] = KEY_AT;
    henkanTable[KEY_X        ] = KEY_BRACKET_OPEN;
    henkanTable[KEY_C        ] = KEY_BRACKET_CLOSE;
    henkanTable[KEY_V        ] = KEY_HAT;
    henkanTable[KEY_B        ] = KEY_X;
    henkanTable[KEY_N        ] = KEY_B;
    henkanTable[KEY_M        ] = KEY_BACKSLASH;
    henkanTable[KEY_COMMA    ] = KEY_DOWN;
    henkanTable[KEY_PERIOD   ] = KEY_YEN;
    henkanTable[KEY_SLASH    ] = KEY_HAN_ZEN;
}

bool remove(uint8_t *buf, uint8_t key){
    uint8_t index;

    for (index = 2; index < LEN; ++index) {
        if (buf[index] == key) break;
    }

    if (index == LEN) return false;

    for (; index < LEN - 1; ++index) {
        buf[index] = buf[index + 1];
    }

    buf[LEN - 1] = 0;

    return true;
}

void changeCapsLockToControl(uint8_t *buf){
    if (remove(buf, KEY_CAPS_LOCK)) buf[0] |= MODIFIER_CTRL_LEFT;
}

void changeMuhenkanToShift(uint8_t *buf) {
    if (remove(buf, KEY_MUHENKAN)) buf[0] |= MODIFIER_SHIFT_LEFT;
}

bool henkanPrev = false;

void handleCustomModifier(uint8_t *buf) {
    bool henkan = remove(buf, KEY_HENKAN);

    // modifierが変化した時にはキーを出力しない
    if (!henkanPrev && henkan || henkanPrev && !henkan) {
        for (int i = 2; i < LEN; i++) {
            buf[i] = 0;
        }
    } else if (henkan){
        for (int i = 2; i < LEN; i++){
            buf[i] = henkanTable[buf[i]];
        }
    } else {
        for (int i = 2; i < LEN; i++){
            buf[i] = neutralTable[buf[i]];
        }
    }

    henkanPrev = henkan;
}


class HIDKeyboardReportParser : public HIDReportParser {
public:
    virtual void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
#if OUTPUT_TO_SERIAL
        Serial.print("SRC:  ");
        for (int i = 0; i < LEN; i++) {
            Serial.print(buf[i], HEX);
            Serial.print(" ");
        }
        Serial.println("");
#endif

        changeCapsLockToControl(buf);
        changeMuhenkanToShift(buf);
        handleCustomModifier(buf);

#if OUTPUT_TO_SERIAL
        Serial.print("DEST: ");
        for (int i = 0; i < len; i++) {
            Serial.print(buf[i], HEX);
            Serial.print(" ");
        }
        Serial.println("");
#endif

        Keyboard.sendReport((KeyReport *)buf);
    }
};


class HIDMouseReportParser : public MouseReportParser {
protected:
    virtual void OnMouseMove(MOUSEINFO *mouseInfo)        { Mouse.move((signed char)(mouseInfo->dX * 1.5), (signed char)(mouseInfo->dY * 1.5), 0); }
    virtual void OnLeftButtonDown(MOUSEINFO *mouseInfo)   { Mouse.press(MOUSE_LEFT); }
    virtual void OnLeftButtonUp(MOUSEINFO *mouseInfo)     { Mouse.release(MOUSE_LEFT); }
    virtual void OnRightButtonDown(MOUSEINFO *mouseInfo)  { Mouse.press(MOUSE_RIGHT); }
    virtual void OnRightButtonUp(MOUSEINFO *mouseInfo)    { Mouse.release(MOUSE_RIGHT); }
    virtual void OnMiddleButtonDown(MOUSEINFO *mouseInfo) { Mouse.press(MOUSE_MIDDLE); }
    virtual void OnMiddleButtonUp(MOUSEINFO *mouseInfo)   { Mouse.release(MOUSE_MIDDLE); }
};

USB    Usb;
USBHub Hub(&Usb);

HIDBoot<USB_HID_PROTOCOL_KEYBOARD | USB_HID_PROTOCOL_MOUSE> HidComposite(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>                          HidKeyboard(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE>                             HidMouse(&Usb);

HIDKeyboardReportParser keyboardReportParser;
HIDMouseReportParser mouseReportParser;

void setup() {
    int usbStatus = Usb.Init();

#if OUTPUT_TO_SERIAL
    Serial.begin(115200);
    while (!Serial);
    if (usbStatus == -1) {
        Serial.println("OSC did not start.");
    }
#endif

    initNeutralTable();
    initHenkanTable();

    HidComposite.SetReportParser(0, &keyboardReportParser);
    HidComposite.SetReportParser(1, &mouseReportParser);
    HidKeyboard.SetReportParser(0, &keyboardReportParser);
    HidMouse.SetReportParser(0, &mouseReportParser);
    Mouse.begin();
    Keyboard.begin();
}

void loop() {
    Usb.Task();
}
