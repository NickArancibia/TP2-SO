include Makefile.inc

ifdef MMTYPE
    MM := memory/buddy.c
else
    MM := memory/memoryManager.c
endif

KERNEL=kernel.bin
SOURCES=$(wildcard *.c processes/*.c drivers/*.c) $(MM)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
OBJDIR=obj
STATICLIBS=

all: $(KERNEL) move_objects

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)


%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

$(OBJDIR):
	mkdir -p $(OBJDIR)
	chmod 777 $(OBJDIR) 

move_objects: $(OBJDIR)
	mv *.o $(OBJDIR)
	mv asm/*.o $(OBJDIR)
clean:
	rm -rf $(OBJDIR) *.bin *.elf

.PHONY: all clean
