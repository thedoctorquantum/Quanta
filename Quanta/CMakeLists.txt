file(GLOB_RECURSE SOURCE Source/*.c Source/*.cpp)
file(GLOB_RECURSE INCLUDE Source/*.h Source/*.hpp)

add_library("Quanta" "${SOURCE}" "${INCLUDE}")

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