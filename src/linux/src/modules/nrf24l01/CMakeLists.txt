add_library (nrf24l01 SHARED nrf24l01.cxx)
target_link_libraries (nrf24l01 gpio spi)
