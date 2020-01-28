################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/ziad/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="/home/ziad/ti/ccs910/ccs/tools/compiler/ti-cgt-arm_18.12.2.LTS/include" --include_path="/home/ziad/Desktop/GraduationProject_BigRepo/Adaptive-Cruise-Control/Application/freertos_demo" --include_path="/home/ziad/ti/SW-EK-TM4C123GXL-2.1.4.178/examples/boards/ek-tm4c123gxl" --include_path="/home/ziad/ti/SW-EK-TM4C123GXL-2.1.4.178" --include_path="/home/ziad/ti/SW-EK-TM4C123GXL-2.1.4.178/third_party" --include_path="/home/ziad/ti/SW-EK-TM4C123GXL-2.1.4.178/third_party/FreeRTOS/Source/include" --include_path="/home/ziad/ti/SW-EK-TM4C123GXL-2.1.4.178/third_party/FreeRTOS" --include_path="/home/ziad/ti/SW-EK-TM4C123GXL-2.1.4.178/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --advice:power=all -g --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_TM4C123_RB1 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --ual --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


