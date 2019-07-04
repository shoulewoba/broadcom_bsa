cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

set(SRC_FILES
    DeviceIOTest.cpp
    bt_test.cpp
)


add_executable(deviceio_test ${SRC_FILES})
target_include_directories(deviceio_test PUBLIC
        "${deviceio_test_SOURCE_DIR}/librkmediaplayer/source/include"
        "${deviceio_test_SOURCE_DIR}/DeviceIO/include" )
target_link_libraries(deviceio_test pthread DeviceIo asound rkmediaplayer)

install(TARGETS deviceio_test DESTINATION bin)
