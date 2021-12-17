.PHONY: arduino clean old-arduino

SRCDIR=./src
OBJDIR=./build
MAIN=$(SRCDIR)/main

PORT=/dev/ttyUSB0
CPU=atmega328

DEVICE=arduino:avr:nano:cpu=$(CPU)

arduino:
	@arduino-cli compile \
	  --build-path $(OBJDIR) \
	  -b $(DEVICE) \
	  --build-property build.extra_flags=-DARDUINO \
	  $(MAIN)
	@arduino-cli upload \
	  --input-dir $(OBJDIR) \
	  -p $(PORT) \
	  -b $(DEVICE) \
	  $(MAIN)

old-arduino:
	@arduino-cli compile \
	  --build-path $(OBJDIR) \
	  -b $(DEVICE)old \
	  --build-property build.extra_flags=-DARDUINO \
	  $(MAIN)
	@arduino-cli upload \
	  --input-dir $(OBJDIR) \
	  -p $(PORT) \
	  -b $(DEVICE)old \
	  $(MAIN)

clean:
	rm -rf $(OBJDIR)/*
