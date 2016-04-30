.KEEP_STATE:

CC		= gcc
MKDIR_P = mkdir -p
#LDLIBS		= 
#CFLAGS		= -ansi  -g -pipe -Wall 
CFLAGS		= -std=c11 -g -pipe -D_XOPEN_SOURCE=700 -Wall -lpthread
OBJDIR=obj
SRCDIR=src

SRCFILES = server.c sock.c logging.c
SRCS     = $(patsubst %.c,$(SRCDIR)/%.c, $(SRCFILES))
OBJS     = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCFILES))

#$(info src = $(SRCS))
#$(info obj = $(OBJS))

.PHONY: directory

all: directory server

directory: $(OBJDIR)

server:	$(OBJS)
	$(CC) -o $@  $(LDLIBS) $^

$(OBJDIR):
	$(MKDIR_P) $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $<


#,clean:		;rm -f $(OBJS) 
#,cleanall:	;rm -f $(OBJS) $(EXEC)
#,touch:         ;touch $(SRCS) $(OBJS:.o=.h) $(ADDI)


