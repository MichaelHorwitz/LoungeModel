#ifndef SHAPES_H
#define SHAPES_H

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

struct SceneObject
{
    vec3 **vertices;
    vec4 *colors;
    std::vector<SceneObject *> children;
    virtual void applyMatrix(mat4x4);
    virtual GLfloat *toVertexArray();
    virtual GLfloat *toColorArray();
    virtual int numPoints();
    virtual void initVerCol(vec4 color);
    virtual std::string printColor();
    virtual std::string printVertices();

    virtual int numVertices();
    virtual int numColors();

    virtual void addFloorsAndWalls();
    virtual void addPanels();
    virtual void addChairs();
    virtual void addTables();
    static SceneObject *makeBasicScene();

    static float cts(float);
    static vec2 coordinateToScreen(vec2);
    static vec2 cts(float x, float y);
    static vec3 coordinateToScreen(vec3);
    static vec3 cts(float x, float y, float z);
    static std::string printVec4(vec4);
    static std::string printVec3(vec3);
};
struct Triangle : public SceneObject
{
    Triangle(vec3, vec3, vec3, vec4 = vec4(1.0f, 0.0f, 0.0f, 1.0f));
    int numVertices();
    int numColors();
    int numPoints();
};
struct Rectangle : public SceneObject
{
    Rectangle(vec3, vec3, vec4 = vec4(1.0f, 0.0f, 0.0f, 1.0f));
    Rectangle(vec3, vec3, vec3, vec3, vec4 = vec4(1.0f, 0.0f, 0.0f, 1.0f));
};
struct Cuboid : public SceneObject
{
    Cuboid(vec3, vec3, vec3, vec3, float, vec4 = vec4(1.0f, 0.0f, 0.0f, 1.0f));
    Cuboid(vec3, vec3, vec4 = vec4(1.0f, 0.0f, 0.0f, 1.0f));
    Cuboid(vec3, vec3, vec3, vec3, vec3, vec3, vec3, vec3, vec4 = vec4(1.0f, 0.0f, 0.0f, 1.0f));
};
struct OpenCuboid : public SceneObject
{
    OpenCuboid(vec3, vec3, vec3, vec3, vec3, vec3, vec3, vec3, vec4 = vec4(1.0f, 0.0f, 0.0f, 1.0f));
};
struct BlueCuboidOttoman : public SceneObject
{
    BlueCuboidOttoman(vec2);
};
struct TinyTable : public SceneObject
{
    TinyTable(vec2);
};

struct Cylinder : public SceneObject
{
    Cylinder(vec3, float, float, int, vec4 = vec4(1.0f, 0.0f, 0.0f, 1.0f));
};
struct BigTable : public SceneObject
{
    BigTable(vec2);
};
struct TubChair : public SceneObject
{
public:
    enum Facing
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,
    };
    TubChair(vec2, Facing = Facing::RIGHT);
};
struct Booth : public SceneObject {
    Booth(vec2);
};

#endif /*SHAPES_H*/