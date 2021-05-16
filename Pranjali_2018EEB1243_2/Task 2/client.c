// Task 2: Client Side Code
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

//Port on which server is listening
#define PORT 8000
#define MAXSZ 200

int main()
{
    printf("\n**CLIENT SIDE**\n");

    struct sockaddr_in server_address;
    char postfix[MAXSZ], result[MAXSZ];

    int n;

    // Step 1: Creating socket using socket() system call 
	// Usage: socket(domain, type, protocol)
    // It creates an endpoint for communication and returns a descriptor.
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0) 
	{ 
		printf("Socket Creation Failed\n");
		exit(0); 
	}

	printf("Socket Created Successfully\n");
    
    // Assign domain, IP address and Port
    memset(&server_address,0,sizeof(server_address));
    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr=htonl(INADDR_ANY);
    server_address.sin_port=htons(PORT);
	
    // Step 2: Connect to server using connect() system call
	// Usage: connect(int client_fd, struct sockaddr *server_address, int server_address_len);
	// It binds the socket to the address and port number specified in addr
    int connect_return = connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address));
	if (connect_return < 0) 
	{ 
		printf("Failed to connect\n"); 
		exit(0);
	}

	printf("Succesfully connected to server\n"); 
    
    while(1)
    {
        printf("Enter the postfix expression that is to be evaluated: ");
        fgets(postfix,MAXSZ,stdin);
        n=strlen(postfix)+1;

        // Step 3: Sending String to the server using send()
	    // Usage: send(int client_fd, char *buffer, int nbytes, int flags)
        send(client_fd, postfix, n, 0);

        // Step 4: Receive the String from server using read()
	    // Usage: read(int client_fd, char *buffer, int len)
        n=recv(client_fd, result, MAXSZ, 0);

        printf("Value of expression received from server: %s\n",result);
    }
    
    // Step 5: Closing the socket connection
	close(client_fd);

    return 0;
}