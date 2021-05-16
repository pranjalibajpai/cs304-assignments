// Task 1: Client Side Code
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 

//Port on which server is listening
#define PORT 3000
 
int main() 
{ 
	printf("\n**CLIENT SIDE**\n");

	struct sockaddr_in client_address, server_address; 
	char sent_string[500], received_string[500]; 
	
	printf("Enter the string to be sent to server: ");
	scanf("%[^\n]s", sent_string); 

	// Step 1: Creating socket using socket() system call 
	// Usage: socket(domain, type, protocol)
	// It creates an endpoint for communication and returns a descriptor
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0) 
	{ 
		printf("Socket Creation Failed\n");
		exit(0); 
	}

	printf("Socket Created Successfully\n");

	memset(&server_address, '0', sizeof(server_address)); 

	// Assign domain and Port
	server_address.sin_family = AF_INET; 
	server_address.sin_port = htons(PORT); 

	// Converting IPv4 and IPv6 addresses from text to binary form 
	int convert_return = inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
	if (convert_return <= 0) 
	{ 
		printf("Address not supported\n"); 
		exit(0);
	} 

	// Step 2: Connect to server using connect() system call
	// Usage: connect(int client_fd, struct sockaddr *server_address, int server_address_len);
	// It binds the socket to the address and port number specified in addr
	int connect_return = connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address));
	if (connect_return < 0) 
	{ 
		printf("Failed to connect\n"); 
		exit(0);
	}

	printf("Succesfully connected\n"); 

	// Step 3: Sending String to the server using send()
	// Usage: send(int client_fd, char *buffer, int nbytes, int flags)
	send(client_fd, sent_string, sizeof(sent_string), 0); 

	// Step 4: Receive the String from server using read()
	// Usage: read(int client_fd, char *buffer, int len)
	int received_val = read(client_fd, received_string, sizeof(received_string)); 

	printf("String sent by server: %s\n", received_string); 

	// Step 5: Closing the socket connection
	close(client_fd);

	return 0; 
} 
