// Task 1: Server Side Code
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 

// Port on which server is listening
#define PORT 3000

int main() 
{ 
	printf("\n**SERVER SIDE**\n");
	struct sockaddr_in server_address;
	int server_address_len=sizeof(server_address);  
	char received_string[500]; 

	// Step 1: Creating socket using socket() system call 
	// Usage: socket(domain, type, protocol)
	// It creates an endpoint for communication and returns a descriptor.
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) 
	{ 
		printf("Socket Creation Failed\n");
		exit(0); 
	}

	printf("Socket Created Successfully\n");

	// Assign domain, IP address and Port
	server_address.sin_family = AF_INET; 
	server_address.sin_addr.s_addr = INADDR_ANY; 
	server_address.sin_port = htons(PORT); 

	// Step 2: Binding server's address using bind() system call
	// Usage: bind(int server_fd, const struct sockaddr *server_address, socklen_t server_address_len);
	// It binds the socket to the address and port number specified in addr
	int bind_return = bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address));
	if (bind_return < 0) 
	{
		printf("Binding Failed\n"); 
		exit(0); 
	}

	printf("Binding Done Succesfully\n");

	// Step 3: Converting socket to listening socket using listen() system call
	// Usage: listen(int server_fd, int backlog);
    // It puts the server socket in a passive mode, where it waits for the client to approach the server to make a connection
	int listen_return = listen(server_fd, 3);
	if (listen_return < 0) 
	{
		printf("Cannot Listen to the port\n");
		exit(0);
	}

	printf("Listening To Port %d...\n", PORT); 

	// Step 4: Waiting For client connection to complete using accept() system call
	// Usage: accept(int server_fd, struct sockaddr *server_address, socklen_t *server_address_len)
	int client_socket = accept(server_fd, (struct sockaddr*)&server_address, (socklen_t*)&server_address_len);
	if (client_socket < 0) 
	{ 
		printf("Cannot establish connection between client and server\n");
		exit(0);
	}

	printf("Client and Server Connected Succesfully\n"); 

	// Step 5: Receiving the client request using recv() system call
	// Usage: recv(int server_fd, char *buffer, int nbytes, int flags);
	int received_request = recv(client_socket, received_string, sizeof(received_string), 0);
	
	printf("String received from client: %s\n", received_string);

	// Step 6: Performing the task, reversing the received string
	int left=0, right=strlen(received_string)-1;
	while(left<right)
	{
		char temp=received_string[left];
		received_string[left]=received_string[right];
		received_string[right]=temp;
		left++;
		right--;
	}

	// Step 7: Sending the result to the client using send() system call
	// Usage: send(int server_fd, char *buffer, int nbytes, int flags);
	send(client_socket, received_string, sizeof(received_string), 0);

	printf("Reversed string sent to the client: %s\n", received_string); 

	// Step 8: Close the socket connection using close()
	close(server_fd);

	return 0; 
} 
