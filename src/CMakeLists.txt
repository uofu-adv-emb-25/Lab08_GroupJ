# This is the main binary. List your C files here.
add_executable(can_tx
    can_tx.c
    )

add_library(can2040 ${CMAKE_HOME_DIRECTORY}/lib/can2040/src/can2040.c)
target_include_directories(can2040 PUBLIC ${CMAKE_HOME_DIRECTORY}/lib/can2040/src)
target_link_libraries(can2040 PRIVATE
  cmsis_core
  hardware_structs
  pico_stdlib
)

pico_set_program_name(can_tx "test")
pico_set_program_version(can_tx "0.1")

# Modify the below lines to enable/disable output over UART/USB.
# These are mutually exclusive, only set one.
pico_enable_stdio_uart(can_tx 0)
pico_enable_stdio_usb(can_tx 1)

# Add the libraries to the build
target_link_libraries(can_tx PRIVATE
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
    target_link_libraries(can_tx PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Another pico macro which generates the uf2 file in addition to the elf
pico_add_extra_outputs(can_tx)

find_program(OPENOCD openocd)
find_program(PICOTOOL picotool)

add_custom_target(flash ${PICOTOOL} load -f $<TARGET_FILE:can_tx> DEPENDS can_tx)

add_custom_target(openocd_flash ${OPENOCD} -s ${OPENOCD_PATH} -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c 'adapter speed 5000; program "$<TARGET_FILE:can_tx>" verify reset exit' DEPENDS can_tx)
