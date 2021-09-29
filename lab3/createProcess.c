#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){
	int a = fork();
	printf("%d\n", a);
	/*switch (fork()){
		case 0:
			printf("i am child: pid=%d\n", getpid());
			break;
		default:
                        printf("i am parent: pid=%d\n", getpid());
                        break;
		case -1:
                        perror("Fork failed");
			break;
	}*/
	return 0;
}
