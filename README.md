# Algebra groups

Cyclic groups and direct sums with typed elements.

```cmake
include(FetchContent)

FetchContent_Declare(
    njr_algebra_groups
    GIT_REPOSITORY git@github.com:nathanjrussell/algebra-groups.git
    GIT_TAG v0.1.0
)

FetchContent_MakeAvailable(njr_algebra_groups)

target_link_libraries(my_app PRIVATE njr::algebra_groups)
```
