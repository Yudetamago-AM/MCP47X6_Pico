add_executable(Speed_Test Speed_Test.cpp)

target_link_libraries(Speed_Test pico_stdlib hardware_i2c MCP47X6)

pico_enable_stdio_usb(Speed_Test 1)
pico_enable_stdio_uart(Speed_Test 0)

pico_add_extra_outputs(Speed_Test)