cmake_minimum_required(VERSION 3.10)

# Set the project name
project(JeuAvion)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable with additional source files
add_executable(JeuAvion
        JeuAvion.cpp
        Entites.cpp
        Interface.cpp
)

# Include directories
include_directories(C:/Users/amran/CLionProjects/Projet_Jeu/S2/JeuAvion/JeuAvion/include)

# Link libraries (if any)
# target_link_libraries(JeuAvion <library_name>)