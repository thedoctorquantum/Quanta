file(GLOB_RECURSE SOURCE "Source/*.h" "Source/*.cpp")

add_executable("Sandbox" "${SOURCE}")

target_link_libraries("Sandbox" PUBLIC "Quanta")

file(COPY "Resources" DESTINATION "${CMAKE_BINARY_DIR}/Sandbox")