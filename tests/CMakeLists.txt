find_package(Catch2 REQUIRED)
include(Catch)

add_executable(dirprod_test main.cpp dirprod.cpp)
target_include_directories(dirprod_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(dirprod_test dirprod Catch2::Catch2)

catch_discover_tests(dirprod_test)
