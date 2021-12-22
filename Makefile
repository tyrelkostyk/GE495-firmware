.PHONY: arduino clean dry

SRCDIR=./src
OBJDIR=./build
MAIN=$(SRCDIR)/main

PORT=/dev/ttyUSB0
CPU=atmega328old

DEVICE=arduino:avr:nano:cpu=$(CPU)

# Compile and upload to the Arduino
arduino: dry
	@arduino-cli upload \
	  --input-dir $(OBJDIR) \
	  -p $(PORT) \
	  -b $(DEVICE)$(1) \
	  $(MAIN)

# Just compile (no upload)
dry:
	@arduino-cli compile \
	  --build-path $(OBJDIR) \
	  -b $(DEVICE) \
	  --warnings default \
	  --build-property build.extra_flags=-D_ARDUINO \
	  $(MAIN)
	
# Remove build artifacts and compiled binaries
clean:
	rm -rf $(OBJDIR)/*
