################################################################################
##
## (C) COPYRIGHT 2016 TECHNOLUTION BV, GOUDA NL
## | =======          I                   ==          I    =
## |    I             I                    I          I
## |    I   ===   === I ===  I ===   ===   I  I    I ====  I   ===  I ===
## |    I  /   \ I    I/   I I/   I I   I  I  I    I  I    I  I   I I/   I
## |    I  ===== I    I    I I    I I   I  I  I    I  I    I  I   I I    I
## |    I  \     I    I    I I    I I   I  I  I   /I  \    I  I   I I    I
## |    I   ===   === I    I I    I  ===  ===  === I   ==  I   ===  I    I
## |                 +---------------------------------------------------+
## +----+            |  +++++++++++++++++++++++++++++++++++++++++++++++++|
##      |            |             ++++++++++++++++++++++++++++++++++++++|
##      +------------+                          +++++++++++++++++++++++++|
##                                                         ++++++++++++++|
##                                                                  +++++|
##
################################################################################
## RISC-V workshop elf gcc project file
################################################################################

################################################################################
## Global
################################################################################

# Root of RISC-V tools installation. Note that we expect to find the spike
# simulator header files here under $(RISCV)/include/spike .
RISCV 	= /usr/local/msc-riscv32
CROSS	= riscv32-unknown-elf
ISA		= RV32IM 

BUILD_DIR 		= work

CONFIG			= Makefile

################################################################################
## Project variables
################################################################################
TARGET 		= payload
CTARGET		= payload

LINK_FILE = link.ld

# flags to trigger warning generation
WFLAGS = -Wall -Wextra -Wshadow -Wpointer-arith -Wbad-function-cast \
         -Wcast-align -Wsign-compare -Waggregate-return \
         -Wstrict-prototypes -Wno-implicit-function-declaration \
         -Wmissing-declarations -Wunused \
         -Wimplicit -Wswitch -Wreturn-type #-Wredundant-decls  -Wmissing-prototypes 

DBG_FLAGS = -O0

AFLAGS =  -march=$(ISA) $(WFLAGS) $(DBG_FLAGS) -x assembler-with-cpp -c
CFLAGS =  -march=$(ISA) $(WFLAGS) $(DBG_FLAGS) -c
LFLAGS =  -march=$(ISA) -nostartfiles -static

AFLAGS += -fno-strict-aliasing -fno-builtin -D__gracefulExit
CFLAGS += -fomit-frame-pointer -fno-strict-aliasing -fno-builtin \
	    -D__gracefulExit	    
CFLAGS += -D_BUILD_HEX_TIME_=$(HEX_TIME) -D_BUILD_HEX_DATE_=$(HEX_DATE)

# application
# support files
ASM_SOURCES = 
C_SOURCES   = c_payload.c
			  

LIBS 		=
STD_LIBS	=

INCLUDE_DIRS 	= 	

BUILD_DIRS  = $(sort $(addprefix $(BUILD_DIR)/,$(dir $(OBJS))) $(BUILD_DIR)/)
OBJS 		= $(ASM_SOURCES:.S=.o) $(C_SOURCES:.c=.o)

################################################################################
#### Default rules
################################################################################

    ############################################################################
    ## Phony targets
    ############################################################################
.PHONY: clean all code binary

all: $(TARGET).elf $(TARGET).raw $(TARGET).dump

clean: 
	@$(call print_cmd_info,"CLEANUP",$@)
	@$(RM) -r $(BUILD_DIR)
	@$(RM) $(LINT_REPORT)

test:
	echo $(BUILD_DIRS)

################################################################################
#### Environment setup
################################################################################
    ############################################################################
    ## variables
    ############################################################################

VPATH += $(BUILD_DIR)/ $(BUILD_DIRS)

HEX_DATE        := $(shell date +0x%Y%m%d)
HEX_TIME        := $(shell date +0x%H%M%S)

.SUFFIXES:		#delete all known suffixes
.NOTPARALLEL:
    ############################################################################
    ## includes
    ############################################################################

    ############################################################################
    ## Functions
    ############################################################################
print_cmd_info		= printf "[%s] - %-20s : %s\n" `date +%H:%M:%S` $(1) $(2)
print_cmd_info_nonl	= printf "[%s] - %-20s : %s " `date +%H:%M:%S` $(1) $(2)

    ############################################################################
    ## Rules
    ############################################################################
	
$(BUILD_DIRS)	:
	@$(call print_cmd_info,"MK WORK DIR",$(dir $@))
	@mkdir -p $(dir $@)


$(LINK_FILE): $(BUILD_DIR)/exploit.ld

$(BUILD_DIR)/exploit.ld: $(BUILD_DIR)/main.nm
	@$(call print_cmd_info,"MK LINKER SCRIPT", $@)
	@../exploit/tools/link_generator.py -n $< -o $@
	
$(BUILD_DIR)/main.nm: ../appl/microsemi/work/main.elf 
	@$(call print_cmd_info,"MK SYMBOL FILE", $@)
	@$(NM) $(<) > $(@)
	
../appl/microsemi/work/main.elf:
	

################################################################################
#### RISC-V GCC toolchain
################################################################################

    ############################################################################
    ## variables
    ############################################################################

AS		= $(CROSS)-as
LD		= $(CROSS)-ld
CPP		= $(CROSS)-cpp
CC		= $(CROSS)-gcc
OBJDUMP	= $(CROSS)-objdump
OBJCOPY	= $(CROSS)-objcopy
NM		= $(CROSS)-nm
SIZE	= $(CROSS)-size

    ############################################################################
    ## includes
    ############################################################################

ifneq ($(MAKECMDGOALS),clean)
-include $(OBJS:%.o=$(BUILD_DIR)/%.d)
endif

    ############################################################################
    ## Rules
    ############################################################################

$(BUILD_DIR)/%.d: %.c $(CONFIG) | $(BUILD_DIRS)
	@$(call print_cmd_info,"DEPEND",$@)
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS:%=-I%) -MT $(<:c=o) -MM $< > $(@) || ($(RM) $(BUILD_DIR)/$(@); false)

$(BUILD_DIR)/%.d: %.s $(CONFIG) | $(BUILD_DIRS)
	@$(call print_cmd_info,"DEPEND",$@)
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS:%=-I%) -MT $(<:s=o) -MM $< > $(@) || ($(RM) $(BUILD_DIR)/$(@); false)

$(BUILD_DIR)/%.d: %.S $(CONFIG) | $(BUILD_DIRS)
	@$(call print_cmd_info,"DEPEND",$@)
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS:%=-I%) -MT $(<:S=o) -MM $< > $(@) || ($(RM) $(BUILD_DIR)/$(@); false)


%.o: %.c %.d $(CONFIG) | $(BUILD_DIRS)
	@$(call print_cmd_info,"COMPILE",$@)
	@$(CC) -Wa,-adhlns=$(BUILD_DIR)/$(*).lst $(INCLUDE_DIRS:%=-I%) $(CFLAGS) $< -o $(BUILD_DIR)/$(@)

%.o: %.s %.d $(CONFIG) | $(BUILD_DIRS)
	@$(call print_cmd_info,"ASSEMBLE",$@)
	@$(CC) -Wa,-adhlns=$(BUILD_DIR)/$(*).lst $(INCLUDE_DIRS:%=-I%) $(AFLAGS) $< -o $(BUILD_DIR)/$(@)

%.o: %.S %.d $(CONFIG) | $(BUILD_DIRS)
	@$(call print_cmd_info,"ASSEMBLE",$@)
	@$(CC) -Wa,-adhlns=$(BUILD_DIR)/$(*).lst $(INCLUDE_DIRS:%=-I%) $(AFLAGS) $< -o $(BUILD_DIR)/$(@)

%.elf: $(OBJS) $(LIBS) $(CONFIG) $(LINK_FILE) | $(BUILD_DIRS)
	@$(call print_cmd_info,"LINKING",$@)
	@$(CC) $(LFLAGS) -Wl,-T -Wl,$(LINK_FILE) -Wl,-Map=$(BUILD_DIR)/$(*).map $(OBJS:%=$(BUILD_DIR)/%) $(LIBS) $(STD_LIBS:%=-l%) -o $(BUILD_DIR)/$(@)

%.hex: %.elf $(CONFIG) | $(BUILD_DIRS)
	@$(call print_cmd_info,"GEN HEX",$@)
	@$(OBJCOPY) -O ihex $(BUILD_DIR)/$(<) $(BUILD_DIR)/$(@)

%.nm: %.elf $(CONFIG) | $(BUILD_DIRS)
	@$(call print_cmd_info,"GEN NM",$@)
	@$(NM) $(BUILD_DIR)/$(<) > $(BUILD_DIR)/$(@)

%.srec: %.elf $(CONFIG) | $(BUILD_DIRS)
	@$(call print_cmd_info,"GEN HEX",$@)
	@$(OBJCOPY) -O srec $(BUILD_DIR)/$(<) $(BUILD_DIR)/$(@)

%.raw: %.elf $(CONFIG) | $(BUILD_DIRS)
	@$(call print_cmd_info,"GEN BIN",$@)
	@$(OBJCOPY) -O binary --strip-all $(BUILD_DIR)/$(<) $(BUILD_DIR)/$(@)

%.nm: %.elf $(BUILD_DIR_DEP) $(CONFIG)
	@$(call print_cmd_info,"GEN NM",$@)
	@$(NM) $(BUILD_DIR)/$(<F) > $(BUILD_DIR)/$(@F)

%.srec: %.elf $(BUILD_DIR_DEP) $(CONFIG)
	@$(call print_cmd_info,"GEN HEX",$@)
	@$(OBJCOPY) -O srec $(BUILD_DIR)/$(<F) $(BUILD_DIR)/$(@F)

%.dump: %.elf $(BUILD_DIR_DEP) $(CONFIG)
	@$(call print_cmd_info,"GEN DUMP",$@)
	@$(OBJDUMP) --disassemble-all --disassemble-zeroes --section=.text --section=.rodata --section=.sbss --section=.sbss2 --section=.data  $(BUILD_DIR)/$(<F) > $(BUILD_DIR)/$(@F)

.PHONY: size
size: $(TARGET).elf $(BUILD_DIR_DEP) $(CONFIG)
	@$(call print_cmd_info,"GEN DUMP",$@)
	@$(SIZE) $(BUILD_DIR)/$(<F)

