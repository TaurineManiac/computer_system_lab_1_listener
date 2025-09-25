#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#pragma warning(disable:4996)
#include <windows.h>

using namespace std;

string chooseQuarter(double x,double y) {
    if (x == 0 && y == 0) {
        return "точка 0";
    }
    else if (x==0) {
        return "на прямой y";
    }
    else if (y==0) {
        return "на прямой x";
    }
    else if (x>0 && y>0) {
        return "точка в первой четверти";
    }
    else if (x>0 && y<0) {
        return "точка в четвёртой четверти";
    }
    else if (x<0 && y<0) {
        return "точка в третей четверти";
    }
    else if (x<0 && y>0) {
        return "точка в второй четверти";
    }
}

int main() {

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    cout << "13.Осуществить взаимодействие клиента и сервера на основе протокола TCP/IP. \n"
            "Функционирование клиента и сервера реализовать следующим образом: клиент посылает серверу координаты точки Х и У в "
            "декартовой системе координат.\n "
            "Определить в какой координатной четверти находится данная точка.\n" << endl;
    WSAData wSAData;
    WORD wVersion = MAKEWORD(2, 2);
    //для себя поясняю что первая цифра это номер максимальный версии, который мы тут можем использовать
    //а вторая это та, что будет использоваться
    if (WSAStartup(wVersion, &wSAData)!=0) {
        cout << "WSAStartup failed with error: " << WSAGetLastError() << endl;
        //если библиотека загрузилась удачно, то она вернёт значение 0
        WSACleanup();
        return 1;
    }

    SOCKADDR_IN addr;
    int size = sizeof(addr);
    //переменная предназначеная для хранения адреса, для инет протоколов используется sockaddr_in
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //это работае так что адрес это поле в виде класса в классе SOCKADDR_IN, поэтому чтобы установить конкретное поле класса адреса,нужно обратиться к полю
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    //    семейство адресов интернет,тип  сокета(тут потоквый,т.е. с соединением), оператор, при 0 подберается автоматически по умолчанию(IPPROTO_TCP)
    bind(sock, (SOCKADDR*)&addr, sizeof(addr));
    listen(sock, SOMAXCONN);
    //если я здесь поставлю ограничение, например 3, то тогда только 3 станут в очередь, а остальные выдадут ошибку

    SOCKET newConnection;
    newConnection = accept(sock, (SOCKADDR*)&addr, &size);
    //я создал новый канал(сокет), чтобы перейти от одноканального к многоканальному, чтобы основной сокет продолжал слушать, а другой уже обрабатывался
    if (newConnection == INVALID_SOCKET) {
        cout << "accept failed with error: " << WSAGetLastError() << endl;
    }
    else {
        cout << "Connection established." << endl;
    }
    char strX[256];
    char strY[256];
    recv(newConnection,strX,sizeof(strX),0);
    recv(newConnection,strY,sizeof(strY),0);
    cout << strX << " " << strY << endl;
    double x = strtod(strX, NULL);
    double y = strtod(strY, NULL);
    string result = chooseQuarter(x,y);
    send(newConnection, result.c_str(), result.size()+ 1, 0);

    closesocket(newConnection);
    closesocket(sock);
    WSACleanup();
}