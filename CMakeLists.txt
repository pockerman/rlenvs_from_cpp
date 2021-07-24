CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
MESSAGE(STATUS "Using CMake ${CMAKE_VERSION}")

PROJECT(Gym_FromCpp VERSION 0.0.1 LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Prevent in-source builds.
# -----------------------------------------------------------------------------

IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	MESSAGE( FATAL_ERROR  "In-source build is not possible. Choose an empty directory for build output.")
ENDIF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

# Be sure to avoid troubles with library paths when using old policy
IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
	CMAKE_POLICY(SET CMP0048 NEW)
ENDIF(COMMAND cmake_policy)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_COMPILER /usr/bin/g++-10)
SET(CMAKE_C_COMPILER /usr/bin/gcc-10)
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_INSTALL_PREFIX /home/alex/qi3/py_from_cpp/install)
SET(CMAKE_LINKER_FLAGS "-pthread")


# find Boost
FIND_PACKAGE(Boost 1.65.0 REQUIRED COMPONENTS python system)

if(Boost_FOUND)
  if(Boost_LIBRARY_DIR)
    MESSAGE( STATUS "Boost_LIBRARY_DIR not empty using it: ${Boost_LIBRARY_DIR}" )
  elseif(BOOST_LIBRARYDIR)
      MESSAGE( STATUS "Boost_LIBRARY_DIR empty, but BOOST_LIBRARYDIR is set. Setting Boost_LIBRARY_DIR to: ${BOOST_LIBRARYDIR}" )
      set(Boost_LIBRARY_DIR ${BOOST_LIBRARYDIR})
  elseif(Boost_LIBRARY_DIRS)
      MESSAGE( STATUS "Boost_LIBRARY_DIR empty, but Boost_LIBRARY_DIRS is set. Setting Boost_LIBRARY_DIR to: ${Boost_LIBRARY_DIRS}" )
      set(Boost_LIBRARY_DIR ${Boost_LIBRARY_DIRS})
  elseif(Boost_LIBRARY_DIR_RELEASE)
      MESSAGE( STATUS "Boost_LIBRARY_DIR empty, but Boost_LIBRARY_DIR_RELEASE is set. Setting Boost_LIBRARY_DIR to: ${Boost_LIBRARY_DIR_RELEASE}" )
      set(Boost_LIBRARY_DIR ${Boost_LIBRARY_DIR_RELEASE})
  elseif(Boost_LIBRARY_DIR_DEBUG)
      MESSAGE( STATUS "Boost_LIBRARY_DIR empty, but Boost_LIBRARY_DIR_DEBUG is set. Setting Boost_LIBRARY_DIR to: ${Boost_LIBRARY_DIR_RELEASE}" )
      set(Boost_LIBRARY_DIR ${Boost_LIBRARY_DIR_DEBUG})
  else()
	MESSAGE( WARNING "Boost_LIBRARY_DIR empty, BOOST_LIBRARYDIR empty, Boost_LIBRARY_DIRS empty: might miss libraries at linking" )
  endif()
else()
  MESSAGE( FATAL_ERROR "Boost was not found!")
endif()

MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "Boost lib: ${Boost_LIBRARY_DIR}")
MESSAGE(STATUS "Boost include: ${Boost_INCLUDE_DIRS}")

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(src/)


FILE(GLOB SRCS src/gymfcpp/*.cpp)

#TARGET_SOURCES(gymfcpplib PUBLIC ${SRCS})

ADD_LIBRARY(gymfcpplib SHARED ${SRCS})
#ADD_SUBDIRECTORY(src/pfcpp)

SET_TARGET_PROPERTIES(gymfcpplib PROPERTIES LINKER_LANGUAGE CXX)
INSTALL(TARGETS gymfcpplib DESTINATION ${CMAKE_INSTALL_PREFIX})
MESSAGE(STATUS "Installation destination at: ${CMAKE_INSTALL_PREFIX}")

# Add the examples
ADD_SUBDIRECTORY(examples)

# Add the tests
ADD_SUBDIRECTORY(tests)
