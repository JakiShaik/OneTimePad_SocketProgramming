#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>

void arrayshuffle();

void arrayshuffle( char a[])
  {
    int k,j;
    char temp;
    for(k=26;k>=0;k--)
      {
	srand(time(0));
	j = rand()%(k+1);
	//swap                                                                                                                                                                                                  
	temp = a[j];
	a[j] = a[k];
	a[k] = temp;
      }
  }

int main(int argc,char * argv[]) {

  if(argc < 2) {
    
    printf("Usage: keygen <lengthofkey>\n");
    exit(1);
  }
  int lengthofkey,i;
  char array[27] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' '};
  lengthofkey = atoi(argv[1]);
  char contenttowrite[lengthofkey]; //we have to add one extra new line char at end
  //printf("length is %d\n",lengthofkey);
  for(i=0;i<lengthofkey;i++) {
    int randomnumber;
    srand(time(NULL));
    randomnumber = rand() % 27;
    arrayshuffle(array);
    //printf("array[random] is %c\n",array[randomnumber]);
    contenttowrite[i] = array[randomnumber];
    //sleep(1);
  }
  //printf("i value is %d\n",i);
  //contenttowrite[i] = '\n';
  contenttowrite[i] = '\0';
  printf("%s\n",contenttowrite);
  exit(0);
}
