if (TARGET imgui_node_editor)
    return()
endif()

set(_imgui_node_editor_SourceDir ${CMAKE_SOURCE_DIR}/3rd/imgui-node-editor/NodeEditor)
set(_imgui_node_editor_BinaryDir ${CMAKE_BINARY_DIR}/3rd/imgui-node-editor/NodeEditor)
set(_imgui_node_editor_IncludeDir 
    ${CMAKE_BINARY_DIR}/3rd/imgui-node-editor/NodeEditor/Include
    ${CMAKE_BINARY_DIR}/3rd/imgui-node-editor/NodeEditor/Source
)

add_subdirectory(${_imgui_node_editor_SourceDir} ${_imgui_node_editor_BinaryDir})

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)

find_package_handle_standard_args(
    imgui_node_editor
    REQUIRED_VARS
        _imgui_node_editor_SourceDir
        _imgui_node_editor_IncludeDir
)

