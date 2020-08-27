set(imgui_INCLUDE_DIR ${G4Y_ROOT_PATH}/3rd/include/imgui)
set(imgui_INCLUDE_DIRS 
    ${imgui_INCLUDE_DIR}
    ${imgui_INCLUDE_DIR}/examples
)

set(imgui_LIBRARY ${G4Y_ROOT_PATH}/3rd/libs/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>/imgui.lib)
set(imgui_LIBRARYS "${imgui_LIBRARY}")

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    imgui
    DEFAULT_MSG
        imgui_INCLUDE_DIR
        imgui_LIBRARY
        imgui_LIBRARYS
)

mark_as_advanced(imgui_INCLUDE_DIR imgui_LIBRARY imgui_LIBRARYS)