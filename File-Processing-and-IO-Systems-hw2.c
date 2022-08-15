#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#define __USE_GNU 1
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <time.h>

int main(){
//sequential read
	int *buffer_r = valloc(sizeof(char) * 4096);
	int t;
	struct timeval start;
	struct timeval end;
	int fd=open("file",O_RDONLY);
	if(fd == -1){
		printf("open error\n");
		return 0;
	}
	gettimeofday(&start,NULL);
	int ret;
	for(int i=0 ;i < 25600; i++){
		ret = read( fd,  buffer_r,  4096);
	}
	gettimeofday(&end,NULL);

	if(ret==-1){
		printf("read error\n");
		return 0;
	}
//	free(buffer_r);
//	close(fd);

	t=1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);

	printf("sequential read time= %d us\n",t);
	
//sequential write	
	int *buffer_w = valloc(sizeof(char) * 2048);
	int fd_w=open("file",O_DIRECT | O_WRONLY | O_CREAT );
	if(fd_w == -1){
		printf("open error\n");
		return 0;
	}
	gettimeofday(&start,NULL);
	for(int i=0 ;i < 51200; i++){
		ret = write( fd_w,  buffer_w,  4096);
	}
	fsync(fd_w);
	gettimeofday(&end,NULL);

	if(ret==-1){
		printf("write error\n");
		return 0;
	}
//	free(buffer_w);
//	close(fd_w);

	t=1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);

	printf("sequential write time= %d us\n",t);
	
//random read
	int a;
 	srand(time(NULL));
	gettimeofday(&start,NULL);
	for(int i=0 ;i < 25600; i++){
		a=(rand()%25600);
		lseek( fd,  a*4096, SEEK_SET);
		ret = read( fd,  buffer_r,  4096);
	}
	gettimeofday(&end,NULL);

	if(ret==-1){
		printf("read error\n");
		return 0;
	}
	free(buffer_r);
	close(fd);

	t=1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);

	printf("random read time= %d us\n",t);
	
//random buffered write_1
	gettimeofday(&start,NULL);
	for(int i=0 ;i < 50000; i++){
		a=(rand()%50000);
		lseek( fd,  a*4096, SEEK_SET);
		ret = write( fd_w,  buffer_w,  4096);
	}
	gettimeofday(&end,NULL);

	if(ret==-1){
		printf("write error\n");
		return 0;
	}
//	free(buffer_w);
//	close(fd_w);

	t=1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);

	printf("random write time= %d us\n",t);

//random buffered write_2
	gettimeofday(&start,NULL);
	for(int i=0 ;i < 50000; i++){
		a=(rand()%50000);
		lseek( fd,  a*4096, SEEK_SET);
		ret = write( fd_w,  buffer_w,  4096);
	}
	fsync(fd_w);
	gettimeofday(&end,NULL);

	if(ret==-1){
		printf("write error\n");
		return 0;
	}
	free(buffer_w);
	close(fd_w);

	t=1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);

	printf("random write time= %d us\n",t);	
	
	return 0;



}
