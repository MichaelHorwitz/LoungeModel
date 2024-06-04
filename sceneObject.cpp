#include "sceneObject.h"

#include <algorithm>

void SceneObject::applyMatrix(mat4x4 m)
{
    if (!children.empty())
    {
        for (SceneObject* so : children)
        {
            so->applyMatrix(m);
        }
    }
    else
    {
        for (int i = 0; i < numPoints(); i++)
        {
            (*vertices[i]) = m * vec4((*vertices[i]), 1);
        }
    }
}

GLfloat* SceneObject::toVertexArray()
{
    int n = numVertices();
    GLfloat *result = new GLfloat[numVertices()];
    if (!children.empty())
    {
        int count = 0;
        for (SceneObject* so : children)
        {
            GLfloat *temp = so->toVertexArray();
            for (int j = 0; j < so->numVertices(); j++)
            {
                result[count++] = temp[j];
            }
            delete[] temp;
        }
    }
    else
    {
        int count = 0;
        for (int i = 0; i < numPoints(); i++)
        {
            result[count++] = (*vertices[i])[0];
            result[count++] = (*vertices[i])[1];
            result[count++] = (*vertices[i])[2];
        }
    }

    return result;   
}

GLfloat* SceneObject::toColorArray()
{
    GLfloat *result = new GLfloat[numColors()];
    if (!children.empty())
    {
        int count = 0;
        for (SceneObject* so : children)
        {
            GLfloat *temp = so->toColorArray();
            for (int j = 0; j < so->numColors(); j++)
            {
                result[count++] = temp[j];
            }
            delete[] temp;
        }
    }
    else
    {
        int count = 0;
        for (int i = 0; i < numPoints(); i++)
        {
            result[count++] = colors[i][0];
            result[count++] = colors[i][1];
            result[count++] = colors[i][2];
            result[count++] = colors[i][3];
        }
    }
    return result;
}

int SceneObject::numPoints()
{
    int count = 0;
    for (SceneObject* so : children)
    {
        count += so->numPoints();
    }
    return count;
}

void SceneObject::initVerCol(vec4 color) {
    int n = numPoints();
    vertices = new vec3*[n];
    int count = 0;
    for(SceneObject* so : children)
    {
        for (int j = 0; j < so->numPoints(); j++)
        {
            vertices[count++] = so->vertices[j];
        }
    }
    colors = new vec4[n];
    for (int i = 0; i < n; i++)
    {
        colors[i] = color;
    }
}

int SceneObject::numVertices()
{
    int count = 0;
    for (SceneObject* so : children)
    {
        count += so->numVertices();
    }
    return count;
}
int SceneObject::numColors()
{
    int count = 0;
    for (SceneObject* so : children)
    {
        count += so->numColors();
    }
 
    return count;
}

SceneObject * SceneObject::makeBasicScene() {
    auto* so = new SceneObject();
    SceneObject* floor = new Rectangle(
        // vec3(coordinateToScreen(
        // vec2(50, 15)), 0.99),
        cts(50, 15, 0.05),
        //vec3(coordinateToScreen(vec2(0,0)), 0.9999999999),
        cts(0,0,0.05),
        vec4(0.5, 0.5, 0.5, 0.99)
        );
    so->children.push_back(floor);
    SceneObject* cube = new Cuboid(
        vec3(cts(35,    12.5,  10)), // fur
        vec3(cts(35,    2.5,   10)), // ful
        vec3(cts(15,    2.5,   10)), // fll
        vec3(cts(15,    12.5,  10)), // flr
        vec3(cts(35,    12.5,  0.1)), // bur
        vec3(cts(35,    2.5,   0.1)), // bul
        vec3(cts(15,    2.5,   0.1)), // bll
        vec3(cts(15,    12.5,  0.1)), // blr
        vec4(1, 0, 0, 1) // color
    );
    so->children.push_back(cube);
    return so;
}

vec2 SceneObject::coordinateToScreen(vec2 inCoord) {
    vec2 ret = vec2(inCoord);
    ret.x = -1 + inCoord.x/50 * 2;
    ret.y =  -1 + (17.5 + inCoord.y)/50 * 2;
    return ret;
}

vec2 SceneObject::cts(float x, float y) {
    return coordinateToScreen(vec2(x, y));
}

vec3 SceneObject::coordinateToScreen(vec3 inCoord) {
    vec2 xyVec = coordinateToScreen(vec2(inCoord.x, inCoord.y));
    vec3 ret = vec3(xyVec, 1 - inCoord.z / 50 * 2);
    return ret;
}

vec3 SceneObject::cts(float x, float y, float z) {
    return coordinateToScreen(vec3(x, y, z));
}

Triangle::Triangle(vec3 p1, vec3 p2, vec3 p3, vec4 color)
{
    children = std::vector<SceneObject*>();
    int n = 3;
    vertices = new vec3*[n];
    colors = new vec4[n];
    vertices[0] = new vec3(p1);
    vertices[1] = new vec3(p2);
    vertices[2] = new vec3(p3);
    colors = new vec4[n];
    for (int i = 0; i < 3; i++)
    {
        colors[i] = color;
    }
}

int Triangle::numVertices()
{
    return 9;
}

int Triangle::numColors()
{
    return 12;   
}

int Triangle::numPoints()
{
    return 3;
}

Rectangle::Rectangle(vec3 ur, vec3 ll, vec4 color)
{
    children = std::vector<SceneObject*>();
    Triangle* t1 = new Triangle(
        vec3(ur[0], ur[1], ur[2]),
        vec3(ll[0], ur[1], ur[2]),
        vec3(ur[0], ll[1], ur[2]),
        color
    );
    Triangle* t2 = new Triangle(
        vec3(ll[0], ll[1], ur[2]),
        vec3(ll[0], ur[1], ur[2]),
        vec3(ur[0], ll[1], ur[2]),
        color
    );
    children.push_back(t1);
    children.push_back(t2);
    int n = numPoints();
    vertices = new vec3*[n];
    int count = 0;
    for(SceneObject* so : children)
    {
        for (int j = 0; j < so->numPoints(); j++)
        {
            vertices[count++] = so->vertices[j];
        }
    }

    colors = new vec4[n];
    for (int i = 0; i < n; i++)
    {
        colors[i] = color;
    }
}

Rectangle::Rectangle(vec3 ur, vec3 ul, vec3 ll, vec3 lr, vec4 color) {
    children = std::vector<SceneObject*>();
    children.push_back(
        new Triangle(
            vec3(ur),
            vec3(ul),
            vec3(lr)
    ));
    children.push_back(
        new Triangle(
        vec3(ul),
        vec3(ll),
        vec3(lr)
    ));
    int n = numPoints();
    vertices = new vec3*[n];
    int count = 0;
    for(SceneObject* so : children)
    {
        for (int j = 0; j < so->numPoints(); j++)
        {
            vertices[count++] = so->vertices[j];
        }
    }
    colors = new vec4[n];
    for (int i = 0; i < n; i++)
    {
        colors[i] = color;
    }
}

Cuboid::Cuboid(vec3 ur, vec3 ul, vec3 ll, vec3 lr, float depth, vec4) {
    Cuboid::Cuboid(vec3 ur, vec3 ul, vec3 ll, vec3 lr, float depth, vec4 color) {
    vec3 bur(ur.x, ur.y, ur.z - depth);
    vec3 bul(ul.x, ul.y, ul.z - depth);
    vec3 bll(ll.x, ll.y, ll.z - depth);
    vec3 blr(lr.x, lr.y, lr.z - depth);

    *this = Cuboid(ur, ul, ll, lr, bur, bul, bll, blr, color);
}
}

Cuboid::Cuboid(vec3 fur, vec3 ful, vec3 fll, vec3 flr, vec3 bur, vec3 bul, vec3 bll, vec3 blr, vec4 color) {
    children = std::vector<SceneObject*>();
    children.push_back(new Rectangle(fur, ful, fll, flr, color));
    children.push_back(new Rectangle(bur, bul, bll, blr, color));
    children.push_back(new Rectangle(fur, bur, blr, flr, color));
    children.push_back(new Rectangle(ful, bul, bll, fll, color));
    children.push_back(new Rectangle(fur, bur, bul, ful, color));
    children.push_back(new Rectangle(flr, blr, bll, fll, color));
    this->SceneObject::initVerCol(color);
}
