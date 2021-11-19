file(GLOB_RECURSE SOURCE "Source/*.h" "Source/*.cpp")

add_executable("QuantaPkg" "${SOURCE}")

set_target_properties("QuantaPkg" PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(
    "QuantaPkg"
)