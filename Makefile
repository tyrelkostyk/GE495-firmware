.PHONY: arduino clean

SRCDIR=./src
OBJDIR=./build
MAIN=$(SRCDIR)/main

CPU=atmega328

DEVICE=arduino:avr:nano:cpu=$(CPU)

arduino:
	@arduino-cli compile \
	  --build-path $(OBJDIR) \
	  -b $(DEVICE) \
	  $(MAIN)
	@arduino-cli upload \
	  --input-dir $(OBJDIR) \
	  -p /dev/ttyUSB0 \
	  -b $(DEVICE) \
	  $(MAIN)
