#

CROSS_COMPILE 	= ae32000-elf-uclibc-

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)ranlib
ELF2FLT	= $(CROSS_COMPILE)elf2flt

CFLAGS	= -O2 -Wall -Wstrict-prototypes -Dlinux -D__linux__ -Dunix -D__uClinux__ -DEMBED

COMPILER_PREFIX=/usr/local/ae32000-elf-uclibc-tools
LDFLAGS = -r -Xlinker -T$(COMPILER_PREFIX)/lib/ae32000-elf2flt.ld
LDFLAGS_DBG = -Xlinker -T$(COMPILER_PREFIX)/lib/ae32000-elf2flt.ld

LDLIBS	= -lm -lc -lpthread


export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP RANLIB CFLAGS

PROJ = SoC_App

OBJS = main.o huro.o \
    ./library/graphic_api.o \
	./library/imageProcess.o \
    ./library/robot_protocol.o \
    ./library/uart_api.o \
    ./library/robot_action.o \
	./mission/DEFAULT.o \
    ./mission/MISSION_1_YELLOW_BARRICADE.o \
    ./mission/MISSION_2_RED_BRIDGE.o \
    ./mission/MISSION_3_AVOID_BOMB.o \
    ./mission/MISSION_4_JUMP_HURDLE.o \
    ./mission/MISSION_5_GREEN_BRIDGE.o \
    ./mission/MISSION_6_KICK_BALL.o \
    ./mission/MISSION_7_YELLOW_HOLE_BRIDGE.o \
    ./mission/MISSION_8_CREVASSE.o \
    ./mission/MISSION_9_LAST_BARRICADE.o \
    ./mission/MISSION_10_BLUE_GATE.o


all: $(PROJ)

$(PROJ): $(OBJS)
	$(CC) $(LDFLAGS) -o $@.elf $^ $(LDLIBS)
	$(CC) $(LDFLAGS_DBG) -o $@.dbg $^ $(LDLIBS)
	$(ELF2FLT) $@.elf
	mv $@.elf.bflt $@
	#${OBJDUMP} -Dtx $@.elf > $@.elf.dis
	#${OBJDUMP} -Dtx $@.dbg > $@.dbg.dis

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.s.o:
	$(AS) $< -o $@

clean:
	rm -f *.o **/*.o *.elf *.bflt *.dis $(PROJ) *.dbg ImageLoad_Test
