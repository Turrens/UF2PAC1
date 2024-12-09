#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
using namespace std;
#pragma comment(lib, "Ws2_32.lib") // Enlazar Winsock

#define PORT 8080

int main() {
    WSADATA wsaData;
    SOCKET client_socket;
    struct sockaddr_in server_address;
    const char* message = "Hola servidor!";
    char buffer[1024] = {0};

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error al inicializar Winsock.\n";
        return -1;
    }

    // Crear el socket del cliente
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cerr << "Error al crear el socket: " << WSAGetLastError() << "\n";
        WSACleanup();
        return -1;
    }

    // Configurar la dirección del servidor
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // Convertir IP a formato adecuado

    if (server_address.sin_addr.s_addr == INADDR_NONE) {
        cerr << "Error en la dirección IP.\n";
        closesocket(client_socket);
        WSACleanup();
        return -1;
    }

    // Conectar al servidor
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        cerr << "Error al conectar con el servidor: " << WSAGetLastError() << "\n";
        closesocket(client_socket);
        WSACleanup();
        return -1;
    }

    cout << "Connexió establerta amb el servidor.\n";

    // Enviar el mensaje al servidor
    send(client_socket, message, strlen(message), 0);
    cout << "Enviant missatge: " << message << endl;

    // Recibir la respuesta del servidor
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0'; // Asegurar el fin de la cadena
        cout << "Resposta del servidor: " << buffer << endl;
    } else {
        cerr << "Error al recibir la respuesta: " << WSAGetLastError() << "\n";
    }

    // Cerrar el socket
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
