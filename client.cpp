#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <chrono>

#define PORT 12000

int main()
{

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
	servaddr.sin_family = AF_INET;		   // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY; // localhost
	servaddr.sin_port = htons(PORT);	   // port number


	struct timeval tv;

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	//setting up timeout. This will allow use to cancel the recvfrom function after 1 second.
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

	//for loop to call the 10 pings
	for (int i = 0; i < 10; i++)
	{
		auto startTime = std::chrono::high_resolution_clock::now();

		time_t start, end;
		start = time(NULL);

		//Receive the client packet along with the address it is coming from
		n = sendto(sockfd, (const char *)buffer, strlen(buffer),
				   MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));

		n = recvfrom(sockfd, (char *)buffer, sizeof(buffer),
					 MSG_WAITALL, (struct sockaddr *)&servaddr, &len);

		end = time(NULL);

		auto endTime = std::chrono::high_resolution_clock::now();

		//std::cout << "n = " << n << std::endl;

		//std::cout << difftime(end, start) << std::endl;

		std::cout << i + 1 << ") ";

		//tracking time using this because it's easier for me to directly
		//compare time in an if statement
		if(difftime(end, start) >= 1){
			std::cout << "No response recieved" << std::endl;
			continue;
		} 

		auto timeElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

		std::cout << "RTT = " << timeElapsed << " ns" << std::endl;
	}

	return 0;
}
