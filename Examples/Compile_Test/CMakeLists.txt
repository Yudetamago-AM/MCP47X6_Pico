add_executable(Compile_Test Compile_Test.cpp)

target_link_libraries(Compile_Test pico_stdlib hardware_i2c MCP47X6)

pico_enable_stdio_usb(Compile_Test 1)
pico_enable_stdio_uart(Compile_Test 0)

pico_add_extra_outputs(Compile_Test)