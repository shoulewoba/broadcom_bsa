cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

PROJECT (rkmediaplayer)

if(CPU_ARCH STREQUAL "arm")
        message("CPU_ARCH is ${CPU_ARCH}")
        install(FILES ${CMAKE_SOURCE_DIR}/librkmediaplayer/source/lib32/librkmediaplayer.so DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
endif()
if(CPU_ARCH STREQUAL "aarch64")
        message("CPU_ARCH is ${CPU_ARCH}")
        install(FILES ${CMAKE_SOURCE_DIR}/librkmediaplayer/source/lib64/librkmediaplayer.so DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
endif()
install(DIRECTORY ${CMAKE_SOURCE_DIR}/librkmediaplayer/source/include/DeviceIo DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
