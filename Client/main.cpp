#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <string>
#include <exception>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma warning(disable: 4996)

SOCKET Connection;

enum Packet {
	P_ChatMessage,
	P_Test
};

bool ProcessPacket(Packet packettype) {
	switch (packettype) {
	case P_ChatMessage:
	{
		int msg_size;
		recv(Connection, (char*)& msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connection, msg, msg_size, NULL);
		std::cout << msg << std::endl;
		delete[] msg;
		break;
	}
	case P_Test:
		std::cout << "Test packet. \n";
		break;
	default:
		std::cout << "Unrecogized packet:" << packettype << std::endl;
		break;
	}
	return true;
}

void ClientHandler() {
	Packet packettype;
	while (true) {
		recv(Connection, (char*)& packettype, sizeof(Packet), NULL);

		if (!ProcessPacket(packettype)) {
			break;
		}
	}
	closesocket(Connection);
}

int main(int arcg, char* argv[]) {
	try {
		//WSAStartup
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 1);
		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			std::cout << "Error" << std::endl;
			exit(1);
		}

		SOCKADDR_IN addr;
	    std::string ip;
	    std::string nick;
		std::cout << "Please, enter ip: " << std::endl;
		std::cin >> ip;
		std::cout << "Please, enter your nick: ";
		std::cin >> nick;
		int sizeofaddr = sizeof(addr);
		addr.sin_addr.s_addr = inet_addr(ip.c_str());
		addr.sin_port = htons(1111);
		addr.sin_family = AF_INET;

		Connection = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(Connection, (SOCKADDR*)& addr, sizeof(addr)) != 0) {
			std::cout << "Error: failed connection to server.\n";
			return 1;
		}
		std::cout << "Connected!\n";

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

		std::string msg1;
		while (true) {
			std::getline(std::cin, msg1);
			std::string fullmsg;
			fullmsg = "[" + nick + "]: " + msg1;
			int msg_size = fullmsg.size();
			Packet packettype = P_ChatMessage;
			send(Connection, (char*)& packettype, sizeof(Packet), NULL);
			send(Connection, (char*)& msg_size, sizeof(int), NULL);
			send(Connection, fullmsg.c_str(), msg_size, NULL);
			Sleep(10);
		}
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}


	system("pause");
	return 0;
}