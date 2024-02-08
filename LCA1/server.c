#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

#define MAX_CLIENTS 5

struct client_details
{
    int sockid;
    char name[80];
};

void *handle_client(void *arg)
{
    struct client_details *client = (struct client_details *)arg;
    char message[200];
    // int flag=0;
    
        ssize_t name_len = read(client->sockid, client->name, sizeof(client->name));
        if (name_len <= 0)
        {
            close(client->sockid);
            free(client);
            pthread_exit(NULL);
        }
        client->name[name_len] = '\0';
        printf("%s has connected\n", client->name);
        while (1)
        {
            ssize_t message_len = read(client->sockid, message, sizeof(message));
            if (message_len <= 0)
            {
                
                break;
            }
            message[message_len] = '\0'; 

            if (strcmp(message, "bye") == 0)
            { // Remove '\n' from "bye"
                printf("%s has left the chat.\n", client->name);
                break;
            }

            printf("%s sent - %s\n", client->name, message);
        }
    

    close(client->sockid);
    free(client);
    pthread_exit(NULL);
}

int main()
{
    int server_sockid;
    struct sockaddr_in server_addr;
    pthread_t threads[MAX_CLIENTS];

    server_sockid = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockid == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(9129);

    if (bind(server_sockid, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Bind failed");
        close(server_sockid);
        exit(EXIT_FAILURE);
    }

    printf("Server is waiting for client connections - \n");
    listen(server_sockid, MAX_CLIENTS);

    while (1)
    {
        struct sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);

        int client_sockid = accept(server_sockid, (struct sockaddr *)&client_address, &client_len);
        if (client_sockid == -1)
        {
            perror("Acceptance failed");
            continue;
        }

        struct client_details *client = (struct client_details *)malloc(sizeof(struct client_details));
        if (client == NULL)
        {
            perror("Memory allocation has failed");
            close(client_sockid);
            continue;
        }

        client->sockid = client_sockid;
        pthread_create(&threads[MAX_CLIENTS], NULL, handle_client, (void *)client);
    }

    close(server_sockid);
    exit(EXIT_SUCCESS);
}