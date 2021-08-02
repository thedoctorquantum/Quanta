file(GLOB_RECURSE SOURCE Source/*.c Source/*.h Source/*.cpp Source/*.hpp)

add_library("Quanta" "${SOURCE}")

set_target_properties("Quanta" PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories("Quanta" 
    PUBLIC 
    
    "${CMAKE_CURRENT_SOURCE_DIR}/Include/"
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