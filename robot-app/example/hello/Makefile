COMPILER_PREFIX=/usr/local/ae32000-elf-uclibc-tools
CROSS=ae32000-elf-uclibc-
CC=$(CROSS)gcc
ELF2FLT=$(CROSS)elf2flt

CFLAGS += -Dlinux -D__linux__ -Dunix -D__unix__ -D__uClinux__ -DEMBED
LDFLAGS += -r -Xlinker -T$(COMPILER_PREFIX)/lib/ae32000-elf2flt.ld
TRG = hello
ALLOBJS = hello.o
ALLSRCS = $(ALLOBJS:.o=.c)
all:$(TRG)
$(TRG) : $(ALLOBJS)
	$(CC) $(LDFLAGS) $(ALLOBJS) -o $@.elf
	$(ELF2FLT) $(TRG).elf
	mv $(TRG).elf.bflt $(TRG)

%o : %c
	$(CC) -c $(CFLAGS) $< -o $@

clean :
	rm -f *.o *.elf *.bflt $(TRG)
