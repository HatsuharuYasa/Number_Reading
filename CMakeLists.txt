cmake_minimum_required(VERSION 3.10)
project(NumReadingProject)
set(PROJECT_NAME "NumReadingProject")

#Define source file
set(SOURCE_FILES src/main.cpp src/gridcanvas.cpp src/inference.cpp)

#Add the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

#WXWIDGET LIBRARY CONFIGURATION-------
#Find wxWidgets, modify the "wxWidgets_ROOT_DIR" to your wxwidget installation location
set(wxWidgets_ROOT_DIR $ENV{USERLIBS}/wxWidget)
set(wxWidgets_LIB_DIR ${wxWidgets_ROOT_DIR}/lib/vc_x64_lib)
set(wxWidgets_INCLUDE_DIR ${wxWidgets_ROOT_DIR}/include)

set(wxWidgets_CONFIGURATION mswu)

#Add the wxWidgets include directory
include_directories(${wxWidgets_INCLUDE_DIR})

find_package(wxWidgets REQUIRED COMPONENTS core base)

#Include wxWidgets use file
include(${wxWidgets_USE_FILE})

#Link wxWidgets libraries
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

#ONNXRUNTIME LIBRARY CONFIGURATION--------------
#Set the Onnxruntime directory, modify the "ONNXRUNTIME_ROOTDIR" to your actual onnxruntime installation location
set(ONNXRUNTIME_ROOT $ENV{USERLIBS}/onnxruntime)
set(ONNXRUNTIME_INCLUDE_DIR ${ONNXRUNTIME_ROOT}/include)
set(ONNXRUNTIME_LIB_DIR ${ONNXRUNTIME_ROOT}/build/Windows/RelWithDebInfo/RelWithDebInfo)
set(ONNXRUNTIME_LIBRARY ${ONNX_RUNTIME_LIB_DIR}/onnxruntime.lib)

#Add the onnxruntime include directory
include_directories(${PROJECT_NAME}
    ${ONNXRUNTIME_INCLUDE_DIR}
    ${ONNXRUNTIME_INCLUDE_DIR}/onnxruntime/core/session
)

#Link onnxruntime libraries
target_link_directories(${PROJECT_NAME} PUBLIC ${ONNXRUNTIME_LIB_DIR})
target_link_libraries(${PROJECT_NAME} ${ONNXRUNTIME_LIBRARY})

#Set the subsystem to Windows
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

#POST BUILD COMMAND---------
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ONNXRUNTIME_LIB_DIR}/onnxruntime.dll"
        "${CMAKE_BINARY_DIR}/DEBUG/onnxruntime.dll"
    COMMENT "Copying onnxruntime.dll after build."
)
