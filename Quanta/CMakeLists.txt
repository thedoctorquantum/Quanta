file(GLOB_RECURSE SOURCE Source/*.c Source/*.cpp)
file(GLOB_RECURSE INCLUDE Source/*.h Source/*.hpp)

add_library("Quanta" "${SOURCE}" "${INCLUDE}")

set_target_properties("Quanta" PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories("Quanta" 
    PUBLIC 
    
    "${CMAKE_CURRENT_SOURCE_DIR}/Source"
    "${CMAKE_CURRENT_SOURCE_DIR}/External/glm"

    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/External/stb_image"
)

add_library("imgui" 
    "${CMAKE_CURRENT_SOURCE_DIR}/External/imgui/imconfig.h" 
    "${CMAKE_CURRENT_SOURCE_DIR}/External/imgui/imgui_draw.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/External/imgui/imgui_internal.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/External/imgui/imgui_tables.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/External/imgui/imgui_widgets"
    "${CMAKE_CURRENT_SOURCE_DIR}/External/imgui/imgui.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/External/imgui/imgui.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/External/imgui/imstb_rectpack.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/External/imgui/imstb_textedit.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/External/imgui/imstb_truetype.h"
)

set_target_properties("imgui" PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories("imgui" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/External/imgui")

add_subdirectory("External/glfw")
add_subdirectory("External/glad")

target_link_libraries("Quanta" 
    PRIVATE "glfw"
    PRIVATE "glad"
    PUBLIC "imgui"
)