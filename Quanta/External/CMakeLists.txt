add_subdirectory("assimp")
add_subdirectory("glad")
add_subdirectory("glfw")
add_subdirectory("libsndfile")
add_subdirectory("openal-soft")
add_subdirectory("stb_image")

add_library("imgui" 
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imconfig.h" 
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_internal.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_rectpack.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_textedit.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_truetype.h"
)

set_target_properties("imgui" PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories("imgui" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/External/imgui")