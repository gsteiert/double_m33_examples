include ../make.mk

INC += \
  ./ \
  $(TOP)/lib \
  $(TOP)/board 

# Example source
EXAMPLE_SOURCE += $(wildcard *.c)
SRC_C += $(addprefix $(CURRENT_PATH)/, $(EXAMPLE_SOURCE))
SRC_C += $(TOP)/lib/ssd1306.c 

# board_test example is special example that doesn't enable device or host stack
# This can cause some TinyUSB API missing, this hack to allow us to fill those API
# to pass the compilation process
CFLAGS +=	\
	-D"tud_int_handler(x)= " \

include ../rules.mk
