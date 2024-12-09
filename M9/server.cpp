#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
using namespace std;
#pragma comment(lib, "Ws2_32.lib") // Enlazar Winsock

#define PORT 8080

int main() {
    WSADATA wsaData;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char* response = "Hola des del servidor!";

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error al inicializar Winsock.\n";
        return -1;
    }

    // Crear el socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cerr << "Error al crear el socket: " << WSAGetLastError() << "\n";
        WSACleanup();
        return -1;
    }

    // Configurar la dirección del servidor
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Asociar el socket al puerto
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        cerr << "Error al hacer el bind: " << WSAGetLastError() << "\n";
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    // Escuchar conexiones entrantes
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        cerr << "Error al escuchar: " << WSAGetLastError() << "\n";
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    cout << "Esperant connexions...\n";

    // Aceptar una conexión
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) == INVALID_SOCKET) {
        cerr << "Error al aceptar la conexión: " << WSAGetLastError() << "\n";
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    // Recibir el mensaje del cliente
    int bytes_received = recv(new_socket, buffer, sizeof(buffer), 0);
    if (bytes_received == SOCKET_ERROR) {
        cerr << "Error al recibir datos: " << WSAGetLastError() << "\n";
    } else {
        buffer[bytes_received] = '\0'; // Asegurarse de que el buffer está terminado en '\0'
        cout << "Missatge rebut del client: " << buffer << endl;
    }

    // Enviar la respuesta al cliente
    if (send(new_socket, response, strlen(response), 0) == SOCKET_ERROR) {
        cerr << "Error al enviar la respuesta: " << WSAGetLastError() << "\n";
    } else {
        cout << "Resposta enviada al client.\n";
    }

    // Cerrar el socket
    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();

    return 0;
}
