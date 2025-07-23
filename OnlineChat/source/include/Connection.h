#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <functional>
#include <atomic>

#ifdef _WIN32
	#include <winsock2.h>
	#pragma comment(lib, "ws2_32.lib")
#else
// Linux/Mac
#endif

class Connection {
public:
	using ErrorCallback = std::function<void(const std::string&)>;
	using MessageCallback = std::function<void(const std::string&)>;

	Connection() = default;
	~Connection();
	
	// Network functions
	bool InitializeConnection(ErrorCallback error);
	bool StartServer(int port);
	bool ConnectToServer(const std::string& ip, int port);
	
	void Disconnect();

	void SendMessageConnection(const std::string& message);

	void SetMessageCallback(MessageCallback callback) { 
		onMessage = callback; 
	}

	// If there is, then it's true
	bool isName() const {
		return !name.empty();
	}

	bool isPort() const {
		return serverPort > 0;
	}

	bool isIP() const {
		return !serverIP.empty();
	}

	bool isConnected() const {
		return connected;
	}

	void Connect() {
		connected = true;
	}

	std::string serverIP;
	std::string name;

	int serverPort = 0;

	std::atomic<bool> connected{ false };
	std::atomic<bool> isServer{ false };

private:
#ifdef _WIN32
	SOCKET serverSocket = INVALID_SOCKET;
	SOCKET clientSocket = INVALID_SOCKET;
#endif // _WIN32

	std::atomic<bool> running{ false };
	ErrorCallback onError;
	MessageCallback onMessage;

	void ServerThread(int port);
	void ClientThread();
	void HandleError(const std::string& message);

};

#endif