set(PROJECT_NAME GameEngine)

################################################################################
# Source groups
################################################################################
set(AI_Behaviour_Tree
    "./AI/BehaviorTrees/BehaviourAction.h"
    "./AI/BehaviorTrees/BehaviourNode.h"
    "./AI/BehaviorTrees/BehaviourNodeWithChildren.h"
    "./AI/BehaviorTrees/BehaviourSelector.h"
    "./AI/BehaviorTrees/BehaviourSelector.cpp"
    "./AI/BehaviorTrees/BehaviourSequence.h"
    "./AI/BehaviorTrees/BehaviourSequence.cpp"
)
source_group("AI\\Behaviour Trees" FILES ${AI_Behaviour_Tree})

set(AI_Pushdown_Automata
    "./AI/PushdownAutomata/PushdownMachine.h"
    "./AI/PushdownAutomata/PushdownMachine.cpp"
    "./AI/PushdownAutomata/PushdownState.h"
    "./AI/PushdownAutomata/PushdownState.cpp"
)
source_group("AI\\Pushdown Automata" FILES ${AI_Pushdown_Automata})

set(AI_State_Machine
    "./AI/StateMachine/State.h"
    "./AI/StateMachine/StateMachine.h"  
    "./AI/StateMachine/StateMachine.cpp"
    "./AI/StateMachine/StateMachine.h"
    "./AI/StateMachine/StateTransition.h"
)
source_group("AI\\State Machine" FILES ${AI_State_Machine})

set(AI_Pathfinding
    "./AI/PathFinding/NavigationGrid.h"
    "./AI/PathFinding/NavigationGrid.cpp"  
    "./AI/PathFinding/NavigationMesh.cpp"
    "./AI/PathFinding/NavigationMesh.h"
    "./AI/PathFinding/NavigationMap.h"
    "./AI/PathFinding/NavigationPath.h"
    "./AI/PathFinding/SecurityCallbackClass.h"
)
source_group("AI\\Pathfinding" FILES ${AI_Pathfinding})


set(Asset_Handling
    "./AssetsHandling/Assets.cpp"
    "./AssetsHandling/Assets.h"
    "./AssetsHandling/AssetLoader.h"
    "./AssetsHandling/SimpleFont.cpp"
    "./AssetsHandling/SimpleFont.h"
    "./AssetsHandling/TextureLoader.cpp"
    "./AssetsHandling/TextureLoader.h"
    "./AssetsHandling/TextureWriter.cpp"
    "./AssetsHandling/TextureWriter.h"
    "./AssetsHandling/CollisionDetection.cpp"
    "./AssetsHandling/CollisionDetection.h"
    "./AssetsHandling/Ray.h"
)
source_group("Asset Handling" FILES ${Asset_Handling})

set(Debugging
    "./Debugging/Debug.cpp"
    "./Debugging/Debug.h"
)
source_group("Debugging" FILES ${Debugging})

set(Entity_Components
    "./EntityComponents/GameObject.cpp"
    "./EntityComponents/GameObject.h"
    "./EntityComponents/StateGameObject.cpp"
    "./EntityComponents/StateGameObject.h"
    "./EntityComponents/RenderObject.cpp"
    "./EntityComponents/RenderObject.h"
    "./EntityComponents/Transform.cpp"
    "./EntityComponents/Transform.h"
    "./EntityComponents/Layer.h"
    "./EntityComponents/ParticleSystem.h"
    "./EntityComponents/ParticleSystem.cpp"
    "./EntityComponents/Particle.h"
    "./EntityComponents/Particle.cpp"
    "./EntityComponents/SceneNode.h"
    "./EntityComponents/SceneNode.cpp"
    "./EntityComponents/AnimatedObject.h"
    "./EntityComponents/AnimatedObject.cpp"
)
source_group("Entity Components" FILES ${Entity_Components})

set(Graphics_Camera
    "./Graphics/Camera/Camera.cpp"
    "./Graphics/Camera/Camera.h"
)
source_group("Graphics\\Camera" FILES ${Graphics_Camera})

set(Graphics_Lights
    "./Graphics/Lights/Light.h"
    "./Graphics/Lights/PointLight.h"
    "./Graphics/Lights/SpotLight.h"
    "./Graphics/Lights/DirectionalLight.h"
)
source_group("Graphics\\Lights" FILES ${Graphics_Lights})

set(Graphics_Renderer
    #"./Graphics/Renderer/GameTechRenderer.h"
    #"./Graphics/Renderer/GameTechRenderer.cpp"
)
source_group("Graphics\\Renderer" FILES ${Graphics_Renderer})

if(USE_VULKAN)
list(APPEND Graphics_Renderer "./Graphics/Renderer/GameTechVulkanRenderer.h")
list(APPEND Graphics_Renderer "./Graphics/Renderer/GameTechVulkanRenderer.cpp")
endif()

set(Graphics_Renderering_Support
    "./Graphics/Renderer/RenderingSupport/MeshAnimation.cpp"
    "./Graphics/Renderer/RenderingSupport/MeshAnimation.h"
    "./Graphics/Renderer/RenderingSupport/MeshGeometry.cpp"
    "./Graphics/Renderer/RenderingSupport/MeshGeometry.h"
    "./Graphics/Renderer/RenderingSupport/MeshMaterial.cpp"
    "./Graphics/Renderer/RenderingSupport/MeshMaterial.h"
    "./Graphics/Renderer/RenderingSupport/RendererBase.cpp"
    "./Graphics/Renderer/RenderingSupport/RendererBase.h"
    "./Graphics/Renderer/RenderingSupport/ShaderBase.cpp"
    "./Graphics/Renderer/RenderingSupport/ShaderBase.h"
    "./Graphics/Renderer/RenderingSupport/TextureBase.cpp"
    "./Graphics/Renderer/RenderingSupport/TextureBase.h"
)
source_group("Graphics\\Renderer\\Rendering Support" FILES ${Graphics_Renderering_Support})

set(Maths
    "./Maths/Maths.cpp"
    "./Maths/Maths.h"
    "./Maths/Matrix2.cpp"
    "./Maths/Matrix2.h"
    "./Maths/Matrix3.cpp"
    "./Maths/Matrix3.h"
    "./Maths/Matrix4.cpp"
    "./Maths/Matrix4.h"
    "./Maths/Plane.cpp"
    "./Maths/Plane.h"
    "./Maths/Quaternion.cpp"
    "./Maths/Quaternion.h"
    "./Maths/Vector2.cpp"
    "./Maths/Vector2.h"
    "./Maths/Vector3.cpp"
    "./Maths/Vector3.h"
    "./Maths/Vector4.cpp"
    "./Maths/Vector4.h"
)
source_group("Maths" FILES ${Maths})

set(Scene
    "./Scene/GameWorld.h"
    "./Scene/GameWorld.cpp" 
    "./Scene/PlayerBase.h" 
    "./Scene/PlayerBase.cpp" 
    "./Scene/AnimationController.cpp"
    "./Scene/AnimationController.h"
)
source_group("Scene" FILES ${Scene})

set(Scene_Terrain
    "./Scene/Terrain/Terrain.h"
    "./Scene/Terrain/Terrain.cpp" 
    "./Scene/Terrain/TerrainTexturePack.h"
)
source_group("Scene\\Terrain" FILES ${Scene_Terrain})

set(Windowing_and_Input
    "./WindowingAndInput/GameTimer.cpp"
    "./WindowingAndInput/GameTimer.h"
    "./WindowingAndInput/Keyboard.cpp"
    "./WindowingAndInput/Keyboard.h"
    "./WindowingAndInput/Mouse.cpp"
    "./WindowingAndInput/Mouse.h"
    "./WindowingAndInput/Window.cpp"
    "./WindowingAndInput/Window.h"
    "./WindowingAndInput/InputBase.cpp"
    "./WindowingAndInput/InputBase.h"
)
source_group("Windowing and Input" FILES ${Windowing_and_Input})

set(Windowing_and_Input__Win32
    "./WindowingAndInput/Win32/Win32Keyboard.cpp"
    "./WindowingAndInput/Win32/Win32Keyboard.h"
    "./WindowingAndInput/Win32/Win32Mouse.cpp"
    "./WindowingAndInput/Win32/Win32Mouse.h"
    "./WindowingAndInput/Win32/Win32Window.cpp"
    "./WindowingAndInput/Win32/Win32Window.h"
    "./WindowingAndInput/Win32/XBoxGamepad.cpp"
    "./WindowingAndInput/Win32/XBoxGamepad.h" 
)
source_group("Windowing and Input\\Win32" FILES ${Windowing_and_Input__Win32})

set(Windowing_and_Input__PS4
    "./WindowingAndInput/PS4/PS4Gamepad.cpp"
    "./WindowingAndInput/PS4/PS4Gamepad.h"
    "./WindowingAndInput/PS4/PS4Window.cpp"
    "./WindowingAndInput/PS4/PS4Window.h"
    "./WindowingAndInput/PS4/PS4Frame.cpp"
    "./WindowingAndInput/PS4/PS4Frame.h"
    "./WindowingAndInput/PS4/PS4MemoryAware.cpp"
    "./WindowingAndInput/PS4/PS4MemoryAware.h"

)
source_group("Windowing and Input\\PS4" FILES ${Windowing_and_Input__PS4})

set(Utils
    "Utils.h"
)
source_group("Utils" FILES ${Utils})

set(ALL_FILES
    ${AI_Behaviour_Tree}
    ${AI_Pushdown_Automata}
    ${AI_State_Machine}
    ${AI_Pathfinding}
    ${Asset_Handling}
    ${Debugging}
    ${Entity_Components}
    ${Graphics_Camera}
    ${Graphics_Lights}
    ${Graphics_Renderer}
    ${Graphics_Renderering_Support}
    ${Maths}
    ${Scene}
    ${Scene_Terrain}
    ${Windowing_and_Input}
    ${Windowing_and_Input__Win32}
    ${Windowing_and_Input__PS4}
    ${Utils}
)

#set_source_files_properties(${ALL_FILES} PROPERTIES LANGUAGE CXX)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE GameEngine)

set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)
################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "_CRT_SECURE_NO_WARNINGS;"
    "_MBCS"
    "NOMINMAX"
    # ";"
    #"UNICODE"
    #"_UNICODE"
    #"WIN32_LEAN_AND_MEAN"
    #"_WINSOCK_DEPRECATED_NO_WARNINGS"
    #"_WINSOCKAPI_"   
    #"_WINSOCK2API_"
)

################################################################################
# Compile and link options
################################################################################
if(WIN32) 
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Middleware/ReactPhysics3D/Win32") 
else()
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Middleware/ReactPhysics3D/ORBIS") 
endif() 

#Import the ReactPhysics3D library 
find_package(ReactPhysics3D REQUIRED)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od
        >
        $<$<CONFIG:Release>:
            /O2;
            /Oi;
            /Gy
        >
        /permissive-;
        /std:c++latest;
        /sdl;
        /W3;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
    )
else()
	target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            -O0
        >
        #-frtti;
        -fexceptions;
        -std=c++17;
        #${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        #${DEFAULT_CXX_EXCEPTION_HANDLING};
    )
    include_directories("${CMAKE_PREFIX_PATH}/include")
endif()

include_directories("./")
include_directories("Maths/")
include_directories("WindowingAndInput/")
include_directories("WindowingAndInput/Win32/")
include_directories("WindowingAndInput/PS4/")
include_directories("Physics/")
include_directories("Physics/CollisionDetection/")
include_directories("Scene/")
include_directories("Scene/Terrain")
include_directories("../Networking/")
include_directories("Graphics/Renderer/")
include_directories("Graphics/Renderer/RenderingSupport/")
include_directories("Graphics/Camera/")
include_directories("Graphics/Lights/")
include_directories("EntityComponents/")
include_directories("Debugging/")
include_directories("AssetsHandling/")
include_directories("AI/BehaviorTrees/")
include_directories("AI/PathFinding/")
include_directories("AI/PushdownAutomata/")
include_directories("AI/StateMachine/")
include_directories("../OpenGLRendering/")
include_directories("../GNMRendering/")
include_directories("../Middleware/ImGui/")

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "ReactPhysics3D::ReactPhysics3D"
)

target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")