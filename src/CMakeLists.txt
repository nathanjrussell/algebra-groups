add_library(algebra_groups INTERFACE)
add_library(algebra::groups ALIAS algebra_groups)
target_compile_features(algebra_groups INTERFACE cxx_std_20)
target_include_directories(algebra_groups INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)
target_link_libraries(algebra_groups INTERFACE
    algebra::core
)
