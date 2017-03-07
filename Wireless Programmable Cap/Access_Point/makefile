################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./Components/simpliciti/nwk_applications/nwk_freq.obj" \
"./Components/simpliciti/nwk_applications/nwk_ioctl.obj" \
"./Components/simpliciti/nwk_applications/nwk_join.obj" \
"./Components/simpliciti/nwk_applications/nwk_link.obj" \
"./Components/simpliciti/nwk_applications/nwk_mgmt.obj" \
"./Components/simpliciti/nwk_applications/nwk_ping.obj" \
"./Components/simpliciti/nwk_applications/nwk_security.obj" \
"./Components/simpliciti/nwk/nwk.obj" \
"./Components/simpliciti/nwk/nwk_QMgmt.obj" \
"./Components/simpliciti/nwk/nwk_api.obj" \
"./Components/simpliciti/nwk/nwk_frame.obj" \
"./Components/simpliciti/nwk/nwk_globals.obj" \
"./Components/mrfi/mrfi.obj" \
"./Components/bsp/bsp.obj" \
"./Applications/main_AP.obj" \
"./Applications/nv_obj.obj" \
"./Applications/virtual_com_cmds.obj" \
"./Applications/vlo_rand.obj" \
"../lnk_msp430f2274.cmd" \
$(GEN_CMDS__FLAG) \
-l"rts430.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include Components/simpliciti/nwk_applications/subdir_vars.mk
-include Components/simpliciti/nwk/subdir_vars.mk
-include Components/mrfi/subdir_vars.mk
-include Components/bsp/subdir_vars.mk
-include Applications/subdir_vars.mk
-include subdir_rules.mk
-include Components/simpliciti/nwk_applications/subdir_rules.mk
-include Components/simpliciti/nwk/subdir_rules.mk
-include Components/mrfi/subdir_rules.mk
-include Components/bsp/subdir_rules.mk
-include Applications/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
WirelessProgrammableCap.out \

EXE_OUTPUTS__QUOTED += \
"WirelessProgrammableCap.out" \


# All Target
all: WirelessProgrammableCap.out

# Tool invocations
WirelessProgrammableCap.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/bin/cl430" --cmd_file="D:/Users/arner/git/WirelessCap/Wireless Programmable Cap/Configuration/Access_Point/smpl_config.dat" --cmd_file="D:/Users/arner/git/WirelessCap/Wireless Programmable Cap/Configuration/smpl_nwk_config.dat"  -vmsp -g --define=__MSP430F2274__ --define=MRFI_CC2500 --diag_warning=225 --printf_support=minimal -z -m"WirelessProgrammableCap.map" --stack_size=80 --heap_size=80 --warn_sections -i"C:/ti/ccsv6/ccs_base/msp430/include" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/lib" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-msp430_4.4.7/include" --reread_libs --xml_link_info="WirelessProgrammableCap_linkInfo.xml" --rom_model -o "WirelessProgrammableCap.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "Components\simpliciti\nwk_applications\nwk_freq.pp" "Components\simpliciti\nwk_applications\nwk_ioctl.pp" "Components\simpliciti\nwk_applications\nwk_join.pp" "Components\simpliciti\nwk_applications\nwk_link.pp" "Components\simpliciti\nwk_applications\nwk_mgmt.pp" "Components\simpliciti\nwk_applications\nwk_ping.pp" "Components\simpliciti\nwk_applications\nwk_security.pp" "Components\simpliciti\nwk\nwk.pp" "Components\simpliciti\nwk\nwk_QMgmt.pp" "Components\simpliciti\nwk\nwk_api.pp" "Components\simpliciti\nwk\nwk_frame.pp" "Components\simpliciti\nwk\nwk_globals.pp" "Components\mrfi\mrfi.pp" "Components\bsp\bsp.pp" "Applications\main_AP.pp" "Applications\nv_obj.pp" "Applications\virtual_com_cmds.pp" 
	-$(RM) "Components\simpliciti\nwk_applications\nwk_freq.obj" "Components\simpliciti\nwk_applications\nwk_ioctl.obj" "Components\simpliciti\nwk_applications\nwk_join.obj" "Components\simpliciti\nwk_applications\nwk_link.obj" "Components\simpliciti\nwk_applications\nwk_mgmt.obj" "Components\simpliciti\nwk_applications\nwk_ping.obj" "Components\simpliciti\nwk_applications\nwk_security.obj" "Components\simpliciti\nwk\nwk.obj" "Components\simpliciti\nwk\nwk_QMgmt.obj" "Components\simpliciti\nwk\nwk_api.obj" "Components\simpliciti\nwk\nwk_frame.obj" "Components\simpliciti\nwk\nwk_globals.obj" "Components\mrfi\mrfi.obj" "Components\bsp\bsp.obj" "Applications\main_AP.obj" "Applications\nv_obj.obj" "Applications\virtual_com_cmds.obj" "Applications\vlo_rand.obj" 
	-$(RM) "Applications\vlo_rand.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

