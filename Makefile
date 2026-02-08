MAKEFLAGS += --no-builtin-rules

# disable ds-rom output
export RUST_LOG = ds_rom::rom::rom=warn

# Ensure the build fails if a piped command fails
SHELL = /usr/bin/env bash
.SHELLFLAGS = -o pipefail -c

# path to decomp, defaults to the submodule's path
STGZ_DECOMP_DIR ?= resources/decomp

# game region, only eur is supported atm
REGION := eur

### project tools ###

MAKE = make
MKDIR ?= mkdir
CMAKE ?= cmake
RM ?= rm
CP ?= cp -v

# python
VENV := .venv
PYTHON ?= $(VENV)/bin/python3

# download tool (took from st decomp)
DL_TOOL := $(PYTHON) tools/download_tool.py -p tools/

# rom patcher tool
ROM_PATCHER := $(PYTHON) tools/rom_patcher.py

# ds-rom
DSROM := tools/dsrom

# armips setup
ARMIPS_DIR := tools/armips
ARMIPS ?= $(ARMIPS_DIR)/build/armips

# main source/objects
BUILD_DIR := build/$(REGION)
ASM_FILES := $(wildcard src/*.s)
C_FILES := $(wildcard src/*.c)
CPP_FILES := $(wildcard src/*.cpp)
OBJ := $(foreach f,$(ASM_FILES),$(BUILD_DIR)/$(f:.s=.o)) $(foreach f,$(C_FILES),$(BUILD_DIR)/$(f:.c=.o)) $(foreach f,$(CPP_FILES),$(BUILD_DIR)/$(f:.cpp=.o)) $(BUILD_DIR)/thumb-$(REGION).o

# hooks source/objects
HOOKS_BUILD_DIR := hooks/build/$(REGION)
HOOKS_SRC := $(wildcard hooks/src/*.c)
HOOKS_OBJ := $(foreach f,$(HOOKS_SRC:hooks/%=%),$(HOOKS_BUILD_DIR)/$(f:.c=.o))

# region addresses
ifeq ($(REGION),eur)
OVL018_ADDR := 0x020C4840
OVLGZ_ADDR := 0x0218A380
HOOK_INIT := 0x020C4DD0
HOOK_UPDATE := 0x02013464
else
$(error "Region not supported: $(REGION)")
endif

OVLGZ_SIZE := 0x20000
HOOKS_SIZE := 0x1E0
HOOKS_ADDR := 0x01FFFE20

# compiler settings
CC := arm-none-eabi-gcc -marm -mthumb-interwork -march=armv5te -mtune=arm946e-s -nostdlib -nodefaultlibs -nostartfiles
WARNINGS := -Wall -Wno-multichar -Wno-unknown-pragmas
INCLUDES := -I include -I $(STGZ_DECOMP_DIR)/include -I $(STGZ_DECOMP_DIR)/libs/c/include
CPP_DEFINES := -DGZ_OVL_ID=114
CFLAGS := -Os -fno-short-enums -fomit-frame-pointer -ffast-math -fno-builtin -fshort-wchar $(WARNINGS) $(INCLUDES) $(CPP_DEFINES)
CPP_FLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=c++2c

ELF := $(BUILD_DIR)/ovgz.elf
BIN := $(ELF:.elf=.bin)
MAP := $(ELF:.elf=.map)
LD := $(CC)
LDFLAGS := -T libs/ovgz.ld -Llibs -lst-$(REGION) -Wl,-Map,$(MAP) -specs=nosys.specs -Wl,--gc-sections -Wl,--defsym=OVLGZ_ADDR=$(OVLGZ_ADDR) -Wl,--defsym=OVLGZ_SIZE=$(OVLGZ_SIZE)
OBJCOPY := arm-none-eabi-objcopy

# create output directories
$(shell mkdir -p $(BUILD_DIR)/src)
$(shell mkdir -p $(HOOKS_BUILD_DIR)/src)

### project settings ###

EXTRACT_DIR := extract
EXTRACTED_DIR := $(EXTRACT_DIR)/$(REGION)
OUT_ROM := stgz.nds

EXTRACTED_REL := ../../../$(EXTRACTED_DIR)
ARMIPS_ARGS ?= \
				-strequ OVL018_BIN "$(EXTRACTED_REL)/arm9_overlays/ov018.bin" \
				-strequ OVL018_MOD_BIN "$(EXTRACTED_REL)/arm9_overlays/ov018_mod.bin" \
				-strequ ARM9_BIN "$(EXTRACTED_REL)/arm9/arm9.bin" \
				-strequ ARM9_MOD_BIN "$(EXTRACTED_REL)/arm9/arm9_mod.bin" \
				-strequ ITCM_BIN "$(EXTRACTED_REL)/arm9/itcm.bin" \
				-strequ ITCM_MOD_BIN "$(EXTRACTED_REL)/arm9/itcm_mod.bin" \
				-equ OVL018_ADDR $(OVL018_ADDR) \
				-equ HOOKS_SIZE $(HOOKS_SIZE) \
				-equ HOOKS_ADDR $(HOOKS_ADDR) \
				-equ HOOK_UPDATE $(HOOK_UPDATE) \
				-equ HOOK_INIT $(HOOK_INIT)

### project targets ###

all: build
	$(DSROM) build --config $(EXTRACTED_DIR)/config.yaml --rom $(OUT_ROM)

extract:
	$(DSROM) extract --rom $(EXTRACT_DIR)/baserom_st_$(REGION).nds --path $(EXTRACTED_DIR)

build: $(HOOKS_OBJ) $(BIN)
	$(ROM_PATCHER) -e $(EXTRACTED_DIR) -o $(OBJ) -m $(OVLGZ_SIZE) -j $(HOOKS_OBJ) -n $(HOOKS_SIZE) -a $(OVLGZ_ADDR) -d $(HOOKS_BUILD_DIR) --elf $(ELF) --map $(MAP)
	$(ARMIPS) $(HOOKS_BUILD_DIR)/setup.asm $(ARMIPS_ARGS)

venv:
# Create the virtual environment if it doesn't exist.
# Delete the virtual environment directory if creation fails.
	test -d $(VENV) || python3 -m venv $(VENV) || { rm -rf $(VENV); false; }
	$(PYTHON) -m pip install -U pip
	$(PYTHON) -m pip install -U -r tools/requirements.txt

libs:
	$(PYTHON) tools/gen_libs.py -d $(STGZ_DECOMP_DIR) -b build

init: venv libs
	sha1sum -c $(EXTRACT_DIR)/baserom_st_$(REGION).sha1
	$(DL_TOOL) dsrom v0.6.1
ifeq ("$(wildcard $(ARMIPS_DIR))", "")
	$(error armips not found!)
else
ifeq ("$(wildcard $(ARMIPS_DIR)/out)", "")
	$(MKDIR) $(ARMIPS_DIR)/out && cd $(ARMIPS_DIR)/out && $(CMAKE) -DCMAKE_BUILD_TYPE=Release .. && $(CMAKE) --build .
endif
endif

setup: extract

clean:
	$(RM) $(ELF) $(BIN) $(MAP) $(OBJ) $(HOOKS_OBJ) $(OUT_ROM)

.PHONY: build clean extract libs init setup venv

### misc project recipes ###

# process auto-generated thumb definitions (necessary to avoid crashes when calling thumb functions)
$(BUILD_DIR)/thumb-$(REGION).o: build/thumb-$(REGION).s
	$(CC) $(CFLAGS) -fverbose-asm -Os -x assembler-with-cpp -fomit-frame-pointer -c "$<" -o "$@"

# compile S source file to assembly code
$(BUILD_DIR)/src/%.o: src/%.s
	$(CC) $(CFLAGS) -fverbose-asm -Os -x assembler-with-cpp -fomit-frame-pointer -c "$<" -o "$@"

# compile C source file to object file
$(BUILD_DIR)/src/%.o: src/%.c
	$(CC) $(CFLAGS) -c "$<" -o "$@"

# compile C++ source file to object file
$(BUILD_DIR)/src/%.o: src/%.cpp
	$(CC) $(CPP_FLAGS) -c "$<" -o "$@"

# compile C source file to object file
$(HOOKS_BUILD_DIR)/src/%.o: hooks/src/%.c
	$(CC) $(CFLAGS) -c "$<" -o "$@"

$(ELF): $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

$(BIN): $(ELF)
	$(OBJCOPY) -S -O binary $< $@
	$(CP) $@ $(EXTRACTED_DIR)/arm9_overlays/ovgz.bin
