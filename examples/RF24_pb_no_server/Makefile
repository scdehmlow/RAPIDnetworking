#############################################################################
#
# Makefile for librf24mesh examples on Raspberry Pi
#
# By:  TMRh20
# Date:    2013/09
#
# Description:
# ------------
# use make all and make install to install the examples
# You can change the install directory by editing the prefix line
#
# Include the nanopb provided Makefile rules
include ../../extra/nanopb.mk
#
#
prefix := /usr/local

ARCH=armv6zk
ifeq "$(shell uname -m)" "armv7l"
ARCH=armv7-a
endif

# Detect the Raspberry Pi from cpuinfo
#Count the matches for BCM2708 or BCM2709 in cpuinfo
RPI=$(shell cat /proc/cpuinfo | grep Hardware | grep -c BCM2708)
ifneq "${RPI}" "1"
RPI=$(shell cat /proc/cpuinfo | grep Hardware | grep -c BCM2709)
endif

ifeq "$(RPI)" "1"
# The recommended compiler flags for the Raspberry Pi
CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=$(ARCH) -mtune=arm1176jzf-s -std=c++0x
endif
# flags for gcc
CFLAGS= -Wall -Werror -g -O0
CFLAGS += -I$(NANOPB_DIR)
CSRC = RF24Mesh_Example_Master.cpp	# The main program
CSRC += nanopb_RPI.pb.c			# The compiled protocol definition
CSRC += $(NANOPB_DIR)/pb_encode.c	# The nanopb encoder
CSRC += $(NANOPB_DIR)/pb_decode.c	# The nanopb decoder
CSRC += $(NANOPB_DIR)/pb_common.c	# The nanopb common parts

RF24Mesh_Example: $(CSRC)
	$(CXX) $(CCFLAGS) -I$(NANOPB_DIR) -lrf24-bcm -lrf24network -lrf24mesh -oRF24Mesh_Example_Master $(CSRC)

# Build rule for the protocol
nanopb_RPI.pb.c: nanopb_RPI.proto
	$(PROTOC) $(PROTOC_OPTS) --nanopb_out=. nanopb_RPI.proto


# define all programs
#PROGRAMS = RF24Mesh_Example_Master RF24Mesh_Example
#SOURCES = ${PROGRAMS:=.cpp} #pb_encode.c pc_decode.c nanopb_RPI.pb.c

#all: ${PROGRAMS}

#${PROGRAMS}: ${SOURCES}
#	gcc $(CFLAGS) $(CSOURCES) -o nano.o 
#	$(CXX) ${CCFLAGS} -Wall -I../ -lrf24-bcm -lrf24network -lrf24mesh nano.o $@.cpp -o $@
#
clean:
	rm -rf $(PROGRAMS)
	
install: all
	test -d $(prefix) || mkdir $(prefix)
	test -d $(prefix)/bin || mkdir $(prefix)/bin
	for prog in $(PROGRAMS); do \
	  install -m 0755 $$prog $(prefix)/bin; \
	done

.PHONY: install
