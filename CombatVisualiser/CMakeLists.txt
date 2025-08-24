cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# list source files here
add_executable(${PROJECT_NAME}
  "Main.cpp"

  "Input/PlayerCommand.cpp"

  "Components/Character/HealthComponent.cpp"
  "Components/Character/MovementComponent.cpp"
  "Components/Character/HealthBarComponent.cpp"

  "Components/Map/ShapeGridComponent.cpp"
  "Components/Map/ShapeHoverComponent.cpp"
  "Components/Map/ShapeModifierComponent.cpp"
  
  
  "Components/FPSComponent.cpp"

  
 "Levels/Levels.cpp"
 "Events/Events.h" "Behavior/MenuStates.h" "Behavior/MenuStates.cpp") 

 # enable the leak detector tool given by Digital Arts & Entertainment
set(VLD_DIR "$ENV{ProgramFiles\(x86\)}/Visual Leak Detector")
  IF(EXISTS ${VLD_DIR})
    set(VLD_INCLUDE_DIR "${VLD_DIR}/include")
    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(VLD_LIBRARY "${VLD_DIR}/lib/Win64/vld.lib")
    else()
        set(VLD_LIBRARY "${VLD_DIR}/lib/Win32/vld.lib")
    endif()
  endif()


# Use MSVC compiler
target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${ENGINE_NAME})
target_include_directories(${ENGINE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


if(WIN32)
     # add post build commands

  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/Data" "${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}/Data"
  )
endif()