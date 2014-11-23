add_library (pcd8544 SHARED pcd8544.cxx gfx.cxx)
target_link_libraries (pcd8544 gpio spi)
