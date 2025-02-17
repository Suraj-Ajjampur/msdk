# This file can be used to set build configuration
# variables.  These variables are defined in a file called 
# "Makefile" that is located next to this one.

# For instructions on how to use this system, see
# https://analogdevicesinc.github.io/msdk/USERGUIDE/#build-system

# **********************************************************

# Add your config here!

# If you have secure version of MCU (MAX32666), set SBT=1 to generate signed binary
# For more information on how sing process works, see
# https://www.analog.com/en/education/education-library/videos/6313214207112.html
SBT=0

# This Low-Power example shuts down some SRAM instances.
# To ensure application code does not use the SRAM that will
# be shut down, the available SRAM size is reduced with a
# custom linkerfile.  The section below selects the
# appropriate file depending on whether the SBTs are used or not.
ifeq ($(SBT),1)
# This linkerfile contains extra sections for compatibility with the Secure Boot Tools (SBT).
override LINKERFILE = lp-sla.ld
else
# This linkerfile is for use with standard non-secure applications.
override LINKERFILE = lp-nonsecure.ld
endif # SBT
