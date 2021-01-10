CFLAGS += \
  -flto \
  -mthumb \
  -mabi=aapcs \
  -mcpu=cortex-m33 \
  -mfloat-abi=hard \
  -mfpu=fpv5-sp-d16 \
  -DCPU_LPC55S69JBD100_cm33_core0 \
  -DCFG_TUSB_MCU=OPT_MCU_LPC55XX \
  -DCFG_TUSB_MEM_SECTION='__attribute__((section(".data")))' \
  -DCFG_TUSB_MEM_ALIGN='__attribute__((aligned(64)))'

# mcu driver cause following warnings
CFLAGS += -Wno-error=unused-parameter -Wno-error=float-equal

MCU_DIR = $(TOP)/nxp/sdk/devices/LPC55S69

# All source paths should be relative to the top level.
LD_FILE = board/LPC55S69_cm33_core0_uf2.ld

SRC_C += \
	$(MCU_DIR)/system_LPC55S69_cm33_core0.c \
	$(MCU_DIR)/drivers/fsl_clock.c \
	$(MCU_DIR)/drivers/fsl_gpio.c \
	$(MCU_DIR)/drivers/fsl_power.c \
	$(MCU_DIR)/drivers/fsl_reset.c \
	$(MCU_DIR)/drivers/fsl_usart.c \
	$(MCU_DIR)/drivers/fsl_flexcomm.c \
	$(TOP)/sct_neopixel/sct_neopixel.c

INC += \
	$(MCU_DIR)/../../CMSIS/Include \
	$(MCU_DIR) \
	$(MCU_DIR)/drivers \
	$(TOP)/sct_neopixel

SRC_S += $(MCU_DIR)/gcc/startup_LPC55S69_cm33_core0.S

LIBS += $(MCU_DIR)/gcc/libpower_hardabi.a

# For TinyUSB port source
VENDOR = nxp
CHIP_FAMILY = lpc_ip3511

# For freeRTOS port source
FREERTOS_PORT = ARM_CM33_NTZ/non_secure

# For flash-jlink target
JLINK_DEVICE = LPC55S69

# flash using pyocd
flash: $(BUILD)/$(BOARD)-firmware.hex 
	pyocd flash -t LPC55S69 $<
