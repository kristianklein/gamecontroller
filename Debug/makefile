################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /home/klein/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.o" \
"./tasks.o" \
"./tm4c123gh6pm_startup_ccs_gcc.o" \
"./include/ADC0.o" \
"./include/amplitude.o" \
"./include/button.o" \
"./include/file.o" \
"./include/file_func.o" \
"./include/gyro.o" \
"./include/hid.o" \
"./include/ir_sensor.o" \
"./include/launchpad.o" \
"./include/messages.o" \
"./include/queue.o" \
"./include/rtcs.o" \
"./include/semaphore.o" \
"./include/string.o" \
"./include/swtimer.o" \
"./include/sysblink.o" \
"./include/systick.o" \
"./include/uart0.o" \
"./include/uart1.o" \
"/home/klein/ti/TivaWare TM4C123G/driverlib/ccs/Debug/driverlib.lib" \
-Wl,-T"../tm4c123gh6pm.lds" \
$(GEN_CMDS__FLAG) \
-Wl,--start-group \
-lm \
-Wl,--end-group \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include include/subdir_vars.mk
-include subdir_rules.mk
-include include/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
semesterprojekt.out \

EXE_OUTPUTS__QUOTED += \
"semesterprojekt.out" \

BIN_OUTPUTS += \
semesterprojekt.hex \

BIN_OUTPUTS__QUOTED += \
"semesterprojekt.hex" \


# All Target
all: $(OBJS) $(LDS_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "semesterprojekt.out"

# Tool invocations
semesterprojekt.out: $(OBJS) $(LDS_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: GNU Linker'
	"/home/klein/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-gcc" -DPART_TM4C123GH6PM -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -specs="nosys.specs" -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wl,-Map,"semesterprojekt.map" --entry ResetISR -o"semesterprojekt.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

semesterprojekt.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: GNU Objcopy Utility'
	"/home/klein/ti/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-objcopy"  $(EXE_OUTPUTS__QUOTED) "semesterprojekt.hex"
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.o" "tasks.o" "tm4c123gh6pm_startup_ccs_gcc.o" "include/ADC0.o" "include/amplitude.o" "include/button.o" "include/file.o" "include/file_func.o" "include/gyro.o" "include/hid.o" "include/ir_sensor.o" "include/launchpad.o" "include/messages.o" "include/queue.o" "include/rtcs.o" "include/semaphore.o" "include/string.o" "include/swtimer.o" "include/sysblink.o" "include/systick.o" "include/uart0.o" "include/uart1.o" 
	-$(RM) "main.d" "tasks.d" "tm4c123gh6pm_startup_ccs_gcc.d" "include/ADC0.d" "include/amplitude.d" "include/button.d" "include/file.d" "include/file_func.d" "include/gyro.d" "include/hid.d" "include/ir_sensor.d" "include/launchpad.d" "include/messages.d" "include/queue.d" "include/rtcs.d" "include/semaphore.d" "include/string.d" "include/swtimer.d" "include/sysblink.d" "include/systick.d" "include/uart0.d" "include/uart1.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

