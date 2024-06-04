#ifndef SHAPES_H
#define SHAPES_H

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

struct SceneObject{
    vec3** vertices;
    vec4* colors;
    std::vector<SceneObject*> children;
    virtual void applyMatrix(mat4x4);
    virtual GLfloat* toVertexArray();
    virtual GLfloat* toColorArray();
    virtual int numPoints();

    virtual int numVertices();
    virtual int numColors();
};
struct Triangle : public SceneObject{
    Triangle(vec3, vec3, vec3, vec4 = vec4(1.0f, 0.0f, 0.0f, 1.0f));
    int numVertices();
    int numColors();
    int numPoints();
};
struct Rectangle : public SceneObject{
    Rectangle(vec3, vec3, vec4 = vec4(1.0f, 0.0f, 0.0f, 1.0f));
};


#endif /*SHAPES_H*/