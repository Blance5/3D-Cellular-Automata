#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>


#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 980

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }
    
    GLCall(glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3));
    GLCall(glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3));
    GLCall(glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE));

    const char* glsl_version = "#version 130";
    
 
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    GLCall(glfwMakeContextCurrent(window));

    GLCall(glfwSwapInterval(1));

    if (glewInit() != GLEW_OK) {
        std::cout << "glewInit Failed!\n";
    }

    float positions[] = {
        -50.0f, -50.0f, 0.0f, 0.0f, // 0
        50.0f, -50.0f, 1.0f, 0.0f,// 1
        50.0f, 50.0f, 1.0f, 1.0f,// 2
        -50.0f, 50.0f, 0.0f, 1.0f// 3
    };

    float positions2[] = {
    // positions        
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f
    };

    unsigned int indices2[] = {
    0, 1, 2,
    2, 3, 0,
    0, 4, 5,
    5, 1, 0,
    1, 5, 6,
    6, 2, 1,
    2, 6, 7,
    7, 3, 2,
    3, 7, 4,
    4, 0, 3,
    4, 7, 6,
    6, 5, 4
    };

    // index buffer
    unsigned int indicies[] {
        0, 1, 2,
        2, 3, 0
    };

    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));

    VertexArray va;
    VertexBuffer vb(positions2, sizeof(positions2));


    //VA 
    VertexBufferLayout layout;
    layout.Push(3, GL_FLOAT);
    layout.PrintElements();
    va.AddBuffer(vb, layout);


    // index buffer object
    IndexBuffer ib(indices2, sizeof(indices2) / sizeof(unsigned int));


    // START GLM SHENANIGANS

    // PROJECTION
    float aspectRatio = SCREEN_WIDTH / SCREEN_HEIGHT;
    float fov = glm::radians(45.0f); // 45 degree field of view
    
    //glm::mat4 projection = glm::perspective(fov, aspectRatio, 0.1f, 100.0f);



    // MODEL
    //glm::mat4 model = glm::mat4(1.0f); // identity matrix
    glm::mat4 model = glm::mat4(1.0f);

    // VIEW
    //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    


    //-----------------
    glm::vec3 cameraPosition(0.0f, 0.0f, -3.0f);
    glm::vec3 upVector(0.0f, 0.0f, 1.0f);
    // Define the target position (corner of the cube)
    glm::vec3 targetPosition(0.5f, 0.5f, 0.5f); // Replace with the actual position of the corner


    // Create the view matrix using glm::lookAt
    // Set up view matrix
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);  // Camera positioned at (5, 0, 5)
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); // Camera looking at the origin
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);  // Up vector
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, up);





    // Combine the model, view, and projection matrices
    //glm::mat4 MVP = projection * view * model;
     // adjust screenWidth and screenHeight accordingly
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);











    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    // tex7.jpg is a good cell texture also try 8
    Texture texture("res/textures/tex3.png");
    //texture.Bind();
    // slot 0
    //shader.SetUniform1i("u_Texture", 0);
    
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;

    // imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    glm::vec3 translation(200, 200, 0);
    


    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Update model matrix for rotation
        static float angle = 0.0f;
        angle += 0.1f; // Adjust the rotation speed as needed
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around y-axis

        // Set up transformation matrix and pass it to the shader
        glm::mat4 MVP = projection * view * model; // Assuming you have projection and view matrices set up


        //modelMatrix = glm::translate(glm::mat4(1.0f), translation);
        //MVP = projectionMatrix * viewMatrix * modelMatrix;

        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", MVP);

        // DRAW CALL
        renderer.Draw(va, ib, shader);



        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        static float f = 0.0f;
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();


        /*GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));*/

        




        if (r > 1.0f) {
            increment = -0.003f;
        } else if (r < 0.0f) {
            increment = 0.003f;
        }
        r += increment;



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));



        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}