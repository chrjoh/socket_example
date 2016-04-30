.KEEP_STATE:

CC      = gcc
MKDIR_P = mkdir -p
LDLIBS  = -lpthread
CFLAGS  = -std=c11 -g -pipe -D_XOPEN_SOURCE=700 -Wall
OBJDIR  = obj
SRCDIR  = src
EXEC    = server

SRCFILES = server.c sock.c logging.c session.c
SRCS     = $(patsubst %.c,$(SRCDIR)/%.c, $(SRCFILES))
OBJS     = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCFILES))

#$(info src = $(SRCS))
#$(info obj = $(OBJS))

.PHONY: directory

all: directory $(EXEC)

directory: $(OBJDIR)

$(EXEC):	$(OBJS)
	$(CC) -o $@  $(LDLIBS) $^

$(OBJDIR):
	$(MKDIR_P) $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $<

,clean:    ;rm -f $(OBJS) 
,cleanall: ;rm -f $(OBJS) $(EXEC)
,touch:    ;touch $(SRCS) $(SRCS:.c=.h) $(ADDI)


