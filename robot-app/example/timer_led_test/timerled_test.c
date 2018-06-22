#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define TIMER_LED_FILE_NAME		"/dev/timer_led"

#define TIMER_LED_BLINK         0x25001
#define TIMER_LED_SETNUM        0x25002

int main(int argc, char **argv)
{
	int fd, num;
	
	if(argc > 2) {
		fprintf(stderr, "Usage 1 : timer_ledtest\n");
		fprintf(stderr, "Usage 2 : timer_ledtest [num]\n");
		exit(1);
	}

	fd = open(TIMER_LED_FILE_NAME, O_RDWR);
	if(fd < 0) {
		fprintf(stderr, "%s open failed!!\n", TIMER_LED_FILE_NAME);
		exit(1);
	}

	if (argc == 1) {
		if ( ioctl(fd, TIMER_LED_BLINK, 0) ) {
			fprintf(stderr, "ioctl error\n");
			exit(1);
		}

		printf("Timer LED Blink Start\n");
	}

	else if (argc == 2) {
		num = atoi(argv[1]);

		if ( ioctl(fd, TIMER_LED_SETNUM, num) ) {
			fprintf(stderr, "ioctl error\n");
			exit(1);
		}

		printf("Timer LED Blink Number : %d\n", num);

		if ( ioctl(fd, TIMER_LED_BLINK, 0) ) {
			fprintf(stderr, "ioctl error\n");
			exit(1);
		}
		printf("Timer LED Blink Start\n");
	}

	return(0);
}
