#include "include/Connection.h"
#include <ws2tcpip.h>
#include <thread>
#include <iostream>

Connection::~Connection() {
	Disconnect();
}

bool Connection::InitializeConnection(Connection::ErrorCallback error) {
	onError = error;

#ifdef _WIN32
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		HandleError("WSAStartup failed");
		return false;
	}
#endif // _WIN32
	return true;
}

void Connection::HandleError(const std::string& message) {
	if (onError)
	{
		onError(message);
	}

#ifdef _WIN32
	std::cerr << message << ". Error: " << WSAGetLastError() << "\n";
#endif // _WIN32
}

bool Connection::StartServer(int port) {
	if (running)
	{
		return false;
	}

	isServer = true;
	std::thread([this, port] { ServerThread(port); }).detach();
	
	return true;
}

void Connection::ServerThread(int port) {
#ifdef _WIN32
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serverSocket == INVALID_SOCKET)
	{
		HandleError("Failed to create server socket");
		return;
	}

	sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)))
	{
		HandleError("Bind failed");
		closesocket(serverSocket);

		return;
	}

	if (listen(serverSocket, SOMAXCONN)) {
		HandleError("Listen failed");
		closesocket(serverSocket);

		return;
	}

	running = true;
	connected = true;

	sockaddr_in clientAddr;
	int clientSize = sizeof(clientAddr);
	clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);

	if (clientSocket == INVALID_SOCKET) {
		HandleError("Accept failed");
		
		running = false;
		connected = false;

		return;
	}

	char clientIP[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
	
	HandleError("Client connected: " + std::string(clientIP));

	ClientThread();
#endif // _WIN32
}

bool Connection::ConnectToServer(const std::string& ip, int port) {
	if (running) {
		return false;
	}

	isServer = false;

	std::thread([this, ip, port] {
	#ifdef _WIN32
		clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (clientSocket == INVALID_SOCKET)
		{
			HandleError("Failed to create client socket");
			return;
		}

		sockaddr_in serverAddr;
		
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(port);
		
		inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

		if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)))
		{
			HandleError("Connection failed");
			closesocket(clientSocket);

			return;
		}

		connected = true;
		running = true;

		HandleError("Connected to server!");
		ClientThread();
	#endif // _WIN32
	}).detach();

	return true;
}

void Connection::ClientThread() {
	char buffer[4096];

	while (running) {
		int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

		if (bytesReceived <= 0) 
		{
			Disconnect();
			HandleError("Connection lost");

			break;
		}

		std::string message(buffer, bytesReceived);

		if (onMessage) 
		{
			onMessage(message);
		}
	}
}

void Connection::SendMessageConnection(const std::string& message) {
	if (!connected) {
		return;
	}

#ifdef _WIN32
	send(clientSocket, message.c_str(), message.size(), 0);
#endif // _WIN32
}

void Connection::Disconnect() {
	running = false;
	connected = false;
	isServer = false;

#ifdef _WIN32
	if (clientSocket != INVALID_SOCKET) {
		closesocket(clientSocket);
		
		clientSocket = INVALID_SOCKET;
	}
	if (serverSocket != INVALID_SOCKET) {
		closesocket(serverSocket);

		serverSocket = INVALID_SOCKET;
	}
#endif // _WIN32
}