#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<fcntl.h>

void error(const char * message) { perror(message); exit(1);}

int main(int argc, char *argv[]) {

  int socketFD, portNumber, charswritten, charsRead;
  struct sockaddr_in serverAddress;
  struct hostent* serverHostInfo;
  char buffer[2000],ciphertextbuffer[256],keytextbuffer[256];
  int ciphertextfd,keytextfd;
  
  if(argc<4) {fprintf(stderr, "USAGE: %s ciphertext key port\n",argv[0]);exit(1);}

  ciphertextfd = open(argv[1],O_RDWR,S_IRUSR | S_IWUSR);
  if(ciphertextfd == -1) error("ERROR: otp_enc, opening plaintextfd");
  memset(ciphertextbuffer,'\0',sizeof(ciphertextbuffer));
  read(ciphertextfd,ciphertextbuffer,sizeof(ciphertextbuffer));
  
  keytextfd = open(argv[2],O_RDWR,S_IRUSR | S_IWUSR);
  if(keytextfd == -1) error("ERROR: otp_enc, opening keytextfd");
  memset(keytextbuffer,'\0',sizeof(keytextbuffer));
  read(keytextfd,keytextbuffer,strlen(ciphertextbuffer));
  
  //if(strlen(plaintextbuffer) > strlen(keytextbuffer)) {fprintf(stderr, "Key is too short\n");exit(1);}
  
  //printf("strlen of keytext buffer is %d\n",strlen(keytextbuffer));
  //printf("keytext buffer is %s\n",keytextbuffer);
  //printf("strlen of plaintext is %d\n",strlen(plaintextbuffer));
  //printf("plaintextbuffer is %s\n",plaintextbuffer);
  memset(buffer,'\0',2000);
  memset((char *) &serverAddress,'\0',sizeof(serverAddress));
  portNumber = atoi(argv[3]);
  
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(portNumber);
  
  serverHostInfo = gethostbyname("localhost");
  if(serverHostInfo == NULL) {fprintf(stderr,"ERROR: No such host\n"); exit(0);}
  
  memcpy((char *) &serverAddress.sin_addr.s_addr, (char *)serverHostInfo->h_addr, serverHostInfo->h_length);
  
  socketFD = socket(AF_INET, SOCK_STREAM,0);
  if(socketFD<0) error("ERROR: Opening socket otp_dec");
  
  if(connect(socketFD, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) error("Error: Connecting to server, otp_dec");
  
  //printf("CLIENT: Enter text you want to send and then hit Enter\n");
  //fgets(buffer, sizeof(buffer)-1,stdin);
  //buffer[strcspn(buffer,"\n")] = '\0';
  //strcat(plaintextbuffer,keytextbuffer);
  strcpy(buffer,"D");
  strcat(buffer,ciphertextbuffer);
  //strcat(buffer,"#");
  strcat(buffer,keytextbuffer);
  strcat(buffer,"\0");
  charswritten = send(socketFD, buffer, strlen(buffer),0);
  if(charswritten<0) error("ERROR: Sending packets otp_enc");
  if(charswritten < strlen(buffer)) printf("Not all the chars have sent\n");
  memset(buffer,'\0',sizeof(buffer));
  
  charsRead = recv(socketFD, buffer, sizeof(buffer)-1,0);
  if(charsRead < 0) error("ERROR: recieving data otp_dec");
  //char newbuf[1000];
  //itoa(buffer,newbuf,16);
  //sprintf(newbuf,"%X",buffer);
  printf("%s\n",buffer);
  
  close(socketFD);
  return 0;

}
