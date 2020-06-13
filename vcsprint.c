#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wiringPi.h>

#include "Nokia/Nokia.h"
#include "Nokia/Images.h"

#define BUF_SIZE 500

char console_path[]="/dev/vcs1";

void filter_utf8(char*in, char*out){
    int size = strlen(in);
    for(int i=0,j=0; i<size; i++){
        if(in[i] < 0x80){
            out[j] = in[i];
            j++;
        }
        else{
            // Unicode detected -> replace with single ' '
            out[j] = '.';
            j++;
            int num_bytes = 0;
            char this_byte = in[i];
            while(0x80 & this_byte && num_bytes < 4 && in[i+num_bytes] & 0xc0 == 0x80){
                num_bytes++;
                this_byte = this_byte<<1;
            }
            if(num_bytes == 0) num_bytes = 1;
            //printf("%d num bytes\n",num_bytes);
            i += num_bytes - 1;
        }

    }
}

int main(int argc, char*argv[]) {
	int fd;
    char*path=console_path;
	if(argc == 2){
		printf("Reading the file %s\n",argv[1]);
        path=argv[1];
	}
    fd = open(path, O_RDONLY);

	wiringPiSetup();
	lcdInit();

	while(1){
        char buf[BUF_SIZE] = {0};
        char filter_buf[BUF_SIZE] = {0};
		int n = read(fd, &buf, BUF_SIZE-1);
		if(n <= 0)break;
        filter_utf8(buf, filter_buf);
        //printf("*********************************\n");
		//for(int i=0;i<strlen(buf);i++)printf("%x:%c,",(int)buf[i],buf[i]);
        //printf("\n");
        //printf("=================================\n");
		//for(int i=0;i<strlen(filter_buf);i++)printf("%x:%c,",(int)filter_buf[i],filter_buf[i]);
        //printf("\n");
        //printf("*********************************\n");
		lcdPrintFormatted((const char*)filter_buf);
        lseek(fd, 0, 0);
        usleep(20000);
        //usleep(2000000);
	}
	return 0;
}
