# beater

Code for my Ben Eater 6502 computer

---

There are a few differences from my computer and the BE6502 (hence the need for
a custom target):

* I do not have the UART chip on my computer.
* The VIA to character LCD mappings are a little different, which you can see
  in [src/lcd.c](src/lcd.c).
* I don't use whatever EEPROM Ben Eater does, I instead have the SST39SF010A.
  If you want to use `make deploy` to flash your EEPROM, change the make file
  appropriately.
* I have four buttons attached to the VIA chip on pins PA4-PA7, and those
  buttons are the basis for user input (see [src/button.c](src/button.c)).

## Building

The code is written in C instead of 6502 assembly, meaning you'll need a C
compiler. The one I'm using is [llvm-mos](https://llvm-mos.org/). It seems to
output pretty good stuff, and it also has the whole familiar suite of C
tooling since it can output ELF files.

1. Download and extract the llvm-mos SDK. Put (or symlink) the SDK to a folder
   called `llvm-mos` at the root of the project.
2. `make` will output a rom image to `out/rom.img` and an elf file to
   `out/rom.img.elf`.
   - `make objdump` will output a disassembly of the rom elf file, if you're
     interested in that for whatever reason.
3. Use `minipro` or your choice of software to flash the rom to your eeprom.
    - Optionally, you can use `make deploy`, which will run `minipro` for you.
4. Profit.
