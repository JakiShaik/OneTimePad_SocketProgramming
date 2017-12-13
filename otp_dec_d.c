#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>


void error(const char * message) { perror(message); exit(1);}

int ourascii(char c) {
  
  switch(c){
    
  case 'A': return 1;
  case 'B': return 2;
  case 'C': return 3;
  case 'D': return 4;
  case 'E': return 5;
  case 'F': return 6;
  case 'G': return 7;
  case 'H': return 8;
  case 'I': return 9;
  case 'J': return 10;
  case 'K': return 11;
  case 'L': return 12;
  case 'M': return 13;
  case 'N': return 14;
  case 'O': return 15;
  case 'P': return 16;
  case 'Q': return 17;
  case 'R': return 18;
  case 'S': return 19;
  case 'T': return 20;
  case 'U': return 21;
  case 'V': return 22;
  case 'W': return 23;
  case 'X': return 24;
  case 'Y': return 25;
  case 'Z': return 26;
  case ' ': return 27;
  }
}


char asctochar(int x) {switch(x) {case 1: return 'A'; case 2: return 'B'; case 3: return 'C'; case 4: return 'D'; case 5: return 'E'; case 6: return 'F'; case 7: return 'G'; case 8: return 'H';
  case 9: return 'I'; case 10: return 'J'; case 11: return 'K'; case 12: return 'L'; case 13: return 'M'; case 14: return 'N'; case 15: return 'O'; case 16: return 'P'; case 17: return 'Q';
  case 18: return 'R'; case 19: return 'S'; case 20: return 'T'; case 21: return 'U'; case 22: return 'V'; case 23: return 'W'; case 24: return 'X'; case 25: return 'Y';case 26: return 'Z'; 
  case 27: return ' ';}}


int main(int argc, char *argv[]) {

  int listenSocketFD, establishedConnFD, portNumber,charsRead;
  socklen_t sizeofclientinfo;
  char buffer[2000];
  struct sockaddr_in serverAddress,clientAddress;

  if(argc < 2) {fprintf(stderr,"USAGE: %s port\n",argv[0]); exit(1);}
  
  memset((char *)&serverAddress,'\0',sizeof(serverAddress));
  
  portNumber = atoi(argv[1]);
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(portNumber);
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  
  listenSocketFD = socket(AF_INET,SOCK_STREAM,0);

  if(listenSocketFD <0 ) {error("Error opening socket otp_enc_d");}
  if(bind(listenSocketFD, (struct sockaddr *) &serverAddress,sizeof(serverAddress))<0)
    error("Error on binding otp_enc_d");
  listen(listenSocketFD,5);
  pid_t pid;

  while(1) {
  sizeofclientinfo = sizeof(clientAddress);
  establishedConnFD = accept(listenSocketFD,(struct sockaddr *)&clientAddress,&sizeofclientinfo);
  if(establishedConnFD < 0) error("Error on accept otp_enc_d");
  pid = fork();
  if(pid<0) {fprintf(stderr,"Error in new process creation (otp_enc)");exit(1);}
  if(pid == 0) {
    memset(buffer,'\0',2000);
    charsRead = recv(establishedConnFD,buffer,2000,0);
    if(charsRead<0) error("Error on recieving otp_enc_d");
    
    char * str[2];
    char * found;
    int i=0,n=0,pascii,kascii,tascii;
    //printf("at line 44\n");
    char *string;
    char cipher[2000];
    //long number;
    memset(cipher,'\0',sizeof(cipher));
    string = buffer;
    //printf("lline 47\n");
    //printf("string is %s\n",string);
    if (string[0] == 'D') 
      memmove(string, string+1, strlen(string));
    //printf("After new code string is %s\n",string);
    while( (found = strsep(&string,"\n")) != NULL ) {
      //printf("%s\n",found);
      str[i] = found;
      i++;
    }
    //printf("str[0] is %s\n",str[0]);
    //printf("str[1] is %s\n",str[1]);
    //printf("str[0] len is %d\n",strlen(str[0]));
    //printf("str[0][0] is %c\n",str[0][0]);
    //printf("strlen os str[1] is %d\n",strlen(str[1]));
    //printf("SERVER: I recieved this ciphertext from the client: \"%s\"\n",str[0]);
    //printf("SERVER: I recieved this key from client: \"%s\"\n",str[1]);
    for(i=0;i<strlen(str[0]);i++) {
      //printf("<---%c--->",str[0][i]);
      //printf("<<--%d--->>\n",str[1][i]);
      //printf("%02X",str[0][i]^str[1][i]);
      //sscanf(cipher,"%02X",str[0][i]^str[1][i]);
      pascii = ourascii(str[0][i]);
      kascii = ourascii(str[1][i]);
      tascii = pascii - kascii;
      if(tascii <= 0) {
	tascii = tascii + 27;
      }
      cipher[i] = asctochar(tascii);
      
      //cipher[i] = str[0][i]^str[1][i];
    }
    //int count;
    //count = i-1;
    //printf("\n");
    //for(i=0;i<sizeof(cipher);i++) {
    //printf("cipher is %c\n",cipher[i]);
    //}
    //printf("length of cipher is %u\n",sizeof(cipher));
    //charsRead = send(establishedConnFD,"I am the server, and I got your message",39,0);
    //char converted[4002];
    //for(i=0;i<count+1;i++)
    //sprintf(&converted[i*2],"%02X",cipher[i]);
    //printf("plaintext is %s\n",cipher);
    charsRead = send(establishedConnFD,cipher,strlen(cipher),0);
    if(charsRead < 0) error("Error on sending in otp_enc_d");
    close(establishedConnFD);
  }
  else {
    close(establishedConnFD);
  }
  }
  close(listenSocketFD);
  return 0;
}
