objs += $(addprefix ion/src/nspire/, \
  init.o\
)

objs += $(addprefix ion/src/nspire/boot/, main.o)

objs += $(addprefix ion/src/shared/, \
  crc32.o \
  console_line.o \
  console_stdio.o \
  events_modifier.o \
  power.o \
  random.o \
  dummy/backlight.o \
  dummy/battery.o \
  dummy/fcc_id.o \
  dummy/led.o \
  dummy/serial_number.o \
  dummy/stack.o \
  dummy/usb.o \
)

.PHONY: %_memory_map
%_memory_map:
	@echo "========== MEMORY MAP ========="
	@echo "No memory map on simulator."
	@echo "==============================="
