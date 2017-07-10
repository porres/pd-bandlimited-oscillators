############
# Makefile #
############

# library name

lib.name = else

#########################################################################
# sources
#########################################################################

sawtooth~.class.sources := classes/sawtooth~.c


#########################################################################

# include Makefile.pdlibbuilder

include pd-lib-builder/Makefile.pdlibbuilder
