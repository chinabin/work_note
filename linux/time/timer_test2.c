#include <stdio.h>    // printf
#include <stdlib.h>    // exit
#include <unistd.h>
#include <fcntl.h>    // open

#define FILE_NAME    "/dev/second"

int main(int args, char *argv[])
{
    int fd;

    printf("\r\nstart.");

    // open file
    fd=open(FILE_NAME,O_RDONLY|O_NONBLOCK);
    if( fd<0 )    
    {
        printf("\r\nopen file err,fd=%d",fd);
        exit(-1);
    }
    
    while(1);

    close(fd);
    exit(0);
}