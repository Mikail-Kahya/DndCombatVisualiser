add_library(imguiLib
	./imgui.cpp
    ./imgui_draw.cpp
    ./imgui_widgets.cpp
    ./imgui_tables.cpp
    ./backends/imgui_impl_sdl2.cpp
    ./backends/imgui_impl_sdlrenderer2.cpp
    ./imgui_plot.cpp
)

target_compile_features(imguiLib PUBLIC cxx_std_20)

target_include_directories(imguiLib PRIVATE
  "."
)