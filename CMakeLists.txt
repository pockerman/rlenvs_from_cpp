CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
MESSAGE(STATUS "Using CMake ${CMAKE_VERSION}")

SET(RLENVSCPP_VERSION_MAJOR 1)
SET(RLENVSCPP_VERSION_MINOR 0)
SET(RLENVSCPP_VERSION_PATCH 14)

SET(RLENVSCPP_VERSION "${RLENVSCPP_VERSION_MAJOR}.${RLENVSCPP_VERSION_MINOR}.${RLENVSCPP_VERSION_PATCH}")
MESSAGE(STATUS "rlenvscpplib Version ${RLENVSCPP_VERSION}")

# create a CMake project
PROJECT(rlenvscpp VERSION ${RLENVSCPP_VERSION} LANGUAGES CXX)

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
SET(CMAKE_CXX_COMPILER /usr/bin/g++-11)
SET(CMAKE_C_COMPILER /usr/bin/gcc-11)
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_LINKER_FLAGS "-pthread")


IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
	SET(CMAKE_INSTALL_PREFIX  ${PROJECT_SOURCE_DIR}/install/dbg)
	SET(CMAKE_CXX_FLAGS "-g ") # -Wall -Wextra")
    SET(RLENVSCPP_DEBUG ON)
ELSEIF(CMAKE_BUILD_TYPE STREQUAL "Release")
	SET(CMAKE_INSTALL_PREFIX  ${PROJECT_SOURCE_DIR}/install/opt)
	SET(CMAKE_CXX_FLAGS "-O2")
    SET(RLENVSCPP_DEBUG OFF)
ENDIF()

# where to install the library

SET(ENABLE_TESTS_FLAG ON)
SET(ENABLE_EXAMPLES_FLAG ON)
SET(ENABLE_DOC_FLAG OFF)

# only neccessary if tests are enabled
SET(GTEST_INCLUDE_DIR /home/alex/MySoftware/gtest/install/include)
SET(GTEST_LIB_DIR /home/alex/MySoftware/gtest/install/lib)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

configure_file(config.h.in ${PROJECT_SOURCE_DIR}/src/rlenvs/rlenvscpp_config.h @ONLY)
configure_file(version.h.in ${PROJECT_SOURCE_DIR}/src/rlenvs/rlenvscpp_version.h @ONLY)


# Find boost
FIND_PACKAGE(Boost 1.74.0 REQUIRED)
IF(Boost_FOUND)
	MESSAGE( STATUS  "Found needed Boost C++ library.")
	SET(Boost_USE_SHARED_LIBS ON)
ELSE()
	MESSAGE( FATAL_ERROR  "Boost C++ library is required but not found.")
ENDIF()


IF( USE_PYTORCH )
	LIST(APPEND CMAKE_PREFIX_PATH ${PYTORCH_PATH})
	FIND_PACKAGE(Torch REQUIRED CONFIG)
ENDIF()

MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "Boost lib: ${Boost_LIBRARY_DIR}")
MESSAGE(STATUS "Boost include: ${Boost_INCLUDE_DIRS}")

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

IF( USE_PYTORCH )
	INCLUDE_DIRECTORIES(${TORCH_INCLUDE_DIRS})
ENDIF()

INCLUDE_DIRECTORIES(src/)

MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "Boost lib: ${Boost_LIBRARY_DIR}")
MESSAGE(STATUS "Boost include: ${Boost_INCLUDE_DIRS}")
MESSAGE(STATUS "Torch include: ${TORCH_INCLUDE_DIRS}")


FILE(GLOB SRCS src/rlenvs/*.cpp
               src/rlenvs/api_server/*.cpp
               src/rlenvs/detail/*.cpp
               src/rlenvs/envs/gymnasium/*.cpp
               src/rlenvs/envs/gymnasium/toy_text/*.cpp
               src/rlenvs/envs/gymnasium/classic_control/*.cpp
               src/rlenvs/envs/gymnasium/vector/*.cpp
               src/rlenvs/envs/gdrl/*.cpp
               src/rlenvs/envs/gym_pybullet_drones/*.cpp
   )

ADD_LIBRARY(rlenvscpplib SHARED ${SRCS})

SET_TARGET_PROPERTIES(rlenvscpplib PROPERTIES LINKER_LANGUAGE CXX)
INSTALL(TARGETS rlenvscpplib DESTINATION ${CMAKE_INSTALL_PREFIX})
MESSAGE(STATUS "Installation destination at: ${CMAKE_INSTALL_PREFIX}")

IF(ENABLE_EXAMPLES_FLAG)
	# Add the examples
	ADD_SUBDIRECTORY(examples)
ELSE()
	MESSAGE(WARNING "Examples have not been enabled")
ENDIF()

IF(ENABLE_TESTS_FLAG)
	# Add the tests
	ADD_SUBDIRECTORY(tests)
ELSE()
	MESSAGE(WARNING "Tests have not been enabled")
ENDIF()


IF(ENABLE_DOC_FLAG)
	# Add the tests
	ADD_SUBDIRECTORY(doc)
ELSE()
	MESSAGE(WARNING "Documentation will not be generated")
ENDIF()
