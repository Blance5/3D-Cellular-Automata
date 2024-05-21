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
#include "Cube.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 980

#define MAXROWS 100
#define MAXCOLS 100
#define MAXLAYERS 100

void calcBuffers(std::vector<float> & aliveCubesVertices, std::vector<unsigned int> & indices, std::vector<std::vector<std::vector<Cube>>> allCubeVertices, unsigned int baseIndices[]) {
    indices.clear();
    aliveCubesVertices.clear();
    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {
            for (int k = 0; k < MAXLAYERS; k++) {
                if (allCubeVertices[i][j][k].aliveStatus() != 0) {
                    for (int l = 0; l < 24; l++) {
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getPoints()[l].x);
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getPoints()[l].y);
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getPoints()[l].z);
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getNormals()[l].x);
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getNormals()[l].y);
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getNormals()[l].z);
                    }
                }
            }
        }
    }

    // get indcies of aliveCube - iterate every cube
    for (int i = 0; i < aliveCubesVertices.size() / (24 * 6); i++) {
        for (int j = 0; j < 36; j++) {
            indices.push_back(baseIndices[j] + (i * 24)); 
        }
    }
}

void updateCubes() {
    // for each cube get amount of alive neighbors
    
    // 
}

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

    

    


    std::vector<std::vector<std::vector<Cube>>> allCubeVertices(MAXROWS,
        std::vector<std::vector<Cube>>(MAXCOLS, std::vector<Cube>(MAXLAYERS)));
    // initialize all possible verticies
    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {
            for (int k = 0; k < MAXLAYERS; k++) {
                allCubeVertices[i][j][k] = Cube(i, j, k, false);
            }
        }
    }

    std::vector<unsigned int> indices;
    unsigned int baseIndices[] = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };

    allCubeVertices[0 + 50][0 + 50][0 + 50].setAliveStatus(1);
    allCubeVertices[0 + 50][1 + 50][0 + 50].setAliveStatus(1);
    allCubeVertices[0 + 50][0 + 50][1 + 50].setAliveStatus(1);
    allCubeVertices[0 + 50][1 + 50][1 + 50].setAliveStatus(1);
    allCubeVertices[2 + 50][0 + 50][0 + 50].setAliveStatus(1);
    allCubeVertices[2 + 50][1 + 50][0 + 50].setAliveStatus(1);
    allCubeVertices[2 + 50][0 + 50][1 + 50].setAliveStatus(1);
    allCubeVertices[2 + 50][1 + 50][1 + 50].setAliveStatus(1);
    std::vector<float> aliveCubesVertices;

    // STEPS TO DRAW
    // 1. iterate through every cube
    // 2. get alive verticies
    // 3. get incides from alive vertices
    // 4. copy vectors to arrays
    // 5. create new va, vb, ib

    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {
            for (int k = 0; k < MAXLAYERS; k++) {
                if (allCubeVertices[i][j][k].aliveStatus() != 0) {
                    for (int l = 0; l < 24; l++) {
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getPoints()[l].x);
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getPoints()[l].y);
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getPoints()[l].z);
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getNormals()[l].x);
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getNormals()[l].y);
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getNormals()[l].z);
                    }
                }
            }
        }
    }

    
    


    // get indcies of aliveCube - iterate every cube
    for (int i = 0; i < aliveCubesVertices.size() / (24 * 6); i++) {
        for (int j = 0; j < 36; j++) {
            indices.push_back(baseIndices[j] + (i * 24)); 
        }
    }
    
    float * finalVertices = new float[aliveCubesVertices.size()];
    unsigned int * finalIndices = new unsigned int[indices.size()];
    std::copy(indices.begin(), indices.end(), finalIndices);
    std::copy(aliveCubesVertices.begin(), aliveCubesVertices.end(), finalVertices);


    

    //unsigned int * indicies2 = new unsigned int[36 * numActiveCubes];

    

    // index buffer
    /*unsigned int indicies[] {
        0, 1, 2,
        2, 3, 0
    };*/

    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //GLCall(glEnable(GL_DEPTH_TEST)); // Enable depth testing
    for (int i = 0; i < 36 * 3; i++) {
        //std::cout << normals[i] << std::endl;
    }

    VertexArray va;
    VertexBuffer vb(finalVertices, sizeof(float) * aliveCubesVertices.size());
    //VertexBuffer nb(normals, sizeof(normals));


    //VA 
    VertexBufferLayout layout;
    layout.Push(3, GL_FLOAT);
    layout.Push(3, GL_FLOAT);
    va.AddBuffer(vb, layout);

    /*va.Bind();
    vb.Bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void *)3);*/
    

    //VertexBufferLayout layout2;
    //layout.Push(3, GL_FLOAT);
    //layout2.SetAttribLocation(1, "normal");
    //va.AddBuffer(nb, layout); 

    // index buffer object
    IndexBuffer ib(finalIndices, indices.size());

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
    




    // Create the view matrix using glm::lookAt
    // Set up view matrix
    glm::vec3 cameraPos(55.0f, 55.0f, 55.0f);  // Camera positioned at (5, 0, 5)
    glm::vec3 cameraTarget = glm::vec3(50.0f, 50.0f, 50.0f); // Camera looking at the origin
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
    
    // Set light position and color through uniforms
    shader.SetUniform3f("u_LightPosition", 100.0f, 100.0f, 50.0f);
    shader.SetUniform3f("u_LightColor", 1.0f, 1.0f, 1.0f);
    shader.SetUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
    
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

    


    float r = 0.0f;
    float increment = 0.05f;
    int count = 0;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();



        // cube calculations
        if (count % 1 == 0) {
            // calculate alive status
            updateCubes();
            // get usable buffers from allCubeVertices and alive status
            calcBuffers(aliveCubesVertices, indices, allCubeVertices, baseIndices);
        }
        
        finalVertices = new float[aliveCubesVertices.size()];
        finalIndices = new unsigned int[indices.size()];
        std::copy(indices.begin(), indices.end(), finalIndices);
        std::copy(aliveCubesVertices.begin(), aliveCubesVertices.end(), finalVertices);

        IndexBuffer ib(finalIndices, indices.size());
        VertexBuffer vb(finalVertices, sizeof(float) * aliveCubesVertices.size());
        va.AddBuffer(vb, layout);


        // Update model matrix for rotation
        static float angle = 0.0f;

        glm::vec3 modelCenter(50.0f, 50.0f, 50.0f); // Center of the model

        // Translate the model to the origin
        glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), -modelCenter);

        // Rotate around y-axis
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

        // Translate the model back to its original position
        glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), modelCenter);

        // Combine the transformations
        glm::mat4 model = translateBack * rotate * translateToOrigin;
        angle += 0.1f; // Adjust the rotation speed as needed
        //glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around y-axis

        // Set up transformation matrix and pass it to the shader
        glm::mat4 MVP = projection * view * model; // Assuming you have projection and view matrices set up


        //modelMatrix = glm::translate(glm::mat4(1.0f), translation);
        //MVP = projectionMatrix * viewMatrix * modelMatrix;

        shader.Bind();
        shader.SetUniform4f("u_Color", 0.6, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", MVP);
        view = glm::lookAt(cameraPos, cameraTarget, up);  // Camera positioned at (5, 0, 5)

        // DRAW CALL
        renderer.Draw(va, ib, shader);

        delete finalVertices;
        delete finalIndices;

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        static float f = 0.0f;
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        ImGui::SliderFloat3("Translation", &cameraPos.x, -100.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        //ImGui::SliderFloat3("Translationy", &translation.y, -10.0f, 10.0f); 
        //ImGui::SliderFloat3("Translationz", &translation.z, -10.0f, 10.0f); 
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

        count++;

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