#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>

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

#define MAXROWS 200
#define MAXCOLS 200
#define MAXLAYERS 200

#define ALIVEREQ 9
#define BIRTHREQ 4
#define TOTALSTATES 2

#define DEFAULTX 100.0f
#define DEFAULTY 100.0f
#define DEFAULTZ 100.0f

// low = 10, high ~= 1100

/*
PERFORMANCE IMPROVEMENETS:
1. maintain vector of all alive points instead of cube vector
2. only calculate neighbors of neighbors of alive points
3. reduce neighbor calculations by not double counting neighbors of points, initialize with -1 and only calculate if -1
4. maintain vector with alive points in i, j, k format to help with generating be able to calculate neighbors of neighbors of alive points


*/

int gcountLines = 0;
int gcountCubes = 0;

void addBorder(std::vector<float> & allSpaces) {
    for (int i = 0; i < MAXROWS; i++) {
        for (int l = 0; l < 24; l++) {
            allSpaces.push_back(i + Cube::GetPoint(l).x);
            allSpaces.push_back(0 + Cube::GetPoint(l).y);
            allSpaces.push_back(0 + Cube::GetPoint(l).z);


            allSpaces.push_back(Cube::GetNormal(l).x);
            allSpaces.push_back(Cube::GetNormal(l).y);
            allSpaces.push_back(Cube::GetNormal(l).z);

            allSpaces.push_back(1.0f);
            allSpaces.push_back(0.0f);
            allSpaces.push_back(1.0f);
        }

        for (int l = 0; l < 24; l++) {
            allSpaces.push_back(i + Cube::GetPoint(l).x);
            allSpaces.push_back(MAXCOLS - 1 + Cube::GetPoint(l).y);
            allSpaces.push_back(MAXLAYERS - 1 + Cube::GetPoint(l).z);


            allSpaces.push_back(Cube::GetNormal(l).x);
            allSpaces.push_back(Cube::GetNormal(l).y);
            allSpaces.push_back(Cube::GetNormal(l).z);

            allSpaces.push_back(1.0f);
            allSpaces.push_back(0.0f);
            allSpaces.push_back(1.0f);
        }

        for (int l = 0; l < 24; l++) {
            allSpaces.push_back(i + Cube::GetPoint(l).x);
            allSpaces.push_back(0 + Cube::GetPoint(l).y);
            allSpaces.push_back(MAXLAYERS - 1 + Cube::GetPoint(l).z);


            allSpaces.push_back(Cube::GetNormal(l).x);
            allSpaces.push_back(Cube::GetNormal(l).y);
            allSpaces.push_back(Cube::GetNormal(l).z);

            allSpaces.push_back(1.0f);
            allSpaces.push_back(0.0f);
            allSpaces.push_back(1.0f);
        }

        for (int l = 0; l < 24; l++) {
            allSpaces.push_back(i + Cube::GetPoint(l).x);
            allSpaces.push_back(MAXCOLS - 1 + Cube::GetPoint(l).y);
            allSpaces.push_back(0 + Cube::GetPoint(l).z);


            allSpaces.push_back(Cube::GetNormal(l).x);
            allSpaces.push_back(Cube::GetNormal(l).y);
            allSpaces.push_back(Cube::GetNormal(l).z);

            allSpaces.push_back(1.0f);
            allSpaces.push_back(0.0f);
            allSpaces.push_back(1.0f);
        }
    }

    for (int j = 0; j < MAXROWS; j++) {
        for (int l = 0; l < 24; l++) {
            allSpaces.push_back(0 + Cube::GetPoint(l).x);
            allSpaces.push_back(j + Cube::GetPoint(l).y);
            allSpaces.push_back(0 + Cube::GetPoint(l).z);


            allSpaces.push_back(Cube::GetNormal(l).x);
            allSpaces.push_back(Cube::GetNormal(l).y);
            allSpaces.push_back(Cube::GetNormal(l).z);

            allSpaces.push_back(1.0f);
            allSpaces.push_back(0.0f);
            allSpaces.push_back(1.0f);
        }

        for (int l = 0; l < 24; l++) {
            allSpaces.push_back(MAXROWS - 1 + Cube::GetPoint(l).x);
            allSpaces.push_back(j + Cube::GetPoint(l).y);
            allSpaces.push_back(MAXLAYERS - 1 + Cube::GetPoint(l).z);


            allSpaces.push_back(Cube::GetNormal(l).x);
            allSpaces.push_back(Cube::GetNormal(l).y);
            allSpaces.push_back(Cube::GetNormal(l).z);

            allSpaces.push_back(1.0f);
            allSpaces.push_back(0.0f);
            allSpaces.push_back(1.0f);
        }

        for (int l = 0; l < 24; l++) {
            allSpaces.push_back(0 + Cube::GetPoint(l).x);
            allSpaces.push_back(j + Cube::GetPoint(l).y);
            allSpaces.push_back(MAXLAYERS - 1 + Cube::GetPoint(l).z);


            allSpaces.push_back(Cube::GetNormal(l).x);
            allSpaces.push_back(Cube::GetNormal(l).y);
            allSpaces.push_back(Cube::GetNormal(l).z);

            allSpaces.push_back(1.0f);
            allSpaces.push_back(0.0f);
            allSpaces.push_back(1.0f);
        }

        for (int l = 0; l < 24; l++) {
            allSpaces.push_back(MAXROWS - 1 + Cube::GetPoint(l).x);
            allSpaces.push_back(j + Cube::GetPoint(l).y);
            allSpaces.push_back(0 + Cube::GetPoint(l).z);


            allSpaces.push_back(Cube::GetNormal(l).x);
            allSpaces.push_back(Cube::GetNormal(l).y);
            allSpaces.push_back(Cube::GetNormal(l).z);

            allSpaces.push_back(1.0f);
            allSpaces.push_back(0.0f);
            allSpaces.push_back(1.0f);
        }
    }

    for (int k = 0; k < MAXROWS; k++) {
        for (int l = 0; l < 24; l++) {
            allSpaces.push_back(0 + Cube::GetPoint(l).x);
            allSpaces.push_back(0 + Cube::GetPoint(l).y);
            allSpaces.push_back(k + Cube::GetPoint(l).z);


            allSpaces.push_back(Cube::GetNormal(l).x);
            allSpaces.push_back(Cube::GetNormal(l).y);
            allSpaces.push_back(Cube::GetNormal(l).z);

            allSpaces.push_back(1.0f);
            allSpaces.push_back(0.0f);
            allSpaces.push_back(1.0f);
        }

        for (int l = 0; l < 24; l++) {
            allSpaces.push_back(MAXROWS - 1 + Cube::GetPoint(l).x);
            allSpaces.push_back(MAXCOLS - 1 + Cube::GetPoint(l).y);
            allSpaces.push_back(k + Cube::GetPoint(l).z);


            allSpaces.push_back(Cube::GetNormal(l).x);
            allSpaces.push_back(Cube::GetNormal(l).y);
            allSpaces.push_back(Cube::GetNormal(l).z);

            allSpaces.push_back(1.0f);
            allSpaces.push_back(0.0f);
            allSpaces.push_back(1.0f);
        }

        for (int l = 0; l < 24; l++) {
            allSpaces.push_back(0 + Cube::GetPoint(l).x);
            allSpaces.push_back(MAXCOLS - 1 + Cube::GetPoint(l).y);
            allSpaces.push_back(k + Cube::GetPoint(l).z);


            allSpaces.push_back(Cube::GetNormal(l).x);
            allSpaces.push_back(Cube::GetNormal(l).y);
            allSpaces.push_back(Cube::GetNormal(l).z);

            allSpaces.push_back(1.0f);
            allSpaces.push_back(0.0f);
            allSpaces.push_back(1.0f);
        }

        for (int l = 0; l < 24; l++) {
            allSpaces.push_back(MAXROWS - 1 + Cube::GetPoint(l).x);
            allSpaces.push_back(0 + Cube::GetPoint(l).y);
            allSpaces.push_back(k + Cube::GetPoint(l).z);


            allSpaces.push_back(Cube::GetNormal(l).x);
            allSpaces.push_back(Cube::GetNormal(l).y);
            allSpaces.push_back(Cube::GetNormal(l).z);

            allSpaces.push_back(1.0f);
            allSpaces.push_back(0.0f);
            allSpaces.push_back(1.0f);
        }
    }
    
}

float * calcBuffers(unsigned int ** allIndices,
    int *** allCubeVertices, unsigned int baseIndices[], std::vector<glm::vec3> & alivePoints,
    float ** allSpaces, int spaceIdx, std::vector<int> & arraySizes) {

    auto start = std::chrono::high_resolution_clock::now();
    alivePoints.clear();

    std::cout << "cb1\n";

    // finding num alive cubes
    int numAliveCubes = 0;
    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {
            for (int k = 0; k < MAXLAYERS; k++) {
                if (allCubeVertices[i][j][k] != 0) {
                    numAliveCubes++;
                }
            }
        }
    }

    arraySizes.push_back(numAliveCubes * 24 * 9);

    float * space = new float[numAliveCubes * 24 * 9];
    for (int i = 0; i < 24 * 9 * numAliveCubes; i++) {
        space[i] = 0.0f;
    }
    

    int curCubeCount = 0;
    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {
            for (int k = 0; k < MAXLAYERS; k++) {

                //std::cout << "start: " << i << " " << j << " " << k << std::endl;

                if (allCubeVertices[i][j][k] == 0) {
                    continue;
                }

                // maintain alivePoints
                if (allCubeVertices[i][j][k] == 1) {
                    alivePoints.push_back(glm::vec3(i, j, k));
                    //std::cout << "ADDED TO ALIVE: " << i << " " << j << " " << k << std::endl;
                }
                
                gcountCubes++;
                for (int l = 0; l < 24; l++) {
                    // could be wrong mapping between i j k and x y 
                    //std::cout << "SQUARE : " << l << std::endl;
                    //std::cout << i * Cube::GetPoint(l).x << j * Cube::GetPoint(l).y << k * Cube::GetPoint(l).z << std::endl;

                    // ALSO add points to corresponding space
                    //std::cout << "f1\n";
                    gcountLines++;
                    space[(l * 9) + (curCubeCount * 24 * 9)] = i + Cube::GetPoint(l).x;
                    space[(l * 9 + 1) + (curCubeCount * 24 * 9)] = j + Cube::GetPoint(l).y;
                    space[l * 9 + 2 + + (curCubeCount * 24 * 9)] = k + Cube::GetPoint(l).z;

                    //std::cout << "f2\n";
                    space[l * 9 + 3 + + (curCubeCount * 24 * 9)] = Cube::GetNormal(l).x;
                    space[l * 9 + 4 + + (curCubeCount * 24 * 9)] = Cube::GetNormal(l).y;
                    space[l * 9 + 5 + + (curCubeCount * 24 * 9)] = Cube::GetNormal(l).z;
                    //std::cout << "f3\n";
                    space[l * 9 + 6 + + (curCubeCount * 24 * 9)] = 0.4f;
                    space[l * 9 + 7 + + (curCubeCount * 24 * 9)] = 1 - (allCubeVertices[i][j][k] * 1.0f / TOTALSTATES);
                    space[l * 9 + 8 + + (curCubeCount * 24 * 9)] = 0.8f;
                }
                curCubeCount++;

                /*if (allCubeVertices[i][j][k].aliveStatus() != 0) {
                    
                    for (int l = 0; l < 24; l++) {
                        
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getPoints()[l].x);
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getPoints()[l].y);
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getPoints()[l].z);
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getNormals()[l].x);
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getNormals()[l].y);
                        aliveCubesVertices.push_back(allCubeVertices[i][j][k].getNormals()[l].z);
                        aliveCubesVertices.push_back(0.4f);
                        //aliveCubesVertices.push_back(1.0f);
                        aliveCubesVertices.push_back(1 - (allCubeVertices[i][j][k].aliveStatus() * 1.0f / TOTALSTATES));
                        aliveCubesVertices.push_back(0.84f);
                        
                    }
                    
                }*/
            }
        }
    }

    
    //addBorder(allSpaces[spaceIdx]);
    
    unsigned int * indices = new unsigned int[numAliveCubes * 36];
    // get indcies of aliveCube - iterate every cube
    for (int i = 0; i < numAliveCubes; i++) {
        for (int j = 0; j < 36; j++) {
            indices[j + i * 36] = baseIndices[j] + (i * 24);
        }
    }

    allIndices[spaceIdx] = indices;
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    auto elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds);
    std::cout << "time in updatebuffer : " << elapsed_milliseconds.count() << " milliseconds" << std::endl;

    return space;
}

void calculateNeighbors(int *** allCubeVertices, int *** neighborMap, int i, int j, int k) {
    //auto start = std::chrono::high_resolution_clock::now();
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
                if (allCubeVertices[i + l][j + m][k + n] == 1) {
                    neighbors++;
                }
            }
        }
    }
    neighborMap[i][j][k] = neighbors;
    //std::cout << i << " " << j << " " << k << " " << neighbors << std::endl;
    
    /*auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    auto elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds);
    std::cout << "time in neighborcounter : " << elapsed_milliseconds.count() << " milliseconds" << std::endl;*/
}

void updateCubes(int *** allCubeVertices, std::vector<glm::vec3> & alivePoints) {
    auto start = std::chrono::high_resolution_clock::now();
    // for each cube get amount of alive neighbors - Create neighbormap
    int*** neighborMap = new int**[MAXLAYERS];
    for (int i = 0; i < MAXLAYERS; ++i) {
        neighborMap[i] = new int*[MAXROWS];
        for (int j = 0; j < MAXROWS; ++j) {
            neighborMap[i][j] = new int[MAXCOLS];
        }
    }

    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {
            for (int k = 0; k < MAXLAYERS; k++) {
                neighborMap[i][j][k] = -1;
            }
        }
    }

    //std::vector<glm::vec3> checkedPoints;
    
    // only calculate neighbors of neighbors of alive cubes
    for (glm::vec3 point : alivePoints) {
        int i = point.x;
        int j = point.y;
        int k = point.z;
        ///std::cout << i << " " << j << " " << k << " "  << std::endl;
        //std::cout << "VALUES IN ALIVEPOINTS " << i << " " << j << " " << k << std::endl;
        // get neighbors of alive point
        for (int l = -1; l < 2; l++) {
            for (int m = -1; m < 2; m++) {
                for (int n = -1; n < 2; n++) {

                    if (i + l < 0 || i + l >= MAXROWS || j + m < 0 || j + m >= MAXCOLS || k + n < 0 || k + n >= MAXLAYERS) {
                        continue;
                    }

                    //int repeatSearches = std::count(checkedPoints.begin(), checkedPoints.end(), glm::vec3(i + l, j + m, k + n));

                    //if (repeatSearches > 0) {
                      //  continue;
                    //}

                    //checkedPoints.push_back(glm::vec3(i + l, j + m, k + n));
                    if (neighborMap[i + l][j + m][k + n] != -1) {
                        continue;
                    }

                    calculateNeighbors(allCubeVertices, neighborMap, i + l, j + m, k + n);
                    // maintain which points have already had neighbors calculated for
                }
            }
        }
    }
    

    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {
            for (int k = 0; k < MAXLAYERS; k++) {
                if (neighborMap[i][j][k] == -1) {
                    neighborMap[i][j][k] = 0;
                }
            }
        }
    }
    /*auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    auto elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds);
    std::cout << "Neighbor stuff : " << elapsed_milliseconds.count() << " milliseconds" << std::endl;*/
    // decode float to int ->

   
    
    /*std::cout << neighborMap[0 + 100][0 + 100][0 + 100] << std::endl;
    std::cout << neighborMap[0 + 100][1 + 100][0 + 100] << std::endl;
    std::cout << neighborMap[0 + 100][0 + 100][1 + 100] << std::endl;
    std::cout << neighborMap[0 + 100][1 + 100][1 + 100] << std::endl;
    std::cout << neighborMap[2 + 100][0 + 100][0 + 100] << std::endl;
    std::cout << neighborMap[2 + 100][1 + 100][0 + 100] << std::endl;
    std::cout << neighborMap[2 + 100][0 + 100][1 + 100] << std::endl;
    std::cout << neighborMap[2 + 100][1 + 100][1 + 100] << std::endl;*/
    
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
                if (neighborCount > 0) {
                    //std::cout << i << " " << j << " " << k << " " << neighborCount << std::endl;
                }
                //std::cout << "NEW " << std::endl;
                if (allCubeVertices[i][j][k] > 1) {
                    futureMap[i][j][k] = allCubeVertices[i][j][k] + 1;
                    //std::cout << "PROBLEM!" << std::endl;
                } else if (allCubeVertices[i][j][k] == 1) {
                    // alivereq
                    if (neighborCount == ALIVEREQ) {
                        futureMap[i][j][k] = 1;
                        //std::cout << i << " " << j << " "  << k  << " " << neighborCount << " Count:" << neighborCount << " ALIVE SET ALIVE" << std::endl;
                    } else {
                        futureMap[i][j][k] = 2;
                        //std::cout << i << " " << j << " "  << k  << " " << neighborCount << " Count:" << neighborCount << " ALIVE SET DEAD" << std::endl;
                    }
                } else {
                    // birthreq
                    if (neighborCount == BIRTHREQ) {
                        futureMap[i][j][k] = 1;
                        //std::cout << i << " " << j << " " <<   k  <<  " " << neighborCount << " Count:" << neighborCount << " DEAD SET ALIVE" << std::endl;
                    } else {
                        futureMap[i][j][k] = 0;
                    }
                }

                if (futureMap[i][j][k] >= TOTALSTATES) {
                    futureMap[i][j][k] = 0;
                }
                //std::cout << "Map value: " << futureMap[i][j][k] << std::endl;
            }
        }
    }

    

    // apply futuremap to allCubeVertices
    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {
            for (int k = 0; k < MAXLAYERS; k++) {
                allCubeVertices[i][j][k] = futureMap[i][j][k];
            }
        }
    }


    for (int i = 0; i < MAXLAYERS; ++i) {
        for (int j = 0; j < MAXROWS; ++j) {
            delete[] futureMap[i][j]; // Free innermost arrays (int arrays)
        }
        delete[] futureMap[i]; // Free middle arrays (int* arrays)
    }
    delete[] futureMap;

    for (int i = 0; i < MAXLAYERS; ++i) {
        for (int j = 0; j < MAXROWS; ++j) {
            delete[] neighborMap[i][j]; // Free innermost arrays (int arrays)
        }
        delete[] neighborMap[i]; // Free middle arrays (int* arrays)
    }
    delete[] neighborMap;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    auto elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds);
    std::cout << "Time in updateCube : " << elapsed_milliseconds.count() << " milliseconds" << std::endl;
    
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
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cellular Automata in 3 Dimensions", NULL, NULL);
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

    

    


    int*** allCubeVertices = new int**[MAXLAYERS];
    for (int i = 0; i < MAXLAYERS; ++i) {
        allCubeVertices[i] = new int*[MAXROWS];
        for (int j = 0; j < MAXROWS; ++j) {
            allCubeVertices[i][j] = new int[MAXCOLS];
        }
    }
    // initialize all possible verticies
    for (int i = 0; i < MAXROWS; i++) {
        for (int j = 0; j < MAXCOLS; j++) {
            for (int k = 0; k < MAXLAYERS; k++) {
                allCubeVertices[i][j][k] = 0;
            }
        }
    }

    std::vector<unsigned int> indices;
    unsigned int ** allIndices;
    unsigned int baseIndices[] = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };

    allCubeVertices[0 + 100][0 + 100][0 + 100] = 1;
    allCubeVertices[0 + 100][1 + 100][0 + 100] = 1;
    allCubeVertices[0 + 100][0 + 100][1 + 100] = 1;
    allCubeVertices[0 + 100][1 + 100][1 + 100] = 1;
    allCubeVertices[2 + 100][0 + 100][0 + 100] = 1;
    allCubeVertices[2 + 100][1 + 100][0 + 100] = 1;
    allCubeVertices[2 + 100][0 + 100][1 + 100] = 1;
    allCubeVertices[2 + 100][1 + 100][1 + 100] = 1;
    
    /*allCubeVertices[4 + 100][0 + 100][0 + 100] = 1;
    allCubeVertices[4 + 100][1 + 100][0 + 100] = 1;
    allCubeVertices[4 + 100][0 + 100][1 + 100] = 1;
    allCubeVertices[4 + 100][1 + 100][1 + 100] = 1;
    
    allCubeVertices[6 + 100][0 + 100][0 + 100] = 1;
    allCubeVertices[6 + 100][1 + 100][0 + 100] = 1;
    allCubeVertices[6 + 100][0 + 100][1 + 100] = 1;
    allCubeVertices[6 + 100][1 + 100][1 + 100] = 1;
    
    allCubeVertices[3 + 100][0 + 52][0 + 100] = 1;
    allCubeVertices[3 + 100][1 + 52][0 + 100] = 1;
    allCubeVertices[3 + 100][0 + 52][1 + 100] = 1;
    allCubeVertices[3 + 100][1 + 52][1 + 100] = 1;
    allCubeVertices[5 + 100][0 + 52][0 + 100] = 1;
    allCubeVertices[5 + 100][1 + 52][0 + 100] = 1;
    allCubeVertices[5 + 100][0 + 52][1 + 100] = 1;
    allCubeVertices[5 + 100][1 + 52][1 + 100] = 1;*/

    /*srand(time(NULL)); // Seed the random number generator

    int numCubes = 10000;// Generate a random number between 0 and 10

    for (int i = 0; i < numCubes; i++) {
        int x = rand() % 51 - 25; // Generate a random number between -5 and 5
        int y = rand() % 51 - 25; // Generate a random number between -5 and 5
        int z = rand() % 51 - 25; // Generate a random number between -5 and 5

        allCubeVertices[100 + x][100 + y][100 + z] = 1;
    }*/



    // STEPS TO DRAW
    // 1. iterate through every cube
    // 2. get alive verticies
    // 3. get incides from alive vertices
    // 4. copy vectors to arrays
    // 5. create new va, vb, ib

    
    float * finalVertices;
    unsigned int * finalIndices;


    

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
    glm::vec3 cameraPos(275.0f, 275.0f, 275.0f);  // Camera positioned at (5, 0, 5)
    glm::vec3 cameraTarget = glm::vec3(DEFAULTX, DEFAULTY, DEFAULTZ); // Camera looking at the origin
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);  // Up vector
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, up);
    




    // Combine the model, view, and projection matrices
    //glm::mat4 MVP = projection * view * model;
     // adjust screenWidth and screenHeight accordingly
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 800.0f);











    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    // tex7.jpg is a good cell texture also try 8
    Texture texture("res/textures/tex3.png");
    //texture.Bind();
    // slot 0
    //shader.SetUniform1i("u_Texture", 0);
    
    // Set light position and color through uniforms
    shader.SetUniform3f("u_LightPosition", 200.0f, 300.0f, 200.0f);
    shader.SetUniform3f("u_LightColor", 1.0f, 1.0f, 1.0f);
    shader.SetUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
    
    
    va.Unbind();
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
    int count = 1;

    std::vector<glm::vec3> alivePoints;

    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    auto elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds);
    /* Loop until the user closes the window */

    int spaceToRender = 0;

    

    // cube calculations
    std::vector<int> arraySizes;

    int ITERATIONS = 140;
    float ** allSpaces = new float*[ITERATIONS];
    allIndices = new unsigned int*[ITERATIONS];
    for (int i = 0; i < ITERATIONS; i++) {
        std::cout << "ITERATION: " << i << std::endl;
        // get usable buffers from allCubeVertices and alive status
        allSpaces[i] = calcBuffers(allIndices, allCubeVertices, baseIndices, alivePoints, allSpaces, i, arraySizes);
        

        updateCubes(allCubeVertices, alivePoints);

    }

    std::cout << "\n\nDONE LOADING\n";

    std::cout << "array size: " << arraySizes[spaceToRender] << std::endl;
    
    /*for (int i = 0; i < allIndices[spaceToRender].size(); i++) {
        std::cout << allIndices[spaceToRender][i] << " ";
        if ((i + 1) % 6 == 0) {
            std::cout << std::endl;
        }
    }*/

    //std::cout << "SIZE: " << allSpaces[spaceToRender].size() << std::endl;
    //std::cout << "gcountL: " << gcountLines << std::endl;
    //std::cout << "gcountC: " << gcountCubes << std::endl;

    /*for (int i = 0; i < arraySizes[spaceToRender]; i++) {
        std::cout << allSpaces[spaceToRender][i] << " ";
        if ((i + 1) % 9 == 0) {
            std::cout << std::endl;
        }
    }*/
    

    while (!glfwWindowShouldClose(window))
    {
        auto start = std::chrono::high_resolution_clock::now();
    
        /* Render here */
        renderer.Clear();


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (count % 20 == 0) {
            
            //spaceToRender++;
            if (spaceToRender < ITERATIONS - 1) {
                spaceToRender++;
            }
        }
        
        

        IndexBuffer ib(allIndices[spaceToRender], 36 * arraySizes[spaceToRender] / (24 * 9));
        std::cout << "BEFOR\n";
        VertexBuffer vb(allSpaces[spaceToRender], sizeof(float) * arraySizes[spaceToRender]);
        std::cout << "num floats in buffer: " << arraySizes[spaceToRender] << std::endl;
        va.AddBuffer(vb, layout);

        std::cout << "count: " << count << std::endl;
        // Update model matrix for rotati   on
        static float angle = 0.0f;

        glm::vec3 modelCenter(100.0f, 100.0f, 100.0f); // Center of the model

        // Translate the model to the origin
        glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), -modelCenter);

        // Rotate around y-axis
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

        // Translate the model back to its original position
        glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), modelCenter);

        // Combine the transformations
        glm::mat4 model = translateBack * rotate * translateToOrigin;
        // % n is 10,000x angle
        angle += 0.2; // Adjust the rotation speed as needed
        //glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around y-axis

        shader.Bind();
        shader.SetUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
        //cameraPos = glm::vec3(60.0f + angle / 12, 60.0f + angle / 15, 60.0f);  // Camera positioned at (5, 0, 5)
        view = glm::lookAt(cameraPos, cameraTarget, up);


        // Set up transformation matrix and pass it to the shader
        glm::mat4 MVP = projection * view * model; // Assuming you have projection and view matrices set up


        //modelMatrix = glm::translate(glm::mat4(1.0f), translation);
        //MVP = projectionMatrix * viewMatrix * modelMatrix;

        
        shader.SetUniform4f("u_Color", 0.6, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", MVP);
        view = glm::lookAt(cameraPos, cameraTarget, up);  // Camera positioned at (5, 0, 5)
        

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        auto elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds);
        std::cout << "before draw : " << elapsed_milliseconds.count() << " milliseconds" << std::endl;
        start = std::chrono::high_resolution_clock::now();
        // DRAW CALL
        renderer.Draw(va, ib, shader);

        
        
        
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        static float f = 0.0f;
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        ImGui::SliderFloat3("Translation", &cameraPos.x, -200.0f, 200.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        //ImGui::SliderFloat3("Translationy", &translation.y, -10.0f, 10.0f); 
        //ImGui::SliderFloat3("Translationz", &translation.z, -10.0f, 10.0f); 
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();


        /*GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));*/

        

        


       



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        count++;
        
        end = std::chrono::high_resolution_clock::now();
        elapsed_seconds = end - start;
        elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_seconds);
        std::cout << "after draw : " << elapsed_milliseconds.count() << " milliseconds" << std::endl;
        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    for (int i = 0; i < ITERATIONS; i++) {
        delete[] allSpaces[i];
    }
    delete[] allSpaces;

    for (int i = 0; i < ITERATIONS; i++) {
        delete[] allIndices[i];
    }
    delete[] allIndices;


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}