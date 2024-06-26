#include "sceneObject.h"

#include <algorithm>
#include "../../../../usr/include/glm/ext/matrix_transform.hpp"

constexpr float floorOffset = 0.05;
constexpr vec4 blue = vec4(55 / 255.0, 90 / 255.0, 128 / 255.0, 1);
constexpr vec4 black = vec4(5 / 255.0, 5 / 255.0, 5 / 255.0, 1);
constexpr vec4 white = vec4(240 / 255.0, 240 / 255.0, 240 / 255.0, 1);
constexpr vec4 offWhite = vec4(220 / 255.0, 220 / 255.0, 220 / 255.0, 1);
constexpr vec4 offOffWhite = vec4(200 / 255.0, 200 / 255.0, 200 / 255.0, 1);
constexpr vec4 floorgrey = vec4(60 / 255.0, 55 / 255.0, 55 / 255.0, 1);
constexpr vec4 wallgrey = vec4(85 / 255.0, 85 / 255.0, 85 / 255.0, 1);
constexpr vec4 wallBlack = vec4(15 / 255.0, 15 / 255.0, 15 / 255.0, 1);
constexpr vec4 yellow = vec4(200 / 255.0, 200 / 255.0, 50 / 255.0, 1);
constexpr vec4 brown = vec4(71 / 255.0, 58 / 255.0, 48 / 255.0, 1);

constexpr vec4 grey = vec4(0.5, 0.5, 0.5, 1);
constexpr vec4 glassBlue = vec4(124 / 255.0, 183 / 255.0, 194 / 255.0, 0.5);
constexpr vec4 chairGrey = vec4(180 / 255.0, 180 / 255.0, 180 / 255.0, 1);


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
    // std::cout << color.x << " " << color.y << " " << color.z << endl;
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
std::string SceneObject::printVertices()
{
    int n = numPoints();
    std::string ret = "";
    for (int i = 0; i < n; ++i)
    {
        ret += "i: ";
        ret += std::to_string(i);
        ret += "\n";
        ret += printVec3(*vertices[i]);
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
        floorgrey));
    this->children.push_back(new Rectangle(
        cts(50, 16, 1.5),
        cts(0, 16, 1.5),
        cts(0, 15, 0),
        cts(50, 15, 0),
        white));
    this->children.push_back(new Rectangle(
        cts(50, 15, 3),
        cts(0, 15, 3),
        cts(0, 16, 1.5),
        cts(50, 16, 1.5),
        offWhite));
    this->children.push_back(new Rectangle(
        cts(50, 15, 3),
        cts(0, 15, 3),
        cts(0, 18, 14),
        cts(50, 18, 14),
        white));
    this->children.push_back(new OpenCuboid(
        cts(50, 18, 15),
        cts(0, 18, 15),
        cts(0, 18, 14),
        cts(50, 18, 14),
        cts(50, 20, 15),
        cts(0, 20, 15),
        cts(0, 20, 14),
        cts(50, 20, 14),
        offWhite));
    this->children.push_back(new Rectangle(
        cts(50, 18, 15),
        cts(0, 18, 15),
        cts(0, 20, 20),
        cts(50, 20, 20),
        white));
    this->children.push_back(new Rectangle(
        cts(50, -1, 1.5),
        cts(0, -1, 1.5),
        cts(0, 0, 0),
        cts(50, 0, 0),
        white));
    this->children.push_back(new Rectangle(
        cts(50, 0, 3),
        cts(0, 0, 3),
        cts(0, -1, 1.5),
        cts(50, -1, 1.5),
        offWhite));
    this->children.push_back(new Rectangle(
        cts(50, 0, 3),
        cts(0, 0, 3),
        cts(0, -3, 14),
        cts(50, -3, 14),
        white));
    this->children.push_back(new OpenCuboid(
        cts(50, -3, 15),
        cts(0, -3, 15),
        cts(0, -3, 14),
        cts(50, -3, 14),
        cts(50, -5, 15),
        cts(0, -5, 15),
        cts(0, -5, 14),
        cts(50, -5, 14),
        offWhite));
    this->children.push_back(new Rectangle(
        cts(50, -3, 15),
        cts(0, -3, 15),
        cts(0, -5, 20),
        cts(50, -5, 20),
        white));
    this->children.push_back(new Rectangle(
        cts(50, 20, 20),
        cts(50, -5, 0),
        wallgrey
    ));
    this->children.push_back(new Cuboid(
        cts(50, 11.5, 20),
        cts(48.5,3.5,0),
        wallBlack
    ));
    this->children.push_back(new Rectangle(
        cts(0.01, -5, 20),
        cts(0, 20, 0),
        glassBlue
    ));
}
void SceneObject::addPanels()
{
    constexpr auto white = vec4(0.9, 0.9, 0.9, 1);
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
    this->children.push_back(new Cuboid(
    cts(38, 8, 0),
    cts(42, 8.25, 3),
    white));
}
void SceneObject::addChairs()
{
    this->children.push_back(new KioskTable(vec2(2, 7.5)));
    for (int i = 0; i < 5; ++i)
    {
        this->children.push_back(new BlueCuboidOttoman(vec2(14 + i * 7, 1)));
        this->children.push_back(new TinyTable(vec2(14 + i * 7, 2.5)));
    }
    this->children.push_back(new Cylinder(
        cts(24, 4.5, 0.25),
        cts(0.5),
        cts(0.45),
        12,
        blue));
    this->children.push_back(new Cylinder(
        cts(24, 5.5, 0.25),
        cts(0.5),
        cts(0.45),
        12,
        blue));
    this->children.push_back(new Cylinder(
        cts(26, 4.5, 0.25),
        cts(0.5),
        cts(0.45),
        12,
        blue));
    this->children.push_back(new Cylinder(
        cts(26, 5.5, 0.25),
        cts(0.5),
        cts(0.45),
        12,
        blue));
    this->children.push_back(new Cylinder(
        cts(24, 11.5, 0.25),
        cts(0.5),
        cts(0.45),
        12,
        blue));
    this->children.push_back(new Cylinder(
        cts(24, 12.5, 0.25),
        cts(0.5),
        cts(0.45),
        12,
        blue));
    this->children.push_back(new Cylinder(
        cts(26, 11.5, 0.25),
        cts(0.5),
        cts(0.45),
        12,
        blue));
    this->children.push_back(new Cylinder(
        cts(26, 12.5, 0.25),
        cts(0.5),
        cts(0.45),
        12,
        blue));

    for (int i = 0; i < 4; ++i)
    {
        this->children.push_back(new TubChair(vec2(10.30 + 0.75 / 2, 5.5 + 0.75 / 2 + i * 1.05)));
    }
    for (int i = 0; i < 4; ++i)
    {
        this->children.push_back(new TubChair(
            vec2(20 - 0.75 / 2, 5.5 + 0.75 / 2 + i * 1.05),
            TubChair::Facing::LEFT));
    }
    for (int i = 0; i < 3; ++i)
    {
        this->children.push_back(new TubChair(
            vec2(32 + i * 1.05, 9.25 + 0.75 / 2),
            TubChair::Facing::UP));
    }
    for (int i = 0; i < 4; ++i)
    {
        this->children.push_back(new TubChair(
            vec2(35.5 - 0.75 / 2, 10 + 0.75 / 2 + i * 1.05),
            TubChair::Facing::LEFT));
    }
    for (int i = 0; i < 3; ++i)
    {
        this->children.push_back(new TubChair(
            vec2(29 - 0.75 / 2, 10 + 0.75 / 2 + i * 1.05),
            TubChair::Facing::LEFT));
    }
}
void SceneObject::addTables()
{
    this->children.push_back(new BigTable(
        vec2(25, 5.0)));
    this->children.push_back(new BigTable(
        vec2(25, 12.0)));
    this->children.push_back(new Booth(
        vec2(7.5,12)));
    this->children.push_back(new Booth(
        vec2(40, 5.5)));
}
SceneObject *SceneObject::makeBasicScene()
{
    auto *so = new SceneObject();
    so->addPanels();
    so->addChairs();
    so->addTables();
    so->addFloorsAndWalls();
    mat4 rotationMatrix = glm::rotate(mat4(1.0f), glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
    so->applyMatrix(transpose(rotationMatrix));
    return so;
}
float SceneObject::cts(float x)
{
    return x / 50 * 2;
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
    vec3 ret = vec3(xyVec, inCoord.z / 50 * 2 - floorOffset);
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

std::string SceneObject::printVec3(vec3 inVec)
{
    std::string ret = "";
    ret += std::to_string(inVec.x);
    ret += "\t";
    ret += std::to_string(inVec.y);
    ret += "\t";
    ret += std::to_string(inVec.z);
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
    *this = Rectangle(
        ur,
        vec3(ur.x, ll.y, ur.z),
        ll,
        vec3(ll.x, ur.y, ll.z),
        color);
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
    // std::cout << color.x << " " << color.y << " " << color.z << endl;
    *this = Cuboid(fur, ful, fll, flr, bur, bul, bll, blr, color);
}

Cuboid::Cuboid(vec3 fur, vec3 ful, vec3 fll, vec3 flr, vec3 bur, vec3 bul, vec3 bll, vec3 blr, vec4 color)
{
    children = std::vector<SceneObject *>();
    children.push_back(new Rectangle(fur, ful, fll, flr, color));
    children.push_back(new Rectangle(bur, bul, bll, blr, color));
    children.push_back(new Rectangle(fur, bur, blr, flr, color));
    children.push_back(new Rectangle(ful, bul, bll, fll, color));
    children.push_back(new Rectangle(fur, bur, bul, ful, color));
    children.push_back(new Rectangle(flr, blr, bll, fll, color));
    // std::cout << color.x << " " << color.y << " " << color.z << endl;

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

OpenCuboid::OpenCuboid(vec3 fur, vec3 ful, vec3 fll, vec3 flr, vec3 bur, vec3 bul, vec3 bll, vec3 blr, vec4 color)
{
    children = std::vector<SceneObject *>();
    // children.push_back(new Rectangle(fur, ful, fll, flr, color));
    children.push_back(new Rectangle(bur, bul, bll, blr, offOffWhite));
    children.push_back(new Rectangle(fur, bur, blr, flr, color));
    children.push_back(new Rectangle(ful, bul, bll, fll, color));
    children.push_back(new Rectangle(fur, bur, bul, ful, color));
    children.push_back(new Rectangle(flr, blr, bll, fll, color));
    this->initVerCol(color);
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

TinyTable::TinyTable(vec2 origin)
{
    children = std::vector < SceneObject * > ();
    children.push_back(new Cylinder(
        cts(origin.x, origin.y, 0.01),
        cts(0.1),
        cts(0.2),
        12,
        black));
    children.push_back(new Cylinder(
        cts(origin.x, origin.y, 0.11),
        cts(0.5),
        cts(0.05),
        12,
        black));
    children.push_back(new Cylinder(
        cts(origin.x, origin.y, 0.51),
        cts(0.1),
        cts(0.25),
        12,
        offWhite));
}

KioskTable::KioskTable(vec2 origin)
{
    children = std::vector<SceneObject *>();
    children.push_back(new Cuboid(
        cts(origin.x + 0.5, origin.y + 1, 1.2),
        cts(origin.x - 0.5, origin.y - 1, 0),
        black));
    children.push_back(new Cuboid(
        cts(origin.x + 0.5, origin.y + 1.5, 1.2),
        cts(origin.x - 1, origin.y + 1, 0),
        black));
}

Cylinder::Cylinder(vec3 origin, float height, float radius, int numTriangles, vec4 color)
{
    children = std::vector<SceneObject *>();
    std::vector<vec3> points;
    float angleStep = 2 * M_PI / numTriangles;
    for (int i = 0; i < numTriangles; i++)
    {
        vec3 point;
        point.x = origin.x + radius * cos(i * angleStep);
        point.y = origin.y + radius * sin(i * angleStep);
        point.z = origin.z + height / 2;
        points.push_back(point);
    }
    for (int i = 0; i < points.size(); ++i)
    {
        int j = (i + 1) % points.size();
        children.push_back(new Triangle(
            vec3(points.at(i).x, points.at(i).y, points.at(i).z),
            vec3(points.at(j).x, points.at(j).y, points.at(j).z),
            vec3(origin.x, origin.y, origin.z + height / 2),
            color));
    }
    for (int i = 0; i < points.size(); ++i)
    {
        int j = (i + 1) % points.size();
        children.push_back(new Triangle(
            vec3(points.at(i).x, points.at(i).y, points.at(i).z - height),
            vec3(points.at(j).x, points.at(j).y, points.at(j).z - height),
            vec3(origin.x, origin.y, origin.z - height / 2),
            color));
    }
    for (int i = 0; i < points.size(); ++i)
    {
        int j = (i + 1) % points.size();
        vec3 ur = vec3(points.at(i).x, points.at(i).y, points.at(i).z);
        vec3 ll = vec3(points.at(j).x, points.at(j).y, points.at(j).z - height);
        children.push_back(new Rectangle(
            vec3(points.at(i).x, points.at(i).y, points.at(i).z),          // 0
            vec3(points.at(j).x, points.at(j).y, points.at(j).z),          // 1
            vec3(points.at(j).x, points.at(j).y, points.at(j).z - height), // 3
            vec3(points.at(i).x, points.at(i).y, points.at(i).z - height), // 2
            color));
    }
    initVerCol(color);
}

BigTable::BigTable(vec2 origin)
{
    children = std::vector<SceneObject *>();
    constexpr float tableWidth = 1.5;
    constexpr float tableHeight = 3;
    children.push_back(new Cuboid(
        cts(origin.x + tableWidth / 2, origin.y + tableHeight / 2, 1),
        cts(origin.x - tableWidth / 2, origin.y - tableHeight / 2, 1.3),
        white));
    constexpr float legSize = 0.1;
    children.push_back(new Cuboid(
        cts(origin.x + tableWidth / 2 - legSize / 2 + legSize / 2, origin.y + tableHeight / 2 - legSize / 2 + legSize / 2, 0),
        cts(origin.x + tableWidth / 2 - legSize / 2 - legSize / 2, origin.y + tableHeight / 2 - legSize / 2 - legSize / 2, 1),
        black));
    children.push_back(new Cuboid(
        cts(origin.x + tableWidth / 2 - legSize / 2 + legSize / 2, origin.y - (tableHeight / 2 - legSize / 2) + legSize / 2, 0),
        cts(origin.x + tableWidth / 2 - legSize / 2 - legSize / 2, origin.y - (tableHeight / 2 - legSize / 2) - legSize / 2, 1),
        black));
    children.push_back(new Cuboid(
        cts(origin.x - (tableWidth / 2 - legSize / 2) + legSize / 2, origin.y + tableHeight / 2 - legSize / 2 + legSize / 2, 0),
        cts(origin.x - (tableWidth / 2 - legSize / 2) - legSize / 2, origin.y + tableHeight / 2 - legSize / 2 - legSize / 2, 1),
        black));
    children.push_back(new Cuboid(
        cts(origin.x - (tableWidth / 2 - legSize / 2) + legSize / 2, origin.y - (tableHeight / 2 - legSize / 2) + legSize / 2, 0),
        cts(origin.x - (tableWidth / 2 - legSize / 2) - legSize / 2, origin.y - (tableHeight / 2 - legSize / 2) - legSize / 2, 1),
        black));
}

TubChair::TubChair(vec2 origin, Facing facing)
{
    children = std::vector<SceneObject *>();
    constexpr float baseWidth = 0.75;
    constexpr float baseHeight = 0.35;
    constexpr float heightFromFloor = 0.15;
    children.push_back(new Cuboid(
        cts(origin.x + baseWidth / 2, origin.y + baseWidth / 2, heightFromFloor + baseHeight),
        cts(origin.x - baseWidth / 2, origin.y - baseWidth / 2, heightFromFloor),
        chairGrey));
    vec2 sideOrigin;
    if (facing == Facing::RIGHT)
    {
        sideOrigin = vec2(origin.x - (baseWidth / 2 - baseHeight / 2), origin.y);
    }
    if (facing == Facing::LEFT)
    {
        sideOrigin = vec2(origin.x + (baseWidth / 2 - baseHeight / 2), origin.y);
    }
    if (facing == Facing::RIGHT || facing == LEFT)
    {
        children.push_back(new Cuboid(
            cts(sideOrigin.x + baseHeight / 2, sideOrigin.y + baseWidth / 2, heightFromFloor + baseHeight + baseWidth),
            cts(sideOrigin.x - baseHeight / 2, sideOrigin.y - baseWidth / 2, heightFromFloor + baseHeight),
            chairGrey));
    }
    if (facing == UP)
    {
        sideOrigin = vec2(origin.x, origin.y - (baseWidth / 2 - baseHeight / 2));
    }
    if (facing == UP || facing == DOWN)
    {
        children.push_back(new Cuboid(
            cts(sideOrigin.x + baseWidth / 2, sideOrigin.y + baseHeight / 2, heightFromFloor + baseHeight + baseWidth),
            cts(sideOrigin.x - baseWidth / 2, sideOrigin.y - baseHeight / 2, heightFromFloor + baseHeight),
            chairGrey));
    }
    constexpr float legSize = 0.05;
    children.push_back(new Cuboid(
        cts(origin.x + baseWidth / 2 - legSize / 2 + legSize / 2, origin.y + baseWidth / 2 - legSize / 2 + legSize / 2, 0),
        cts(origin.x + baseWidth / 2 - legSize / 2 - legSize / 2, origin.y + baseWidth / 2 - legSize / 2 - legSize / 2, heightFromFloor),
        black));
    children.push_back(new Cuboid(
        cts(origin.x + baseWidth / 2 - legSize / 2 + legSize / 2, origin.y - (baseWidth / 2 - legSize / 2) + legSize / 2, 0),
        cts(origin.x + baseWidth / 2 - legSize / 2 - legSize / 2, origin.y - (baseWidth / 2 - legSize / 2) - legSize / 2, heightFromFloor),
        black));
    children.push_back(new Cuboid(
        cts(origin.x - (baseWidth / 2 - legSize / 2) + legSize / 2, origin.y + baseWidth / 2 - legSize / 2 + legSize / 2, 0),
        cts(origin.x - (baseWidth / 2 - legSize / 2) - legSize / 2, origin.y + baseWidth / 2 - legSize / 2 - legSize / 2, heightFromFloor),
        black));
    children.push_back(new Cuboid(
        cts(origin.x - (baseWidth / 2 - legSize / 2) + legSize / 2, origin.y - (baseWidth / 2 - legSize / 2) + legSize / 2, 0),
        cts(origin.x - (baseWidth / 2 - legSize / 2) - legSize / 2, origin.y - (baseWidth / 2 - legSize / 2) - legSize / 2, heightFromFloor),
        black));
}



Booth::Booth(vec2 origin)
{
    children = std::vector<SceneObject *>();
    constexpr float panelWidth = 0.2;
    constexpr float panelLength = 4;
    constexpr float panelHeight = 2;
    constexpr float seatFromGround = 0.5;
    constexpr float seatHeight = 0.3;
    children.push_back(new Cuboid(
        cts(origin.x + panelWidth/2, origin.y + panelLength/2, panelHeight),
        cts(origin.x - panelWidth/2, origin.y - panelLength/2, 0),
        brown));
    children.push_back(new Cuboid(
        cts(origin.x + panelLength/2, origin.y + panelLength/2 + panelWidth/2, panelHeight),
        cts(origin.x - panelLength/2, origin.y + panelLength/2 - panelWidth/2, 0),
        brown
    ));
    children.push_back(new Cuboid(
        cts(origin.x + panelLength/2, origin.y + panelWidth/2, panelHeight),
        cts(origin.x - panelLength/2, origin.y - panelWidth/2, 0),
        brown
    ));
    children.push_back(new Cuboid(
        cts(origin.x + panelLength/2, origin.y - panelLength/2 + panelWidth/2, panelHeight),
        cts(origin.x - panelLength/2, origin.y - panelLength/2 - panelWidth/2, 0),
        brown
    ));
    children.push_back(new Cuboid(
        cts(origin.x + panelLength/2, origin.y + panelLength/2 - panelWidth/2, seatFromGround + seatHeight),
        cts(origin.x + panelWidth/2, origin.y + panelWidth/2, seatFromGround),
        yellow
    ));
    children.push_back(new Cuboid(
    cts(origin.x - panelLength/2, origin.y + panelLength/2 - panelWidth/2, seatFromGround + seatHeight),
    cts(origin.x - panelWidth/2, origin.y + panelWidth/2, seatFromGround),
    yellow
));
    children.push_back(new Cuboid(
    cts(origin.x - panelLength/2, origin.y -( panelLength/2 - panelWidth/2), seatFromGround + seatHeight),
    cts(origin.x - panelWidth/2, origin.y - panelWidth/2, seatFromGround),
    yellow
));
    children.push_back(new Cuboid(
cts(origin.x + panelLength/2, origin.y -( panelLength/2 - panelWidth/2), seatFromGround + seatHeight),
cts(origin.x + panelWidth/2, origin.y - panelWidth/2, seatFromGround),
yellow
));

}
