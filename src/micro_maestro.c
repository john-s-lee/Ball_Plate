#include "../include/micro_maestro.h"

int init_maestro()
{
	const char * device = "/dev/ttyACM0";
	int fd = open(device, O_RDWR | O_NOCTTY); 
	struct termios options;
	tcgetattr(fd, &options);
	options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	options.c_oflag &= ~(ONLCR | OCRNL);
	tcsetattr(fd, TCSANOW, &options);

	
	//Set Speed limits for the servo as MAESTRO_SPEED defined in micro_maestro.h
	unsigned char command[] = {0x87, 0, MAESTRO_SPEED & 0x7F, MAESTRO_SPEED >> 7 & 0x7F};
	if (write(fd, command, sizeof(command)) == -1)
	{
		perror("error writing");
		return -1;
	}
	unsigned char command2[] = {0x87, 1, MAESTRO_SPEED & 0x7F, MAESTRO_SPEED >> 7 & 0x7F};
	if (write(fd, command2, sizeof(command)) == -1)
	{
		perror("error writing");
		return -1;
	}
	
	//Set Acceleration limits for the servo as MAESTRO_ACC defined in micro_maestro.h
	unsigned char command3[] = {0x89, 0, MAESTRO_SPEED & 0x7F, MAESTRO_ACC >> 7 & 0x7F};
	if (write(fd, command3, sizeof(command)) == -1)
	{
		perror("error writing");
		return -1;
	}
	unsigned char command4[] = {0x89, 1, MAESTRO_SPEED & 0x7F, MAESTRO_ACC >> 7 & 0x7F};
	if (write(fd, command4, sizeof(command)) == -1)
	{
		perror("error writing");
		return -1;
	}

	return fd;
}

int maestroSetTarget(int fd, unsigned char channel, unsigned short target)
{
  unsigned char command[] = {0x84, channel, target & 0x7F, target >> 7 & 0x7F};
  if (write(fd, command, sizeof(command)) == -1)
  {
    perror("error writing");
    return -1;
  }
  return 0;
}

int maestroGetPosition(int fd, unsigned char channel)
{
  unsigned char command[] = {0x90, channel};
  if(write(fd, command, sizeof(command)) == -1)
  {
    perror("error writing");
    return -1;
  }
   
  unsigned char response[2];
  if(read(fd,response,2) != 2)
  {
    perror("error reading");
    return -1;
  }
   
  return response[0] + 256*response[1];
}

void close_maestro(int fd)
{
	close(fd);
}

void maestroSetSpeed(int fd, int speed)
{
	//Set Speed limits for the servo as MAESTRO_SPEED defined in micro_maestro.h
	unsigned char command[] = {0x87, 0, speed & 0x7F, speed >> 7 & 0x7F};
	if (write(fd, command, sizeof(command)) == -1)
	{
		perror("error writing");
		return;
	}
	unsigned char command2[] = {0x87, 1, speed & 0x7F, speed >> 7 & 0x7F};
	if (write(fd, command2, sizeof(command)) == -1)
	{
		perror("error writing");
		return;
	}
}