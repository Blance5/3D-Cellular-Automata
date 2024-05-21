#pragma once


#include <vector>

#define TOTALSTATES 5

struct Point {
    float x;
    float y;
    float z;
};

struct Normal {
    float x;
    float y;
    float z;
};

class Cube {
public:
    

    int aliveStatus() {
        return alive;
    }

    void setAliveStatus(int alive) {
        this->alive = alive;
    }

    std::vector<Point> getPoints() {
        std::vector<Point> p;
        for (int i = 0; i < 24; i++) {
            p.push_back(points[i]);
        }
        return p;
    }

    std::vector<Normal> getNormals() {
        std::vector<Normal> n;
        for (int i = 0; i < 24; i++) {
            n.push_back(normals[i]);
        }
        return n;
    }

    void setColor() {
        color[0] = 0.88f;
        color[1] = 1.0f * (alive * 1.0f / TOTALSTATES);
        color[2] = 0.88f;
    }
    
    /*
    
    
    0.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     1.0f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, 
     1.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 
    0.0f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f, 
    
    0.0f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     1.0f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 
    0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,

    0.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
    0.0f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f, 
    0.0f, 0.0f,  1.0f, -1.0f,  0.0f,  0.0f,

     1.0f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     1.0f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     1.0f, 0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,

     1.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
     1.0f, 0.0f,  1.0f,  0.0f, -1.0f,  0.0f,
    0.0f, 0.0f,  1.0f,  0.0f, -1.0f,  0.0f,
    0.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,

    0.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     1.0f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f
    
    
    */

    Cube() : alive(0) {
    }

    Cube(float x, float y, float z, int alive) : alive(alive) {
        // Initialize all points and normals to zero
        points[0] = {x + 0.0f, y + 0.0f, z + 0.0f}; // front face
        points[1] = {x + 1.0f, y + 0.0f, z + 0.0f}; 
        points[2] = {x + 1.0f, y + 1.0f, z + 0.0f}; 
        points[3] = {x + 0.0f, y + 1.0f, z + 0.0f};      

        points[4] = {x + 0.0f, y + 0.0f, z + 1.0f}; // back face
        points[5] = {x + 1.0f, y + 0.0f, z + 1.0f};
        points[6] = {x + 1.0f, y + 1.0f, z + 1.0f};      
        points[7] = {x + 0.0f, y + 1.0f, z + 1.0f};     

        points[8] = {x + 0.0f, y + 1.0f, z + 1.0f}; //   left face
        points[9] = {x + 0.0f, y + 1.0f, z + 0.0f};     
        points[10] = {x + 0.0f, y + 0.0f, z + 0.0f};       
        points[11] = {x + 0.0f, y + 0.0f, z + 1.0f}; 

        points[12] = {x + 1.0f, y + 1.0f, z + 0.0f}; //   right face
        points[13] = {x + 1.0f, y + 0.0f, z + 0.0f};
        points[14] = {x + 1.0f, y + 0.0f, z + 1.0f}; 
        points[15] = {x + 1.0f, y + 1.0f, z + 1.0f};     

        points[16] = {x + 1.0f, y + 0.0f, z + 0.0f}; // bottom face
        points[17] = {x + 1.0f, y + 0.0f, z + 1.0f}; 
        points[18] = {x + 0.0f, y + 0.0f, z + 1.0f};
        points[19] = {x + 0.0f, y + 0.0f, z + 0.0f};

        points[20] = {x + 0.0f, y + 1.0f, z + 0.0f};  //     top face
        points[21] = {x + 1.0f, y + 1.0f, z + 0.0f};      
        points[22] = {x + 1.0f, y + 1.0f, z + 1.0f};      
        points[23] = {x + 0.0f, y + 1.0f, z + 1.0f};     


        normals[0] = {0.0f,  0.0f, -1.0f}; // front face
        normals[1] = {0.0f,  0.0f, -1.0f};
        normals[2] = {0.0f,  0.0f, -1.0f};
        normals[3] = {0.0f,  0.0f, -1.0f};

        normals[4] = {0.0f,  0.0f,  1.0f}; // back face
        normals[5] = {0.0f,  0.0f,  1.0f};
        normals[6] = {0.0f,  0.0f,  1.0f};
        normals[7] = {0.0f,  0.0f,  1.0f};

        normals[8] = {-1.0f,  0.0f,  0.0f}; // left face
        normals[9] = {-1.0f,  0.0f,  0.0f};
        normals[10] = {-1.0f,  0.0f,  0.0f};
        normals[11] = {-1.0f,  0.0f,  0.0f};

        normals[12] = {1.0f,  0.0f,  0.0f}; // right face
        normals[13] = {1.0f,  0.0f,  0.0f};
        normals[14] = {1.0f,  0.0f,  0.0f};
        normals[15] = {1.0f,  0.0f,  0.0f};

        normals[16] = {0.0f, -1.0f,  0.0f}; // bottom face
        normals[17] = {0.0f, -1.0f,  0.0f};
        normals[18] = {0.0f, -1.0f,  0.0f};
        normals[19] = {0.0f, -1.0f,  0.0f};

        normals[20] = {0.0f,  1.0f,  0.0f}; // top face
        normals[21] = {0.0f,  1.0f,  0.0f};
        normals[22] = {0.0f,  1.0f,  0.0f};
        normals[23] = {0.0f,  1.0f,  0.0f};
    }

private:
    Point points[24];
    Normal normals[24];
    int alive;
    int color[3];
};