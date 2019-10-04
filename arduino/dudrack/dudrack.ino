#include <hidboot.h>
#include <hiduniversal.h>
#include <usbhub.h>

#include <HID-Project.h>

#define OUTPUT_TO_SERIAL 0

#define NUM_REPORT_KEYS 6

#define KEY_COLON         KEY_QUOTE
#define KEY_AT            KEY_LEFT_BRACE
#define KEY_LEFT_BRACKET  KEY_RIGHT_BRACE
#define KEY_RIGHT_BRACKET KEY_NON_US_NUM
#define KEY_HAN_ZEN       KEY_TILDE
#define KEY_HAT           KEY_EQUAL
#define KEY_JP_BACKSLASH  KEY_INTERNATIONAL1
#define KEY_HIRAGANA      KEY_INTERNATIONAL2
#define KEY_YEN           KEY_INTERNATIONAL3
#define KEY_HENKAN        KEY_INTERNATIONAL4
#define KEY_MUHENKAN      KEY_INTERNATIONAL5

#define KEY_COUNT 0xE8

#define MOUSE_MOVE_COEFFICIENT 4.0
#define MOUSE_SCROLL_DENOMINATOR 4

#define MODE_NEUTRAL 0
#define MODE_DUDRACK 1
#define MODE_STG 2
#define MODE_COUNT 3

#define MAPPING_NEUTRAL 0
#define MAPPING_HENKAN 1
#define MAPPING_COUNT 2

#define STICK_NEUTRAL 0
#define STICK_DOWN 0x7FFF
#define STICK_UP -0x7FFF
#define STICK_LEFT -0x7FFF
#define STICK_RIGHT 0x7FFF

#define STICK_DOWN_KEY  KEY_Z
#define STICK_UP_KEY    KEY_X
#define STICK_LEFT_KEY  KEY_AT
#define STICK_RIGHT_KEY KEY_LEFT_BRACKET
#define BUTTON_1_KEY    KEY_ENTER
#define BUTTON_2_KEY    KEY_RIGHT_ALT
#define BUTTON_3_KEY    KEY_PRINTSCREEN
#define BUTTON_4_KEY    KEY_LEFT_SHIFT
#define BUTTON_5_KEY    KEY_CAPS_LOCK
#define BUTTON_6_KEY    KEY_A
#define BUTTON_7_KEY    KEY_SPACE

USB    Usb;
USBHub Hub(&Usb);

HIDBoot<USB_HID_PROTOCOL_KEYBOARD | USB_HID_PROTOCOL_MOUSE> HidComposite(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>                          HidKeyboard(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE>                             HidMouse(&Usb);

KeyboardKeycode keyMappings[MAPPING_COUNT][KEY_COUNT];
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
    keyMappings[MAPPING_NEUTRAL][KEY_CAPS_LOCK] = KEY_LEFT_CTRL;

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
    keyMappings[MAPPING_HENKAN][KEY_S        ] = KEY_ESC;
    keyMappings[MAPPING_HENKAN][KEY_D        ] = KEY_ENTER;
    keyMappings[MAPPING_HENKAN][KEY_F        ] = KEY_BACKSPACE;
    keyMappings[MAPPING_HENKAN][KEY_G        ] = KEY_DELETE;
    keyMappings[MAPPING_HENKAN][KEY_H        ] = KEY_AT;
    keyMappings[MAPPING_HENKAN][KEY_J        ] = KEY_JP_BACKSLASH;
    keyMappings[MAPPING_HENKAN][KEY_K        ] = KEY_LEFT_BRACKET;
    keyMappings[MAPPING_HENKAN][KEY_L        ] = KEY_RIGHT_BRACKET;
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
    keyMappings[MAPPING_HENKAN][KEY_CAPS_LOCK] = KEY_LEFT_CTRL;
}

struct {
    int currentMode = MODE_DUDRACK;

    bool henkanPressed = false;
    bool muhenkanPressed = false;
    bool spacePressed = false;
    bool leftShiftPressed = false;
    bool noEventBetweenSpaceEvents = false;
} keyboardState;

struct {
    int scrollRemainder = 0;

    bool rightPressed = false;
    bool noMouseMoveBetweenRightEvents = false;
} mouseState;

struct {
    bool downPressed = false;
    bool upPressed = false;
    bool leftPressed = false;
    bool rightPressed = false;
} gamepadState;

void releaseLeftShift() {
    if (!keyboardState.muhenkanPressed
        && !keyboardState.spacePressed
        && !keyboardState.leftShiftPressed) {
        Keyboard.release(KEY_LEFT_SHIFT);
    }
}

void releaseAllKeys() {
    keyboardState.henkanPressed = false;
    keyboardState.muhenkanPressed = false;
    keyboardState.spacePressed = false;

    gamepadState.downPressed = false;
    gamepadState.upPressed = false;
    gamepadState.leftPressed = false;
    gamepadState.rightPressed = false;

    Keyboard.releaseAll();
    Gamepad.releaseAll();
}

bool handleModeSwitch(uint8_t mod, KeyboardKeycode key) {
    MODIFIERKEYS modifiers = *((MODIFIERKEYS*)&mod);

    if (modifiers.bmRightShift) {
        switch (key) {
            case KEY_1:
                keyboardState.currentMode = MODE_NEUTRAL;

                return true;

            case KEY_2:
                keyboardState.currentMode = MODE_DUDRACK;

                return true;

            case KEY_3:
                keyboardState.currentMode = MODE_STG;

                return true;
        }
    }

    return false;
}

void dudrackKeyDown(KeyboardKeycode key) {
    switch (key) {
        case KEY_LEFT_SHIFT:
            keyboardState.leftShiftPressed = true;
            keyboardState.noEventBetweenSpaceEvents = false;
            Keyboard.press(KEY_LEFT_SHIFT);

            return;

        case KEY_HENKAN:
            keyboardState.henkanPressed = true;

            return;

        case KEY_MUHENKAN:
            Keyboard.press(KEY_LEFT_SHIFT);
            keyboardState.muhenkanPressed = true;

            return;

        case KEY_SPACE:
            Keyboard.press(KEY_LEFT_SHIFT);
            keyboardState.spacePressed = true;
            keyboardState.noEventBetweenSpaceEvents = true;

            return;

        default:
            keyboardState.noEventBetweenSpaceEvents = false;

            if (keyboardState.henkanPressed || keyboardState.muhenkanPressed) {
                Keyboard.press(keyMappings[MAPPING_HENKAN][key]);
            } else {
                Keyboard.press(keyMappings[MAPPING_NEUTRAL][key]);
            }
    }
}

void dudrackKeyUp(KeyboardKeycode key) {
    switch (key) {
        case KEY_LEFT_SHIFT:
            keyboardState.leftShiftPressed = false;
            releaseLeftShift();

            return;

        case KEY_HENKAN:
            keyboardState.henkanPressed = false;

            return;

        case KEY_MUHENKAN:
            keyboardState.muhenkanPressed = false;
            releaseLeftShift();

            return;

        case KEY_SPACE:
            keyboardState.spacePressed = false;
            releaseLeftShift();
            if (keyboardState.noEventBetweenSpaceEvents) {
                Keyboard.press(KEY_SPACE);
                Keyboard.release(KEY_SPACE);
            }

            return;

        default:
            Keyboard.release(keyMappings[MAPPING_HENKAN][key]);
            Keyboard.release(keyMappings[MAPPING_NEUTRAL][key]);
    }
}

void stgKeyDown(KeyboardKeycode key) {
    switch (key) {
        case STICK_DOWN_KEY:
            gamepadState.downPressed = true;
            Gamepad.yAxis(STICK_DOWN);

            break;

        case STICK_UP_KEY:
            gamepadState.upPressed = true;
            Gamepad.yAxis(STICK_UP);

            break;

        case STICK_LEFT_KEY:
            gamepadState.leftPressed = true;
            Gamepad.xAxis(STICK_LEFT);

            break;

        case STICK_RIGHT_KEY:
            gamepadState.rightPressed = true;
            Gamepad.xAxis(STICK_RIGHT);

            break;

        case BUTTON_1_KEY:
            Gamepad.press(1);

            break;

        case BUTTON_2_KEY:
            Gamepad.press(2);

            break;

        case BUTTON_3_KEY:
            Gamepad.press(3);

            break;

        case BUTTON_4_KEY:
            Gamepad.press(4);

            break;

        case BUTTON_5_KEY:
            Gamepad.press(5);

            break;

        case BUTTON_6_KEY:
            Gamepad.press(6);

            break;

        case BUTTON_7_KEY:
            Gamepad.press(7);

            break;
    }

    Gamepad.write();
}

void stgKeyUp(KeyboardKeycode key) {
    switch (key) {
        case STICK_DOWN_KEY:
            gamepadState.downPressed = false;
            Gamepad.yAxis(gamepadState.upPressed ? STICK_UP : STICK_NEUTRAL);

            break;

        case STICK_UP_KEY:
            gamepadState.upPressed = false;
            Gamepad.yAxis(gamepadState.downPressed ? STICK_DOWN : STICK_NEUTRAL);

            break;

        case STICK_LEFT_KEY:
            gamepadState.leftPressed = false;
            Gamepad.xAxis(gamepadState.rightPressed ? STICK_RIGHT : STICK_NEUTRAL);

            break;

        case STICK_RIGHT_KEY:
            gamepadState.rightPressed = false;
            Gamepad.xAxis(gamepadState.leftPressed ? STICK_LEFT : STICK_NEUTRAL);

            break;

        case BUTTON_1_KEY:
            Gamepad.release(1);

            break;

        case BUTTON_2_KEY:
            Gamepad.release(2);

            break;

        case BUTTON_3_KEY:
            Gamepad.release(3);

            break;

        case BUTTON_4_KEY:
            Gamepad.release(4);

            break;

        case BUTTON_5_KEY:
            Gamepad.release(5);

            break;

        case BUTTON_6_KEY:
            Gamepad.release(6);

            break;

        case BUTTON_7_KEY:
            Gamepad.release(7);

            break;
    }

    Gamepad.write();
}

class : public KeyboardReportParser {
public:
    virtual void OnKeyDown(uint8_t modifiers, uint8_t key) {
        if (handleModeSwitch(modifiers, key)) {
            releaseAllKeys();

            return;
        }

        keyDown(key);
    }

    virtual void OnKeyUp(uint8_t modifiers, uint8_t key) {
        keyUp(key);
    }

    virtual void OnControlKeysChanged(uint8_t before, uint8_t after) {
#if OUTPUT_TO_SERIAL
        Serial.print("modifiers change: ");
        Serial.print(before, HEX);
        Serial.print(" -> ");
        Serial.println(after, HEX);
#endif

        MODIFIERKEYS beforeModifiers = *((MODIFIERKEYS*)&before);
        MODIFIERKEYS afterModifiers = *((MODIFIERKEYS*)&after);

        if (!beforeModifiers.bmLeftCtrl && afterModifiers.bmLeftCtrl) {
            keyDown(KEY_LEFT_CTRL);
        } else if (beforeModifiers.bmLeftCtrl && !afterModifiers.bmLeftCtrl) {
            keyUp(KEY_LEFT_CTRL);
        }

        if (!beforeModifiers.bmLeftShift && afterModifiers.bmLeftShift) {
            keyDown(KEY_LEFT_SHIFT);
        } else if (beforeModifiers.bmLeftShift && !afterModifiers.bmLeftShift) {
            keyUp(KEY_LEFT_SHIFT);
        }

        if (!beforeModifiers.bmLeftAlt && afterModifiers.bmLeftAlt) {
            keyDown(KEY_LEFT_ALT);
        } else if (beforeModifiers.bmLeftAlt && !afterModifiers.bmLeftAlt) {
            keyUp(KEY_LEFT_ALT);
        }

        if (!beforeModifiers.bmLeftGUI && afterModifiers.bmLeftGUI) {
            keyDown(KEY_LEFT_GUI);
        } else if (beforeModifiers.bmLeftGUI && !afterModifiers.bmLeftGUI) {
            keyUp(KEY_LEFT_GUI);
        }

        if (!beforeModifiers.bmRightCtrl && afterModifiers.bmRightCtrl) {
            keyDown(KEY_RIGHT_CTRL);
        } else if (beforeModifiers.bmRightCtrl && !afterModifiers.bmRightCtrl) {
            keyUp(KEY_RIGHT_CTRL);
        }

        if (!beforeModifiers.bmRightShift && afterModifiers.bmRightShift) {
            keyDown(KEY_RIGHT_SHIFT);
        } else if (beforeModifiers.bmRightShift && !afterModifiers.bmRightShift) {
            keyUp(KEY_RIGHT_SHIFT);
        }

        if (!beforeModifiers.bmRightAlt && afterModifiers.bmRightAlt) {
            keyDown(KEY_RIGHT_ALT);
        } else if (beforeModifiers.bmRightAlt && !afterModifiers.bmRightAlt) {
            keyUp(KEY_RIGHT_ALT);
        }

        if (!beforeModifiers.bmRightGUI && afterModifiers.bmRightGUI) {
            keyDown(KEY_RIGHT_GUI);
        } else if (beforeModifiers.bmRightGUI && !afterModifiers.bmRightGUI) {
            keyUp(KEY_RIGHT_GUI);
        }
    }

private:
    void keyDown(KeyboardKeycode key) {
#if OUTPUT_TO_SERIAL
        Serial.print("mode:  ");
        Serial.print(keyboardState.currentMode, HEX);
        Serial.print(" key down:  ");
        Serial.println(key, HEX);
#endif

        switch (keyboardState.currentMode) {
            case MODE_NEUTRAL:
                Keyboard.press(key);
                return;

            case MODE_DUDRACK:
                dudrackKeyDown(key);
                return;

            case MODE_STG:
                stgKeyDown(key);
                return;
        }
    }

    void keyUp(KeyboardKeycode key) {
#if OUTPUT_TO_SERIAL
        Serial.print("mode:  ");
        Serial.print(keyboardState.currentMode, HEX);
        Serial.print(" key up:  ");
        Serial.println(key, HEX);
#endif

        switch (keyboardState.currentMode) {
            case MODE_NEUTRAL:
                Keyboard.release(key);
                return;

            case MODE_DUDRACK:
                dudrackKeyUp(key);
                return;

            case MODE_STG:
                stgKeyUp(key);
                return;
        }
    }
} keyboardReportParser;

class : public MouseReportParser {
protected:
    virtual void OnMouseMove(MOUSEINFO *mouseInfo)        {
        if (mouseState.rightPressed) {
            mouseState.noMouseMoveBetweenRightEvents = false;

            int dy = -mouseInfo->dY + mouseState.scrollRemainder;
            int adjustedDy = dy / MOUSE_SCROLL_DENOMINATOR;
            mouseState.scrollRemainder = dy % MOUSE_SCROLL_DENOMINATOR;

#if OUTPUT_TO_SERIAL
            Serial.print("mouse scroll: ");
            Serial.println(adjustedDy);
            Serial.print("mouse scroll remainder: ");
            Serial.println(mouseState.scrollRemainder);
#endif
            Mouse.move(0, 0, adjustedDy);
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

    virtual void OnRightButtonDown(MOUSEINFO *mouseInfo)  {
        mouseState.rightPressed = true;
        mouseState.noMouseMoveBetweenRightEvents = true;
    }

    virtual void OnRightButtonUp(MOUSEINFO *mouseInfo)    {
        mouseState.rightPressed = false;

        if (mouseState.noMouseMoveBetweenRightEvents) {
            Mouse.press(MOUSE_RIGHT);
            Mouse.release(MOUSE_RIGHT);
        }
    }
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

    Mouse.begin();
    Keyboard.begin();
    Gamepad.begin();
}

void loop() {
    Usb.Task();
}
