EXAMPLE_MOD_DIR := $(USERMOD_DIR)

# Add all C files to SRC_USERMOD.
SRC_USERMOD += $(EXAMPLE_MOD_DIR)/stm32_port/drv_timer.c
SRC_USERMOD += $(EXAMPLE_MOD_DIR)/stm32_port/drv_dac.c
SRC_USERMOD += $(EXAMPLE_MOD_DIR)/mpy_port/mod_powerADDA.c
SRC_USERMOD += $(EXAMPLE_MOD_DIR)/../../lib/stm32lib/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac.c

# We can add our module folder to include paths if needed
# This is not actually needed in this example.
CFLAGS_USERMOD += -I$(EXAMPLE_MOD_DIR) -DR_QUE_USE_MALLOC
CEXAMPLE_MOD_DIR := $(USERMOD_DIR)
