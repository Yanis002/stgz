MAKEFLAGS += --no-builtin-rules

# Ensure the build fails if a piped command fails
SHELL = /usr/bin/env bash
.SHELLFLAGS = -o pipefail -c

### project tools ###

# rm
RM := rm

# python
VENV := .venv
PYTHON ?= $(VENV)/bin/python3

# download tool (took from st decomp)
DL_TOOL := $(PYTHON) tools/download_tool.py -p tools/

# ds-rom
DSROM := tools/dsrom

### project settings ###

# game region, only eur is supported atm
REGION := eur
ifneq ($(REGION),eur)
$(error "Region not supported: $(REGION)")
endif

EXTRACT_DIR := extract
EXTRACTED_DIR := $(EXTRACT_DIR)/$(REGION)
OUT_ROM := stgz.nds

### project targets ###

extract:
	$(DSROM) extract --rom $(EXTRACT_DIR)/baserom_st_$(REGION).nds --path $(EXTRACTED_DIR)

build:
	$(DSROM) build --config $(EXTRACTED_DIR)/config.yaml --rom $(OUT_ROM)

venv:
# Create the virtual environment if it doesn't exist.
# Delete the virtual environment directory if creation fails.
	test -d $(VENV) || python3 -m venv $(VENV) || { rm -rf $(VENV); false; }
	$(PYTHON) -m pip install -U pip
	$(PYTHON) -m pip install -U -r tools/requirements.txt

init: venv
	$(DL_TOOL) dsrom v0.6.1
	sha1sum -c $(EXTRACT_DIR)/baserom_st_$(REGION).sha1

setup: extract

.PHONY: build extract init setup venv
