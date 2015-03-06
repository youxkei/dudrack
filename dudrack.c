#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int neutralTable[KEY_CNT];
void initNeutralTable(){
    int i;
    for (i = 0; i < KEY_CNT; ++i){
        neutralTable[i] = i;
    }

    neutralTable[KEY_Q         ] = KEY_APOSTROPHE;
    neutralTable[KEY_W         ] = KEY_COMMA;
    neutralTable[KEY_E         ] = KEY_DOT;
    neutralTable[KEY_R         ] = KEY_P;
    neutralTable[KEY_T         ] = KEY_Y;
    neutralTable[KEY_Y         ] = KEY_F;
    neutralTable[KEY_U         ] = KEY_G;
    neutralTable[KEY_I         ] = KEY_C;
    neutralTable[KEY_O         ] = KEY_R;
    neutralTable[KEY_P         ] = KEY_L;
    neutralTable[KEY_LEFTBRACE ] = KEY_SLASH;
    neutralTable[KEY_S         ] = KEY_O;
    neutralTable[KEY_D         ] = KEY_E;
    neutralTable[KEY_F         ] = KEY_U;
    neutralTable[KEY_G         ] = KEY_I;
    neutralTable[KEY_H         ] = KEY_D;
    neutralTable[KEY_J         ] = KEY_H;
    neutralTable[KEY_K         ] = KEY_T;
    neutralTable[KEY_L         ] = KEY_N;
    neutralTable[KEY_SEMICOLON ] = KEY_S;
    neutralTable[KEY_APOSTROPHE] = KEY_MINUS;
    neutralTable[KEY_Z         ] = KEY_SEMICOLON;
    neutralTable[KEY_X         ] = KEY_Q;
    neutralTable[KEY_C         ] = KEY_J;
    neutralTable[KEY_V         ] = KEY_K;
    neutralTable[KEY_B         ] = KEY_X;
    neutralTable[KEY_N         ] = KEY_B;
    neutralTable[KEY_M         ] = KEY_M;
    neutralTable[KEY_COMMA     ] = KEY_W;
    neutralTable[KEY_DOT       ] = KEY_V;
    neutralTable[KEY_SLASH     ] = KEY_Z;

    neutralTable[KEY_CAPSLOCK] = KEY_LEFTCTRL;
    neutralTable[KEY_MUHENKAN] = KEY_LEFTSHIFT;
}

int henkanTable[KEY_CNT];
void initHenkanTable(){
    int i;
    for (i = 0; i < KEY_CNT; ++i){
        henkanTable[i] = i;
    }

    henkanTable[KEY_Q         ] = KEY_1;
    henkanTable[KEY_W         ] = KEY_2;
    henkanTable[KEY_E         ] = KEY_3;
    henkanTable[KEY_R         ] = KEY_4;
    henkanTable[KEY_T         ] = KEY_5;
    henkanTable[KEY_Y         ] = KEY_6;
    henkanTable[KEY_U         ] = KEY_7;
    henkanTable[KEY_I         ] = KEY_8;
    henkanTable[KEY_O         ] = KEY_9;
    henkanTable[KEY_P         ] = KEY_0;
    henkanTable[KEY_LEFTBRACE ] = KEY_SLASH;
    henkanTable[KEY_A         ] = KEY_TAB;
    henkanTable[KEY_S         ] = KEY_ESC;
    henkanTable[KEY_D         ] = KEY_ENTER;
    henkanTable[KEY_F         ] = KEY_BACKSPACE;
    henkanTable[KEY_G         ] = KEY_DELETE;
    henkanTable[KEY_H         ] = KEY_LEFTBRACE;
    henkanTable[KEY_J         ] = KEY_RO;
    henkanTable[KEY_K         ] = KEY_RIGHTBRACE;
    henkanTable[KEY_L         ] = KEY_BACKSLASH;
    henkanTable[KEY_SEMICOLON ] = KEY_YEN;
    henkanTable[KEY_APOSTROPHE] = KEY_MINUS;
    henkanTable[KEY_Z         ] = KEY_LEFT;
    henkanTable[KEY_X         ] = KEY_DOWN;
    henkanTable[KEY_C         ] = KEY_UP;
    henkanTable[KEY_V         ] = KEY_RIGHT;
    henkanTable[KEY_B         ] = KEY_X;
    henkanTable[KEY_N         ] = KEY_B;
    henkanTable[KEY_M         ] = KEY_GRAVE;
    henkanTable[KEY_COMMA     ] = KEY_HOME;
    henkanTable[KEY_DOT       ] = KEY_END;
    henkanTable[KEY_SLASH     ] = KEY_Z;

    henkanTable[KEY_CAPSLOCK] = KEY_LEFTCTRL;
    henkanTable[KEY_MUHENKAN] = KEY_LEFTSHIFT;
}

static int do_terminate = 0;

void exit_with_error(const char* str) {
    perror(str);
    exit(EXIT_FAILURE);
}

void send_event(int fd, int type, int code, int value) {
    struct input_event event;
    memset(&event, 0, sizeof(event));

    gettimeofday(&event.time, NULL);

    event.type = type;
    event.code = code;
    event.value = value;

    if (write(fd, &event, sizeof(event)) < 0) {
        exit_with_error("error: write");
    }
}

void ioctl_set(int fd, int set, int value) {
    if (ioctl(fd, set, value) < 0) {
        exit_with_error("error: ioctl failed");
    }
}

void create_uinput_device (int fd) {
    struct uinput_user_dev uidev;
    memset(&uidev, 0, sizeof(uidev));

    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "Dudrack");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0xDEAD;
    uidev.id.product = 0xBEEF;
    uidev.id.version = 1;

    if (write(fd, &uidev, sizeof(uidev)) < 0) {
        exit_with_error("create_uinput_device: write");
    }

    if (ioctl(fd, UI_DEV_CREATE) < 0) {
        exit_with_error("create_uinput_device: ioctl");
    }
}

void destroy_uinput_device(int fd) {
    if (ioctl(fd, UI_DEV_DESTROY) < 0) {
        exit_with_error("destroy_uinput_device: ioctl");
    }
}

void on_signal(int signal) {
    printf("TERM\n");
    do_terminate = 1;
}

void set_signal_handler() {
    sigset_t mask;
    sigemptyset(&mask);
    signal(SIGTERM, on_signal);
    signal(SIGINT, on_signal);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
}

int main(int argc, char** argv) {
    int output_device, input_device, i;

    if (argc != 2) {
        exit_with_error("Usage: dudrack <INPUT_DEVICE_EVENT>");
    }

    input_device = open(argv[1], O_RDONLY);
    if (input_device < 0) {
        exit_with_error("error: cannot open input device");
    }

    output_device = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (output_device < 0) {
        exit_with_error("error: cannot open /dev/uinput");
    }

    ioctl_set(output_device, UI_SET_EVBIT, EV_KEY);
    for (i = 0; i < KEY_CNT; ++i){
        ioctl_set(output_device, UI_SET_KEYBIT, i);
    }

    create_uinput_device(output_device);

    struct input_event event;
    int size_read;

    ioctl(input_device, EVIOCGRAB, 1);

    set_signal_handler();

    initNeutralTable();
    initHenkanTable();

    for (i = 0; i < KEY_CNT; ++i) {
        send_event(output_device, EV_KEY, i, 0);
    }

    int is_henkan = 0;
    while (!do_terminate && (size_read = read(input_device, &event, sizeof(struct input_event)))) {
        if (size_read < 0) {
            continue;
        }

        if (event.type == EV_KEY) {
            if (event.code == KEY_HENKAN) {
                is_henkan = event.value;
            } else {
                if (event.value == 1) {
                    int key_code = is_henkan ? henkanTable[event.code] : neutralTable[event.code];
                    send_event(output_device, EV_KEY, key_code, 1);
                }

                if (event.value == 0) {
                    send_event(output_device, EV_KEY, neutralTable[event.code], 0);
                    send_event(output_device, EV_KEY, henkanTable[event.code], 0);
                }
            }

            send_event(output_device, EV_SYN, SYN_REPORT, 0);
        }
    }

    ioctl(input_device, EVIOCGRAB, 0);

    destroy_uinput_device(output_device);

    close(output_device);
    close(input_device);

    exit(EXIT_SUCCESS);
}
