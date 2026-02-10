# NOTE: bno085_controller

add_library(bno085_controller STATIC bno085_controller.cpp bno085_controller.h)

target_include_directories(
  bno085_controller PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                           ${CMAKE_SOURCE_DIR}/src/shared/types)

target_link_libraries(bno085_controller PUBLIC pico_stdlib hardware_i2c
                                               BNO08x_Pico_Library)
