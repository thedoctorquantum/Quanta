file(GLOB_RECURSE SOURCE Source/*.c Source/*.h Source/*.cpp Source/*.hpp)

add_library("${CMAKE_PROJECT_NAME}" "${SOURCE}")

set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories("${CMAKE_PROJECT_NAME}" 
    PUBLIC 
    
    "${CMAKE_CURRENT_SOURCE_DIR}/Include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/External/glm"
    "${CMAKE_CURRENT_SOURCE_DIR}/External/imgui"
)

add_subdirectory("External")

target_link_libraries("${CMAKE_PROJECT_NAME}" 
    PUBLIC 
    
    "imgui"
    
    PRIVATE 

    "glfw"
    "glad"
    "stb_image"
    "OpenAL"
    "sndfile"
)