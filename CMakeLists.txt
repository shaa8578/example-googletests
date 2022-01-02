cmake_minimum_required(VERSION 3.10)

project(EXAMPLE_GOOGLETESTS LANGUAGES CXX)
set(PROJECT_LABEL example-googletests)

#*******************************************************************************
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

#*******************************************************************************
if(NOT (CMAKE_BUILD_TYPE))
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Тип сборки" FORCE)
  message(STATUS "Build type: ${CMAKE_BUILD_TYPE} (default)")
else()
  message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif(NOT (CMAKE_BUILD_TYPE))

#*******************************************************************************
find_package(Git REQUIRED)
find_package(Threads REQUIRED)

include(CTest)

#*******************************************************************************
find_path(GOOGLETEST_PATH ".git"
  PATHS "${CMAKE_SOURCE_DIR}/googletest"
  DOC "SerializerPack: Библиотека модульных тестов"
  NO_DEFAULT_PATH
)
if(${GOOGLETEST_PATH} STREQUAL "GOOGLETEST_PATH-NOTFOUND")
  find_package(Git)
  set(GOOGLETEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/googletest")
  set(GOOGLETEST_GIT "https://github.com/google/googletest.git")
  execute_process(
    COMMAND ${GIT_EXECUTABLE} clone ${GOOGLETEST_GIT} ${GOOGLETEST_PATH}
    RESULT_VARIABLE GIT_RESULT
    OUTPUT_VARIABLE GIT_OUTPUT)
  if(NOT (GIT_RESULT EQUAL "0") )
    message(FATAL_ERROR "Error downloading project googletest")
  endif()
endif()

set(BUILD_GMOCK)
add_subdirectory(googletest)

#*******************************************************************************
set(SOURCES_FILES
  main.cpp
)

#*******************************************************************************
set(TEST_FILES
  tst/tst_simple.cpp
)

#*******************************************************************************
add_executable(${PROJECT_LABEL}
  ${SOURCES_FILES}
  ${TEST_FILES}
)

add_test(NAME Test_EXAMPLE_GOOGLETESTS COMMAND ${PROJECT_LABEL})

target_link_libraries(${PROJECT_LABEL} PRIVATE
  Threads::Threads
  gtest gtest_main gmock gmock_main
)

target_compile_definitions(${PROJECT_LABEL} PRIVATE
  cxx_auto_type
  cxx_generic_lambdas
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  target_compile_options(${PROJECT_LABEL} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wno-deprecated-copy
    -Wnonnull
    # -save-temps
  )
endif()

target_include_directories(${PROJECT_LABEL} PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${GTEST_INCLUDE_DIR}
)
