cmake_minimum_required(VERSION 3.29)
project(Simple_Shape_Drawer)

set(CMAKE_CXX_STANDARD 20)

add_executable(Simple_Shape_Drawer main.cpp
        core/point.cpp
        core/point.h
        core/shape.h
        core/shape.cpp
        algorithms/filling_algorithm.h
        algorithms/drawing_algorithm.h
        io/file_manager.cpp
        io/file_manager.h
        rendering/screen_writer.cpp
        rendering/screen_writer.h
        rendering/menu_creator.h
        app/app_manager.cpp
        app/app_manager.h
        algorithms/clipping_algorithm.h
        algorithms/clipping_algorithm.h)
