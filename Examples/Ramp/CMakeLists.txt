add_executable(Ramp Ramp.cpp)

target_link_libraries(Ramp pico_stdlib hardware_i2c MCP47X6)

pico_enable_stdio_usb(Ramp 1)
pico_enable_stdio_uart(Ramp 0)

pico_add_extra_outputs(Ramp)