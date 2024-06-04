#include "sceneObject.h"
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