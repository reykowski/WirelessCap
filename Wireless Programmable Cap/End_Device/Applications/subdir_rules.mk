################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Applications/main_ED.obj: ../Applications/main_ED.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/bin/cl430" --cmd_file="D:/CCS/workspace_v6_1/Wireless Programmable Cap/Configuration/smpl_nwk_config.dat" --cmd_file="D:/CCS/workspace_v6_1/Wireless Programmable Cap/Configuration/End_Device/smpl_config.dat"  -vmsp -g --define=__MSP430F2274__ --define=MRFI_CC2500 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/include" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/bsp/boards/EZ430RF" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/bsp/drivers" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/bsp" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/mrfi" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/simpliciti/nwk" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/simpliciti/nwk_applications" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="Applications/main_ED.pp" --obj_directory="Applications" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Applications/nv_obj.obj: ../Applications/nv_obj.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/bin/cl430" --cmd_file="D:/CCS/workspace_v6_1/Wireless Programmable Cap/Configuration/smpl_nwk_config.dat" --cmd_file="D:/CCS/workspace_v6_1/Wireless Programmable Cap/Configuration/End_Device/smpl_config.dat"  -vmsp -g --define=__MSP430F2274__ --define=MRFI_CC2500 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/include" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/bsp/boards/EZ430RF" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/bsp/drivers" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/bsp" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/mrfi" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/simpliciti/nwk" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/simpliciti/nwk_applications" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="Applications/nv_obj.pp" --obj_directory="Applications" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Applications/vlo_rand.obj: ../Applications/vlo_rand.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/bin/cl430" --cmd_file="D:/CCS/workspace_v6_1/Wireless Programmable Cap/Configuration/smpl_nwk_config.dat" --cmd_file="D:/CCS/workspace_v6_1/Wireless Programmable Cap/Configuration/End_Device/smpl_config.dat"  -vmsp -g --define=__MSP430F2274__ --define=MRFI_CC2500 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/include" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/bsp/boards/EZ430RF" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/bsp/drivers" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/bsp" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/mrfi" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/simpliciti/nwk" --include_path="D:/CCS/workspace_v6_1/SimpliciTI Components/simpliciti/nwk_applications" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="Applications/vlo_rand.pp" --obj_directory="Applications" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


