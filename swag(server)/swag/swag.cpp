#define WIN32_LEAN_AND_MEAN
#include  <Windows.h>
#include <WS2tcpip.h>
#include <Winsock2.h> // подключаем необходимые библиотеки
#include <iostream>

using namespace std;
int main()

{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    WSADATA wsaData;
    ADDRINFO hints;
    ADDRINFO* addrResult;
    const char* sendBuffer = "Hello from Server"; 

    SOCKET ConnectSocket = INVALID_SOCKET;
    SOCKET ListenSocket = INVALID_SOCKET;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    char recvBuffer[512];
    // создаем необходимые переменные


    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cout << "беда" << endl;
        return 1;
    } // здесь мы задаем сообщение об ошибке с самого начала


    result = getaddrinfo(NULL, "666", &hints, &addrResult); // здесь мы получаем пармаетры для нашего соединения
    if (result != 0) {
        cout << "беда 2" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }// обрабатываем ошибку

    ListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol); // создаем сокет, куда передаем значения нашего адреса для подключения
    if (ListenSocket == INVALID_SOCKET) {
        cout << "Сокет не создан" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }// обрабатываем ошибку

    result = bind(ListenSocket, addrResult->ai_addr, addrResult->ai_addrlen); // создаем подключение между нашим сокетом и прослушиваемым
    if (result == SOCKET_ERROR) {
        cout << "binding error to server <3" << endl;
        closesocket(ListenSocket);
        ListenSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        return 1;
    }// обрабатываем ошибку

    result = listen(ListenSocket, SOMAXCONN); // прослушиваем сокет
    if (result == SOCKET_ERROR) {
        cout << "Error to listen <3" << endl;
        closesocket(ListenSocket);
        ListenSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        return 1;
    }// обрабатываем ошибку

    ConnectSocket = accept(ListenSocket, NULL, NULL); // принимаем подключение сокета
    if (result == SOCKET_ERROR) {
        cout << "accepting socket failed <3" << endl;
        WSACleanup();
        return 1;
    }// обрабатываем ошибку


    do {
        ZeroMemory(recvBuffer, 512, 0);
        result = recv(ConnectSocket, recvBuffer, 512, 0); // получаем значения о размере полученных данных
        if (result > 0) {

            cout << "received: " << result << "bytes" << endl;
            cout << "received data: " << recvBuffer << endl;

            result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);

            if (result == SOCKET_ERROR) {
                cout << "send failde error" << endl;
                freeaddrinfo(addrResult);
                WSACleanup();
                return 1;
            } // обрабатываем ошибку

        } // получаем сообщения от сокета(клиента)
        else if (result == 0) {
            cout << "Connection closed" << endl;
        }
        else {
            cout << "received with ERRROR" << endl;
        } // ошибки

    } while (result > 0);

    result = shutdown(ConnectSocket, SD_SEND); // закрываем соединение и передаем код 
    if (result == SOCKET_ERROR) {
        cout << "shutdown failde error" << endl;
        freeaddrinfo(addrResult); // освобождение памяти
        WSACleanup();
        return 1;
    }
    closesocket(ListenSocket); // закрываем подключение
    freeaddrinfo(addrResult);
    WSACleanup();
    return 1;
}