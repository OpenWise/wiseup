add_subdirectory (modules)
add_subdirectory (arch)

include_directories (${PROJECT_SOURCE_DIR}/include)

add_library( hiredis SHARED IMPORTED )
set_property (TARGET hiredis PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/libs/libhiredis.so)

add_executable (wise-nrfd wise-nrfd.cxx wiseDBMng.cxx filelog.cxx wise_ipc.cxx wise_client_handler.cxx jsoncpp.cpp nrfTaskMng.cxx screen.cxx)
target_link_libraries (wise-nrfd nrf24l01 pcd8544 gpio spi hiredis event ${CMAKE_THREAD_LIBS_INIT})
