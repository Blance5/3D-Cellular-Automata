#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>


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

//#define SCREEN_WIDTH 1200
//#define SCREEN_HEIGHT 980

#define SCREEN_WIDTH 1910
#define SCREEN_HEIGHT 1170


#define MAXROWS 100
#define MAXCOLS 100
#define MAXLAYERS 100

#define a1 6
#define a2 7
#define a3 8
#define b1 6
#define b2 7
#define b3 8


// low = 10, high ~= 150
#define RENDERSPEED 50

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
                        aliveCubesVertices.push_back(0.5f);
                        //aliveCubesVertices.push_back(1.0f);
                        aliveCubesVertices.push_back(1 - (allCubeVertices[i][j][k].aliveStatus() * 1.0f / TOTALSTATES));
                        aliveCubesVertices.push_back(0.8f);
                    }
                }
            }
        }
    }

    // get indcies of aliveCube - iterate every cube
    for (int i = 0; i < aliveCubesVertices.size() / (24 * 9); i++) {
        for (int j = 0; j < 36; j++) {
            indices.push_back(baseIndices[j] + (i * 24)); 
        }
    }
}

void calculateNeighbors(std::vector<std::vector<std::vector<Cube>>> allCubeVertices, int *** neighborMap) {
    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {
            for (int k = 0; k < MAXLAYERS; k++) {
                int neighbors = 0;
                for (int l = -1; l < 2; l++) {
                    for (int m = -1; m < 2; m++) {
                        for (int n = -1; n < 2; n++) {
                            if (l == 0 && m == 0 && n == 0) {
                                continue;
                            }
                            if (i + l < 0 || i + l >= MAXROWS || j + m < 0 || j + m >= MAXCOLS || k + n < 0 || k + n >= MAXLAYERS) {
                                continue;
                            }

                            // only neighbors with an alive status of 1 are counted
                            // CAN CHANGE THIS TO MODIFY BEHAVIOR AKA COUNT ALL DYING CELLS AS WELL
                            if (allCubeVertices[i + l][j + m][k + n].aliveStatus() == 1) {
                                neighbors++;
                            }
                        }
                    }
                }
                neighborMap[i][j][k] = neighbors;
            }
        }
    }
}

void updateCubes(std::vector<std::vector<std::vector<Cube>>> & allCubeVertices) {
    // for each cube get amount of alive neighbors - Create neighbormap
    int*** neighborMap = new int**[MAXLAYERS];
    for (int i = 0; i < MAXLAYERS; ++i) {
        neighborMap[i] = new int*[MAXROWS];
        for (int j = 0; j < MAXROWS; ++j) {
            neighborMap[i][j] = new int[MAXCOLS];
        }
    }

    calculateNeighbors(allCubeVertices, neighborMap);

    
    std::cout << neighborMap[0 + 50][0 + 50][0 + 50] << std::endl;
    std::cout << neighborMap[0 + 50][1 + 50][0 + 50] << std::endl;
    std::cout << neighborMap[0 + 50][0 + 50][1 + 50] << std::endl;
    std::cout << neighborMap[0 + 50][1 + 50][1 + 50] << std::endl;
    std::cout << neighborMap[2 + 50][0 + 50][0 + 50] << std::endl;
    std::cout << neighborMap[2 + 50][1 + 50][0 + 50] << std::endl;
    std::cout << neighborMap[2 + 50][0 + 50][1 + 50] << std::endl;
    std::cout << neighborMap[2 + 50][1 + 50][1 + 50] << std::endl;

    
    
    // create 3d array representing future alive status of each cube
    int*** futureMap = new int**[MAXLAYERS];
    for (int i = 0; i < MAXLAYERS; ++i) {
        futureMap[i] = new int*[MAXROWS];
        for (int j = 0; j < MAXROWS; ++j) {
            futureMap[i][j] = new int[MAXCOLS];
        }
    }

    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {
            for (int k = 0; k < MAXLAYERS; k++) {
                int neighborCount = neighborMap[i][j][k];
                if (allCubeVertices[i][j][k].aliveStatus() > 1) {
                    futureMap[i][j][k] = allCubeVertices[i][j][k].aliveStatus() + 1;
                } else if (allCubeVertices[i][j][k].aliveStatus() == 1) {
                    if (neighborCount == a1 || neighborCount == a2 || neighborCount == a3) {
                        futureMap[i][j][k] = 1;
                    } else {
                        futureMap[i][j][k] = 2;
                    }
                } else {
                    if (neighborCount == b1 || neighborCount == b2 || neighborCount == b3) {
                        futureMap[i][j][k] = 1;
                    } else {
                        futureMap[i][j][k] = 0;
                    }
                }

                if (futureMap[i][j][k] >= TOTALSTATES) {
                    futureMap[i][j][k] = 0;
                }
            }
        }
    }

    // apply futuremap to allCubeVertices
    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {
            for (int k = 0; k < MAXLAYERS; k++) {
                allCubeVertices[i][j][k].setAliveStatus(futureMap[i][j][k]);
            }
        }
    }

    for (int i = 0; i < MAXLAYERS; ++i) {
        for (int j = 0; j < MAXROWS; ++j) {
            delete[] neighborMap[i][j]; // Delete each int[MAXCOLS] array
        }
        delete[] neighborMap[i]; // Delete each int*[MAXROWS] array
    }
    delete[] neighborMap; // Delete the int**[MAXLAYERS] array

    for (int i = 0; i < MAXLAYERS; ++i) {
        for (int j = 0; j < MAXROWS; ++j) {
            delete[] futureMap[i][j]; // Delete each int[MAXCOLS] array
        }
        delete[] futureMap[i]; // Delete each int*[MAXROWS] array
    }
    delete[] futureMap; // Delete the int**[MAXLAYERS] array
}

void initRandomVertices(std::vector<std::vector<std::vector<Cube>>> & allCubeVertices, int x, int y, int z) {
    std::srand(std::time(nullptr));
    int random_value = std::rand();
    for (int i = 50 - x; i < 50 + x; i++) {
        for (int j = 50 - y; j < 50 + y; j++) {
            for (int k = 50 - z; k < 50 + z; k++) {
                if (random_value % 5 == 0) {
                    allCubeVertices[i][j][k].setAliveStatus(1);
                }
                random_value = std::rand();
            }
        }
    }

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

    allCubeVertices[4 + 50][0 + 50][0 + 50].setAliveStatus(1);
    allCubeVertices[4 + 50][1 + 50][0 + 50].setAliveStatus(1);
    allCubeVertices[4 + 50][0 + 50][1 + 50].setAliveStatus(1);
    allCubeVertices[4 + 50][1 + 50][1 + 50].setAliveStatus(1);
    allCubeVertices[-2 + 50][0 + 50][0 + 50].setAliveStatus(1);
    allCubeVertices[-2 + 50][1 + 50][0 + 50].setAliveStatus(1);
    allCubeVertices[-2 + 50][0 + 50][1 + 50].setAliveStatus(1);
    allCubeVertices[-2 + 50][1 + 50][1 + 50].setAliveStatus(1);

    initRandomVertices(allCubeVertices, 10, 10, 10);
    
    std::vector<float> aliveCubesVertices;

    // STEPS TO DRAW
    // 1. iterate through every cube
    // 2. get alive verticies
    // 3. get incides from alive vertices
    // 4. copy vectors to arrays
    // 5. create new va, vb, ib

    


    

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

    VertexArray va;
    //VertexBuffer nb(normals, sizeof(normals));


    //VA 
    VertexBufferLayout layout;
    layout.Push(3, GL_FLOAT);
    layout.Push(3, GL_FLOAT);
    layout.Push(3, GL_FLOAT);

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
    glm::vec3 cameraPos(100.0f, 100.0f, 100.0f);  // Camera positioned at (5, 0, 5)
    glm::vec3 cameraTarget = glm::vec3(50.0f, 50.0f, 50.0f); // Camera looking at the origin
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);  // Up vector
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, up);
    




    // Combine the model, view, and projection matrices
    //glm::mat4 MVP = projection * view * model;
     // adjust screenWidth and screenHeight accordingly
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.01f, 200.0f);











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
    float angle = 0.0f;
    /* Loop until the user closes the window */

    float * finalVertices;
    unsigned int * finalIndices;
    
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();



        // cube calculations
        if (count % (15000 / 4) == 0) {
            // get usable buffers from allCubeVertices and alive status
            calcBuffers(aliveCubesVertices, indices, allCubeVertices, baseIndices);
            // calculate alive status
            updateCubes(allCubeVertices);
        }
        
        finalVertices = new float[aliveCubesVertices.size()];
        finalIndices = new unsigned int[indices.size()];
        std::copy(indices.begin(), indices.end(), finalIndices);
        std::copy(aliveCubesVertices.begin(), aliveCubesVertices.end(), finalVertices);

        IndexBuffer ib(finalIndices, indices.size());
        VertexBuffer vb(finalVertices, sizeof(float) * aliveCubesVertices.size());
        va.AddBuffer(vb, layout);


        // Update model matrix for rotation
        

        glm::vec3 modelCenter(50.0f, 50.0f, 50.0f); // Center of the model

        // Translate the model to the origin
        glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), -modelCenter);

        // Rotate around y-axis
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

        // Translate the model back to its original position
        glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), modelCenter);

        // Combine the transformations
        glm::mat4 model = translateBack * rotate * translateToOrigin;
        // % n is 10,000x angle
        angle += 0.004; // Adjust the rotation speed as needed
        //glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around y-axis

        shader.Bind();
        
        //cameraPos = glm::vec3(60.0f + angle / 12, 60.0f + angle / 15, 60.0f);  // Camera positioned at (5, 0, 5)
        //view = glm::lookAt(cameraPos, cameraTarget, up);
        view = glm::lookAt(cameraPos, cameraTarget, up); 

        // Set up transformation matrix and pass it to the shader
        glm::mat4 MVP = projection * view * model; // Assuming you have projection and view matrices set up


        //modelMatrix = glm::translate(glm::mat4(1.0f), translation);
        //MVP = projectionMatrix * viewMatrix * modelMatrix;

        
        shader.SetUniform4f("u_Color", 0.6, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", MVP);
        

        // DRAW CALL
        renderer.Draw(va, ib, shader);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        static float f = 0.0f;
        /*ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        ImGui::SliderFloat3("Translation", &cameraPos.x, -100.0f, 100.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        //ImGui::SliderFloat3("Translationy", &translation.y, -10.0f, 10.0f); 
        //ImGui::SliderFloat3("Translationz", &translation.z, -10.0f, 10.0f); 
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();*/


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
        delete[] finalVertices; // Free the memory allocated for the float array
        delete[] finalIndices;  // Free the memory allocated for the unsigned int array
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