cmake_minimum_required(VERSION 3.29)
project(Simple_Shape_Drawer)

set(CMAKE_CXX_STANDARD 20)

add_executable(Simple_Shape_Drawer main.cpp
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
        algorithms/clipping_algorithm.h
        io/input_reader.h
        core/shapes/line.cpp
        core/shapes/line.h
        core/shapes/circle.cpp
        core/shapes/circle.h
        core/shapes/ellipse.cpp
        core/shapes/ellipse.h
        core/shapes/curve.cpp
        core/shapes/curve.h
        core/shapes/rectangle.cpp
        core/shapes/rectangle.h
        algorithms/Circle/Circle_iterativePolar_drawing_algorithm.cpp
        algorithms/Circle/Circle_iterativePolar_drawing_algorithm.h
        algorithms/Circle/Circle_Polar_drawing_algorithm.cpp
        algorithms/Circle/Circle_Polar_drawing_algorithm.h
        algorithms/line/line_dda_drawing_algorithm.cpp
        algorithms/line/line_dda_drawing_algorithm.h
        algorithms/Filling/IterFloodFill_filling_algorithm.cpp
        algorithms/Filling/IterFloodFill_filling_algorithm.h
)
