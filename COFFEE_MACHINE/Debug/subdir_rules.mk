################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/COFFEE_MACHINE" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/inc" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

heap_2.obj: D:/COLLEGE/6th\ SEM/Embedded\ Programming/CCS/freeRTOS/port/MemMang/heap_2.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/COFFEE_MACHINE" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/inc" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="heap_2.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

list.obj: D:/COLLEGE/6th\ SEM/Embedded\ Programming/CCS/freeRTOS/src/list.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/COFFEE_MACHINE" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/inc" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="list.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

port.obj: D:/COLLEGE/6th\ SEM/Embedded\ Programming/CCS/freeRTOS/port/TivaM4/port.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/COFFEE_MACHINE" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/inc" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="port.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

portasm.obj: D:/COLLEGE/6th\ SEM/Embedded\ Programming/CCS/freeRTOS/port/TivaM4/portasm.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/COFFEE_MACHINE" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/inc" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="portasm.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

queue.obj: D:/COLLEGE/6th\ SEM/Embedded\ Programming/CCS/freeRTOS/src/queue.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/COFFEE_MACHINE" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/inc" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="queue.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_frt.obj: D:/COLLEGE/6th\ SEM/Embedded\ Programming/CCS/freeRTOS/port/TivaM4/startup_frt.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/COFFEE_MACHINE" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/inc" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="startup_frt.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

systick_frt.obj: D:/COLLEGE/6th\ SEM/Embedded\ Programming/CCS/freeRTOS/port/TivaM4/systick_frt.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/COFFEE_MACHINE" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/inc" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="systick_frt.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

tasks.obj: D:/COLLEGE/6th\ SEM/Embedded\ Programming/CCS/freeRTOS/src/tasks.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/COFFEE_MACHINE" --include_path="C:/ti/ccs1110/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/Project/FINAL_PROJECT/freeRTOS/inc" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/port/TivaM4" --include_path="D:/COLLEGE/6th SEM/Embedded Programming/CCS/freeRTOS/inc" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="tasks.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


