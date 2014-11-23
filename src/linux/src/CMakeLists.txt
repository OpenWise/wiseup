add_subdirectory (modules)
add_subdirectory (arch)

find_package (MySQL)
if (MYSQL_FOUND)
    message (STATUS "MySQL found at: ${MYSQL_INCLUDE_DIR}, ${MYSQL_LIBRARIES}")
else (MYSQL_FOUND)
    message (SEND_ERROR "Couldn't find MySQL include files and/or library")
endif (MYSQL_FOUND)

include_directories (${PROJECT_SOURCE_DIR}/include)
add_executable (wise-nrfd wise-nrfd.cxx wise_mysql.cxx wiseDBMng.cxx filelog.cxx wise_ipc.cxx wise_client_handler.cxx jsoncpp.cpp)
target_link_libraries (wise-nrfd nrf24l01 gpio spi ${MYSQL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
include_directories (wise-nrfd PUBLIC ${MYSQL_INCLUDE_DIR})

add_executable (lcd-test lcd_test.cxx)
target_link_libraries (lcd-test pcd8544 gpio spi)
