#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>


int init_maestro();
int maestroSetTarget(int fd, unsigned char channel, unsigned short target);
int maestroGetPosition(int fd, unsigned char channel);
void close_maestro();

