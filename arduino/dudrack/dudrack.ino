#define USBCON

#include <hidboot.h>
#include <hiduniversal.h>
#include <usbhub.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <Joystick.h>

#define OUTPUT_TO_SERIAL 0

#define NUM_REPORT_KEYS 6

#define MODIFIER_LEFT_CTRL 0x1
#define MODIFIER_LEFT_SHIFT 0x2
#define MODIFIER_LEFT_ALT 0x4
#define MODIFIER_RIGHT_SHIFT 0x20
#define MODIFIER_RIGHT_ALT 0x40

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
#define KEY_SPACE 0x2C
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

#define KEY_PAGE_UP 0x4B
#define KEY_PAGE_DOWN 0x4E

#define KEY_COUNT 0xE8

#define MOUSE_MOVE_COEFFICIENT 3.0
#define MOUSE_SCROLL_COEFFICIENT 1.0

#define MODE_NEUTRAL 0
#define MODE_DUDRACK 1
#define MODE_STG 2
#define MODE_COUNT 3

#define MAPPING_NEUTRAL 0
#define MAPPING_HENKAN 1
#define MAPPING_COUNT 2

#define STICK_NEUTRAL 0
#define STICK_DOWN 1
#define STICK_UP -1
#define STICK_LEFT -1
#define STICK_RIGHT 1

#define STICK_DOWN_KEY KEY_A
#define STICK_UP_KEY KEY_S
#define STICK_LEFT_KEY KEY_AT
#define STICK_RIGHT_KEY KEY_BRACKET_OPEN
#define BUTTON_0_MODIFIER bmRightAlt
#define BUTTON_1_KEY KEY_ENTER
#define BUTTON_2_MODIFIER bmLeftAlt
#define BUTTON_3_KEY KEY_TAB
#define BUTTON_4_MODIFIER bmLeftCtrl
#define BUTTON_5_KEY KEY_SPACE

USB    Usb;
USBHub Hub(&Usb);

HIDBoot<USB_HID_PROTOCOL_KEYBOARD | USB_HID_PROTOCOL_MOUSE> HidComposite(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>                          HidKeyboard(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE>                             HidMouse(&Usb);

Joystick_ Joystick;

uint8_t keyMappings[MAPPING_COUNT][KEY_COUNT];
void initKeyMappings() {
    for (int mapping = 0; mapping < MAPPING_COUNT; ++mapping) {
        for (int key = 0; key < KEY_COUNT; ++key) {
            keyMappings[mapping][key] = key;
        }
    }

    keyMappings[MAPPING_NEUTRAL][KEY_Q        ] = KEY_COLON;
    keyMappings[MAPPING_NEUTRAL][KEY_W        ] = KEY_COMMA;
    keyMappings[MAPPING_NEUTRAL][KEY_E        ] = KEY_PERIOD;
    keyMappings[MAPPING_NEUTRAL][KEY_R        ] = KEY_P;
    keyMappings[MAPPING_NEUTRAL][KEY_T        ] = KEY_Y;
    keyMappings[MAPPING_NEUTRAL][KEY_Y        ] = KEY_F;
    keyMappings[MAPPING_NEUTRAL][KEY_U        ] = KEY_G;
    keyMappings[MAPPING_NEUTRAL][KEY_I        ] = KEY_C;
    keyMappings[MAPPING_NEUTRAL][KEY_O        ] = KEY_R;
    keyMappings[MAPPING_NEUTRAL][KEY_P        ] = KEY_L;
    keyMappings[MAPPING_NEUTRAL][KEY_AT       ] = KEY_SLASH;
    keyMappings[MAPPING_NEUTRAL][KEY_S        ] = KEY_O;
    keyMappings[MAPPING_NEUTRAL][KEY_D        ] = KEY_E;
    keyMappings[MAPPING_NEUTRAL][KEY_F        ] = KEY_U;
    keyMappings[MAPPING_NEUTRAL][KEY_G        ] = KEY_I;
    keyMappings[MAPPING_NEUTRAL][KEY_H        ] = KEY_D;
    keyMappings[MAPPING_NEUTRAL][KEY_J        ] = KEY_H;
    keyMappings[MAPPING_NEUTRAL][KEY_K        ] = KEY_T;
    keyMappings[MAPPING_NEUTRAL][KEY_L        ] = KEY_N;
    keyMappings[MAPPING_NEUTRAL][KEY_SEMICOLON] = KEY_S;
    keyMappings[MAPPING_NEUTRAL][KEY_COLON    ] = KEY_MINUS;
    keyMappings[MAPPING_NEUTRAL][KEY_Z        ] = KEY_SEMICOLON;
    keyMappings[MAPPING_NEUTRAL][KEY_X        ] = KEY_Q;
    keyMappings[MAPPING_NEUTRAL][KEY_C        ] = KEY_J;
    keyMappings[MAPPING_NEUTRAL][KEY_V        ] = KEY_K;
    keyMappings[MAPPING_NEUTRAL][KEY_B        ] = KEY_X;
    keyMappings[MAPPING_NEUTRAL][KEY_N        ] = KEY_B;
    keyMappings[MAPPING_NEUTRAL][KEY_M        ] = KEY_M;
    keyMappings[MAPPING_NEUTRAL][KEY_COMMA    ] = KEY_W;
    keyMappings[MAPPING_NEUTRAL][KEY_PERIOD   ] = KEY_V;
    keyMappings[MAPPING_NEUTRAL][KEY_SLASH    ] = KEY_Z;

    keyMappings[MAPPING_HENKAN][KEY_Q        ] = KEY_1;
    keyMappings[MAPPING_HENKAN][KEY_W        ] = KEY_2;
    keyMappings[MAPPING_HENKAN][KEY_E        ] = KEY_3;
    keyMappings[MAPPING_HENKAN][KEY_R        ] = KEY_4;
    keyMappings[MAPPING_HENKAN][KEY_T        ] = KEY_5;
    keyMappings[MAPPING_HENKAN][KEY_Y        ] = KEY_6;
    keyMappings[MAPPING_HENKAN][KEY_U        ] = KEY_7;
    keyMappings[MAPPING_HENKAN][KEY_I        ] = KEY_8;
    keyMappings[MAPPING_HENKAN][KEY_O        ] = KEY_9;
    keyMappings[MAPPING_HENKAN][KEY_P        ] = KEY_0;
    keyMappings[MAPPING_HENKAN][KEY_AT       ] = KEY_SLASH;
    keyMappings[MAPPING_HENKAN][KEY_A        ] = KEY_TAB;
    keyMappings[MAPPING_HENKAN][KEY_S        ] = KEY_ESCAPE;
    keyMappings[MAPPING_HENKAN][KEY_D        ] = KEY_ENTER;
    keyMappings[MAPPING_HENKAN][KEY_F        ] = KEY_BACKSPACE;
    keyMappings[MAPPING_HENKAN][KEY_G        ] = KEY_DELETE;
    keyMappings[MAPPING_HENKAN][KEY_H        ] = KEY_AT;
    keyMappings[MAPPING_HENKAN][KEY_J        ] = KEY_BACKSLASH;
    keyMappings[MAPPING_HENKAN][KEY_K        ] = KEY_BRACKET_OPEN;
    keyMappings[MAPPING_HENKAN][KEY_L        ] = KEY_BRACKET_CLOSE;
    keyMappings[MAPPING_HENKAN][KEY_SEMICOLON] = KEY_YEN;
    keyMappings[MAPPING_HENKAN][KEY_COLON    ] = KEY_MINUS;
    keyMappings[MAPPING_HENKAN][KEY_Z        ] = KEY_LEFT;
    keyMappings[MAPPING_HENKAN][KEY_X        ] = KEY_DOWN;
    keyMappings[MAPPING_HENKAN][KEY_C        ] = KEY_UP;
    keyMappings[MAPPING_HENKAN][KEY_V        ] = KEY_RIGHT;
    keyMappings[MAPPING_HENKAN][KEY_B        ] = KEY_X;
    keyMappings[MAPPING_HENKAN][KEY_N        ] = KEY_HAN_ZEN;
    keyMappings[MAPPING_HENKAN][KEY_M        ] = KEY_MUHENKAN;
    keyMappings[MAPPING_HENKAN][KEY_COMMA    ] = KEY_HOME;
    keyMappings[MAPPING_HENKAN][KEY_PERIOD   ] = KEY_END;
    keyMappings[MAPPING_HENKAN][KEY_SLASH    ] = KEY_HAT;
}

int currentMode = MODE_DUDRACK;

KeyReport keyReport = {};
bool henkanPressed = false;
bool muhenkanPressed = false;
bool spacePressed = false;
bool noEventBetweenSpaceEvents = false;

bool downPressed = false;
bool upPressed = false;
bool leftPressed = false;
bool rightPressed = false;

bool removeFromKeyReport(KeyReport *report, uint8_t key){
    int i;

    for (i = 0; i < NUM_REPORT_KEYS; ++i) {
        if (report->keys[i] == key) {
            break;
        }
    }

    if (i == NUM_REPORT_KEYS) {
        return false;
    }

    for (; i < NUM_REPORT_KEYS - 1; ++i) {
        report->keys[i] = report->keys[i + 1];
    }

    report->keys[NUM_REPORT_KEYS - 1] = 0;

    return true;
}

void pressKey(uint8_t key) {
    for (int i = 0; i < NUM_REPORT_KEYS; ++i) {
        if (keyReport.keys[i] == 0) {
            keyReport.keys[i] = key;

            Keyboard.sendReport(&keyReport);

            return;
        }

        if (keyReport.keys[i] == key) {
            return;
        }
    }
}

void releaseKey(uint8_t key) {
    if (removeFromKeyReport(&keyReport, key)) {
        Keyboard.sendReport(&keyReport);
    }
}

void releaseAllKeys() {
    henkanPressed = false;
    muhenkanPressed = false;
    spacePressed = false;

    downPressed = false;
    upPressed = false;
    leftPressed = false;
    rightPressed = false;

    keyReport.modifiers = 0;

    for (int i = 0; i < NUM_REPORT_KEYS; i++) {
        keyReport.keys[i] = 0;
    }

    Keyboard.sendReport(&keyReport);
}

void pressModifier(uint8_t modifier) {
    keyReport.modifiers |= modifier;
    Keyboard.sendReport(&keyReport);
}

void releaseModifier(uint8_t modifier) {
    keyReport.modifiers &= ~modifier;
    Keyboard.sendReport(&keyReport);
}

void changeModifiers(uint8_t modifiers) {
    keyReport.modifiers = modifiers | (spacePressed ? MODIFIER_LEFT_SHIFT : 0);
    Keyboard.sendReport(&keyReport);
}

bool handleModeSwitch(uint8_t modifiers, uint8_t key) {
    if (modifiers & MODIFIER_RIGHT_SHIFT) {
        switch (key) {
            case KEY_1:
                currentMode = MODE_NEUTRAL;

                return true;

            case KEY_2:
                currentMode = MODE_DUDRACK;

                return true;

            case KEY_3:
                currentMode = MODE_STG;

                return true;
        }
    }

    return false;
}

void dudrackKeyDown(uint8_t key) {
    switch (key) {
        case KEY_HENKAN:
            henkanPressed = true;

            return;

        case KEY_MUHENKAN:
            muhenkanPressed = true;

            return;

        case KEY_SPACE:
            pressModifier(MODIFIER_LEFT_SHIFT);
            spacePressed = true;
            noEventBetweenSpaceEvents = true;

            return;

        default:
            noEventBetweenSpaceEvents = false;

            if (muhenkanPressed) {
                pressModifier(MODIFIER_LEFT_SHIFT);
            }

            if (henkanPressed || muhenkanPressed) {
                pressKey(keyMappings[MAPPING_HENKAN][key]);
            } else {
                pressKey(keyMappings[MAPPING_NEUTRAL][key]);
            }
    }
}

void dudrackKeyUp(uint8_t key) {
    switch (key) {
        case KEY_HENKAN:
            henkanPressed = false;

            return;

        case KEY_MUHENKAN:
            muhenkanPressed = false;
            releaseModifier(MODIFIER_LEFT_SHIFT);

            return;

        case KEY_SPACE:
            spacePressed = false;
            releaseModifier(MODIFIER_LEFT_SHIFT);
            if (noEventBetweenSpaceEvents) {
                pressKey(KEY_SPACE);
                releaseKey(KEY_SPACE);
            }

            return;

        default:
            releaseKey(keyMappings[MAPPING_HENKAN][key]);
            releaseKey(keyMappings[MAPPING_NEUTRAL][key]);

            if (muhenkanPressed) {
                releaseModifier(MODIFIER_LEFT_SHIFT);
            }
    }
}

void dudrackModifiersChange(uint8_t modifiers) {
    changeModifiers(modifiers);
}

void stgKeyDown(uint8_t key) {
    switch (key) {
        case STICK_DOWN_KEY:
            downPressed = true;
            Joystick.setYAxis(STICK_DOWN);

            return;

        case STICK_UP_KEY:
            upPressed = true;
            Joystick.setYAxis(STICK_UP);

            return;

        case STICK_LEFT_KEY:
            leftPressed = true;
            Joystick.setXAxis(STICK_LEFT);

            return;

        case STICK_RIGHT_KEY:
            rightPressed = true;
            Joystick.setXAxis(STICK_RIGHT);

            return;

        case BUTTON_1_KEY:
            Joystick.pressButton(1);

            return;

        case BUTTON_3_KEY:
            Joystick.pressButton(3);

            return;

        case BUTTON_5_KEY:
            Joystick.pressButton(5);

            return;
    }
}

void stgKeyUp(uint8_t key) {
    switch (key) {
        case STICK_DOWN_KEY:
            downPressed = false;
            Joystick.setYAxis(upPressed ? STICK_UP : STICK_NEUTRAL);

            return;

        case STICK_UP_KEY:
            upPressed = false;
            Joystick.setYAxis(downPressed ? STICK_DOWN : STICK_NEUTRAL);

            return;

        case STICK_LEFT_KEY:
            leftPressed = false;
            Joystick.setXAxis(rightPressed ? STICK_RIGHT : STICK_NEUTRAL);

            return;

        case STICK_RIGHT_KEY:
            rightPressed = false;
            Joystick.setXAxis(leftPressed ? STICK_LEFT : STICK_NEUTRAL);

            return;

        case BUTTON_1_KEY:
            Joystick.releaseButton(1);

            return;

        case BUTTON_3_KEY:
            Joystick.releaseButton(3);

            return;

        case BUTTON_5_KEY:
            Joystick.releaseButton(5);

            return;
    }
}

void stgModifiersChange(uint8_t modifiers) {
    MODIFIERKEYS mod = *((MODIFIERKEYS*)&modifiers);

    if (mod.BUTTON_0_MODIFIER) {
        Joystick.pressButton(0);
    } else {
        Joystick.releaseButton(0);
    }

    if (mod.BUTTON_2_MODIFIER) {
        Joystick.pressButton(2);
    } else {
        Joystick.releaseButton(2);
    }

    if (mod.BUTTON_4_MODIFIER) {
        Joystick.pressButton(4);
    } else {
        Joystick.releaseButton(4);
    }
}

class : public KeyboardReportParser {
public:
    virtual void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
        KeyReport *report = (KeyReport*)(buf);

#if OUTPUT_TO_SERIAL
        Serial.print("src modifiers:  ");
        Serial.println(report->modifiers, HEX);
        Serial.print("src keys:  ");
        for (int i = 0; i < NUM_REPORT_KEYS; i++) {
            Serial.print(report->keys[i], HEX);
            Serial.print(" ");
        }
        Serial.println("");
#endif

        if (removeFromKeyReport(report, KEY_CAPS_LOCK)) {
            report->modifiers |= MODIFIER_LEFT_CTRL;
        }

#if OUTPUT_TO_SERIAL
        Serial.print("dest modifiers:  ");
        Serial.println(report->modifiers, HEX);
        Serial.print("dest keys:  ");
        for (int i = 0; i < NUM_REPORT_KEYS; i++) {
            Serial.print(report->keys[i], HEX);
            Serial.print(" ");
        }
        Serial.println("");
#endif

        KeyboardReportParser::Parse(hid, is_rpt_id, len, (uint8_t*)report);

#if OUTPUT_TO_SERIAL
        Serial.println("");
#endif
    }

    virtual void OnKeyDown(uint8_t modifiers, uint8_t key) {
#if OUTPUT_TO_SERIAL
        Serial.print("key down:  ");
        Serial.println(key, HEX);
#endif

        if (handleModeSwitch(modifiers, key)) {
            releaseAllKeys();

            return;
        }

        switch (currentMode) {
            case MODE_NEUTRAL:
                pressKey(key);
                return;

            case MODE_DUDRACK:
                dudrackKeyDown(key);
                return;

            case MODE_STG:
                stgKeyDown(key);
                return;
        }
    }

    virtual void OnKeyUp(uint8_t modifiers, uint8_t key) {
#if OUTPUT_TO_SERIAL
        Serial.print("key up:  ");
        Serial.println(key, HEX);
#endif

        switch (currentMode) {
            case MODE_NEUTRAL:
                releaseKey(key);
                return;

            case MODE_DUDRACK:
                dudrackKeyUp(key);
                return;

            case MODE_STG:
                stgKeyUp(key);
                return;
        }
    }

    virtual void OnControlKeysChanged(uint8_t before, uint8_t after) {
#if OUTPUT_TO_SERIAL
        Serial.print("modifiers change: ");
        Serial.print(before, HEX);
        Serial.print(" -> ");
        Serial.println(after, HEX);
#endif

        switch (currentMode) {
            case MODE_NEUTRAL:
                changeModifiers(after);
                return;

            case MODE_DUDRACK:
                dudrackModifiersChange(after);
                return;

            case MODE_STG:
                stgModifiersChange(after);
                return;
        }
    }
} keyboardReportParser;

class : public MouseReportParser {
protected:
    virtual void OnMouseMove(MOUSEINFO *mouseInfo)        {
        if (muhenkanPressed || henkanPressed) {
#if OUTPUT_TO_SERIAL
            Serial.print("mouse scroll: ");
            Serial.println(min(1, max(-1, (int8_t)(-mouseInfo->dY * MOUSE_SCROLL_COEFFICIENT))));
#endif
            Mouse.move(0, 0, min(1, max(-1, (int8_t)(-mouseInfo->dY * MOUSE_SCROLL_COEFFICIENT))));
        } else {
#if OUTPUT_TO_SERIAL
            Serial.print("mouse move: ");
            Serial.print((int8_t)(mouseInfo->dX * MOUSE_MOVE_COEFFICIENT));
            Serial.print(" ");
            Serial.println((int8_t)(mouseInfo->dY * MOUSE_MOVE_COEFFICIENT));
#endif
            Mouse.move(
                (int8_t)(mouseInfo->dX * MOUSE_MOVE_COEFFICIENT),
                (int8_t)(mouseInfo->dY * MOUSE_MOVE_COEFFICIENT),
                0
            );
        }
    }

    virtual void OnLeftButtonDown(MOUSEINFO *mouseInfo)   { Mouse.press(MOUSE_LEFT); }
    virtual void OnLeftButtonUp(MOUSEINFO *mouseInfo)     { Mouse.release(MOUSE_LEFT); }
    virtual void OnRightButtonDown(MOUSEINFO *mouseInfo)  { Mouse.press(MOUSE_RIGHT); }
    virtual void OnRightButtonUp(MOUSEINFO *mouseInfo)    { Mouse.release(MOUSE_RIGHT); }
} mouseReportParser;


void setup() {
    int usbStatus = Usb.Init();

#if OUTPUT_TO_SERIAL
    Serial.begin(115200);
    while (!Serial);
    if (usbStatus == -1) {
        Serial.println("OSC did not start.");
    }
    Serial.println("Started");
#endif

    initKeyMappings();

    HidComposite.SetReportParser(0, &keyboardReportParser);
    HidComposite.SetReportParser(1, &mouseReportParser);
    HidKeyboard.SetReportParser(0, &keyboardReportParser);
    HidMouse.SetReportParser(0, &mouseReportParser);

    Joystick.setXAxisRange(STICK_UP, STICK_DOWN);
    Joystick.setYAxisRange(STICK_LEFT, STICK_RIGHT);

    Mouse.begin();
    Keyboard.begin();
    Joystick.begin();
}

void loop() {
    Usb.Task();
}
