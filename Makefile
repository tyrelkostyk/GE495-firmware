.PHONY: arduino clean old-arduino

SRCDIR=./src
OBJDIR=./build
MAIN=$(SRCDIR)/main

PORT=/dev/ttyUSB0
CPU=atmega328

DEVICE=arduino:avr:nano:cpu=$(CPU)

# Lets us dynamically add several very similar targets without duplicating a
# lot of code...
define add_arduino_target

$$(info evaluting $(1)arduino_)

$(1)arduino_:
	@arduino-cli compile \
	  --build-path $(OBJDIR) \
	  -b $(DEVICE)$(1) \
	  --build-property build.extra_flags=-DARDUINO \
	  $(MAIN)
	@arduino-cli upload \
	  --input-dir $(OBJDIR) \
	  -p $(PORT) \
	  -b $(DEVICE)$(1) \
	  $(MAIN)

endef

# The basic Arduino target (for new processors like hopefully the Amazon ones)
$(eval $(call add_arduino_target,))

# The "old" Arduino target (for older models like Jeremy's)
$(eval $(call add_arduino_target,old))

# Alias the dynamic ones to nicer names...

arduino: arduino_

old-arduino: oldarduino_

clean:
	rm -rf $(OBJDIR)/*
