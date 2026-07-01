#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "chardev_ioctl.h"

int main(void){
    int len;
    int setLen = 5;
    int fd = open("/dev/chardev", O_RDWR);
    if (fd < 0){
        perror("open");
        return 1;
    }

    if (ioctl(fd, CHARDEV_IOCTL_CLEAR) < 0){
        perror("ioctl CLEAR");
        close(fd);
        return 1;
    }

    if (ioctl(fd, CHARDEV_IOCTL_SETLEN, &setLen) < 0){
        perror("ioctl SETLEN");
        close(fd);
        return 1;
    }

    printf("Set buffer length to %d.\n", setLen);

    if (ioctl(fd, CHARDEV_IOCTL_GETLEN, &len) < 0){
        perror("ioctl GETLEN");
        close(fd);
        return 1;
    }

    printf("Get buffer length is %d.\n", len);

	close(fd);
	return 0;
}