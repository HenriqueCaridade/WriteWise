# name of the program (Minix service)
PROG=proj 

.PATH: ${.CURDIR}/lib/
.PATH: ${.CURDIR}/lib/text
.PATH: ${.CURDIR}/lib/drivers
.PATH: ${.CURDIR}/lib/drivers/serialPort
.PATH: ${.CURDIR}/lib/drivers/graphics
.PATH: ${.CURDIR}/lib/drivers/KBC
.PATH: ${.CURDIR}/lib/drivers/timer
.PATH: ${.CURDIR}/lib/scancode
.PATH: ${.CURDIR}/ui/
.PATH: ${.CURDIR}/data/
.PATH: ${.CURDIR}/typing/

# lib and driver files
TEXT_FILES = text.c
SCANCODE_FILES = scancode.c
DRIVER_FILES = graphics.c KBC.c keyboard.c mouse.c timer.c utils.c serialPort.c
LIB_FILES = lib.c $(TEXT_FILES) $(DRIVER_FILES) $(SCANCODE_FILES)
UI_FILES = ui.c
DATA_FILES = data.c
TYPING_FILES = typing.c

# source code files to be compiled
SRCS = $(LIB_FILES) $(UI_FILES) $(DATA_FILES) $(TYPING_FILES) proj

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic # -D __LCOM_OPTIMIZED__

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

