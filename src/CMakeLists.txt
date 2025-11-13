# This is the main binary. List your C files here.
add_executable(can_high
    can_high.c
    )

add_library(can2040 ${CMAKE_HOME_DIRECTORY}/lib/can2040/src/can2040.c)
target_include_directories(can2040 PUBLIC ${CMAKE_HOME_DIRECTORY}/lib/can2040/src)
target_link_libraries(can2040 PRIVATE
  cmsis_core
  hardware_structs
  pico_stdlib
)

pico_set_program_name(can_high "test")
pico_set_program_version(can_high "0.1")

# Modify the below lines to enable/disable output over UART/USB.
# These are mutually exclusive, only set one.
pico_enable_stdio_uart(can_high 0)
pico_enable_stdio_usb(can_high 1)

# Add the libraries to the build
target_link_libraries(can_high PRIVATE
  pico_stdlib
  can2040
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
)

# The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# variable will be set and the wireless libraries added.
if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(can_high PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Another pico macro which generates the uf2 file in addition to the elf
pico_add_extra_outputs(can_high)

find_program(OPENOCD openocd)
find_program(PICOTOOL picotool)

add_custom_target(flash_high ${PICOTOOL} load -f $<TARGET_FILE:can_high> DEPENDS can_high)

add_custom_target(openocd_flash_high ${OPENOCD} -s ${OPENOCD_PATH} -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c 'adapter speed 5000; program "$<TARGET_FILE:can_high>" verify reset exit' DEPENDS can_high)

# Can Receiver
# This is the main binary. List your C files here.
add_executable(can_low
    can_low.c
    )



pico_set_program_name(can_low "test")
pico_set_program_version(can_low "0.1")

# Modify the below lines to enable/disable output over UART/USB.
# These are mutually exclusive, only set one.
pico_enable_stdio_uart(can_low 0)
pico_enable_stdio_usb(can_low 1)

# Add the libraries to the build
target_link_libraries(can_low PRIVATE
  pico_stdlib
  can2040
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
)

# The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# variable will be set and the wireless libraries added.
if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(can_low PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Another pico macro which generates the uf2 file in addition to the elf
pico_add_extra_outputs(can_low)

find_program(OPENOCD openocd)
find_program(PICOTOOL picotool)

add_custom_target(flash_low ${PICOTOOL} load -f $<TARGET_FILE:can_low> DEPENDS can_low)

add_custom_target(openocd_flash_low ${OPENOCD} -s ${OPENOCD_PATH} -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c 'adapter speed 5000; program "$<TARGET_FILE:can_low>" verify reset exit' DEPENDS can_low)

# # This is the high priority binary. List your C files here.
# add_executable(can_high
#     can_high.c
#     )

# target_include_directories(can2040 PUBLIC ${CMAKE_HOME_DIRECTORY}/lib/can2040/src)
# target_link_libraries(can2040 PRIVATE
#   cmsis_core
#   hardware_structs
#   pico_stdlib
# )
# pico_set_program_name(can_high "test")
# pico_set_program_version(can_high "0.1")

# # Modify the below lines to enable/disable output over UART/USB.
# # These are mutually exclusive, only set one.
# pico_enable_stdio_uart(can_high 0)
# pico_enable_stdio_usb(can_high 1)

# # Add the libraries to the build
# target_link_libraries(can_high PRIVATE
#   pico_stdlib
#   can_2040
#   pico_multicore
#   pico_async_context_freertos
#   FreeRTOS-Kernel-Heap4
# )

# # The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# # variable will be set and the wireless libraries added.
# if(PICO_CYW43_SUPPORTED)
#     # For led support on pico_w
#     target_link_libraries(can_high PRIVATE
#         pico_cyw43_arch_none
#     )
# endif()

# # Another pico macro which generates the uf2 file in addition to the elf
# pico_add_extra_outputs(can_high)

# find_program(OPENOCD openocd)
# find_program(PICOTOOL picotool)

# add_custom_target(flash_can_high ${PICOTOOL} load -f $<TARGET_FILE:can_high> DEPENDS can_high)

# add_custom_target(openocd_flash_can_high ${OPENOCD} -s ${OPENOCD_PATH} -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c 'adapter speed 5000; program "$<TARGET_FILE:can_high>" verify reset exit' DEPENDS can_high)

# # This is the low priority binary. List your C files here.
# add_executable(can_low
#     can_low.c
#     )

# pico_set_program_name(can_low "test")
# pico_set_program_version(can_low "0.1")

# # Modify the below lines to enable/disable output over UART/USB.
# # These are mutually exclusive, only set one.
# pico_enable_stdio_uart(can_low 0)
# pico_enable_stdio_usb(can_low 1)

# # Add the libraries to the build
# target_link_libraries(can_low PRIVATE
#   pico_stdlib
#   can_2040
#   pico_multicore
#   pico_async_context_freertos
#   FreeRTOS-Kernel-Heap4
# )

# # The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# # variable will be set and the wireless libraries added.
# if(PICO_CYW43_SUPPORTED)
#     # For led support on pico_w
#     target_link_libraries(can_low PRIVATE
#         pico_cyw43_arch_none
#     )
# endif()

# # Another pico macro which generates the uf2 file in addition to the elf
# pico_add_extra_outputs(can_low)

# find_program(OPENOCD openocd)
# find_program(PICOTOOL picotool)

# add_custom_target(flash_can_low ${PICOTOOL} load -f $<TARGET_FILE:can_low> DEPENDS can_low)

# add_custom_target(openocd_flash_can_low ${OPENOCD} -s ${OPENOCD_PATH} -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c 'adapter speed 5000; program "$<TARGET_FILE:can_low>" verify reset exit' DEPENDS can_low)
