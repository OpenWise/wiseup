add_executable (nrf_transmitter nrf_transmitter.cxx)
target_link_libraries (nrf_transmitter nrf24l01 gpio spi)

add_executable (nrf_receiver nrf_receiver.cxx)
target_link_libraries (nrf_receiver nrf24l01 gpio spi)

add_executable (gpio-test gpio-test.c)
target_link_libraries (gpio-test nrf24l01 gpio spi)

add_executable (ble-test ble_broadcast.cxx)
target_link_libraries (ble-test nrf24l01 gpio spi)

add_executable (nrf_net_send nrf_net_send.cxx)
target_link_libraries (nrf_net_send nrf24l01 gpio spi)
