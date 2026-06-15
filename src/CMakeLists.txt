add_library(njr_algebra_groups INTERFACE)
add_library(njr::algebra_groups ALIAS njr_algebra_groups)
target_compile_features(njr_algebra_groups INTERFACE cxx_std_20)
target_include_directories(njr_algebra_groups INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)
target_link_libraries(njr_algebra_groups INTERFACE
    njr::algebra_core
)
