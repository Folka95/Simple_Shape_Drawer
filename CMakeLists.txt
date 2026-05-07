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
        algorithms/circle/circle_iterative_polar_drawing_algorithm.cpp
        algorithms/circle/circle_iterative_polar_drawing_algorithm.h
        algorithms/circle/Circle_Polar_drawing_algorithm.cpp
        algorithms/circle/Circle_Polar_drawing_algorithm.h
        algorithms/line/line_dda_drawing_algorithm.cpp
        algorithms/line/line_dda_drawing_algorithm.h
        algorithms/filling/iterive_flood_fill_filling_algorithm.cpp
        algorithms/filling/iterive_flood_fill_filling_algorithm.h
        algorithms/clipping/circle/circle_line_clipping_algorithm.cpp
        algorithms/clipping/circle/circle_line_clipping_algorithm.h
)
