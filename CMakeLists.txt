cmake_minimum_required(VERSION 3.9)
project(direct_product VERSION 0.1.1 LANGUAGES CXX DESCRIPTION "Implementation of direct product iterator and range")

include(CTest)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -pedantic -Wextra)

find_package(Boost REQUIRED)

add_library(direct_product INTERFACE)
target_include_directories(direct_product INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
target_link_libraries(direct_product INTERFACE Boost::boost)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()

install(TARGETS direct_product EXPORT ${PROJECT_NAME}-targets)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
export(TARGETS direct_product FILE ${PROJECT_NAME}-targets.cmake)

install(EXPORT ${PROJECT_NAME}-targets DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION})
export(PACKAGE ${PROJECT_NAME})

configure_file(config.cmake.in ${PROJECT_NAME}-config.cmake COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION})

list(APPEND CPACK_GENERATOR TGZ)
set(CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_PACKAGE_CONTACT "igsha")
include(CPack)
