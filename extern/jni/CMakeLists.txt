#
# Library name and options
#

# Target
set(target jni)

# Print status
message(STATUS "Extern lib ${target}")


#
# Sources
#

# Add platform specific headers path
set(platform_specific_headers "")
if (WIN32)
    set(platform_specific_headers win32)
elseif (APPLE)
    set(platform_specific_headers darwin)
elseif (UNIX)
    set(platform_specific_headers linux)
else ()
    message(FATAL_ERROR "Unknown platform!")
endif ()


#
# Create library
#

# Add library
add_library(${target} INTERFACE)


#
# Include directories
#

target_include_directories(${target}
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${platform_specific_headers}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include/${platform_specific_headers}>
        $<INSTALL_INTERFACE:include>
        )
