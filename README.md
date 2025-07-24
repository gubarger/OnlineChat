# OnlineChat
## How startup file

Unfortunately, the cmake build is terrible and doesn't work, if you don't have vcpkg, then you need to download the dependencies `The initial IDE is Visual Studio 2022`:

```bash
vcpkg install imgui glad glfw3
```

- If there are no dependencies <imgui_impl_glfw.h> and <imgui_impl_opengl3.h>, then you should reinstall imgui:
 
```bash
vcpkg remove imgui --recurse
vcpkg install imgui[glfw-binding,opengl3-binding]
```

- If you suddenly decide to build using `cmake`, then make sure that the flag is passed when configuring the project:

```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=[path_to_vcpkg]/scripts/buildsystems/vcpkg.cmake
```

After all, you need to integrate the installed dependencies:
```bash
vcpkg integrate install
```

# How to communicate

To connect, you just need to write your `IP` and `Port` (the port can be any), after which your friend (or another person) writes `IP` and `Port`, and then clicks `connect`

> <img width="338" height="209" alt="{A4245890-129F-43EA-8232-E335C77E59CD}" src="https://github.com/user-attachments/assets/6b535376-f307-4a46-89ff-3d8a287f78ad" />
- Error and connection logs are displayed in the console and a special `Debug` window.

```C++
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
```

# Technologies
- GLFW
- GLAD
- ImGui
- winsock2
