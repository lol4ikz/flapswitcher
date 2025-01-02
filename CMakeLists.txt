cmake_minimum_required(VERSION 3.31)

# project name
project(FlapSwitcher)

# specify the compiler
set(CMAKE_C_COMPILER avr-gcc)

# set the target microcontroller
set(MCU attiny13a)

#set the programmer
set(PROGRAMMER usbasp)

# set the compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Os -mmcu=${MCU}")

# specify the source files
set(SOURCES src/attiny13.c)

# create the executable
add_executable(attiny13.elf ${SOURCES})

# specify the output format
set_target_properties(attiny13.elf PROPERTIES 
    LINK_FLAGS "-Wl,-Map=attiny13.map"
)

# add a custom command to convert the ELF file to HEX
add_custom_command(TARGET attiny13.elf POST_BUILD
    COMMAND avr-objcopy -j .text -j .data -O ihex $<TARGET_FILE:attiny13.elf> attiny13.hex
)

# add a custom comand to display the size fo the ELF file
add_custom_command(TARGET attiny13.elf POST_BUILD
    COMMAND avr-size --format=avr --mcu=${MCU} $<TARGET_FILE:attiny13.elf>
)

# add a custom target to programm the microcontroller
add_custom_target(program
    COMMAND avrdude -c ${PROGRAMMER} -p ${MCU} -B5 -U flash:w:attiny13.hex:i
    DEPENDS attiny13.elf
    COMMENT "Programming ..."
)
