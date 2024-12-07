/* Project -> Project options -> Parameters -> Linker: -lws2_32 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 53477
#define BUFFER_SIZE 1024

int contaVocali(const char *str) {
    int count = 0;
    char c;
    while ((c = *str++) != '\0') {
        c = tolower(c);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int main() {
    WSADATA wsaData;
    SOCKET serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    int clientAddrLen = sizeof(clientAddr);
    struct hostent *clientHost;

    // Inizializzazione Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Errore: WSAStartup fallito.\n");
        return 1;
    }

    // Creazione del socket UDP
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        printf("Errore: socket() fallito.\n");
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Errore: bind() fallito.\n");
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

   
    // 1. Il server è avviato sulla porta 53477 e resta sempre in ascolto
    printf("Server UDP in ascolto sulla porta %d...\n", PORT);
	while(true) { 		    
	    // 4. Il server riceve la stringa e la visualizza sullo std output;
	   	int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (bytesReceived == SOCKET_ERROR) {
            printf("Errore: recvfrom() fallito.\n");
            continue;
        }
        buffer[bytesReceived] = '\0'; 
        printf("Stringa ricevuta dal client: %s\n", buffer);
        
        clientHost = gethostbyaddr((const char *)&clientAddr.sin_addr, sizeof(clientAddr.sin_addr), AF_INET);
	    if (clientHost == NULL) {
	        printf("Errore nella risoluzione dell'indirizzo del client.\n");
	    } 
	    
	    // 4.5 Il server conta e visualizza il numero di vocali presenti nella stringa ricevuta;
	    int numeroVocali = contaVocali(buffer);
	    printf("Numero di vocali nella stringa: %d\n", numeroVocali);
	
	    // 4.6 Il server invia al client un intero N, pari al numero di vocali trovate;
	    sendto(serverSocket, (const char *)&numeroVocali, sizeof(numeroVocali), 0, (struct sockaddr *)&clientAddr, clientAddrLen);
	}
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

