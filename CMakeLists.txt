#
# CMake options
#

# CMake version
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)


#
# Configure CMake environment
#

# Include cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include cmake modules
include(FetchContent)
include(cmake/Custom.cmake)
include(cmake/GetGitRevisionDescription.cmake)

# Set policies
set_policy(CMP0135 NEW) # Sets the timestamps of extracted contents (FetchContent) to the time of extraction.

#
# Project description and (meta) information
#

# Get git revision
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
string(SUBSTRING "${GIT_SHA1}" 0 12 GIT_REV)
if (NOT GIT_SHA1)
    set(GIT_REV "0")
endif ()

# Meta information about the project
set(META_PROJECT_NAME "dxfeed-graal-api-native")
set(META_PROJECT_DESCRIPTION "C-API wrapper over QD project using GraalVM native-image")
set(META_AUTHOR_ORGANIZATION "Devexperts LLC")
set(META_VERSION_MAJOR "0")
set(META_VERSION_MINOR "1")
set(META_VERSION_PATCH "0")
set(META_VERSION_REVISION "${GIT_REV}")
set(META_VERSION "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_NAME_VERSION "${META_PROJECT_NAME} v${META_VERSION} (${META_VERSION_REVISION})")
set(META_CMAKE_INIT_SHA "${GIT_REV}")

string(MAKE_C_IDENTIFIER ${META_PROJECT_NAME} META_PROJECT_ID)
string(TOUPPER ${META_PROJECT_ID} META_PROJECT_ID)


#
# Declare project
#

# Set project
project(${META_PROJECT_NAME}
        VERSION ${META_VERSION}
        DESCRIPTION ${META_PROJECT_DESCRIPTION}
        LANGUAGES C CXX
        )
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Architecture: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "Project: ${META_PROJECT_NAME}")
message(STATUS "BuildType: ${CMAKE_BUILD_TYPE}")
message(STATUS "SkipTests: ${SKIP_TESTS}")

#
# Project configuration options
#

if (PROJECT_IS_TOP_LEVEL)
    # Generate folders for IDE targets (e.g., VisualStudio solutions)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(IDE_FOLDER "")
endif ()

if (PROJECT_IS_TOP_LEVEL)
    # Set output directories (identically for different platforms)
    set(OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH}/$<CONFIG>)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_$<CONFIG> ${OUTPUT_PATH}/$<CONFIG>)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH}/$<CONFIG>)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_$<CONFIG> ${OUTPUT_PATH}/$<CONFIG>)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH}/$<CONFIG>)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_$<CONFIG> ${OUTPUT_PATH}/$<CONFIG>)
endif ()
message(STATUS "Output path ${OUTPUT_PATH}")

# Create version file
file(WRITE "${OUTPUT_PATH}/${META_PROJECT_NAME}-version" "${META_NAME_VERSION}")


#
# Project modules
#

# Set compile options
include(cmake/CompileOptions.cmake)

# Add sources directory
add_subdirectory(extern)
add_subdirectory(src/apps)
add_subdirectory(src/libs)
if (NOT ${SKIP_TESTS})
    enable_testing() # Must be in top-level file
    add_subdirectory(tests)
endif ()
