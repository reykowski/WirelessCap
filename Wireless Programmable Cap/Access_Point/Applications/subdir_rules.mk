################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Applications/main_AP.obj: ../Applications/main_AP.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/bin/cl430" --cmd_file="D:/Users/arner/git/WirelessCap/Wireless Programmable Cap/Configuration/Access_Point/smpl_config.dat" --cmd_file="D:/Users/arner/git/WirelessCap/Wireless Programmable Cap/Configuration/smpl_nwk_config.dat"  -vmsp -g --define=__MSP430F2274__ --define=MRFI_CC2500 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/include" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/bsp" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/bsp/drivers" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/bsp/boards/EZ430RF" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/mrfi" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/simpliciti/nwk_applications" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/simpliciti/nwk" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="Applications/main_AP.pp" --obj_directory="Applications" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Applications/nv_obj.obj: ../Applications/nv_obj.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/bin/cl430" --cmd_file="D:/Users/arner/git/WirelessCap/Wireless Programmable Cap/Configuration/Access_Point/smpl_config.dat" --cmd_file="D:/Users/arner/git/WirelessCap/Wireless Programmable Cap/Configuration/smpl_nwk_config.dat"  -vmsp -g --define=__MSP430F2274__ --define=MRFI_CC2500 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/include" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/bsp" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/bsp/drivers" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/bsp/boards/EZ430RF" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/mrfi" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/simpliciti/nwk_applications" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/simpliciti/nwk" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="Applications/nv_obj.pp" --obj_directory="Applications" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Applications/virtual_com_cmds.obj: ../Applications/virtual_com_cmds.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/bin/cl430" --cmd_file="D:/Users/arner/git/WirelessCap/Wireless Programmable Cap/Configuration/Access_Point/smpl_config.dat" --cmd_file="D:/Users/arner/git/WirelessCap/Wireless Programmable Cap/Configuration/smpl_nwk_config.dat"  -vmsp -g --define=__MSP430F2274__ --define=MRFI_CC2500 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/include" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/bsp" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/bsp/drivers" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/bsp/boards/EZ430RF" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/mrfi" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/simpliciti/nwk_applications" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/simpliciti/nwk" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="Applications/virtual_com_cmds.pp" --obj_directory="Applications" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Applications/vlo_rand.obj: ../Applications/vlo_rand.asm $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/bin/cl430" --cmd_file="D:/Users/arner/git/WirelessCap/Wireless Programmable Cap/Configuration/Access_Point/smpl_config.dat" --cmd_file="D:/Users/arner/git/WirelessCap/Wireless Programmable Cap/Configuration/smpl_nwk_config.dat"  -vmsp -g --define=__MSP430F2274__ --define=MRFI_CC2500 --include_path="C:/ti/ccsv6/ccs_base/msp430/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/include" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/bsp" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/bsp/drivers" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/bsp/boards/EZ430RF" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/mrfi" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/simpliciti/nwk_applications" --include_path="D:/Users/arner/git/WirelessCap/SimpliciTI Components/simpliciti/nwk" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="Applications/vlo_rand.pp" --obj_directory="Applications" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


