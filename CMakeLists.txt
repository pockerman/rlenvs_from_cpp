CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
MESSAGE(STATUS "Using CMake ${CMAKE_VERSION}")

PROJECT(rl_envs_from_cpp VERSION 0.0.9 LANGUAGES CXX)

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

SET(GYMFCPP_VERSION_MAJOR 0)
SET(GYMFCPP_VERSION_MINOR 0)
SET(GYMFCPP_VERSION_PATCH 9)
SET(GYMFCPP_VERSION "${GYMFCPP_VERSION_MAJOR}.${GYMFCPP_VERSION_MINOR}.${GYMFCPP_VERSION_PATCH}-alpha")
MESSAGE(STATUS "rl_envs_from_cpp Version ${GYMFCPP_VERSION}")

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_COMPILER /usr/bin/g++-11)
SET(CMAKE_C_COMPILER /usr/bin/gcc-11)
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

# where to install the library
SET(INSTALL_PREFIX "/home/alex/qi3/cubeai/external/rlenvs_cpp/install")

# The CUDA compiler. This is required if the
# PyTorch lib supports CUDA
SET(CMAKE_CUDA_COMPILER "/usr/local/cuda/bin/nvcc")

# PyTorch
SET(USE_PYTORCH OFF)
SET(PYTORCH_PATH "/home/alex/MySoftware/libtorch")

SET(GYMFCPP_DEBUG OFF)
SET(PYTHON_LIB python3.10)
SET(PYTHON_LID_DIR /usr/lib/python3.10/config-3.10-x86_64-linux-gnu/)
SET(BOOST_PYTHON_LIB boost_python310)

SET(ENABLE_TESTS_FLAG OFF)
SET(ENABLE_EXAMPLES_FLAG ON)
SET(ENABLE_DOC_FLAG OFF)
SET(CMAKE_LINKER_FLAGS "-pthread")

# only neccessary if tests are enabled
SET(GTEST_INCLUDE_DIR /home/alex/MySoftware/gtest/install/include)
SET(GTEST_LIB_DIR /home/alex/MySoftware/gtest/install/lib)

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
	SET(CMAKE_INSTALL_PREFIX "${INSTALL_PREFIX}/dbg")
	SET(CMAKE_CXX_FLAGS "-g ") # -Wall -Wextra")
	SET(GYMFCPP_DEBUG ON)
ELSEIF(CMAKE_BUILD_TYPE STREQUAL "Release")
	SET(CMAKE_INSTALL_PREFIX "${INSTALL_PREFIX}/opt")
	SET(CMAKE_CXX_FLAGS "-O2")
ENDIF()

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

configure_file(config.h.in ${PROJECT_SOURCE_DIR}/src/rlenvs/rlenvscpp_config.h @ONLY)
configure_file(version.h.in ${PROJECT_SOURCE_DIR}/src/rlenvs/rlenvscpp_version.h @ONLY)

# find Boost
FIND_PACKAGE(Boost 1.74.0 REQUIRED COMPONENTS python310 system)

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

INCLUDE_DIRECTORIES("/usr/include/python3.10/")
INCLUDE_DIRECTORIES(src/)

MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "Boost lib: ${Boost_LIBRARY_DIR}")
MESSAGE(STATUS "Boost include: ${Boost_INCLUDE_DIRS}")
MESSAGE(STATUS "Torch include: ${TORCH_INCLUDE_DIRS}")


FILE(GLOB SRCS src/rlenvs/*.cpp src/rlenvs/detail/*.cpp src/rlenvs/envs/gymnasium/*.cpp
    src/rlenvs/envs/gymnasium/vector/*.cpp)
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
