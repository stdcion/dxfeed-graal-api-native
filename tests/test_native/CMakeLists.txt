#
# Dependencies
#

# Add platform specific archive
set(graalvm_test_native_archive "graalvm-test-native-lib")
if (WIN32)
    set(graalvm_test_native_archive ${graalvm_test_native_archive}-windows.zip)
elseif (APPLE)
    set(graalvm_test_native_archive ${graalvm_test_native_archive}-macos.zip)
elseif (UNIX)
    set(graalvm_test_native_archive ${graalvm_test_native_archive}-linux.zip)
else ()
    message(ERROR "Unknown platform!")
endif ()

# Download library
FetchContent_Declare(GraalvmTestNative
        URL ${GRAALVM_TEST_NATIVE_LIB_BASE_PATH}${GRAALVM_TEST_NATIVE_LIB_VERSION}/${graalvm_test_native_archive}
        )
FetchContent_MakeAvailable(GraalvmTestNative)


#
# Executable name and options
#

# Target name
set(target TestNative)

# Output name
set(output_name ${target})

# Print status
message(STATUS "Test ${target}")


#
# Sources
#

set(include_path "${CMAKE_CURRENT_SOURCE_DIR}/")
set(source_path "${CMAKE_CURRENT_SOURCE_DIR}/")

file(GLOB headers CONFIGURE_DEPENDS
        "${include_path}/*.h"
        "${include_path}/*.hpp"
        )
file(GLOB sources CONFIGURE_DEPENDS
        "${source_path}/*.c"
        "${source_path}/*.cpp"
        "${source_path}/*.h"
        "${source_path}/*.hpp"
        )


#
# Create executable
#

# Build executable
add_executable(${target})

# Add sources to target
target_sources(${target}
        PRIVATE
        "${headers}"
        "${sources}"
        )


#
# Project options
#

set_target_properties(${target}
        PROPERTIES
        ${DEFAULT_PROJECT_OPTIONS}
        FOLDER "${IDE_FOLDER}"
        OUTPUT_NAME "${output_name}"
        )


#
# Include directories
#

target_include_directories(${target}
        PRIVATE
        ${DEFAULT_INCLUDE_DIRECTORIES}
        ${PROJECT_BINARY_DIR}/include
        ${PROJECT_BINARY_DIR}/source/include
        )


#
# Libraries
#

target_link_libraries(${target}
        PRIVATE
        ${DEFAULT_LIBRARIES}
        Catch2::Catch2WithMain
        GraalvmTestNative
        )


#
# Compile definitions
#

target_compile_definitions(${target}
        PRIVATE
        ${DEFAULT_COMPILE_DEFINITIONS}
        )


#
# Compile options
#

target_compile_options(${target}
        PRIVATE
        ${DEFAULT_COMPILE_OPTIONS}
        )


#
# Linker options
#

target_link_options(${target}
        PRIVATE
        ${DEFAULT_LINKER_OPTIONS}
        )


#
# Testing
#

add_test(NAME ${target} COMMAND $<TARGET_FILE:${target}>)


#
# Custom commands
#

# Copy shared library to output directory
add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:GraalvmTestNative> $<TARGET_FILE_DIR:${target}>
        )
