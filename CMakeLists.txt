cmake_minimum_required(VERSION 3.26)
project(DndCombatVisualiser)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_FOLDER CombatVisualiser)

add_subdirectory(includes/imgui)
add_subdirectory(includes/soloud)
add_subdirectory(MkUltra)
add_subdirectory(CombatVisualiser)