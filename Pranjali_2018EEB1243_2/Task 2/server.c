// Task 2: Server Side Code
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<ctype.h>
#include <time.h>
#include <sys/time.h>

// Port on which server is listening
#define PORT 8000
#define MAXSZ 200

int main()
{
    printf("\n**SERVER SIDE**\n");

    // File pointer 
    FILE *file;
    // Opening File in Write Mode
    file=fopen("server_records.txt", "w");
    if(file==NULL)
    printf("Failed to open the file\n");

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
    struct sockaddr_in server_address, client_address;
    
    // Assign domain, IP address and Port
    memset(&server_address,0,sizeof(server_address));
    server_address.sin_family=AF_INET;
    server_address.sin_addr.s_addr=htonl(INADDR_ANY);
    server_address.sin_port=htons(PORT);

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
	int listen_return = listen(server_fd, 5);
	if (listen_return < 0) 
	{
		printf("Cannot Listen to the port\n");
		exit(0);
	}

	printf("Listening To Port %d...\n", PORT);     
    
    char postfix[MAXSZ];
    int client_fd, pid, num_clients=0;
    
    int n;

    while(1)
    {
        int client_address_len=sizeof(client_address);
        
        // Step 4: Waiting For client connection to complete using accept() system call
        // Usage: accept(int server_fd, struct sockaddr *client_address, socklen_t *client_address_len)
        int client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_address_len);
        if (client_fd < 0) 
        { 
            printf("Cannot establish connection between client and server\n");
            exit(0);
        }
        num_clients++;
        
        printf("Successfully connected to Client %d: %s:%d\n", num_clients, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port)); 
        
        // Step 5: Fork a new process when new client is connected
        pid=fork();

        // If child process
        if(pid==0)
        {
            // Step 6: Receiving from client
            while(1)
            {
                struct timeval start, end;
                // Start Time: Time request received from client
                gettimeofday(&start, NULL);
                
                n=recv(client_fd,postfix,MAXSZ,0);
                printf("Postfix expression received from client: %s\n", postfix);
                
                fprintf(file, "Client Id: %s:%d Postfix expression: %s", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), postfix);
                fflush(file);

                if(n==0)
                {
                    close(client_fd);
                    break;
                }

                // Step 7: Evaluating Postfix expression received from client
                double num1, num2, res, num, len=strlen(postfix);
                double stack[100];
                int top=-1, i=0;
                char *tokens, *exp[20];
                

                //Breaking the expression into tokens separated by space
                tokens=strtok(postfix, " ");
                do{
                    exp[i]=tokens;
                    tokens=strtok(NULL, " ");
                    i++;
                }
                while(tokens!=NULL);

                for(int j=0; j<i; j++)
                {
                    if(isdigit(*exp[j]))
                    {
                        stack[++top]=atof(exp[j]);
                    }
                    else
                    {
                        char op=*exp[j];
                        num1=stack[top--];
                        num2=stack[top--];
                        switch(op)
                        {
                            case '+':
                            {
                                res=num1+num2;
                                break;
                            }
                            case '-':
                            {
                                res = num2-num1;
                                break;
                            }
                            case '*':
                            {
                                res = num1*num2;
                                break;
                            }
                            case '/':
                            {
                                res = num2/num1;
                                break;
                            }   
                        }
                        stack[++top]=res;
                    }
                }   
                char ans[10];
                double val=stack[top--];

                // Converting double to char []
                sprintf(ans , "%lf" , val);

                // Step 8: Sending evaluated value back to client
                send(client_fd,ans,sizeof(ans),0);

                // End time
                gettimeofday(&end, NULL);

                // Finding Elapsed Time
                double time_taken=(end.tv_sec - start.tv_sec)*1e6; 
                time_taken = (time_taken+(end.tv_usec-start.tv_usec))*1e-6; 

                // Writing Result To File
                fprintf(file, "Result: %s Time Elasped: %f seconds \n\n", ans, time_taken);
                fflush(file);

                printf("Value of the expression sent to client: %s\n", ans);
            }
            exit(0);
        }
        else
        {
            close(client_fd);
        }
    }

    fclose(file);
    return 0;
}