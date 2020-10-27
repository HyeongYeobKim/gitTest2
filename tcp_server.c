/**
 * 1. 서버는 클라가 접속하면 클라의 IP와 PORTcnffur ex) [127.0.0.1 : 5000] connected
 * 
 * 2. 클라는 서버와 연결이 성공하면, 받고 싶은 파일명을 사용자로부터 입력받아.
 *	-> 이 파일명을 서버에게 전송
 *	ex) Input a file name
 *		script.txt
 * 3. 서버는 클라로부터 파일명을 받았지? open으로 열고 -> 클라에게 파일 내용을 전송
 *	-> 파일전송 이전에, 파일의 크기를 먼저 "정수형"으로 전송
 *	-> 그 데이터 전송
 *	-> 데이터 전송시 "최대 크기 1024 바이트"
 *	=> 파일 길이 "lseek()" 함수
 * 
 * 4. 클라는 서버로부터받은 "파일크기 출력"
 *	-> 받은 파일을 received_script.txt 파일로 저장
 * 
 * 5. 알파벳만 소 -> 대 / 대 -> 소 변경하여 전송 (특수기호 및 공백 유지)
 * 
 * 6. 빌드하는 Makefile작성
 *
 * 7. 코드 / Makefile 깃에 업로드 
 * -> 커밋과정고 ㅏ커밋 결과 제출 (명령어로)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/types.h>

#define MAX_BUF_LEN 1024
#define MAX_FILE_NAME 10

void error_handling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr); // '' 랑 "" 랑 똑같은건가 체크 
	exit(1);
}

int main(int argc, char* argv[])
{
	int serv_sockfd;
	int clnt_sockfd;
	int fd;
	
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	
	char buf[MAX_BUF_LEN];
	char file_name[MAX_FILE_NAME];	// size ...?
	int file_len;	//  Check byte ordering
	
	if (argc != 2)
		error_handling("Argument error : Input -> <PORT> \n");
		
	serv_sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sockfd == -1)
		error_handling("socket() error!\n");
		
	memset(buf, 0, sizeof(buf));
	memset(&serv_addr, 0, sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
		
	if (bind(serv_sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error!\n");
		
	if (listen(serv_sockfd, 5) == -1)
		error_handling("listen() error!\n");
		
	clnt_addr_size = sizeof(clnt_addr);
	printf("Wait client .. \n");
	clnt_sockfd = accept(serv_sockfd, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if (clnt_sockfd == -1)
		error_handling("accept() error!\n");
		
	//--- 		Established		 ---//
	printf("[%s : %d]\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port); // port check
	
	if (read(clnt_sockfd, file_name, sizeof(file_name)) == -1)	// Get file name
		error_handling("read() error!\n");

	printf("name : %s\n ", file_name);
	fd = open(file_name, O_RDONLY, S_IRWXU);	//	file open
	if (fd == -1)
		error_handling("open() error\n");
	
	//--- send file size	
	file_len = lseek(fd, 0, SEEK_END);
	printf("size %d\n", file_len);
	file_len = htonl(file_len);
	
	// printf("file len : %d", file_len);
	if (write(clnt_sockfd, &file_len, sizeof(file_len)) == -1)
		error_handling("write() error \n");

	
		

	//--- send file

	close(fd);
	close(clnt_sockfd);
	close(serv_sockfd);
	
		
	
	return 0;
}
