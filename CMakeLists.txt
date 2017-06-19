cmake_minimum_required(VERSION 3.6)
project(ray_tracing)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp base.h geometry.cpp geometry.h basic_object.cpp basic_object.h object.cpp object.h scene.cpp scene.h light.cpp light.h mesh.cpp gmath.h gmath.cpp mesh.h bezier.h bezier.cpp newton.cpp newton.h convex_hull.h convex_hull.cpp bounding.cpp bounding.h texture.cpp texture.h)
add_executable(ray_tracing ${SOURCE_FILES})
target_link_libraries(ray_tracing ${OpenCV_LIBS})