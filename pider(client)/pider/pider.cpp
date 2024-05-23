#define WIN32_LEAN_AND_MEAN
#include  <Windows.h>
#include <WS2tcpip.h>
#include <Winsock2.h>
#include <iostream>

using namespace std;
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    WSADATA wsaData;
    ADDRINFO hints;
    ADDRINFO* addrResult;
    SOCKET ConnectSocket = INVALID_SOCKET;
    const char* sendBuffer = "Hello from Client";
    const char* sendBuffer2 = "pip";
    char recvBuffer[512];
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    // так же создаем классические соединения
    
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData); // запускаем сервис соединения
    if (result != 0) {
        cout << "беда" << endl;
        return 1;
    }




    result = getaddrinfo("localhost", "666", &hints, &addrResult); // получаем параметры для соединения
    if (result != 0) {
        cout << "беда 2" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    } // обрабатываем ошибку
    ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol); // создаем сокет, куда передаем значения адреса для подключения
    if (ConnectSocket == INVALID_SOCKET) {
        cout << "Сокет не создан" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }// обрабатываем ошибку
    result = connect(ConnectSocket, addrResult->ai_addr, addrResult->ai_addrlen); // создаем подключение
    if (result == SOCKET_ERROR) {
        cout << "нет, сервера нема" << endl;
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        return 1;
    }// обрабатываем ошибку

    result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0); // отправляем на наш сокет sendBuffer
    if (result == SOCKET_ERROR) {
        cout << "send failde error" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }// обрабатываем ошибку
    cout << "send " << result << endl;

    result = send(ConnectSocket, sendBuffer2, (int)strlen(sendBuffer), 0); // отправляем на наш сокет sendBuffer2
    if (result == SOCKET_ERROR) {
        cout << "send failde error" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }// обрабатываем ошибку
    cout << "send " << result << endl;

    result = shutdown(ConnectSocket, SD_SEND); // закрываем подключение
    if (result == SOCKET_ERROR) {
        cout << "shutdown failde error" << endl;
        closesocket(ConnectSocket);
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }// обрабатываем ошибку

    do {
        ZeroMemory(recvBuffer, 512, 0);
        result = recv(ConnectSocket, recvBuffer, 512, 0);
        if (result > 0) {
            cout << "received " << result << " bytes" << endl;
            cout << "received data " << recvBuffer << endl;
        } // получаем сообщения от сокета(сервера)
        else if (result == 0) {
            cout << "Connection closed" << endl;
        }
        else {
            cout << "received with ERRROR" << endl;
        }
        // обрабатываем ошибки
    } while (result > 0);
    freeaddrinfo(addrResult);
    WSACleanup();
    return 1;
}

