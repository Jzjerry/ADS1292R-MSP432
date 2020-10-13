################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/Developing/CCS10.1/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS

GEN_OPTS__FLAG := --cmd_file="D:/Developing/MSP_Proj/tirtos_builds_MSP_EXP432P401R_release_ccs/Debug/configPkg/compiler.opt" 
GEN_CMDS__FLAG := -l"D:/Developing/MSP_Proj/tirtos_builds_MSP_EXP432P401R_release_ccs/Debug/configPkg/linker.cmd" 

ORDERED_OBJS += \
"./ADS1292R.obj" \
"./ADS1292R_SPI.obj" \
"./ESP8266.obj" \
"./Fliter.obj" \
"./LMT70_I2C.obj" \
"./Pan_Tompkins.obj" \
"./Serial_Display.obj" \
"./arm_fir_f32.obj" \
"./arm_fir_init_f32.obj" \
"./main.obj" \
"./syscfg/ti_drivers_config.obj" \
"./main_tirtos.obj" \
"./MPU6050/MPU6050.obj" \
"./MPU6050/MPU6050_I2C.obj" \
"../MSP_EXP432P401R_TIRTOS.cmd" \
-l"ti/display/lib/display.aem4f" \
-l"ti/grlib/lib/ccs/m4f/grlib.a" \
-l"third_party/spiffs/lib/ccs/m4f/spiffs.a" \
-l"ti/drivers/lib/drivers_msp432p401x.aem4f" \
-l"third_party/fatfs/lib/ccs/m4f/fatfs.a" \
-l"ti/dpl/lib/dpl_msp432p401x.aem4f" \
$(GEN_CMDS__FLAG) \
-l"ti/devices/msp432p4xx/driverlib/ccs/msp432p4xx_driverlib.lib" \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include MPU6050/subdir_vars.mk
-include subdir_rules.mk
-include MPU6050/subdir_rules.mk
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
ADS1292R_MSP432P401R.out \

EXE_OUTPUTS__QUOTED += \
"ADS1292R_MSP432P401R.out" \

BIN_OUTPUTS += \
ADS1292R_MSP432P401R.hex \

BIN_OUTPUTS__QUOTED += \
"ADS1292R_MSP432P401R.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "ADS1292R_MSP432P401R.out"

dependents:
	-cd D:/Developing/MSP_Proj/tirtos_builds_MSP_EXP432P401R_release_ccs/Debug && $(MAKE) all

# Tool invocations
ADS1292R_MSP432P401R.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS) D:/Developing/MSP_Proj/tirtos_builds_MSP_EXP432P401R_release_ccs/Debug/tirtos_builds_MSP_EXP432P401R_release_ccs.lib
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"D:/Developing/CCS10.1/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --advice:power=none --define=ARM_MATH_CM4 --define=__FPU_PRESENT -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on -z -m"ADS1292R_MSP432P401R.map" -i"D:/Developing/CCS10.1/simplelink_msp432p4_sdk_3_40_01_02/source" -i"D:/Developing/CCS10.1/simplelink_msp432p4_sdk_3_40_01_02/kernel/tirtos/packages" -i"D:/Developing/CCS10.1/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/lib" --diag_wrap=off --display_error_number --warn_sections --xml_link_info="ADS1292R_MSP432P401R_linkInfo.xml" --rom_model -o "ADS1292R_MSP432P401R.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

ADS1292R_MSP432P401R.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"D:/Developing/CCS10.1/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/bin/armhex"  -o "ADS1292R_MSP432P401R.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "ADS1292R.obj" "ADS1292R_SPI.obj" "ESP8266.obj" "Fliter.obj" "LMT70_I2C.obj" "Pan_Tompkins.obj" "Serial_Display.obj" "arm_fir_f32.obj" "arm_fir_init_f32.obj" "main.obj" "syscfg\ti_drivers_config.obj" "main_tirtos.obj" "MPU6050\MPU6050.obj" "MPU6050\MPU6050_I2C.obj" 
	-$(RM) "ADS1292R.d" "ADS1292R_SPI.d" "ESP8266.d" "Fliter.d" "LMT70_I2C.d" "Pan_Tompkins.d" "Serial_Display.d" "arm_fir_f32.d" "arm_fir_init_f32.d" "main.d" "syscfg\ti_drivers_config.d" "main_tirtos.d" "MPU6050\MPU6050.d" "MPU6050\MPU6050_I2C.d" 
	-$(RMDIR) $(GEN_MISC_DIRS__QUOTED)
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
D:/Developing/MSP_Proj/tirtos_builds_MSP_EXP432P401R_release_ccs/Debug/tirtos_builds_MSP_EXP432P401R_release_ccs.lib:

-include ../makefile.targets

