#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <iostream>
#include <cstring>
#include <fstream>

constexpr useconds_t POLLING_INTERVAL = 1;
constexpr size_t MAX_EPOLL_EVENTS = 16;
constexpr bool LOG_ENABLED = false;
constexpr bool SANDS_ENABLED = true;
constexpr int MAP_SWITCH_KEY = KEY_RIGHTSHIFT;
constexpr int MOUSE_SCROLL_DENOMINATOR = 4;
constexpr int MOUSE_MOVE_COEFFICIENT = 4;

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
    neutralTable[KEY_LEFTMETA] = KEY_LEFTALT;
    neutralTable[KEY_KATAKANAHIRAGANA] = KEY_LEFTALT;
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
    henkanTable[KEY_N         ] = KEY_GRAVE;
    henkanTable[KEY_M         ] = KEY_MUHENKAN;
    henkanTable[KEY_COMMA     ] = KEY_HOME;
    henkanTable[KEY_DOT       ] = KEY_END;
    henkanTable[KEY_SLASH     ] = KEY_EQUAL;

    henkanTable[KEY_CAPSLOCK] = KEY_LEFTCTRL;
    henkanTable[KEY_LEFTMETA] = KEY_LEFTALT;
    henkanTable[KEY_KATAKANAHIRAGANA] = KEY_LEFTALT;
}

void exit_with_error(const char* str) {
    perror(str);
    exit(EXIT_FAILURE);
}

void log(std::string line) {
    if constexpr (LOG_ENABLED) {
        std::ofstream file;

        file.open("/var/log/dudrack.log", std::ios::out | std::ios::app);
        if (file.fail()) {
            throw std::ios_base::failure(std::strerror(errno));
        }

        //make sure write fails with exception if something is wrong
        file.exceptions(file.exceptions() | std::ios::failbit | std::ofstream::badbit);

        file << line << std::endl;
    }
}

void send_event(int fd, int type, int code, int value) {
    struct input_event event = {};

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
    struct uinput_user_dev uidev = {};

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

static int output_fd;

void on_signal(int signal) {
    log("Received signal " + std::to_string(signal));

    log("Destorying uinput");

    destroy_uinput_device(output_fd);

    log("Destoried uinput");

    log("Closing /dev/uinput");

    close(output_fd);

    log("Closed /dev/uinput");

    exit(EXIT_SUCCESS);
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

int open_input_device(int epoll_fd, char const* path) {
    int input_fd = open(path, O_RDONLY);

    if (input_fd < 0) {
        log(std::string("failed to open ").append(path));
        return -1;
    }

    struct epoll_event ev = {};
    ev.events = EPOLLIN;
    ev.data.fd = input_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, input_fd, &ev) != 0) {
        log(std::string("failed to add epoll to input ").append(path));
        return -1;
    }

    ioctl(input_fd, EVIOCGRAB, 1);

    return input_fd;
}

int main(int argc, char const** argv) {
    if (argc <= 1) {
        exit_with_error("Usage: dudrack <INPUT_DEVICE_EVENT>...");
    }

    int const num_inputs = argc - 1;

    log("Creating epoll");

    int epoll_fd = epoll_create(MAX_EPOLL_EVENTS);

    log("Opening /dev/uinput");

    while ((output_fd = open("/dev/uinput", O_WRONLY)) < 0) {
        usleep(POLLING_INTERVAL);
    }

    log("Opened /dev/uinput");

    log("Creating uinput");

    ioctl(output_fd, UI_SET_EVBIT, EV_REL);
    ioctl(output_fd, UI_SET_RELBIT, REL_X);
    ioctl(output_fd, UI_SET_RELBIT, REL_Y);
    ioctl(output_fd, UI_SET_RELBIT, REL_WHEEL);
    ioctl(output_fd, UI_SET_RELBIT, REL_HWHEEL);

    ioctl_set(output_fd, UI_SET_EVBIT, EV_KEY);
    for (int i = 0; i < KEY_CNT; ++i){
        ioctl_set(output_fd, UI_SET_KEYBIT, i);
    }

    create_uinput_device(output_fd);

    log("Created uinput");

    log("Setting signal handlers");

    set_signal_handler();

    log("Set signal handlers");

    initNeutralTable();
    initHenkanTable();

    log("Sending release events of all keys to uinput");

    for (int i = 0; i < KEY_CNT; ++i) {
        send_event(output_fd, EV_KEY, i, 0);
    }

    log("Sent release events of all keys to uinput");

    bool use_dudrack = true;
    struct epoll_event events[MAX_EPOLL_EVENTS];
    struct input_event event;

    while (true) {
        log("Opening input device");

        int input_fds[num_inputs];
        for (int i = 0; i < num_inputs; ++i) {
            while ((input_fds[i] = open_input_device(epoll_fd, argv[i + 1])) < 0) {
                usleep(POLLING_INTERVAL);
            }

            ioctl(input_fds[i], EVIOCGRAB, 1);
        }

        log("Opened input device");

        bool is_left_control = false;
        bool is_right_shift = false;
        bool is_wheel = false;
        bool is_henkan = false;
        bool is_muhenkan = false;
        bool no_event_between_space_events = false;
        int vertical_scroll_remainder = 0;
        int horizontal_scroll_remainder = 0;

        while (true) {
            log("Reading input device");

            int timeout = -1;
            int num_events = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, timeout);

            log("Read input device");

            for (int i = 0; i < num_events; ++i) {
                if (read(events[i].data.fd, &event, sizeof(struct input_event)) != sizeof(struct input_event)) {
                    goto FAILED_TO_READ;
                }

                switch (event.type) {
                    case EV_REL:
                        if (is_wheel) {
                            switch (event.code) {
                                case REL_X: {
                                    int dx = event.value + horizontal_scroll_remainder;
                                    int adjusted_dx = dx / MOUSE_SCROLL_DENOMINATOR;
                                    horizontal_scroll_remainder = dx % MOUSE_SCROLL_DENOMINATOR;

                                    event.code = REL_HWHEEL;
                                    event.value = adjusted_dx;
                                    break;
                                }

                                case REL_Y: {
                                    int dy = -event.value + vertical_scroll_remainder;
                                    int adjusted_dy = dy / MOUSE_SCROLL_DENOMINATOR;
                                    vertical_scroll_remainder = dy % MOUSE_SCROLL_DENOMINATOR;

                                    event.code = REL_WHEEL;
                                    event.value = adjusted_dy;
                                    break;
                                }
                            }
                        } else {
                            event.value *= MOUSE_MOVE_COEFFICIENT;
                        }

                        if (write(output_fd, &event, sizeof(event)) < 0) {
                            exit_with_error("error: write");
                        }
                        break;

                    case EV_KEY:
                        if (is_right_shift && event.code == KEY_2) {
                            use_dudrack = true;
                            continue;
                        } else if (is_right_shift && event.code == KEY_1) {
                            use_dudrack = false;
                            continue;
                        } else if (is_left_control && event.code == KEY_D && event.value < 2) {
                            send_event(output_fd, EV_KEY, BTN_LEFT, event.value);
                            continue;
                        } else if (is_left_control && event.code == KEY_F && event.value < 2) {
                            send_event(output_fd, EV_KEY, BTN_RIGHT, event.value);
                            continue;
                        }

                        switch (event.code) {
                            case KEY_HENKAN:
                            case BTN_RIGHT:
                                is_henkan = event.value;
                                is_wheel = event.value;
                                break;

                            case KEY_MUHENKAN:
                            case BTN_LEFT:
                                send_event(output_fd, EV_KEY, KEY_LEFTSHIFT, event.value);
                                is_muhenkan = event.value;

                                break;

                            case KEY_SPACE:
                                if constexpr (SANDS_ENABLED) {
                                    send_event(output_fd, EV_KEY, KEY_LEFTSHIFT, event.value);

                                    // SandS
                                    if (event.value > 0) {
                                        no_event_between_space_events = true;
                                    } else if (event.value == 0 && no_event_between_space_events) {
                                        send_event(output_fd, EV_KEY, KEY_SPACE, 1);
                                        send_event(output_fd, EV_KEY, KEY_SPACE, 0);
                                    }
                                } else {
                                    send_event(output_fd, EV_KEY, KEY_SPACE, event.value);
                                }
                                break;

                            case KEY_LEFTCTRL:
                                is_left_control = event.value;

                                if (event.value == 0) {
                                    send_event(output_fd, EV_KEY, BTN_LEFT, event.value);
                                    send_event(output_fd, EV_KEY, BTN_RIGHT, event.value);
                                }
                                break;

                            case KEY_RIGHTSHIFT:
                                is_right_shift = event.value;
                                break;

                            default:
                                if (event.value > 0) {
                                    no_event_between_space_events = false;

                                    send_event(
                                      output_fd,
                                      EV_KEY,
                                      use_dudrack
                                      ? ((is_henkan || is_muhenkan)
                                          ? henkanTable[event.code]
                                          : neutralTable[event.code])
                                      : event.code,
                                      event.value
                                    );
                                } else {
                                    if (use_dudrack) {
                                        send_event(output_fd, EV_KEY, neutralTable[event.code], 0);
                                        send_event(output_fd, EV_KEY, henkanTable[event.code], 0);
                                    } else {
                                        send_event(output_fd, EV_KEY, event.code, 0);
                                    }
                                }

                                break;
                        }
                        break;

                    default:
                        if (write(output_fd, &event, sizeof(event)) < 0) {
                            exit_with_error("error: write");
                        }
                        continue;
                }
            }

            send_event(output_fd, EV_SYN, SYN_REPORT, 0);
        }

FAILED_TO_READ:
        for (int i = 0; i < num_inputs; ++i) {
            struct epoll_event ev = {};
            ev.events = EPOLLIN;
            ev.data.fd = input_fds[i];

            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, input_fds[i], &ev);
            ioctl(input_fds[i], EVIOCGRAB, 0);
            close(input_fds[i]);
        }
    }
}
