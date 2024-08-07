project(calc2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE debug)

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# find_package(Boost REQUIRED COMPONENTS program_options)
find_package(ROOT REQUIRED)
find_package(QnTools REQUIRED)

include_directories( ${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${QnTools_INCLUDE_DIR}/QnTools )

set(HEADERS
        src/correlation.h
        src/functions.h
        src/fitter.h
        )
set(SOURCES
        src/correlation.cc
        src/functions.cc
        src/fitter.cc
        )

ROOT_GENERATE_DICTIONARY( G__Calculator ${HEADERS} LINKDEF src/CalculatorLinkDef.h)
add_library(Calculator SHARED ${SOURCES} G__Calculator.cxx )
target_link_libraries(Calculator PUBLIC QnTools::Base ROOT::Tree ROOT::RIO ROOT::MathMore)
target_compile_options(Calculator PUBLIC -Wall -Wextra -Wpedantic)

add_executable(calculate src/main.cpp)
target_link_libraries(calculate PRIVATE Calculator QnTools::Base ROOT::Tree ROOT::RIO ROOT::MathMore )

configure_file(docs/Doxyfile.in ${CMAKE_SOURCE_DIR}/docs/Doxyfile @ONLY)
#
#
#add_library( Calculator INTERFACE )
#target_include_directories( Calculator INTERFACE  ${CMAKE_SOURCE_DIR}/src )
#target_link_libraries( Calculator INTERFACE QnTools::Base ROOT::Tree ROOT::RIO ROOT::MathMore )

#set( CALC_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/src )
#set( QnTools_LIB_PATH QnTools::Base )
#configure_file(src/main.cpp.in ${CMAKE_SOURCE_DIR}/src/main.cpp @ONLY)

#message(STATUS "QnTools include ${QnTools_INCLUDE_DIR}")
#add_library(Calculator SHARED ${SOURCES} G__Calculator.cxx )
#target_link_libraries(Calculator PRIVATE QnTools::Base ROOT::Tree ROOT::RIO ROOT::MathMore)
