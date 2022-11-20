add_executable(lab9.elf main.c gameControl.c bullet.c player.c)
target_link_libraries(lab9.elf ${330_LIBS} touchscreen interrupts intervalTimer buttons_switches)
set_target_properties(lab9.elf PROPERTIES LINKER_LANGUAGE CXX)