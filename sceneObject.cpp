#include "sceneObject.h"

#include <algorithm>

constexpr float floorOffset = 0.05;
constexpr vec4 blue = vec4(55 / 255.0, 90 / 255.0, 128 / 255.0, 1);
constexpr vec4 black = vec4(5 / 255.0, 5 / 255.0, 5 / 255.0, 1);
void SceneObject::applyMatrix(mat4x4 m)
{
    if (!children.empty())
    {
        for (SceneObject *so : children)
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
GLfloat *SceneObject::toVertexArray()
{
    int n = numVertices();
    GLfloat *result = new GLfloat[numVertices()];
    if (!children.empty())
    {
        int count = 0;
        for (SceneObject *so : children)
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
GLfloat *SceneObject::toColorArray()
{
    GLfloat *result = new GLfloat[numColors()];
    if (!children.empty())
    {
        int count = 0;
        for (SceneObject *so : children)
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
            result[count++] = colors[i].x;
            result[count++] = colors[i].y;
            result[count++] = colors[i].z;
            result[count++] = colors[i].w;
        }
    }
    return result;
}
int SceneObject::numPoints()
{
    int count = 0;
    for (SceneObject *so : children)
    {
        count += so->numPoints();
    }
    return count;
}
void SceneObject::initVerCol(vec4 color)
{
    int n = numPoints();
    vertices = new vec3 *[n];
    int count = 0;
    for (SceneObject *so : children)
    {
        for (int j = 0; j < so->numPoints(); j++)
        {
            vertices[count++] = so->vertices[j];
        }
    }
    std::cout << color.x << " " << color.y << " " << color.z << endl;
    colors = new vec4[n];
    for (int i = 0; i < n; i++)
    {
        colors[i] = color;
    }
}
std::string SceneObject::printColor()
{
    int n = numPoints();
    std::string ret = "";
    for (int i = 0; i < n; ++i)
    {
        ret += "i: ";
        ret += std::to_string(i);
        ret += "\n";
        ret += printVec4(colors[i]);
        ret += "\n";
    }
    return ret;
}
int SceneObject::numVertices()
{
    int count = 0;
    for (SceneObject *so : children)
    {
        count += so->numVertices();
    }
    return count;
}
int SceneObject::numColors()
{
    int count = 0;
    for (SceneObject *so : children)
    {
        count += so->numColors();
    }

    return count;
}
void SceneObject::addFloorsAndWalls()
{
    this->children.push_back(new Rectangle(
        cts(50, 15, 0),
        cts(0, 0, 0),
        vec4(0.5, 0.5, 0.5, 0.99)));
}
void SceneObject::addPanels()
{
    constexpr auto white = vec4(0.9, 0.9, 0.9, 0.99);
    this->children.push_back(new Cuboid(
        cts(10, 5.5, 0),
        cts(10.25, 9.5, 3),
        white));
    this->children.push_back(new Cuboid(
        cts(20, 5.5, 0),
        cts(20.25, 9.5, 3),
        white));
    this->children.push_back(new Cuboid(
        cts(5.5, 9.0, 0),
        cts(9.5, 9.25, 3),
        white));
    this->children.push_back(new Cuboid(
        cts(30, 9.0, 0),
        cts(34, 9.25, 3),
        white));
    this->children.push_back(new Cuboid(
        cts(36, 10, 0),
        cts(36.25, 14, 3),
        white));
}
SceneObject *SceneObject::makeBasicScene()
{
    auto *so = new SceneObject();
    so->addFloorsAndWalls();
    so->addPanels();
    so->children.push_back(new BlueCuboidOttoman(vec2(8, 0.5)));
    return so;
}
vec2 SceneObject::coordinateToScreen(vec2 inCoord)
{
    vec2 ret = vec2(inCoord);
    ret.x = -1 + inCoord.x / 50 * 2;
    ret.y = -1 + (17.5 + inCoord.y) / 50 * 2;
    return ret;
}
vec2 SceneObject::cts(float x, float y)
{
    return coordinateToScreen(vec2(x, y));
}
vec3 SceneObject::coordinateToScreen(vec3 inCoord)
{
    vec2 xyVec = coordinateToScreen(vec2(inCoord.x, inCoord.y));
    vec3 ret = vec3(xyVec, 1 - inCoord.z / 50 * 2 - floorOffset);
    return ret;
}

vec3 SceneObject::cts(float x, float y, float z)
{
    return coordinateToScreen(vec3(x, y, z));
}

std::string SceneObject::printVec4(vec4 inVec)
{
    std::string ret = "";
    ret += std::to_string(inVec.x);
    ret += "\t";
    ret += std::to_string(inVec.y);
    ret += "\t";
    ret += std::to_string(inVec.z);
    ret += "\t";
    ret += std::to_string(inVec.w);
    return ret;
}

Triangle::Triangle(vec3 p1, vec3 p2, vec3 p3, vec4 color)
{
    children = std::vector<SceneObject *>();
    int n = 3;
    vertices = new vec3 *[n];
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
    children = std::vector<SceneObject *>();
    Triangle *t1 = new Triangle(
        vec3(ur[0], ur[1], ur[2]),
        vec3(ll[0], ur[1], ur[2]),
        vec3(ur[0], ll[1], ur[2]),
        color);
    Triangle *t2 = new Triangle(
        vec3(ll[0], ll[1], ur[2]),
        vec3(ll[0], ur[1], ur[2]),
        vec3(ur[0], ll[1], ur[2]),
        color);
    children.push_back(t1);
    children.push_back(t2);
    int n = numPoints();
    vertices = new vec3 *[n];
    int count = 0;
    for (SceneObject *so : children)
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

Rectangle::Rectangle(vec3 ur, vec3 ul, vec3 ll, vec3 lr, vec4 color)
{
    children = std::vector<SceneObject *>();
    children.push_back(
        new Triangle(
            vec3(ur),
            vec3(ul),
            vec3(lr),
            color));
    children.push_back(
        new Triangle(
            vec3(ul),
            vec3(ll),
            vec3(lr),
            color));
    int n = numPoints();
    vertices = new vec3 *[n];
    int count = 0;
    for (SceneObject *so : children)
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

Cuboid::Cuboid(vec3 ur, vec3 ul, vec3 ll, vec3 lr, float depth, vec4 color)
{
    vec3 bur(ur.x, ur.y, ur.z - depth);
    vec3 bul(ul.x, ul.y, ul.z - depth);
    vec3 bll(ll.x, ll.y, ll.z - depth);
    vec3 blr(lr.x, lr.y, lr.z - depth);

    *this = Cuboid(ur, ul, ll, lr, bur, bul, bll, blr, color);
}

Cuboid::Cuboid(vec3 fur, vec3 bll, vec4 color)
{
    vec3 ful(fur.x, bll.y, fur.z);
    vec3 fll(bll.x, bll.y, fur.z);
    vec3 flr(bll.x, fur.y, fur.z);
    vec3 bur(fur.x, fur.y, bll.z);
    vec3 bul(fur.x, bll.y, bll.z);
    vec3 blr(bll.x, fur.y, bll.z);
    std::cout << color.x << " " << color.y << " " << color.z << endl;
    *this = Cuboid(fur, ful, fll, flr, bur, bul, bll, blr, color);
}

Cuboid::Cuboid(vec3 fur, vec3 ful, vec3 fll, vec3 flr, vec3 bur, vec3 bul, vec3 bll, vec3 blr, vec4 color)
{
    children = std::vector<SceneObject *>();
    cout << "numPoints: " << numPoints() << endl;
    children.push_back(new Rectangle(fur, ful, fll, flr, color));
    cout << "numPoints: " << numPoints() << endl;
    children.push_back(new Rectangle(bur, bul, bll, blr, color));
    cout << "numPoints: " << numPoints() << endl;
    children.push_back(new Rectangle(fur, bur, blr, flr, color));
    cout << "numPoints: " << numPoints() << endl;
    children.push_back(new Rectangle(ful, bul, bll, fll, color));
    cout << "numPoints: " << numPoints() << endl;
    children.push_back(new Rectangle(fur, bur, bul, ful, color));
    cout << "numPoints: " << numPoints() << endl;
    children.push_back(new Rectangle(flr, blr, bll, fll, color));
    std::cout << color.x << " " << color.y << " " << color.z << endl;

    int n = numPoints();
    vertices = new vec3 *[n];

    int count = 0;
    for (SceneObject *so : children)
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
    // cout << printColor();
}

BlueCuboidOttoman::BlueCuboidOttoman(vec2 origin)
{
    children = std::vector<SceneObject *>();
    // seat
    children.push_back(new Cuboid(
        cts(origin.x + 0.5, origin.y + 0.5, 0.4),
        cts(origin.x - 0.5, origin.y - 0.5, 0.2),
        blue));
    // bottom left leg
    children.push_back(new Cuboid(
        cts(origin.x - 0.45 + 0.05, origin.y - 0.45 + 0.05, 0),
        cts(origin.x - 0.45 - 0.05, origin.y - 0.45 - 0.05, 0.2),
        black));
    children.push_back(new Cuboid(
        cts(origin.x + 0.45 + 0.05, origin.y - 0.45 + 0.05, 0),
        cts(origin.x + 0.45 - 0.05, origin.y - 0.45 - 0.05, 0.2),
        black));
    children.push_back(new Cuboid(
        cts(origin.x - 0.45 + 0.05, origin.y + 0.45 + 0.05, 0),
        cts(origin.x - 0.45 - 0.05, origin.y + 0.45 - 0.05, 0.2),
        black));
    children.push_back(new Cuboid(
        cts(origin.x + 0.45 + 0.05, origin.y + 0.45 + 0.05, 0),
        cts(origin.x + 0.45 - 0.05, origin.y + 0.45 - 0.05, 0.2),
        black));

    this->initVerCol(blue);
}
