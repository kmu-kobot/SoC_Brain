#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define LED_ON 		1
#define LED_OFF 	0

#define BASIC_LED_FILE	"/dev/basic_led"

int main(int argc, char **argv)
{
	int ledfd, i, j, tmp;
	char buf = '\0';
	
	if(argc <= 1) {
		fprintf(stderr, "Usage 1 : ledtest -on  <led on>\n");
        fprintf(stderr, "Usage 2 : ledtest -off <led off>\n");
		fprintf(stderr, "Usage 3 : ledtest -r   <led status Read>\n");
        fprintf(stderr, "Usage 4 : ledtest -w [led on/off Count Number]\n");        
		exit(1);
	}

	ledfd = open(BASIC_LED_FILE, O_RDWR);
	if(ledfd < 0) {
		fprintf(stderr, "%s open failed!!\n", BASIC_LED_FILE);
		exit(1);
	}

    if(strcmp("-off", argv[1]) == 0) {
        buf = LED_OFF;
        write(ledfd, &buf, 1);
        close(ledfd);
    }
    else if(strcmp("-on", argv[1]) == 0) {
        buf = LED_ON;
        write(ledfd, &buf, 1);
        close(ledfd);
    }
    else if(strcmp("-r", argv[1]) == 0) {
        read(ledfd, &buf, 1);
        if(buf & 0x1)
            printf(" -> ON\n");
        else
            printf(" -> OFF\n");
        close(ledfd);
    }
    else if(strcmp("-w", argv[1]) == 0) {
        if(argc <= 2) {
            fprintf(stderr, "Usage : ledtest -w [led on/off Count Number]\n");
            exit(1);
        }
        
        tmp = atoi(argv[2]);
        for(i=0; i<tmp; i++) {
	        buf = LED_ON;
            write(ledfd, &buf, 1);
            for(j=0; j<0xa000000; j++);
            buf = LED_OFF;
            write(ledfd, &buf, 1);
            for(j=0; j<0xa000000; j++);
        }
        close(ledfd);            
    }
	else {
		fprintf(stderr, "Usage 1 : ledtest -on  <led on>\n");
        fprintf(stderr, "Usage 2 : ledtest -off <led off>\n");
		fprintf(stderr, "Usage 3 : ledtest -r   <led status Read>\n");
        fprintf(stderr, "Usage 4 : ledtest -w [led on/off Count Number]\n");
		exit(-1);
	}

	return(0);
}
