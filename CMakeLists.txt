cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread")

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_executable(main main.cpp shader.cpp sceneObject.cpp glad.c)
target_link_libraries(main ${GLEW_LIBRARIES} glfw ${OPENGL_gl_LIBRARY} ${CMAKE_DL_LIBS})

add_custom_target(run
    COMMAND main
    DEPENDS main
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

add_custom_target(cbr
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean -- -j 2
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all -- -j 2
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target run -- -j 2
)