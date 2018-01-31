
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

#include <sys/time.h>

int main(void)
{
	pid_t cpid;
	int var_lcl = 0;
	struct timeval tv;
	struct timeval tv2;

	gettimeofday(&tv, NULL);
	cpid = fork();

	if(cpid >= 0) // fork was successful
	{
		if(cpid == 0) // child process
		{
			gettimeofday(&tv2, NULL);
			printf("Child Process \n");
			printf("time = %ld \n", tv2.tv_usec - tv.tv_usec);
		}
		else //Parent process
		{
			gettimeofday(&tv2, NULL);
			printf("Parent process, child pid = %d\n", cpid);
			printf("time = %ld \n", tv2.tv_usec - tv.tv_usec);
		}
	}
	else // fork failed
	{
		printf("Fork failed!\n");
		return 1;
	}

	return 0;
}
