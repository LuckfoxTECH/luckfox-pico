#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <dirent.h>

#define INPUT_DIR "/dev/input"
#define EVENT_PREFIX "event"

void check_adc_keys_event(const char *device_path) {
    int fd;
    char name[256] = "Unknown";
    struct input_event ev;

    fd = open(device_path, O_RDONLY);
    if (fd < 0) {
        perror("Unable to open device");
        return;
    }

    if (ioctl(fd, EVIOCGNAME(sizeof(name)), name) < 0) {
        perror("Unable to get device name");
        close(fd);
        return;
    }

    if (strstr(name, "adc-keys") != NULL) {
        printf("Found ADC keys device: %s\n", device_path);

        while (read(fd, &ev, sizeof(struct input_event)) > 0) {
            if (ev.type == EV_KEY) {
                if (ev.value == 0) {
                  printf("Key released: code %d\n", ev.code);
                  system("luckfox-config rgb_switch");
                  system("reboot");
              }
            }
        }
    }

    close(fd);
}

int main() {
    struct dirent *entry;
    DIR *dp;


    dp = opendir(INPUT_DIR);
    if (dp == NULL) {
        perror("Unable to open /dev/input directory");
        return EXIT_FAILURE;
    }

    while ((entry = readdir(dp))) {
        if (strncmp(entry->d_name, EVENT_PREFIX, strlen(EVENT_PREFIX)) == 0) {
            char device_path[256];
            snprintf(device_path, sizeof(device_path), "%s/%s", INPUT_DIR, entry->d_name);
            check_adc_keys_event(device_path);
        }
    }

    closedir(dp);
    return EXIT_SUCCESS;
}

