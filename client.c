#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

#define BUFFER_SIZE 1024
#define ARGSIZE 10
#define SIZE 104857600 // 100BM chunk of data.

char *generete_random_data()
{
    char *data;
    if ((data = malloc(SIZE)) == NULL)
    {
        perror("Error while allocating memory.\n");
        exit(1);
    }
    for (size_t i = 0; i < SIZE; i++)
    {
        data[i] = rand() % 100; // random number between 0 and 100.
    }
    return data;
}

long checksum(char *data)
{
    long sum = 0;
    for (int i = 0; i < SIZE; i++)
    {
        sum += data[i];
    }
    return sum;
}

//UDS Stream socket (client) and sending 100MB of data and checksum.
void UDSstream(char *socket_path)
{
    int client_fd;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];
    memset(buffer, '0', BUFFER_SIZE);

    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        perror("[-] Socket failed.\n");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, socket_path, sizeof(server_addr.sun_path) - 1);
    sleep(0.5);
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("[-] Connect failed.\n");
        exit(1);
    }

    printf("[+] Connected to server.\n");
    char *data = generete_random_data();
    long Chcksum = checksum(data);
    printf("Checksum : %ld\n", Chcksum);
    printf("[+] Sending data...\n");
    size_t total_bytes_sent = 0;
    int size = 30000;
    while (total_bytes_sent < SIZE)
    {
        if (total_bytes_sent + size > SIZE)
        {
            size = SIZE - total_bytes_sent;
        }
        ssize_t bytes_sent = send(client_fd, data + total_bytes_sent, size, 0);
        total_bytes_sent += bytes_sent;
    }
    printf("[+] Sent %ld bytes.\n", total_bytes_sent);
    printf("[+] Data sent.\n");
    close(client_fd);
    free(data);
}

//UDS DGRAM socket (client) and sending 100MB of data and checksum.
void UDSdgram(char *socket_path)
{
    int client_fd;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];
    memset(buffer, '0', BUFFER_SIZE);
    
    if ((client_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
    {
        perror("[-] Socket failed.\n");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, socket_path, sizeof(server_addr.sun_path) - 1);

    printf("[+] Connected to server.\n");
    char *data = generete_random_data();
    long Chcksum = checksum(data);
    printf("Checksum : %ld\n", Chcksum);
    printf("[+] Sending data...\n");
    size_t total_bytes_sent = 0;
    int size = 30000;
    while (total_bytes_sent < SIZE)
    {
        if (total_bytes_sent + size > SIZE)
        {
            size = SIZE - total_bytes_sent;
        }
        ssize_t bytes_sent = sendto(client_fd, data + total_bytes_sent, size, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        total_bytes_sent += bytes_sent;
    }
    printf("[+] Sent %ld bytes.\n", total_bytes_sent);
    printf("[+] Data sent.\n");
    close(client_fd);
    free(data);
}

//IPv4 TCP socket (client) and sending 100MB of data and checksum.
void UDPipv6(int port,char *ip)
{
    int client_fd;
    struct sockaddr_in6 server_addr;
    char buffer[BUFFER_SIZE];
    memset(buffer, '0', BUFFER_SIZE);
    if ((client_fd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0)
    {
        perror("[-] Socket failed.\n");
        exit(1);
    }
    server_addr.sin6_family = AF_INET;
    server_addr.sin6_port = htons(port+1);
    if(inet_pton(AF_INET6, ip, &server_addr.sin6_addr) <= 0)
    {
        perror("[-] Invalid address.\n");
        exit(1);
    }
    printf("[+] Connected to server.\n");
    char *data = generete_random_data();
    long Chcksum = checksum(data);
    printf("Checksum : %ld\n",Chcksum);
    printf("[+] Sending data...\n");
    size_t total_bytes_sent = 0;
    int size = 30000;
    while(total_bytes_sent < SIZE){
        if(total_bytes_sent + size > SIZE){
            size = SIZE - total_bytes_sent;
        }
        ssize_t bytes_sent = sendto(client_fd, data+total_bytes_sent, size, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        total_bytes_sent += bytes_sent;
    }
    printf("[+] Sent %ld bytes.\n", total_bytes_sent);
    printf("[+] Data sent.\n");
    close(client_fd);
    free(data);
}


//IPv4 TCP socket (client) and sending 100MB of data and checksum.
void UDPipv4(int port,char *ip)
{
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    memset(buffer, '0', BUFFER_SIZE);
    if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("[-] Socket failed.\n");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port+1);
    if(inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
    {
        perror("[-] Invalid address.\n");
        exit(1);
    }
    printf("[+] Connected to server.\n");
    char *data = generete_random_data();
    long Chcksum = checksum(data);
    printf("Checksum : %ld\n",Chcksum);
    printf("[+] Sending data...\n");
    size_t total_bytes_sent = 0;
    int size = 30000;
    while(total_bytes_sent < SIZE){
        if(total_bytes_sent + size > SIZE){
            size = SIZE - total_bytes_sent;
        }
        ssize_t bytes_sent = sendto(client_fd, data+total_bytes_sent, size, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        total_bytes_sent += bytes_sent;
    }
    printf("[+] Sent %ld bytes.\n", total_bytes_sent);
    printf("[+] Data sent.\n");
    close(client_fd);
    free(data);
}

//IPv4 TCP socket (client) and sending 100MB of data and checksum.
void TCPipv4(char *ip, int port)
{
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    memset(buffer, '0', BUFFER_SIZE);
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("[-] Socket failed.\n");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port+1);
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
    {
        perror("[-] Invalid address.\n");
        exit(1);
    }
    sleep(2);
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("[-] Connection failed.\n");
        exit(1);
    }
    printf("[+] Connected to server.\n");
    char *data = generete_random_data();
    long Chcksum = checksum(data);
    printf("Checksum : %ld\n",Chcksum);
    printf("[+] Sending data...\n");
    int sent = 0;
    sent = send(client_fd, data, SIZE, 0);
    printf("[+] Sent %d bytes.\n", sent);
    printf("[+] Data sent.\n");
    close(client_fd);
    free(data);
}

//IPv6 TCP socket (client) and sending 100MB of data and checksum.
void TCPipv6(char *ip, int port)
{
    int client_fd;
    struct sockaddr_in6 server_addr;
    char buffer[BUFFER_SIZE];
    memset(buffer, '0', BUFFER_SIZE);
    if ((client_fd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
    {
        perror("[-] Socket failed.\n");
        exit(1);
    }
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(port+1);
    if (inet_pton(AF_INET6, ip, &server_addr.sin6_addr) <= 0)
    {
        perror("[-] Invalid address.\n");
        exit(1);
    }
    sleep(2);
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("[-] Connection failed.\n");
        exit(1);
    }
    printf("[+] Connected to server.\n");
    char *data = generete_random_data();
    long Chcksum = checksum(data);
    printf("Checksum : %ld\n",Chcksum);
    printf("[+] Sending data...\n");
    int sent = 0;
    sent = send(client_fd, data, SIZE, 0);
    printf("[+] Sent %d bytes.\n", sent);
    printf("[+] Data sent.\n");
    close(client_fd);
    free(data);
}



void socketFactory(char *type, char *param,int port,char *ip){
    printf("Welcome to the socket factory.\n");
    if(strcmp(type, "uds") == 0)
    {
     char *socket_path = "/tmp/socket";
     if(strcmp(param, "dgram") == 0){
        UDSdgram(socket_path);
     }
     else{
        UDSstream(socket_path);
     }
    }

    if (strcmp(type, "ipv4") == 0)
    {
        if (strcmp(param, "tcp") == 0)
        {
            printf("TCP IPv4\n");
            TCPipv4(ip,port);
        }
        else if(strcmp(param, "udp") == 0)
        {
            printf("UDP IPv4\n");
            UDPipv4(port,ip);
        }
        else
        {
            printf("Invalid parameter.\n");
        }
    }
    if (strcmp(type, "ipv6") == 0)
    {
        ip = "::1";
        if (strcmp(param, "tcp") == 0)
        {
            printf("TCP IPv6\n");
            
            TCPipv6(ip,port);
        }
        else if(strcmp(param, "udp") == 0)
        {
            printf("UDP IPv6\n");
            UDPipv6(port,ip);
        }
        else
        {
            printf("Invalid parameter.\n");
        }
    }
}

void chatTCP(int client_fd){
    int len;
    char buffer[BUFFER_SIZE];
    memset(buffer, '0', BUFFER_SIZE);
    struct pollfd fds[2];
    fds[0].fd = client_fd;
    fds[0].events = POLLIN;
    fds[1].fd = STDIN_FILENO;
    fds[1].events = POLLIN;
    printf("[+] Client Running...\n");
    printf("[-] Chat started.\n");
    while (1)
    {
        poll(fds, 2, -1);

        if (fds[0].revents & POLLIN)
        {
            len = recv(client_fd, buffer, BUFFER_SIZE, 0);
            if (len == 0)
            {
                printf("Server closed the connection.\n");
                close(client_fd);
                exit(1);
            }
            printf("Server: %s", buffer);
            memset(buffer, '0', BUFFER_SIZE);
        }

        if (fds[1].revents & POLLIN)
        {
            fgets(buffer, sizeof(buffer), stdin);
            send(client_fd, buffer, sizeof buffer, 0);
            memset(buffer, '0', BUFFER_SIZE);
        }
    }

}

void sendArgs(int client_fd,char *type,char *param){
    printf("Sending the parameters!.\n");
    printf("Type: %s\n", type);
    printf("Param: %s\n", param);
    send(client_fd, type,ARGSIZE, 0);
    send(client_fd, param, ARGSIZE, 0);
}

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 6)
    {
        perror("Usage: ./client IP PORT\n");
        perror("Usage: ./client IP PORT -p <type> <param> \n");
        return 0;
    }
    int pFlag = 0;

    // Saving the parameters [5] and [6] in string.
    char *type;
    char *param;
    if (argc == 6 && strcmp(argv[3], "-p") == 0)
    {
        pFlag = 1;
        type = argv[4];
        param = argv[5];
    }
    const char *ip_addr = argv[1];
    int port = atoi(argv[2]);
    int client_fd, len;
    struct sockaddr_in server_addr;
  

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0)
    {
        printf("\n Error : Could not create socket \n");
        close(client_fd);
        return 1;
    }
    printf("Socket created.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_aton(ip_addr, &server_addr.sin_addr) == 0)
    {
        perror("Error while converting address \n");
        close(client_fd);
        return 1;
    }

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Connect error.");
        close(client_fd);
        return 1;
    }
    printf("Connected.\n");
    if(pFlag==0){
        chatTCP(client_fd);
    }
    else{
        sendArgs(client_fd,argv[4], argv[5]);
        socketFactory(argv[4],argv[5],port,argv[1]);
    }

    close(client_fd);
    return 0;
}