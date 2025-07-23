#ifndef INTERFACE_H
#define INTERFACE_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>
#include <cstring>
#include <vector>
#include <mutex>

#include "Connection.h"

struct ChatMessage {
	std::string author;
	std::string text;
};

static char inputBuffer[256] = ""; // Buffer for input text

class Interface {
public:
	Interface() {
		memset(clientName, 0, sizeof(clientName)); // Clear buffer
	}

	void SetupStyle() const;
	void DrawInterface();
	void SendMessageInterface();

	void ConnectionSettingsInterface();
	void CreateServerInterface();
	void CreateChatWindow();
	void DrawDebugWindow();

	void AddDebugMessage(const std::string& message);

	void HelpMarker(const char* desc);

	Connection connect;

	std::vector<ChatMessage> chatMessages;

protected:
	char clientName[256]; // Buffer under the name

	std::vector<std::string> debugMessages;
	std::mutex debugMutex;
};

#endif