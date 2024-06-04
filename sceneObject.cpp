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
    SceneObject* floor = new Rectangle(vec3(coordinateToScreen(
        vec2(50, 15)), 0.99),
        vec3(coordinateToScreen(vec2(0,0)), 0.99),
        vec4(0.5, 0.5, 0.5, 0.99)
        );
    so->children.push_back(floor);
    vec3 points[8];
    points[0] = cts(15, 12.5, 10);
    points[1] = cts(15, 12.5, 00);
    points[2] = cts(35, 12.5, 00);
    points[3] = cts(35, 12.5, 10);
    points[4] = cts(15,  2.5, 10);
    points[5] = cts(15,  2.5, 00);
    points[6] = cts(35,  2.5, 00);
    points[7] = cts(35,  2.5, 10);
    SceneObject* cube = new Cuboid(points);
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

Cuboid::Cuboid(vec3 ur, vec3 ul, vec3 ll, vec3 lr, float, vec4) {

}

Cuboid::Cuboid(vec3 points[8], vec4 color) {
//     std::vector<vec3> pointsVector(points, points + 8);
// std::sort(pointsVector.begin(), pointsVector.end(), [](const vec3& a, const vec3& b) {
//     if (a.y != b.y)
//         return a.y < b.y;
//     if (a.x != b.x)
//         return a.x < b.x;
//     return a.z > b.z;
//     });
//     for (int i = 0; i < 8; ++i) {
//         points[i] =pointsVector.at(i);
//     }
    children = std::vector<SceneObject*>();
    Rectangle* rectangles[8];
    int buildOrder[6][4] = {
        {0, 1, 2, 3},
        {0, 3, 4, 7},
        {0, 1, 5, 4},
        {6, 5, 4, 7},
        {6, 2, 3, 7},
        {6, 2, 1, 5},
    };
    for (int i = 0; i < 6; ++i) {
        vec3 vecs[4];
        for (int j = 0; j < 4; ++j) {
            vecs[j] = points[buildOrder[i][j]];
        }
        children.push_back(new Rectangle(
            vecs[0],
            vecs[1],
            vecs[2],
            vecs[3],
            color
        ));
    }
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
