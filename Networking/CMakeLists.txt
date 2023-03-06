set(PROJECT_NAME Networking)

################################################################################
# Source groups
################################################################################

set(Source_Files
    "GameClient.cpp"
    "GameServer.cpp"
    "NetworkBase.cpp"
    "NetworkObject.cpp"
    "NetworkState.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(Header_Files
    "GameClient.h"  
    "GameServer.h"
    "NetworkBase.h"
    "NetworkObject.h"
    "NetworkState.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Networking_enet_Files
    #"./enet/callbacks.h"
    "./enet/callbacks.c"
    "./enet/list.h"
    "./enet/list.c"
    "./enet/protocol.h"
    "./enet/protocol.c"
    "./enet/win32.h"
    "./enet/win32.c"

    "./enet/enet.h"
    "./enet/time.h"
    "./enet/types.h"
    "./enet/utility.h"

    "./enet/compress.c"
    "./enet/host.c"
    "./enet/packet.c"
    "./enet/peer.c"
)
source_group("eNet" FILES ${Networking_enet_Files})

set(ALL_FILES
    ${Source_Files}
    ${Header_Files}
    ${Networking_enet_Files}
)

set_source_files_properties(${ALL_FILES} PROPERTIES LANGUAGE CXX)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Networking)

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
    "UNICODE"
    "_UNICODE"
    "WIN32_LEAN_AND_MEAN"
    "_WINSOCK_DEPRECATED_NO_WARNINGS"
)

################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "GameEngine"
    "ws2_32.lib"
)

include_directories("./")
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
include_directories("../GameEngine/Debugging/")
include_directories("../GameEngine/AssetsHandling/")
include_directories("../GameEngine/AI/BehaviorTrees/")
include_directories("../GameEngine/AI/PathFinding/")
include_directories("../GameEngine/AI/PushdownAutomata/")
include_directories("../GameEngine/AI/StateMachine/")


if(MSVC)
    target_link_libraries(${PROJECT_NAME} PRIVATE "ws2_32.lib")
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")


