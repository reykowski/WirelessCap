################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Components/mrfi/radios/family1/mrfi_radio.obj: ../Components/mrfi/radios/family1/mrfi_radio.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/msp430/bin/cl430" --cmd_file="C:\Users\usd28415\Documents\!D-Drive\Work\Information\MSP430\!MyCode\Wireless Programmable Cap\Configuration\Access_Point\smpl_config.dat" --cmd_file="C:\Users\usd28415\Documents\!D-Drive\Work\Information\MSP430\!MyCode\Wireless Programmable Cap\Configuration\smpl_nwk_config.dat"  -vmsp -g --define=__MSP430F2274__ --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/msp430/include" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="Components/mrfi/radios/family1/mrfi_radio.pp" --obj_directory="Components/mrfi/radios/family1" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

Components/mrfi/radios/family1/mrfi_spi.obj: ../Components/mrfi/radios/family1/mrfi_spi.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/msp430/bin/cl430" --cmd_file="C:\Users\usd28415\Documents\!D-Drive\Work\Information\MSP430\!MyCode\Wireless Programmable Cap\Configuration\Access_Point\smpl_config.dat" --cmd_file="C:\Users\usd28415\Documents\!D-Drive\Work\Information\MSP430\!MyCode\Wireless Programmable Cap\Configuration\smpl_nwk_config.dat"  -vmsp -g --define=__MSP430F2274__ --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/msp430/include" --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="Components/mrfi/radios/family1/mrfi_spi.pp" --obj_directory="Components/mrfi/radios/family1" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


