#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wiringPi.h>

#include "Nokia/Nokia.h"
#include "Nokia/Images.h"

#define BUF_SIZE 500


int main(int argc, char*argv[]) {
	int fd;
	if(argc == 1){
		printf("Reading from STDIN\n");
		fd = STDIN_FILENO;
	}
	else if(argc == 2){
		printf("Reading the file %s\n",argv[1]);
		fd = open(argv[1], O_RDONLY);
	}
	else{
		printf("Usage : %s [path]\n",argv[0]);
	}
	wiringPiSetup();
	lcdInit();
	gr_drawImage(LoZimg);
	gr_updateScreen();
    delay(1000);

    lcdClear();
    for(uint8_t val=0; val<30; val += 1){
        gr_setPixel(1, val, val);
        delay(100);
    }

    for(uint8_t val=0; val<30; val += 1){
        lcdInvertMode();
        delay(100);
        lcdNormalMode();
        delay(100);
    }
    return 0;


	char buf[BUF_SIZE];
	buf[BUF_SIZE] = 0;
	while(1){
		int n = read(fd, &buf, BUF_SIZE-1);
		if(n <= 0)break;
		lcdPrint((const char*)buf);
	}
	return 0;
}
