#Adapted from 
#https://github.com/AtariLynx/programming-tutorial/blob/main/helloworld/Makefile

# Detect Operating System
ifeq ($(OS),Windows_NT)
    # Windows Settings
    RM = del /Q /F
    CP = copy /Y
    # Fix paths to use backslashes if necessary, 
    # though cc65 usually handles forward slashes fine.
    CC65_INC ?= C:/cc65/include
    CA65_INC ?= C:/cc65/asminc
    # Use a dummy touch for Windows or skip it
    TOUCH = type nul >
else
    # Linux/macOS Settings
    RM = rm -f
    CP = cp
    CC65_INC ?= /usr/share/cc65/include
    CA65_INC ?= /usr/share/cc65/asminc
    TOUCH = touch
endif

ifeq ($(BUILD),)
	BUILD=debug
endif

SYS=atari

CO=co65
CC=cc65
AS=ca65
AR=ar65
CL=cl65
CP=cp

ifeq ($(OS),Windows_NT)
RM = del /Q /F
else 
# linux setting
RM = rm -f
endif

ECHO=echo
TOUCH=touch



# Flag for assembler
AFLAGS=-I $(CA65_INC) --debug-info -t $(SYS) -D $(BUILD)

# Flags for C-code compiler
CFLAGS=-I ./lib -t $(SYS) --add-source -g -D $(BUILD)

target = scroll_test.xex
viceLabel = game.lbl

# list of object files
engine = main.o
objects = $(engine)
		  
$(target) : $(objects)
	$(CL) -t $(SYS) -Ln scroll_test.lbl -Wl --dbgfile,scroll_test.dbg -C atari_modifed.cfg -o $@ $(objects) 

# Rule for making a *.o file out of a *.c file
%.o: %.c
	$(CC) $(CFLAGS) -o $(*).s $<
	$(AS) -o $@ $(AFLAGS) $(*).s
ifeq ($(OS),Windows_NT)
	$(RM) $(subst /,\,$*.s)
else
	$(RM) $*.s
endif


# Rule for making a *.o file out of a *.s or *.asm file
%.o: %.s
	$(AS) -o $@ $(AFLAGS) $<
%.o: %.asm
	$(AS) -o $@ $(AFLAGS) $<

all: $(target)

clean :
	$(RM) $(objects)
	$(RM) *.spr
	$(RM) *.s
	$(RM) *.pal
	$(RM) *.xex
	$(RM) *.lbl
	$(RM) *.dbg