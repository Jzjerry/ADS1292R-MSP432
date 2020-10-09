################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
LCD/%.obj: ../LCD/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"D:/Developing/CCS10.1/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/Developing/MSP_Proj/ADS1292R_MSP432P401R" --include_path="D:/Developing/MSP_Proj/ADS1292R_MSP432P401R/Debug" --include_path="D:/Developing/CCS10.1/simplelink_msp432p4_sdk_3_40_01_02/source" --include_path="D:/Developing/CCS10.1/simplelink_msp432p4_sdk_3_40_01_02/source/ti/devices/msp432p4xx/driverlib" --include_path="D:/Developing/CCS10.1/simplelink_msp432p4_sdk_3_40_01_02/source/third_party/CMSIS/Include" --include_path="D:/Developing/CCS10.1/simplelink_msp432p4_sdk_3_40_01_02/source/ti/posix/ccs" --include_path="D:/Developing/CCS10.1/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/include" --advice:power=none --define=ARM_MATH_CM4 --define=__FPU_PRESENT -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="LCD/$(basename $(<F)).d_raw" --include_path="D:/Developing/MSP_Proj/ADS1292R_MSP432P401R/Debug/syscfg" --obj_directory="LCD" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


