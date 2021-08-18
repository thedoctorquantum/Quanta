file(GLOB_RECURSE SOURCE "Source/*.h" "Source/*.cpp")

add_executable("QuantaEditor" "${SOURCE}")

set_target_properties("QuantaEditor" PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(
    "QuantaEditor"
    
    PUBLIC 
    
    "Quanta"
)

file(COPY "Resources" DESTINATION "${CMAKE_BINARY_DIR}/QuantaEditor")