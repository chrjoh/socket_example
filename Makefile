.KEEP_STATE:


CC		= gcc
#LDLIBS		= 
#CFLAGS		= -ansi  -g -pipe -Wall 
CFLAGS		= -std=c11 -g -pipe -D_XOPEN_SOURCE=700 -Wall -lpthread

OBJS		= server.o sock.o
SRCS		= $(OBJSFILES:.o=.c)
EXEC		= server


all:		$(EXEC)

$(EXEC):	$(OBJS)
		$(LINK.c) -o $@  $(OBJS) $(LDLIBS)




,clean:		;rm -f $(OBJS) 
,cleanall:	;rm -f $(OBJS) $(EXEC)
,touch:         ;touch $(SRCS) $(OBJS:.o=.h) $(ADDI)


