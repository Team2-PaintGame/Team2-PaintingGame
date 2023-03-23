cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(BUILD_PS4 ON)
else()
	set(BUILD_PS4 OFF)
endif()
set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(PaintingGame CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

#if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"))
   # message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
#endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
#set(CMAKE_CXX_STANDARD 20)
if(MSVC)
	set(CMAKE_CXX_STANDARD 23)
else()
	set(CMAKE_CXX_STANDARD 17)
endif()
################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(Vulkan)

set(ASSET_ROOT "${CMAKE_SOURCE_DIR}/Assets/" CACHE STRING "" FORCE)
if(WIN32)
add_compile_definitions(ASSETROOTLOCATION="${ASSET_ROOT}") 
else()
add_compile_definitions(ASSETROOTLOCATION="/host/${ASSET_ROOT}") 
endif()

set(USE_VULKAN CACHE BOOL FORCE)
if(USE_VULKAN)
    add_compile_definitions("USEVULKAN") 
endif() 
#if(BUILD_PS4)
#    set(CMAKE_TOOLCHAIN_FILE "C:/Program Files (x86)/SCE/ORBIS/Tools/CMake/PS4.cmake")
#endif()

#set(OPENAL_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/../Middleware/OpenAL/Win32/lib/Debug")
set(OPENAL_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/Middleware/OpenAL/Win32/lib/Debug" CACHE STRING "" FORCE)
set(OPENAL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Middleware/OpenAL/Win32/include" CACHE STRING "" FORCE)

################################################################################
# Sub-projects
################################################################################
add_subdirectory(GameEngine)
if(WIN32)
add_subdirectory(Networking)
add_subdirectory(OpenGLRendering)
else()
add_subdirectory(GNMRendering)
endif()
add_subdirectory(PaintingGame)
if(USE_VULKAN)
    add_subdirectory(VulkanRendering)
endif()
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT PaintingGame)