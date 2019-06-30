#
#This Makefile of audio Module in G827 project
#
#Author:LZG
#Creation Date:2016-09-20
#Modifier:
#Last Modification Date:
#
#

ifeq ($(MAKELEVEL), 0)
$(error Note:This is not a TOP Makefile)
endif

OBJS = audio.o encrypt.o mw_audio.o speex.o

.PHONY : all
all:$(OBJS)

%.o:%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@


