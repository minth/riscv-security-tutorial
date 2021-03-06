################################################################################
##
## (C) COPYRIGHT 2004 TECHNOLUTION BV, GOUDA NL
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
##              A U T O M A T I O N     T E C H N O L O G Y         +++++|
##
################################################################################
## RISC-V workshop elf gcc project file
################################################################################

################################################################################
## Global
################################################################################

# Root of RISC-V tools installation. Note that we expect to find the spike
# simulator header files here under $(RISCV)/include/spike .
RISCV 	= /usr/local/riscv32
CROSS	= riscv32-unknown-elf
ISA		= RV32IM 

TARGET_DIR		= spike
SPIKE_DIR 		= spike
BUILD_DIR 		= $(TARGET_DIR)/work
SRC_DIR 		= src
COMMON_DIR 		= common


CONFIG			= Makefile

################################################################################
## Project variables
################################################################################
TARGET 		= main
CTARGET		= main

LINK_FILE = $(SPIKE_DIR)/arch/link.ld

# flags to trigger warning generation
WFLAGS = -Wall -Wextra -Wshadow -Wpointer-arith -Wbad-function-cast \
         -Wcast-align -Wsign-compare -Waggregate-return \
         -Wstrict-prototypes -Wmissing-prototypes \
         -Wmissing-declarations -Wunused \
         -Wimplicit -Wswitch -Wreturn-type #-Wredundant-decls 

DBG_FLAGS = -O0

AFLAGS =  -march=$(ISA) $(WFLAGS) $(DBG_FLAGS) -x assembler-with-cpp -c
CFLAGS =  -march=$(ISA) $(WFLAGS) $(DBG_FLAGS) -c
LFLAGS =  -march=$(ISA) -nostartfiles -static

AFLAGS += -fno-strict-aliasing -fno-builtin -D__gracefulExit
CFLAGS += -fomit-frame-pointer -fno-strict-aliasing -fno-builtin \
	    -D__gracefulExit	    
CFLAGS += -D_BUILD_HEX_TIME_=$(HEX_TIME) -D_BUILD_HEX_DATE_=$(HEX_DATE)

# FreeRTOS
FREERTOS_SOURCE_DIR = $(COMMON_DIR)/FreeRTOS/Source
FREERTOS_SRC = \
	$(FREERTOS_SOURCE_DIR)/croutine.c \
	$(FREERTOS_SOURCE_DIR)/list.c \
	$(FREERTOS_SOURCE_DIR)/queue.c \
	$(FREERTOS_SOURCE_DIR)/tasks.c \
	$(FREERTOS_SOURCE_DIR)/timers.c \
	$(FREERTOS_SOURCE_DIR)/event_groups.c \
	$(FREERTOS_SOURCE_DIR)/portable/MemMang/heap_2.c 
	
PORT_SRC = $(FREERTOS_SOURCE_DIR)/portable/GCC/RISCV/port.c
PORT_ASM = $(FREERTOS_SOURCE_DIR)/portable/GCC/RISCV/portasm.S

# application
APP_SRC = $(SRC_DIR)/main.c \
	$(TARGET_DIR)/arch/clib.c \
	$(TARGET_DIR)/arch/syscalls.c \
	$(TARGET_DIR)/arch/serial.c \
	$(TARGET_DIR)/led.c \
	$(SRC_DIR)/freertos_hooks.c \
	$(SRC_DIR)/cmd_handler.c \
	$(SRC_DIR)/tunnel.c

# support files
ASM_SOURCES =$(TARGET_DIR)/arch/boot.S $(PORT_ASM)
C_SOURCES 	= $(FREERTOS_SRC) $(PORT_SRC) $(APP_SRC)

LIBS 		=
STD_LIBS	= c

INCLUDE_DIRS = 	src $(RISCV)/include/spike \
				$(FREERTOS_SOURCE_DIR) \
				$(FREERTOS_SOURCE_DIR)/portable/GCC/RISCV \
				$(FREERTOS_SOURCE_DIR)/include \
				$(TARGET_DIR)/ \
				$(TARGET_DIR)/conf \
				$(TARGET_DIR)/arch \
				$(COMMON_DIR)/include

BUILD_DIRS  = $(sort $(addprefix $(BUILD_DIR)/,$(dir $(OBJS))) $(BUILD_DIR)/)
OBJS 		= $(ASM_SOURCES:.S=.o) $(C_SOURCES:.c=.o)

################################################################################
#### Default rules
################################################################################

    ############################################################################
    ## Phony targets
    ############################################################################
.PHONY: clean all code binary run

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).dump

run: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).dump
	@$(call print_cmd_info,"RUNNING SPIKE",$@)
	@spike --isa=RV32IM  $(BUILD_DIR)/$(TARGET).elf 

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
	@$(call print_cmd_info,"MK WORK DIR",$@)
	@mkdir -p $@

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
	@$(CC) $(LFLAGS) -Wl,-T -Wl,$(LINK_FILE) -Wl,-Map=$(BUILD_DIR)/$(*F).map $(OBJS:%=$(BUILD_DIR)/%) $(LIBS) $(STD_LIBS:%=-l%) -o $(BUILD_DIR)/$(@F)

%.hex: %.elf $(CONFIG) | $(BUILD_DIRS)
	@$(call print_cmd_info,"GEN HEX",$@)
	@$(OBJCOPY) -O ihex $(BUILD_DIR)/$(<F) $(BUILD_DIR)/$(@F)

%.nm: %.elf $(CONFIG) | $(BUILD_DIRS)
	@$(call print_cmd_info,"GEN NM",$@)
	@$(NM) $(BUILD_DIR)/$(<F) > $(BUILD_DIR)/$(@F)

%.srec: %.elf $(CONFIG) | $(BUILD_DIRS)
	@$(call print_cmd_info,"GEN HEX",$@)
	@$(OBJCOPY) -O srec $(BUILD_DIR)/$(<F) $(BUILD_DIR)/$(@F)

%.bin: %.elf $(CONFIG) | $(BUILD_DIRS)
	@$(call print_cmd_info,"GEN BIN",$@)
	@$(OBJCOPY) -O binary $(BUILD_DIR)/$(<F) $(BUILD_DIR)/$(@F)

#%.nm: %.elf $(BUILD_DIR_DEP) $(CONFIG)
#	@$(call print_cmd_info,"GEN NM",$@)
#	@$(NM) $(BUILD_DIR)/$(<F) > $(BUILD_DIR)/$(@F)

#%.srec: %.elf $(BUILD_DIR_DEP) $(CONFIG)
#	@$(call print_cmd_info,"GEN HEX",$@)
#	@$(OBJCOPY) -O srec $(BUILD_DIR)/$(<F) $(BUILD_DIR)/$(@F)

%.dump: %.elf $(BUILD_DIR_DEP) $(CONFIG)
	@$(call print_cmd_info,"GEN DUMP",$@)
	@$(OBJDUMP) --disassemble-all --disassemble-zeroes --section=.text --section=.rodata --section=.sbss --section=.sbss2 --section=.data  $(BUILD_DIR)/$(<F) > $(BUILD_DIR)/$(@F)

