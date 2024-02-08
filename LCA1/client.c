#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main() {
    int sockid;
    struct sockaddr_in server_addr;
    char name[80], message[200];

    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    sockid = socket(AF_INET, SOCK_STREAM, 0);
    if (sockid == -1) {
        perror("Socket creation has failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(9129);

    if (connect(sockid, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection has failed!");
        close(sockid);
        exit(EXIT_FAILURE);
    }
    write(sockid, name, strlen(name));
    while (1) {
        printf("Input message ('bye' to exit): ");
        fgets(message, sizeof(message), stdin);
        
        message[strcspn(message, "\n")] = '\0';
        write(sockid, message, strlen(message));

        printf("%s is sending - %s\n",name, message);
        
        if (strcmp(message, "bye") == 0) {
            printf("Chat has been terminated.\n");
            break;
        }

    }

    close(sockid);
    exit(EXIT_SUCCESS);
}