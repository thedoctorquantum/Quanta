file(GLOB_RECURSE SOURCE Source/*.c Source/*.cpp)
file(GLOB_RECURSE INCLUDE Source/*.h Source/*.hpp)

add_library("Quanta" "${SOURCE}" "${INCLUDE}")

set_target_properties("Quanta" PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories("Quanta" 
    PUBLIC 
    
    "${CMAKE_CURRENT_SOURCE_DIR}/Source"
    "${CMAKE_CURRENT_SOURCE_DIR}/External/glm"
    "${CMAKE_CURRENT_SOURCE_DIR}/External/imgui"
)

add_subdirectory("External")

target_link_libraries("Quanta" 
    PUBLIC 
    
    "imgui"
    
    PRIVATE 

    "glfw"
    "glad"
    "stb_image"
    "OpenAL"
    "sndfile"
)