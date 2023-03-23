set(PROJECT_NAME GNMRendering)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "GNMRenderer.h"
    "GNMComputeShader.h"
    "GNMMesh.h"
    "GNMShader.h"
    "GNMTexture.h"
    "GNMAssetLoader.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "GNMRenderer.cpp"
    "GNMComputeShader.cpp"
    "GNMMesh.cpp"
    "GNMShader.cpp"
    "GNMTexture.cpp"
    "GNMAssetLoader.cpp"
)
source_group("Source Files" FILES ${Source_Files})


set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

set_source_files_properties(${ALL_FILES} PROPERTIES LANGUAGE CXX)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE GNMRendering)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    ";"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
#if(MSVC)
#    target_compile_options(${PROJECT_NAME} PRIVATE
#        $<$<CONFIG:Release>:
#            /Oi;
##            /Gy
 #       >
#        /permissive-;
#        /std:c++latest;
#        /sdl;
 #       /W3;
  #      ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
   #     ${DEFAULT_CXX_EXCEPTION_HANDLING};
    #    /Y-
   # )
   # target_link_options(${PROJECT_NAME} PRIVATE
   #     $<$<CONFIG:Release>:
   #         /OPT:REF;
   #         /OPT:ICF
   #     >
   # )
#endif()
if(MSVC)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE
        #$<$<CONFIG:Debug>:
        #    -O0
        #>
        #-frtti;
        #-fexceptions;
        -std=c++17;
        #${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        #${DEFAULT_CXX_EXCEPTION_HANDLING};
    )
endif()
################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "GameEngine"
)

if(NOT WIN32)
set(ADDITIONAL_PS4_DEPENDENCIES
    "libSceDbgVideoOut_stub_weak.a" 
    "libSceVideoOut_stub_weak.a"
    "libSceGnm_debug.a"
    "libSceGnmx_debug.a" 
    "libSceGpuAddress.a" 
    "libSceGnmDriver_stub_weak.a" 
    "libSceShaderBinary.a" 
    "libSceGnf_debug.a" 
    "libScePad_stub_weak.a" 
    "libSceUserService_stub_weak.a" 
    "libSceSysmodule_stub_weak.a" 
    "libSceNgs2_stub_weak.a" 
    "libSceAudioOut_stub_weak.a" 
    "-lSceAudio3d_stub_weak" 
)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC "${ADDITIONAL_PS4_DEPENDENCIES}")
endif()

include_directories("./")
include_directories("../GameEngine/")
include_directories("../GameEngine/Graphics/Renderer/RenderingSupport/")
include_directories("../GameEngine/Maths/")
include_directories("../GameEngine/AssetsHandling/")
include_directories("../GameEngine/WindowingAndInput/")
include_directories("../GameEngine/WindowingAndInput/PS4/")

target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}"  "${SCE_ORBIS_SDK_DIR}/target/samples/sample_code/graphics/api_gnm/toolkit/toolkit_debug.a" )
