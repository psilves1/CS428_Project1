#include <iostream>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <time.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT	 12000


int main() { 

	int sockfd, n;
	socklen_t len;
	char buffer[1024];
	struct sockaddr_in servaddr, cliaddr; 
	
	// Create a UDP socket
	// Notice the use of SOCK_DGRAM for UDP packets
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Fill server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
	servaddr.sin_port = htons(PORT); // port number
    //cliaddr.sin_addr.s_addr = INADDR_ANY;
    //cliaddr.sin_port = htons(PORT);
	
	// Bind the socket with the server address 
	//bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
	
	// random generator
	time_t start, end;
    start = time(NULL);


	//Receive the client packet along with the address it is coming from
	n = sendto(sockfd, (const char *)buffer, strlen(buffer), 
		MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));

	n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), 
			MSG_WAITALL, ( struct sockaddr *) &servaddr, &len);
    
    end = time(NULL);

	std::cout<< "client recieved message from server" << std::endl;

    std::cout << difftime(end, start) << std::endl;

	return 0; 
} 

