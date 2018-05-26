find_path(CATCH_DIR catch.hpp PATH_SUFFIXES catch)

add_executable(direct_product_test direct_product_test.cpp)
target_include_directories(direct_product_test PRIVATE ${CATCH_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(direct_product_test direct_product)

add_test(NAME direct_product_test COMMAND direct_product_test)
