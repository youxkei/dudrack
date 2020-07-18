#include <iostream>
#include <linux/input.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>

constexpr size_t MAX_EVENTS = 16;

int open_input_device(int epoll_fd, char const* path) {
    int input_fd = open(path, O_RDONLY);

    if (input_fd < 0) {
        fprintf(stderr, "failed to open %s\n", path);
        return -1;
    }

    struct epoll_event ev = {};
    ev.events = EPOLLIN;
    ev.data.fd = input_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, input_fd, &ev) != 0) {
        fprintf(stderr, "failed to add epoll to input %s\n", path);
        return -1;
    }

    ioctl(input_fd, EVIOCGRAB, 1);

    return input_fd;
}

int main() {
    struct epoll_event events[MAX_EVENTS];
    struct input_event event;

    int epoll_fd = epoll_create(MAX_EVENTS);

    int keyboard_fd = open_input_device(epoll_fd, "/dev/input/by-id/usb-ARCHISS_Q91JP_000000000000-event-kbd");
    if (keyboard_fd < 0) {
        return 1;
    }

    int mouse_button_fd = open_input_device(epoll_fd, "/dev/input/by-id/usb-ARCHISS_Q91JP_000000000000-if02-event-mouse");
    if (mouse_button_fd < 0) {
        return 1;
    }

    int mouse_fd = open_input_device(epoll_fd, "/dev/input/by-id/usb-2b39_USB_Stick_Pointer-event-mouse");
    if (mouse_fd < 0) {
        return 1;
    }

    int mouse2_fd = open_input_device(epoll_fd, "/dev/input/by-id/usb-I-Rocks_Technology_Co.__LTD._Laser_Mouse-event-mouse");
    if (mouse2_fd < 0) {
        return 1;
    }

    while (true) {
        int timeout = -1;
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, timeout);

        for (int i = 0; i < num_events; ++i) {
            if (read(events[i].data.fd, &event, sizeof(struct input_event))
                == sizeof(struct input_event)) {
                std::cout << event.type << ", " << event.code << ", " << event.value << std::endl;
            } else {
                std::cerr << "read failed" << std::endl;
            }
        }
    }
}
