#pragma once


#include <vector>



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

    static Point GetPoint(int i) {
        return points[i];
    }

    static Normal GetNormal(int i) {
        return normals[i];
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

    Cube() {
        // Initialize all points and normals to zero
        /*
        points[0] = {0.0f, 0.0f, 0.0f}; // front face
        points[1] = {1.0f, 0.0f, 0.0f}; 
        points[2] = {1.0f, 1.0f, 0.0f}; 
        points[3] = {0.0f, 1.0f, 0.0f};      

        points[4] = {0.0f, 0.0f, 1.0f}; // back face
        points[5] = {1.0f, 0.0f, 1.0f};
        points[6] = {1.0f, 1.0f, 1.0f};      
        points[7] = {0.0f, 1.0f, 1.0f};     

        points[8] = {0.0f, 1.0f, 1.0f}; //   left face
        points[9] = {0.0f, 1.0f, 0.0f};     
        points[10] = {0.0f, 0.0f, 0.0f};       
        points[11] = {0.0f, 0.0f, 1.0f}; 

        points[12] = {1.0f, 1.0f, 0.0f}; //   right face
        points[13] = {1.0f, 0.0f, 0.0f};
        points[14] = {1.0f, 0.0f, 1.0f}; 
        points[15] = {1.0f, 1.0f, 1.0f};     

        points[16] = {1.0f, 0.0f, 0.0f}; // bottom face
        points[17] = {1.0f, 0.0f, 1.0f}; 
        points[18] = {0.0f, 0.0f, 1.0f};
        points[19] = {0.0f, 0.0f, 0.0f};

        points[20] = {0.0f, 1.0f, 0.0f};  //     top face
        points[21] = {1.0f, 1.0f, 0.0f};      
        points[22] = {1.0f, 1.0f, 1.0f};      
        points[23] = {0.0f, 1.0f, 1.0f};     


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
        */
    }

private:
    static const Point points[24];
    static const Normal normals[24];
};


const Normal Cube::normals[24] = {
    {0.0f,  0.0f, -1.0f},
    {0.0f,  0.0f, -1.0f},
    {0.0f,  0.0f, -1.0f},
    {0.0f,  0.0f, -1.0f},
    {0.0f,  0.0f,  1.0f},
    {0.0f,  0.0f,  1.0f},
    {0.0f,  0.0f,  1.0f},
    {0.0f,  0.0f,  1.0f},
    {-1.0f,  0.0f,  0.0f},
    {-1.0f,  0.0f,  0.0f},
    {-1.0f,  0.0f,  0.0f},
    {-1.0f,  0.0f,  0.0f},
    {1.0f,  0.0f,  0.0f},
    {1.0f,  0.0f,  0.0f},
    {1.0f,  0.0f,  0.0f},
    {1.0f,  0.0f,  0.0f},
    {0.0f, -1.0f,  0.0f},
    {0.0f, -1.0f,  0.0f},
    {0.0f, -1.0f,  0.0f},
    {0.0f, -1.0f,  0.0f},
    {0.0f,  1.0f,  0.0f},
    {0.0f,  1.0f,  0.0f},
    {0.0f,  1.0f,  0.0f},
    {0.0f,  1.0f,  0.0f}
};

const Point Cube::points[24] = {
    {0.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.0f},
    {1.0f, 0.0f, 0.0f}, 
    {1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, 1.0f},   
};