PROG=proj

.PATH: ${.CURDIR}/controller/
.PATH: ${.CURDIR}/controller/keyboard
.PATH: ${.CURDIR}/controller/mouse
.PATH: ${.CURDIR}/controller/timer
.PATH: ${.CURDIR}/controller/graphic
.PATH: ${.CURDIR}/model/
.PATH: ${.CURDIR}/draw/

SRCS = main.c timer.c utils.c KBC.c keyboard.c graphic.c mouse.c draw.c model.c sprite.c

CFLAGS += -pedantic -D_LCOM_OPTIMIZED_ -O2

DPADD += ${LIBLCF}
LDADD += -Wall -llcf -lxpm -llm
.include <minix.lcom.mk>