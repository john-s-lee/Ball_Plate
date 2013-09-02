/***********************************************************
*
*  playsound.c  Required Build Options -lpthread
*
************************************************************/

#include <pthread.h>

#include "../include/ball_plate.h"
#include "../include/playsound.h"

void  * function1(void * argument);


void playsound(char *sound)
{
	char command[30];
	sprintf(command, "pkill aplay");
	system(command);
	pthread_t t1;
	pthread_create( &t1, NULL, function1,(void*) sound); // create a thread running function1
}

void stopsound()
{
	char command[30];
	sprintf(command, "pkill aplay");
	system(command);
}

void * function1(void * argument)
{
	if (mute%2 == 0)
	{
		char *song = (char*)argument;
		char command[100];
		sprintf(command, "aplay %s", song);
		system(command);
	}
	return 0;
}

