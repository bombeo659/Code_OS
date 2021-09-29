#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main() {
   FILE *fp;
	
   int tmp = 0;
   int counter0 = 0, counter1 = 0;

   int pid0 = fork();
   int pid1 = fork();

   if(pid0 > 0 && pid1 > 0){
	fp = fopen("numbers.txt", "r");
	while(fscanf(fp,"%d\n", &tmp) != EOF){
		if(tmp%2 == 0) counter0++;
		if(tmp%3 == 0) counter1++;
	}
	printf("%d (talented class answer: %d)\n", counter0, counter1);
    	fflush(stdout);
   }  
   if(pid0 > 0 && pid1 == 0){
	counter0 = 0;
	counter1 = 0;
        fp = fopen("numbers.txt", "r");
        while(fscanf(fp,"%d\n", &tmp) != EOF){
                if(tmp%3 == 0) counter0++;
                if(tmp%7 == 0) counter1++;
        }
        printf("%d (talented class answer: %d)\n", counter0, counter1);
        fflush(stdout);
   }
   if(pid0 == 0 && pid1 == 0){
	counter0 = 0;
       	counter1 = 0;
        fp = fopen("numbers.txt", "r");
        while(fscanf(fp,"%d\n", &tmp) != EOF){
                if(tmp%5 == 0) counter0++;
                if(tmp%11 == 0) counter1++;
        }
        printf("%d (talented class answer: %d)\n", counter0, counter1);
        fflush(stdout);
   }

   fclose(fp);
   return 0;
}
