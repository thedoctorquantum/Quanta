file(GLOB_RECURSE SOURCE Source/*.h Source/*.cpp)

add_library("Quanta" "${SOURCE}")

set_target_properties("Quanta" PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories("Quanta" PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/Source"
    "${CMAKE_CURRENT_SOURCE_DIR}/External/glm"
    "${CMAKE_CURRENT_SOURCE_DIR}/External/stb_image"
)

add_subdirectory("External/glad")
add_subdirectory("External/glfw")
add_subdirectory("External/imgui")

target_link_libraries("Quanta" 
    PUBLIC "glfw"
    PUBLIC "glad"
    PUBLIC "imgui"
)