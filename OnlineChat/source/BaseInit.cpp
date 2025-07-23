#include "include/BaseInit.h"
#include "include/Interface.h"
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #error "Error::Unsupported:system. Unfortunately, there is currently no implementation for Linux(X11)."
#elif __APPLE__
    #error "Error::Unsupported:system. Unfortunately, there is currently no implementation for macOS(Metal)."
#endif

#ifdef _WIN32
int BaseInit::initialize(const char* MainWindowTitle, int& wDisplay, int& hDisplay, Interface* _interface) {
    std::cout << "Running on Windows\n";

    // GLFW INIT
    if (!glfwInit())
    {
        throw std::runtime_error("Error::GLFW:not:created!\n");
    }

    // Version OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(wDisplay, hDisplay, MainWindowTitle, nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Error::Window:not:created!\n");
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    // Initialize OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize OpenGL loader!\n");
    }

    // ImGui INIT
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    //style
    _interface->SetupStyle();

    // Setup Platform/Renderer
    if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
    {
        throw std::runtime_error("Failed to initialize ImGui GLFW backend!\n");
    }

    if (!ImGui_ImplOpenGL3_Init("#version 330"))
    {
        throw std::runtime_error("Failed to initialize ImGui OpenGL backend!\n");
    }

    // Network initialization
    if (!_interface->connect.InitializeConnection([_interface](const std::string& error) {
        _interface->AddDebugMessage(error); })) 
    {
        throw std::runtime_error("Network initialization failed");
    }

    _interface->connect.SetMessageCallback([_interface](const std::string& message) {
        size_t colonPos = message.find(':');

        if (colonPos != std::string::npos) 
        {
            std::string author = message.substr(0, colonPos);
            std::string text = message.substr(colonPos + 2);
        
            _interface->chatMessages.push_back({ author, text });
        }
        else 
        {
            _interface->chatMessages.push_back({ "Unknown", message });
        }
    });

    // Main cycle
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Check if window is minimized
        glfwGetFramebufferSize(window, &wDisplay, &hDisplay);

        if (wDisplay == 0 || hDisplay == 0)
        {
            continue; // Skip rendering if window is minimized
        }

        glViewport(0, 0, wDisplay, hDisplay);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // window
        _interface->DrawInterface();

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
#endif