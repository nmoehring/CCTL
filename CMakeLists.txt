cmake_minimum_required(VERSION 3.29)
project(CCTL VERSION 1.0 LANGUAGES CXX)

message("RUNNING CMAKELISTS")

# --- Options ---
option(INSTALL_CCTL_TESTS "Install CCTL Test executable" ON)
option(INSTALL_CCTL "Install CCTL" ON)
option(GTESTING "Run Google Test program." ON)

# --- Fetch gtest ---
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
    GIT_SHALLOW
)

if(MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

# --- Cross-Platform Installation Prefix ---
if(NOT DEFINED CMAKE_INSTALL_PREFIX AND(INSTALL_CCTL OR INSTALL_CCTL_TESTS))
    if(WIN32)
        set(CMAKE_INSTALL_PREFIX "$ENV{LOCALAPPDATA}/CCTL" CACHE PATH "Installation directory" FORCE)
    elseif(APPLE)
        set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.local/CCTL" CACHE PATH "Installation directory" FORCE)
    elseif(UNIX)
        set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation directory" FORCE)
    else()
        message(WARNING "Unknown platform. Using default installation prefix.")
        set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation directory" FORCE)
    endif()
endif()

# --- Create the build directory if it does not exist ---
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR})
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

# --- Compiler Arguments (Cross-Platform) ---
add_library(CCTL_args INTERFACE)
target_compile_features(CCTL_args INTERFACE cxx_std_20)

if(CMAKE_BUILD_TYPE MATCHES Release)
    if(MSVC)
        target_compile_options(CCTL_args INTERFACE /O2) # MSVC Release optimization
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(CCTL_args INTERFACE -O3) # GCC Release optimization
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(CCTL_args INTERFACE -O3) # Clang Release optimization
    endif()
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
    if(MSVC)
        target_compile_options(CCTL_args INTERFACE /Od /Zi) # MSVC Debug optimization
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(CCTL_args INTERFACE -Og -g) # GCC Debug optimization
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(CCTL_args INTERFACE -Og -g) # Clang Debug optimization
    endif()
elseif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    if(MSVC)
        target_compile_options(CCTL_args INTERFACE /O2 /Zi) # MSVC RelWithDebInfo optimization
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(CCTL_args INTERFACE -O3 -g) # GCC RelWithDebInfo optimization
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(CCTL_args INTERFACE -O3 -g) # Clang RelWithDebInfo optimization
    endif()
elseif(CMAKE_BUILD_TYPE MATCHES MinSizeRel)
    if(MSVC)
        target_compile_options(CCTL_args INTERFACE /O1) # MSVC MinSizeRel optimization
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(CCTL_args INTERFACE -Os) # GCC MinSizeRel optimization
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(CCTL_args INTERFACE -Os) # Clang MinSizeRel optimization
    endif()
endif()

# --- Library ---
# target_compile_options(CCTL_args INTERFACE /O2)
add_library(CCTL CCTL.cpp)
target_link_libraries(CCTL CCTL_args)
target_include_directories(CCTL PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# --- Test Executable ---
add_executable(CCTL_test CCTL_test.cpp)
target_link_libraries(CCTL_test
    CCTL
    gtest_main
    CCTL_args
)
target_include_directories(CCTL_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# --- Installation ---
if(INSTALL_CCTL)
    install(TARGETS CCTL CCTL_args EXPORT CCTLTargets)
    install(EXPORT CCTLTargets DESTINATION lib/cmake/CCTL)
    install(DIRECTORY include/ DESTINATION include)
endif()

# Install the test executable if the option is ON
if(INSTALL_CCTL_TESTS)
    install(TARGETS CCTL_test DESTINATION bin)
endif()