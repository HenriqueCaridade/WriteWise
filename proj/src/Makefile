# name of the program (Minix service)
PROG=proj 

.PATH: ${.CURDIR}/lib/
.PATH: ${.CURDIR}/lib/text
.PATH: ${.CURDIR}/lib/drivers
.PATH: ${.CURDIR}/lib/drivers/graphics
.PATH: ${.CURDIR}/lib/drivers/KBC
.PATH: ${.CURDIR}/lib/drivers/timer
.PATH: ${.CURDIR}/ui/
.PATH: ${.CURDIR}/data/

# lib and driver files
LIB_FILES = lib.c
TEXT_FILES = text.c
DRIVER_FILES = graphics.c KBC.c keyboard.c mouse.c timer.c utils.c
UI_FILES = ui.c
DATA_FILES = data.c

# source code files to be compiled
SRCS = $(LIB_FILES) $(TEXT_FILES) $(DRIVER_FILES) $(UI_FILES) $(DATA_FILES) proj

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

