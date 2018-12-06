CONTIKI_PROJECT = hello-world
all: $(CONTIKI_PROJECT)
MAKE_MAC=MAKE_MAC_TSCH
MODULES += os/services/shell
PROJECT_SOURCEFILES += app.c 
MODULES += os/services/orchestra

#MODULES += os/net/app-layer/mqtt
CONTIKI = ../..
include $(CONTIKI)/Makefile.include
