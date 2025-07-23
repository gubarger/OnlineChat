#include "include/Interface.h"
#include <cstdlib>
#include <iostream>

void Interface::SetupStyle() const {
	ImGuiStyle& style = ImGui::GetStyle();

	// Main colors
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.08f, 0.95f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.50f, 0.80f, 0.50f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);

	// Text and accents
	style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.90f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.45f, 0.50f, 1.00f);

	// Headings and buttons
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.07f, 0.10f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.20f, 0.30f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.40f, 0.70f, 0.50f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.50f, 0.80f, 0.80f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.60f, 1.00f, 1.00f);

	// Sliders and controls
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.60f, 1.00f, 0.50f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.70f, 1.00f, 1.00f);

	// Selected elements (checkboxes, radio buttons)
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.80f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.00f, 0.50f, 0.80f, 0.50f);

	// Scrollbar
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 0.40f, 0.70f, 0.50f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 0.50f, 0.80f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 0.60f, 1.00f, 1.00f);

	// Setting up rounding and indentation
	style.WindowRounding = 5.0f;
	style.FrameRounding = 3.0f;
	style.ScrollbarRounding = 5.0f;
	style.GrabRounding = 2.0f;
	style.PopupRounding = 5.0f;
}

void Interface::DrawInterface() {
	// Connection settings
	ConnectionSettingsInterface();
	CreateServerInterface();
	CreateChatWindow();
	DrawDebugWindow();
}

void Interface::ConnectionSettingsInterface() {
	ImGui::Begin("Connection Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		// Client name section
		ImGui::Text("Client configuration");
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::InputText("Client name", clientName, sizeof(clientName)))
		{
			connect.name = clientName;
		}
		ImGui::SameLine();
		HelpMarker("Enter your client display name");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Connection settings section
		ImGui::Text("Network Configuration");
		ImGui::Separator();
		ImGui::Spacing();

		// Using columns
		ImGui::Columns(2, "connectionColumns", false);
		ImGui::SetColumnWidth(0, 80.0f);

		// Port input
		ImGui::Text("Port:");
		ImGui::NextColumn();

		static char portBuf[16] = "0000"; // Default value

		ImGui::SetNextItemWidth(-FLT_MIN);
		if (ImGui::InputText("##Port", portBuf, IM_ARRAYSIZE(portBuf), ImGuiInputTextFlags_CharsDecimal))
		{
			connect.serverPort = atoi(portBuf);
		}

		ImGui::NextColumn();

		// IP input
		ImGui::Text("IP address:");
		ImGui::NextColumn();

		static char ipBuf[64] = "000.0.0.0"; // Default value

		ImGui::SetNextItemWidth(-FLT_MIN);
		if (ImGui::InputText("##IP", ipBuf, IM_ARRAYSIZE(ipBuf), ImGuiInputTextFlags_CharsDecimal))
		{
			connect.serverIP = ipBuf;
		}

		ImGui::Columns(1); // Reset columns

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Connection buttons
		if (ImGui::Button("Connect", ImVec2(120, 30))) 
		{
			if (connect.isIP() && connect.isName() && connect.isPort()) 
			{
				if (!connect.ConnectToServer(connect.serverIP, connect.serverPort))
				{
					AddDebugMessage("Connection attempt failed");
				}
			}
			else 
			{
				AddDebugMessage("Missing connection parameters");
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Disconnect", ImVec2(120, 30)))
		{
			connect.Disconnect();
			AddDebugMessage("Disconnected");
		}
	}
	ImGui::End();
}

void Interface::CreateServerInterface() {
	ImGui::Begin("Connection Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	{
		// Server name section
		ImGui::Text("Server configuration");
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::InputText("Server name", clientName, sizeof(clientName)))
		{
			connect.name = clientName;
		}
		ImGui::SameLine();
		HelpMarker("Enter your server display name");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Server settings section
		ImGui::Text("Network Configuration");
		ImGui::Separator();
		ImGui::Spacing();

		// Using columns
		ImGui::Columns(2, "connectionColumns", false);
		ImGui::SetColumnWidth(0, 80.0f);

		// Port input
		ImGui::Text("Port:");
		ImGui::NextColumn();

		static char portBuf[16] = "0000"; // Default value

		ImGui::SetNextItemWidth(-FLT_MIN);
		if (ImGui::InputText("##Server Port", portBuf, IM_ARRAYSIZE(portBuf), ImGuiInputTextFlags_CharsDecimal))
		{
			connect.serverPort = atoi(portBuf);
		}

		ImGui::NextColumn();

		// IP input
		ImGui::Text("IP address:");
		ImGui::NextColumn();

		static char ipBuf[64] = "000.0.0.0"; // Default value

		ImGui::SetNextItemWidth(-FLT_MIN);
		if (ImGui::InputText("##Server IP", ipBuf, IM_ARRAYSIZE(ipBuf), ImGuiInputTextFlags_CharsDecimal))
		{
			connect.serverIP = ipBuf;
		}

		ImGui::Columns(1); // Reset columns

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Connection buttons
		if (ImGui::Button("Create", ImVec2(120, 30))) 
		{
			if (connect.isPort()) 
			{
				if (!connect.StartServer(connect.serverPort)) 
				{
					AddDebugMessage("Failed to start server");
				}
				else
				{
					AddDebugMessage("Server started on port: " + std::to_string(connect.serverPort));
				}
			}
			else
			{
				AddDebugMessage("Port is required for server");
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Stop", ImVec2(120, 30))) 
		{
			connect.Disconnect();
			AddDebugMessage("Server stopped");
		}
	}
	ImGui::End();
}

void Interface::CreateChatWindow() {
	// Draw chat window
	ImGui::Begin("Chat", nullptr, ImGuiWindowFlags_NoCollapse && ImGuiWindowFlags_AlwaysAutoResize);

	// Message display area
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false,
		ImGuiWindowFlags_HorizontalScrollbar);

	for (const auto& msg : this->chatMessages)
	{
		ImGui::TextWrapped("%s: %s", msg.author.c_str(), msg.text.c_str());
		ImGui::Separator();
	}

	// Automatic scroll down
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
	{
		ImGui::SetScrollHereY(1.0f);
	}
	ImGui::EndChild();

	// Input area
	ImGui::Separator();
	if (ImGui::InputText("##Input", inputBuffer, IM_ARRAYSIZE(inputBuffer),
		ImGuiInputTextFlags_EnterReturnsTrue))
	{
		SendMessageInterface();
	}

	ImGui::SameLine();
	if (ImGui::Button("Send") || (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter)))
	{
		SendMessageInterface();
	}
	ImGui::End();
}

void Interface::DrawDebugWindow() {
	ImGui::Begin("Debug Console", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	std::lock_guard<std::mutex> lock(debugMutex);

	for (const auto& msg : debugMessages) 
	{
		ImGui::TextUnformatted(msg.c_str());
	}

	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) 
	{
		ImGui::SetScrollHereY(1.0f);
	}
	ImGui::End();
}

void Interface::AddDebugMessage(const std::string& message) {
	std::lock_guard<std::mutex> lock(debugMutex);

	debugMessages.push_back(message);

	if (debugMessages.size() > 100) 
	{
		debugMessages.erase(debugMessages.begin());
	}
}

void Interface::SendMessageInterface()
{
	if (inputBuffer[0] == '\0')
	{
		return;
	}

	if (connect.isConnected()) 
	{
		std::string fullMessage = std::string(clientName) + ": " + inputBuffer;
		connect.SendMessageConnection(fullMessage);

		// add local to history
		this->chatMessages.push_back({ clientName, inputBuffer });
	}
	else 
	{
		AddDebugMessage("Not connected to any server");
	}

	memset(inputBuffer, 0, sizeof(inputBuffer));
}

void Interface::HelpMarker(const char* desc) {
	ImGui::TextDisabled("(?)");

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}