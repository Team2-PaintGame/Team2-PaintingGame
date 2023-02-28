set(PROJECT_NAME PaintingGame)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "NetworkedGame.h"
    "NetworkPlayer.h"
    "PaintingGame.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Main.cpp"
    "NetworkedGame.cpp"
    "NetworkPlayer.cpp"
    "PaintingGame.cpp"
)

if(USE_VULKAN)
file(GLOB SHADER_FILES ${ASSET_ROOT}/Shaders/VK/*.*)
endif()

source_group("Source Files" FILES ${Source_Files})

set(GameObjects
    "./GameObjects/Box.h"
    "./GameObjects/Floor.h"
    "./GameObjects/SecurityGuard.h"
    "./GameObjects/SecurityGuard.cpp"
     "./GameObjects/Wall.h"
     "./GameObjects/MuseumItem.h"

)
source_group("Game Objects" FILES ${GameObjects})

set(ImGui
    "../Middleware/ImGui/imgui.cpp"
    "../Middleware/ImGui/imgui.h"
    "../Middleware/ImGui/imgui_demo.cpp"
    "../Middleware/ImGui/imgui_draw.cpp"
    "../Middleware/ImGui/imgui_impl_opengl3.cpp"
    "../Middleware/ImGui/imgui_impl_opengl3.h"
    "../Middleware/ImGui/imgui_impl_opengl3_loader.h"
    "../Middleware/ImGui/imgui_impl_win32.cpp"
    "../Middleware/ImGui/imgui_impl_win32.h"
    "../Middleware/ImGui/imgui_internal.h"
    "../Middleware/ImGui/imgui_widgets.cpp" 
    "../Middleware/ImGui/imgui_tables.cpp" 
    "../Middleware/ImGui/imstb_rectpack.h" 
    "../Middleware/ImGui/imstb_textedit.h" 
    "../Middleware/ImGui/imstb_truetype.h" 
     "../Middleware/ImGui/imconfig.h" 
)
source_group("ImGui Files" FILES ${ImGui})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
    ${GameObjects}
    ${ImGui}
)

if(USE_VULKAN)
    set(ALL_FILES 
        ${Header_Files}
        ${Source_Files}
        ${SHADER_FILES}
    )

foreach (file ${SHADER_FILES})
    get_filename_component(file_name ${file} NAME)
	get_filename_component(file_ext ${file} EXT)
	
	if(file_ext STREQUAL ".h" OR file_ext STREQUAL ".cpp")
		continue()
	endif()
		
	if( file_ext STREQUAL  ".vert" OR
		file_ext STREQUAL  ".frag" OR
		file_ext STREQUAL  ".comp" OR
		file_ext STREQUAL  ".geom" OR
		file_ext STREQUAL  ".tesc" OR
		file_ext STREQUAL  ".tese" OR
		file_ext STREQUAL  ".rgen" OR
		file_ext STREQUAL  ".rint" OR
		file_ext STREQUAL  ".rahit" OR
		file_ext STREQUAL  ".rchit" OR
		file_ext STREQUAL  ".rmiss" OR
		file_ext STREQUAL  ".rcall" OR
		file_ext STREQUAL  ".task" OR
		file_ext STREQUAL  ".mesh"
	)
		message("Adding custom command to ${file}")
		get_filename_component(file_dir ${file} ABSOLUTE)
        set(SPIRV_OUTPUT ${file_name}.spv)
		set(SPIRV_ABS_INPUT ${file_dir})
		set(SPIRV_ABS_OUTPUT ${file_dir}.spv)
		
		add_custom_command(
			OUTPUT ${SPIRV_ABS_OUTPUT}
			
			COMMENT "Compiling GLSL shader:"
			COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V  ${SPIRV_ABS_INPUT} -o ${SPIRV_ABS_OUTPUT}
            DEPENDS ${file}
			VERBATIM
		)
        list(APPEND SPIRV_BINARY_FILES ${SPIRV_OUTPUT})   
	endif()
endforeach()

endif()

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME}  ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE PaintingGame)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)

################################################################################
# Compile definitions
################################################################################
if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "UNICODE;"
        "_UNICODE" 
        "WIN32_LEAN_AND_MEAN"
        "_WINSOCKAPI_"   
        "_WINSOCK2API_"
        "_WINSOCK_DEPRECATED_NO_WARNINGS"
        "NOMINMAX"
        "_CRT_SECURE_NO_WARNINGS;"
    )
endif()

target_precompile_headers(${PROJECT_NAME} PRIVATE
    <vector>
    <map>
    <stack>
    <string>
    <list>
    <thread>
    <atomic>
    <functional>
    <iostream>
    <set>
)
 
if(USE_VULKAN)
target_precompile_headers(${PROJECT_NAME} PRIVATE
    "../VulkanRendering/SmartTypes.h"

    "../VulkanRendering/VulkanRenderer.h"
    "../VulkanRendering/VulkanShader.h"
    "../VulkanRendering/VulkanShaderBuilder.h"
    "../VulkanRendering/VulkanTexture.h"
    "../VulkanRendering/VulkanMesh.h"
    "../VulkanRendering/VulkanPipelineBuilder.h"
    "../VulkanRendering/VulkanDynamicRenderBuilder.h"

    "../VulkanRendering/VulkanDescriptorSetLayoutBuilder.h"
    "../VulkanRendering/VulkanRenderPassBuilder.h"

    "../VulkanRendering/VulkanCompute.h"
    "../VulkanRendering/VulkanComputePipelineBuilder.h"
)
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /Oi;
            /Gy
        >
        /permissive-;
        /std:c++latest;
        /sdl;
        /W3;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /Y-
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
endif()

################################################################################
# Dependencies
################################################################################
if(MSVC)
    target_link_libraries(${PROJECT_NAME} LINK_PUBLIC  "Winmm.lib")
    target_link_libraries(${PROJECT_NAME} LINK_PUBLIC  "xinput.lib")
endif()

include_directories("./GameObjects")
include_directories("../OpenGLRendering/")
include_directories("../Networking/")
include_directories("../GameEngine/")
include_directories("../GameEngine/Maths/")
include_directories("../GameEngine/EntityComponents/")
include_directories("../GameEngine/Scene/")
include_directories("../GameEngine/Scene/Terrain")
include_directories("../GameEngine/WindowingAndInput/")
include_directories("../GameEngine/WindowingAndInput/Win32/")
include_directories("../GameEngine/Physics/")
include_directories("../GameEngine/Physics/CollisionDetection/")
include_directories("../GameEngine/Graphics/Renderer/")
include_directories("../GameEngine/Graphics/Renderer/RenderingSupport/")
include_directories("../GameEngine/Graphics/Camera/")
include_directories("../GameEngine/Graphics/Lights/")
include_directories("../GameEngine/Graphics/UI/")
include_directories("../GameEngine/Debugging/")
include_directories("../GameEngine/AssetsHandling/")
include_directories("../GameEngine/AI/BehaviorTrees/")
include_directories("../GameEngine/AI/PathFinding/")
include_directories("../GameEngine/AI/PushdownAutomata/")
include_directories("../GameEngine/AI/StateMachine/")
include_directories("../Middleware/ImGui/")

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC GameEngine)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC OpenGLRendering)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC Networking)


if(USE_VULKAN)
include_directories("../VulkanRendering/")
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC VulkanRendering)


add_custom_target(
    SHADER_FILES
    DEPENDS ON ${SPIRV_BINARY_FILES}
)

add_dependencies(${PROJECT_NAME} SHADER_FILES)

endif()