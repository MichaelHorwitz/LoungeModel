#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"
//#include "shapes.h"
#include "sceneObject.h"

#define timeDT std::chrono::_V2::steady_clock::time_point

using namespace glm;
using namespace std;
void testSceneObjectBasics(){
    Triangle* trig = new Triangle(
            vec3(0, 0.4, 0),
            vec3(-0.2, 0.2, 0),
            vec3(0.2, 0.2, 0),
            vec4(1.0f, 1.0f, 0.0f, 1.0f)
            );
    Rectangle* rect = new Rectangle(
        vec3(-0.2, 0.2, 0),
        vec3(0.2, 0.2, 0),
        vec4(1.0f, 0.0f, 0.0f, 1.0f)
    );
    SceneObject *so = new SceneObject();
    so->children.push_back(trig);
    so->children.push_back(rect);
    cout << so->numPoints() << endl;
    cout << so->numVertices() << endl;
    cout << so->numColors() << endl;
    int vCount = 0;
    int cCount = 0;
    float* v = so->toVertexArray();
    float* c = so->toColorArray();
    for (int i = 0; i < so->numPoints(); i++)
    {
        cout << "i: " << i << endl;
        for (int i = 0; i < 3; i++)
        {
            cout << v[vCount++] << "\t";
        }
        cout << endl;
        for(int i = 0; i < 4; i++)
        {
            cout << c[cCount++] << "\t";
        }
        cout << endl;
    }
}
int main(){
    SceneObject* so = new Cylinder(
        SceneObject::cts(25, 7.5, 5),
        SceneObject::cts(5),
        SceneObject::cts(6),
        6,
        vec4(1,1,1,1));
    //cout << so->printVertices();
}