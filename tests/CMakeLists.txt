find_package(Catch2 REQUIRED)
include(Catch)

add_executable(direct_product_test main.cpp direct_product_test.cpp)
target_include_directories(direct_product_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(direct_product_test direct_product Catch2::Catch2)

catch_discover_tests(direct_product_test)

install(CODE "execute_process(COMMAND ${CMAKE_MAKE_PROGRAM} test)")
