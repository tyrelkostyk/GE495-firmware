.PHONY: all arduino clean dry

SRCDIR=./src
OBJDIR=./build
MAIN=$(SRCDIR)/main

PORT=/dev/ttyUSB0
CPU=atmega328old

DEVICE=arduino:avr:nano:cpu=$(CPU)

all: arduino

# Compile and upload to the Arduino
arduino: dry
	@arduino-cli upload \
	  --input-dir $(OBJDIR) \
	  -p $(PORT) \
	  -b $(DEVICE) \
	  $(MAIN)

# Just compile (no upload)
dry:
	@arduino-cli compile \
	  --build-path $(OBJDIR) \
	  -b $(DEVICE) \
	  --warnings default \
	  --build-property build.extra_flags=-D_ARDUINO \
	  $(MAIN)

# Compile and upload to the Arduino in DEBUG mode
debug: dry-dbg
	@arduino-cli upload \
	  --input-dir $(OBJDIR) \
	  -p $(PORT) \
	  -b $(DEVICE) \
	  $(MAIN)

# Just compile (no upload) in DEBUG mode
dry-dbg:
	@arduino-cli compile \
	  --build-path $(OBJDIR) \
	  -b $(DEVICE) \
	  --warnings default \
	  --build-property build.extra_flags="-D_ARDUINO -D_DBG" \
	  $(MAIN)

# Remove build artifacts and compiled binaries
clean:
	rm -rf $(OBJDIR)/*
