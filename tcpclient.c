#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;  // 서버에 접속 할 소켓
	char message[BUF_SIZE];  // 서버 보낼 메세지를 저장 할 문자열 버퍼
	int str_len;  // 송수신 메세지의 문자열 길이
	struct sockaddr_in serv_adr;  // 접속할 서버의 주소(어딘지 미리 알아야 함)
	
	// IP와 Port를 지정하지 않았으면 사용법을 알려준다.
	if(argc!=3){
	printf("Usage : %s <IP> <port>\n", argv[0]);  // 사용법 표시
	exit(1);   // 비정상 종료
	}

	sock=socket(PF_INET, SOCK_STREAM, 0);  // TCP(Stream) 프로토콜 생성
	if(sock==-1)  // 안만들어 지면
		error_handling("socket() error"); // 에러표시 후 종료

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
		
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected..........");

	while(1)
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if(!strcmp(message,"q\n")|| !strcmp(message, "Q\n"))
			break;

		write(sock, message, strlen(message));
		str_len=read(sock,message, BUF_SIZE-1);
		message[str_len]=0;
		printf("Message from server : %s", message);
	}
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

