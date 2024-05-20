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



    /*

    CURRENT PLAN:
    store vertex buffer in a vector of vec3s, also maintain a vector of positions that contains same data
    as vec3 vector
    maintain a vector of indices that contains vec3s to draw

    */







    /*float positions2[] = {
    // positions        
    -0.5f, -0.5f, -0.5f, // back face
     0.5f, -0.5f, -0.5f, //  bf
     0.5f,  0.5f, -0.5f, // bf
    -0.5f,  0.5f, -0.5f, // 
    -0.5f, -0.5f,  0.5f, // 
     0.5f, -0.5f,  0.5f, // 
     0.5f,  0.5f,  0.5f, // 
    -0.5f,  0.5f,  0.5f //
    };*/

    float vertexData[] = {
    // positions         // normals
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // back face
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, //  bf
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // bf
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, // 
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, // 
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f  //
};

    /*unsigned int indices2[] = {
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
    };*/

    

    /*unsigned int indices2[] = {
    // back face
    0, 1, 2,
    2, 3, 0,
    // front face
    4, 5, 6,
    6, 7, 4,
    // top face
    0, 4, 5,
    5, 1, 0,
    // bottom face
    2, 6, 7,
    7, 3, 2,
    // left face
    0, 2, 6,
    6, 4, 0,
    // right face
    1, 5, 7,
    7, 3, 1
};*/

/*unsigned int indices2[] = {
    // back face
    0, 1, 2,
    2, 3, 0,
    // front face
    4, 5, 6,
    6, 7, 4,
    // top face
    8, 9, 10,
    10, 11, 8,
    // bottom face
    12, 13, 14,
    14, 15, 12,
    // left face
    16, 17, 18,
    18, 19, 16,
    // right face
    20, 21, 22,
    22, 23, 20
};*/
/*unsigned int indices2[] = {
    // back face
    0, 1, 2,
    2, 3, 0,
    // front face
    4, 5, 6,
    6, 7, 4,
    // left face
    8, 9, 10,
    10, 11, 8,
    // right face
    12, 13, 14,
    14, 15, 12,
    // bottom face
    16, 17, 18,
    18, 19, 16,
    // top face
    20, 21, 22,
    22, 23, 20
};*/

    /*unsigned int indices2[] = {
    // front face
    0, 1, 2, 0, 2, 3,
    // back face
    4, 5, 6, 4, 6, 7,
    // left face
    8, 9, 10, 8, 10, 11,
    // right face
    12, 13, 14, 12, 14, 15,
    // bottom face
    16, 17, 18, 16, 18, 19,
    // top face
    20, 21, 22, 20, 22, 23
};*/

    glm::vec3 positions2vec3[] = {
        glm::vec3(-0.5, -0.5, -0.5),
        glm::vec3(0.5, -0.5, -0.5),
        glm::vec3(0.5, 0.5, -0.5),
        glm::vec3(-0.5, 0.5, -0.5),
        glm::vec3(-0.5, -0.5, 0.5),
        glm::vec3(0.5, -0.5, 0.5),
        glm::vec3(0.5, 0.5, 0.5),
        glm::vec3(-0.5, 0.5, 0.5)
    };
    

    glm::vec3 normalsvec3[12];

    // loop through every triangle
    /*for (int i = 0; i < 12 * 3; i+= 3) {
        glm::vec3 vertex1 = positions2vec3[indices2[i]];
        glm::vec3 vertex2 = positions2vec3[indices2[i + 1]];
        glm::vec3 vertex3 = positions2vec3[indices2[i + 2]];

        glm::vec3 edge1 = vertex2 - vertex1;
        glm::vec3 edge2 = vertex3 - vertex1;
        glm::vec3 norm = glm::cross(edge1, edge2);
        norm = glm::normalize(norm);

        normalsvec3[i] = norm;
        normalsvec3[i + 1] = norm;
        normalsvec3[i + 2] = norm;
    }*/

    /*float normals[36 * 3];

    for (int i = 0; i < 36; i++) {
        normals[i * 3] = normalsvec3[i].x;
        normals[i * 3 + 1] = normalsvec3[i].y;
        normals[i * 3 + 2] = normalsvec3[i].z;
    }*/

    /*float normals[] = {
    // front face normals
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    // back face normals
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    // left face normals
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    // right face normals
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    // bottom face normals
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    // top face normals
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f
};*/

    //int numActiveCubes = 2;

   /*float normals[] = {
        // normals
        -0.5773502691896257f, -0.5773502691896257f, -0.5773502691896257f,
        0.5773502691896257f, -0.5773502691896257f, -0.5773502691896257f,
        0.5773502691896257f,  0.5773502691896257f, -0.5773502691896257f,
        -0.5773502691896257f,  0.5773502691896257f, -0.5773502691896257f,
        -0.5773502691896257f, -0.5773502691896257f,  0.5773502691896257f,
        0.5773502691896257f, -0.5773502691896257f,  0.5773502691896257f,
        0.5773502691896257f,  0.5773502691896257f,  0.5773502691896257f,
        -0.5773502691896257f,  0.5773502691896257f,  0.5773502691896257f
    };*/
   /* std::vector<float> verticesVec;
    // initialize all possible verticies
    for (int i = 0; i < numActiveCubes; i++) {
        for (int j = 0; j < 24; j++) {
            verticesVec.push_back(vertexData[j]);
        }
    }*/
    
    float vertices[] = {
    // First Cube
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,

    // Second Cube (translated +3 units in x direction)
     2.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     3.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     3.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     2.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    
     2.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     3.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     3.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 
     2.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

     2.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
     2.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
     2.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 
     2.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     3.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     3.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     3.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     3.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

     3.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     3.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     2.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     2.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

     2.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     3.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     3.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     2.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
};



    unsigned int indices2[] = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23,

        24, 25, 26, 24, 26, 27,
        28, 29, 30, 28, 30, 31,
        32, 33, 34, 32, 34, 35,
        36, 37, 38, 36, 38, 39,
        40, 41, 42, 40, 42, 43,
        44, 45, 46, 44, 46, 47
    };

    //unsigned int * indicies2 = new unsigned int[36 * numActiveCubes];

    /*for (int i = 0 ; i < numActiveCubes; i++) {
        for (int j = 0; j < 36; j++) {
            indices2[j + i * 36] += i * 24;
        }
    }*/



    

    // index buffer
    unsigned int indicies[] {
        0, 1, 2,
        2, 3, 0
    };

    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //GLCall(glEnable(GL_DEPTH_TEST)); // Enable depth testing
    for (int i = 0; i < 36 * 3; i++) {
        //std::cout << normals[i] << std::endl;
    }

    VertexArray va;
    VertexArray va2;
    VertexBuffer vb(vertices, sizeof(vertices));
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
    




    // Create the view matrix using glm::lookAt
    // Set up view matrix
    glm::vec3 cameraPos(3.0f, 3.0f, 3.0f);  // Camera positioned at (5, 0, 5)
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
    
    // Set light position and color through uniforms
    shader.SetUniform3f("u_LightPosition", 2.0f, 5.0f, 10.0f);
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
        //angle += 0.1f; // Adjust the rotation speed as needed
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around y-axis

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



        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        static float f = 0.0f;
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        ImGui::SliderFloat3("Translation", &cameraPos.x, -10.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
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