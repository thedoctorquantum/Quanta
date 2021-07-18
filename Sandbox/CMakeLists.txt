file(GLOB_RECURSE SOURCE Source/*.h Source/*.cpp)

add_executable("Sandbox" "${SOURCE}")

set_target_properties("Sandbox" PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries("Sandbox" PUBLIC "Quanta")