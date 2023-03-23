set(PROJECT_NAME PaintingGame)

################################################################################
# Source groups
################################################################################
if(WIN32)
set(Header_Files
    "SplitScreenGame.h"
    "SinglePlayerGame.h"
    "PaintingGame.h"
    "GameManager.h"
    "PlatformConfigurations.h"
    "GameAssets.h"
    "NetworkedGame.h"
    "NetworkPlayer.h"
    "EventListener.h"
)
else()
set(Header_Files
    "SplitScreenGame.h"
    "SinglePlayerGame.h"
    "PaintingGame.h"
    "GameManager.h"
    "PlatformConfigurations.h"
    "GameAssets.h"
    "EventListener.h"
)
endif()
source_group("Header Files" FILES ${Header_Files})

if(WIN32)
set(Source_Files
    "Main.cpp"
    "SplitScreenGame.cpp"
    "SinglePlayerGame.cpp"
    "PaintingGame.cpp"
    "GameManager.cpp"
    "GameAssets.cpp"
    "NetworkedGame.cpp"
    "NetworkPlayer.cpp"
    "EventListener.cpp"
)
else()
set(Source_Files
    "Main.cpp"
    "SplitScreenGame.cpp"
    "SinglePlayerGame.cpp"
    "PaintingGame.cpp"
    "GameManager.cpp"
    "GameAssets.cpp"
    "EventListener.cpp"
)
endif()

if(USE_VULKAN)
file(GLOB SHADER_FILES ${ASSET_ROOT}/Shaders/VK/*.*)
endif()

source_group("Source Files" FILES ${Source_Files})

set(GameObjects
    "./GameObjects/Box.h"
    "./GameObjects/Floor.h"
    "./GameObjects/Ink.h"
    "./GameObjects/PaintingObject.h"
    "./GameObjects/HudElement.h"
    "./GameObjects/HudElement.cpp"
    "./GameObjects/Gun.h"
    "./GameObjects/SecurityGuard.h"
    "./GameObjects/SecurityGuard.cpp"
     "./GameObjects/Wall.h"
     "./GameObjects/MuseumItem.h"

    "./GameObjects/HUDOnLoad.h"
)
source_group("Game Objects" FILES ${GameObjects})

set(Player
    "./Player/Player.h"
    "./Player/Player.cpp"
    "./Player/PlayerController.h" 
    "./Player/PlayerController.cpp" 
    "./Player/PlayerControllers.h" 
)
source_group("Player" FILES ${Player})

set(Renderers
    "./Renderers/GNMPaintingGameRenderer.h"
    "./Renderers/GNMPaintingGameRenderer.cpp"
    "./Renderers/OGLPaintingGameRenderer.h"
    "./Renderers/OGLPaintingGameRenderer.cpp"
)
source_group("Renderers" FILES ${Renderers})

set(Screens
    "./Screens/SplashScreen.h"
    "./Screens/SplashScreen.cpp" 
    "./Screens/MainMenuScreen.h"
    "./Screens/MainMenuScreen.cpp" 
    "./Screens/GameScreen.h"
    "./Screens/GameScreen.cpp" 
    "./Screens/GameOverScreen.h"
    "./Screens/GameOverScreen.cpp" 
    "./Screens/LoadingScreen.h"
    "./Screens/LoadingScreen.cpp" 
    "./Screens/ScreenManager.h"
    "./Screens/ScreenManager.cpp"
)
source_group("Screens" FILES ${Screens})

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

if(WIN32)
set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
    ${GameObjects}
    ${Player}
    ${Renderers}
    ${Screens}
    ${ImGui}
)
else()
################################################################################
# PS4 Setup
################################################################################

# Shaders
file(GLOB GNM_SHADER_FILES ${ASSET_ROOT}Shaders/GNM/*.pssl)
source_group("GNM Shader Files" FILES ${GNM_SHADER_FILES})
# Textures
file(GLOB_RECURSE Textures
    ${ASSET_ROOT}Textures/*.jpg
    ${ASSET_ROOT}Textures/*.jpeg
    ${ASSET_ROOT}Textures/*.png
    ${ASSET_ROOT}Textures/*.tga
    ${ASSET_ROOT}Textures/*.JPG
    ${ASSET_ROOT}Textures/*.JPEG
    ${ASSET_ROOT}Textures/*.PNG
    ${ASSET_ROOT}Textures/*.TGA
)
#message(${Textures})
source_group("Textures" FILES ${Textures})
set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
    ${GameObjects}
    ${Player}
    ${Renderers}
    ${Screens}
	${GNM_SHADER_FILES}
    ${Textures}
)

# Shaders
foreach (file ${GNM_SHADER_FILES})
    get_filename_component(file_name ${file} NAME_WE)
	get_filename_component(file_ext ${file} EXT)
		
	if( file_ext STREQUAL  ".pssl")
		message("Compiling PSSL file: ${file}")
		get_filename_component(file_dir ${file} DIRECTORY)
		set_property(SOURCE ${file} PROPERTY VS_TOOL_OVERRIDE WavePsslc) 
        #if(file_name MATCHES "(P|p?)ixel")
		#    set_property(SOURCE ${file} PROPERTY VS_SETTINGS "Profile=sce_ps_orbis" "FileType=Document" "OutputFileName=${file_dir}/${file_name}.sb")
        #else()
        #    set_property(SOURCE ${file} PROPERTY VS_SETTINGS "Profile=sce_vs_vs_orbis" "FileType=Document" "OutputFileName=${file_dir}/${file_name}.sb")
        #endif()
        set_property(SOURCE ${file} PROPERTY VS_SETTINGS "FileType=Document" "OutputFileName=${file_dir}/${file_name}.sb")
	endif()
endforeach()

# Textures
set(IMG2GNF_TOOL_EXECUTEABLE "${SCE_ORBIS_SDK_DIR}/host_tools/bin/orbis-image2gnf")
foreach (file ${Textures})
	get_filename_component(file_name ${file} NAME_WE)
	get_filename_component(file_ext ${file} EXT)
	
	if(file_ext STREQUAL ".jpg" OR 
       file_ext STREQUAL ".jpeg" OR 
	   file_ext STREQUAL ".png" OR 
       file_ext STREQUAL ".tga" OR
	   file_ext STREQUAL ".JPG" OR 
       file_ext STREQUAL ".JPEG" OR 
	   file_ext STREQUAL ".PNG" OR
	   file_ext STREQUAL ".TGA")
		
       message("Reading Texture File:  ${file}")
	   get_filename_component(file_dir ${file} DIRECTORY)
	   get_filename_component(file_abs ${file} ABSOLUTE)
       set(GNF_OUTPUT ${file_name}.gnf)
	   set(GNF_ABS_INPUT ${file_abs})
	   set(GNF_ABS_OUTPUT ${file_dir}/${GNF_OUTPUT})
	   add_custom_command(
	        OUTPUT ${GNF_ABS_OUTPUT}
			
			COMMENT "Converting ${file} to .gnf"
			COMMAND ${IMG2GNF_TOOL_EXECUTEABLE} -i  ${GNF_ABS_INPUT} -o ${GNF_ABS_OUTPUT} -f Auto
            MAIN_DEPENDENCY ${file}
			VERBATIM
	   )
       list(APPEND GNF_FILES ${GNF_OUTPUT})   
	endif()
endforeach()

endif()

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

#target_precompile_headers(${PROJECT_NAME} PRIVATE
#    <vector>
#    <map>
#    <stack>
#    <string>
#    <list>
#    <thread>
#     <atomic>
#    <functional>
#    <iostream>
#    <set>
#)
 
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
else()
	target_compile_options(${PROJECT_NAME} PRIVATE
        #-frtti;
        -fexceptions;
        -std=c++17;
        #${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        #${DEFAULT_CXX_EXCEPTION_HANDLING};
    )
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../Middleware/ReactPhysics3D/ORBIS/include")
endif()

################################################################################
# OpenAL config
################################################################################
include_directories("../Middleware/OpenAL/Win32/include")
link_directories("../Middleware/OpenAL/Win32/lib/Debug/")

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC  ${CMAKE_SOURCE_DIR}/Middleware/OpenAL/Win32/lib/Debug/OpenAL32.lib)

################################################################################
# Dependencies
################################################################################
if(MSVC)
    target_link_libraries(${PROJECT_NAME} LINK_PUBLIC  "Winmm.lib")
    target_link_libraries(${PROJECT_NAME} LINK_PUBLIC  "xinput.lib")
endif()

include_directories(".")
include_directories("./GameObjects")
include_directories("./Renderers")
include_directories("./Screens")
include_directories("./Player")
include_directories("../OpenGLRendering/")
include_directories("../GNMRendering/")
include_directories("../Networking/")
include_directories("../GameEngine/")
include_directories("../GameEngine/Maths/")
include_directories("../GameEngine/EntityComponents/")
include_directories("../GameEngine/Scene/")
include_directories("../GameEngine/Scene/Terrain")
include_directories("../GameEngine/WindowingAndInput/")
include_directories("../GameEngine/WindowingAndInput/Win32/")
include_directories("../GameEngine/WindowingAndInput/PS4/")
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
if(WIN32)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC OpenGLRendering)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC Networking)
else()
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC GNMRendering)
#add_custom_target(
#    GNM_SHADER_FILES
#    DEPENDS ON ${GNF_FILES}
#)

#add_dependencies(${PROJECT_NAME} GNM_SHADER_FILES)
endif()


if(USE_VULKAN)
include_directories("../VulkanRendering/")
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC VulkanRendering)


add_custom_target(
    SHADER_FILES
    DEPENDS ON ${SPIRV_BINARY_FILES}
)
add_dependencies(${PROJECT_NAME} SHADER_FILES)

endif()