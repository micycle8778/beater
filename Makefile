# llvm-mos/bin/mos-clang -Lllvm-mos/mos-platform/common/lib -nostartfiles -Os main.c

SDKBIN=llvm-mos/bin
CC=$(SDKBIN)/mos-clang
CFLAGS= -Os -Iinc -mcpu=mosw65c02 -Illvm-mos/mos-platform/common/include
FINALFLAGS= -Lllvm-mos/mos-platform/common/lib -nostartfiles -mlto-zp=224

out/rom.img: obj/lcd.o obj/timer.o obj/button.o out/ src/main.c
	$(CC) $(CFLAGS) $(FINALFLAGS) obj/* src/main.c -o $@

obj/%.o: src/%.c obj/
	$(CC) $(CFLAGS) $< -c -o $@

%/:
	mkdir -p $@

objdump: out/rom.img
	$(SDKBIN)/llvm-objdump -d out/rom.img.elf

size: out/rom.img
	$(SDKBIN)/llvm-size -d out/rom.img.elf

deploy: out/rom.img
	minipro -p SST39SF010A -w out/rom.img -s

clean:
	rm -rf out/
	rm -rf obj/
