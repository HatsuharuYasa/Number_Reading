cmake_minimum_required(VERSION 3.10)
project(wx-test)

#Find wxWidgets, modify the "wxWidgets_ROOT_DIR" to your wxwidget installation location
set(wxWidgets_ROOT_DIR $ENV{USERLIBS}/wxWidget)
set(wxWidgets_LIB_DIR ${wxWidgets_ROOT_DIR}/lib/vc_x64_lib)
set(wxWidgets_INCLUDE_DIR ${wxWidgets_ROOT_DIR}/include)

set(wxWidgets_CONFIGURATION mswu)

#Set the Onnxruntime directory, modify the "ONNXRUNTIME_ROOTDIR" to your actual onnxruntime installation location
set(ONNXRUNTIME_ROOT $ENV{USERLIBS}/onnxruntime)
set(ONNXRUNTIME_INCLUDE_DIR ${ONNXRUNTIME_ROOT}/include)
set(ONNXRUNTIME_LIB_DIR ${ONNXRUNTIME_ROOT}/build/Windows/RelWithDebInfo/RelWithDebInfo)
set(ONNXRUNTIME_LIBRARY ${ONNX_RUNTIME_LIB_DIR}/onnxruntime.lib)

#Add the wxWidgets include directory
include_directories(${wxWidgets_INCLUDE_DIR})

#Add the onnxruntime include directory
include_directories(wx-test
    ${ONNXRUNTIME_INCLUDE_DIR}
    ${ONNXRUNTIME_INCLUDE_DIR}/onnxruntime/core/session
)

find_package(wxWidgets REQUIRED COMPONENTS core base)
#find_package(ONNXRUNTIME REQUIRED)

#Include wxWidgets use file
include(${wxWidgets_USE_FILE})

#Define source file
set(SOURCE_FILES src/main.cpp src/gridcanvas.cpp src/inference.cpp)

#Add the executable
add_executable(wx-test WIN32 ${SOURCE_FILES})


#Link wxWidgets libraries
target_link_libraries(wx-test ${wxWidgets_LIBRARIES})

#Link onnxruntime libraries
target_link_directories(wx-test PUBLIC ${ONNXRUNTIME_LIB_DIR})
target_link_libraries(wx-test ${ONNXRUNTIME_LIBRARY})

#Set the subsystem to Windows
set_target_properties(wx-test PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
