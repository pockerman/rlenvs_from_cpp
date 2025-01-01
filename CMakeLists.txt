CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
MESSAGE(STATUS "Using CMake ${CMAKE_VERSION}")

SET(RLENVSCPP_VERSION_MAJOR 1)
SET(RLENVSCPP_VERSION_MINOR 5)
SET(RLENVSCPP_VERSION_PATCH 0)

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

OPTION(ENABLE_TESTS_FLAG OFF)
OPTION(ENABLE_EXAMPLES_FLAG OFF)
OPTION(ENABLE_DOC_FLAG OFF)

SET(CMAKE_BUILD_TYPE "Debug")

#SET(CMAKE_CXX_COMPILER /usr/bin/g++-11)
#SET(CMAKE_C_COMPILER /usr/bin/gcc-11)
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_LINKER_FLAGS "-pthread")


IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
	
	SET(CMAKE_INSTALL_PREFIX  ${PROJECT_SOURCE_DIR}/install/dbg)
	SET(CMAKE_CXX_FLAGS "-g ") # -Wall -Wextra")
    SET(RLENVSCPP_DEBUG ON)
	
ELSE(CMAKE_BUILD_TYPE STREQUAL "Release")

	SET(CMAKE_INSTALL_PREFIX  ${PROJECT_SOURCE_DIR}/install/opt)
	SET(CMAKE_CXX_FLAGS "-O2")
    SET(RLENVSCPP_DEBUG OFF)
	
ENDIF()

# where to install the library
IF(ENABLE_TESTS_FLAG)
	# find gtest
	FIND_PACKAGE(GTest REQUIRED)

	IF(GTest_FOUND)
		INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})
		LINK_DIRECTORIES("/usr/local/lib")

		MESSAGE( STATUS  "Found Gtest at ${GTEST_INCLUDE_DIRS}.")
    ELSE()
	    MESSAGE( FATAL_ERROR  "ENABLE_TESTS_FLAG is ON but could not find GTest library.")
	ENDIF()
ENDIF()

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

configure_file(config.h.in ${PROJECT_SOURCE_DIR}/src/rlenvs/rlenvscpp_config.h @ONLY)
configure_file(version.h.in ${PROJECT_SOURCE_DIR}/src/rlenvs/rlenvscpp_version.h @ONLY)


# Find boost
FIND_PACKAGE(Boost 1.74.0 REQUIRED)
FIND_PACKAGE(BLAS REQUIRED)
FIND_PACKAGE(Eigen3 REQUIRED)
IF(Boost_FOUND)
	MESSAGE( STATUS  "Found needed Boost C++ library.")
	SET(Boost_USE_SHARED_LIBS ON)
ELSE()
	MESSAGE( FATAL_ERROR  "Boost C++ library is required but not found.")
ENDIF()

MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "Boost lib: ${Boost_LIBRARY_DIR}")
MESSAGE(STATUS "Boost include: ${Boost_INCLUDE_DIRS}")
MESSAGE(STATUS "Install dst: ${CMAKE_INSTALL_PREFIX}")

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(src/)

FILE(GLOB SRCS src/rlenvs/*.cpp
			   src/rlenvs/envs/*.cpp
               src/rlenvs/envs/api_server/*.cpp
               src/rlenvs/envs/gymnasium/*.cpp
               src/rlenvs/envs/gymnasium/toy_text/*.cpp
               src/rlenvs/envs/gymnasium/classic_control/*.cpp
               src/rlenvs/envs/gymnasium/classic_control/vector/*.cpp
               src/rlenvs/envs/gdrl/*.cpp
               #src/rlenvs/envs/gym_pybullet_drones/*.cpp
               src/rlenvs/envs/grid_world/*.cpp
			   src/rlenvs/envs/connect2/*.cpp
			   src/rlenvs/dynamics/*.cpp
			   src/rlenvs/utils/*.cpp
			   src/rlenvs/utils/io/*.cpp
			   src/rlenvs/utils/geometry/*.cpp
			   src/rlenvs/utils/trajectory/*.cpp
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
