# Makefile for g-epoll-source
 
AR=ar
PP=cpp
CC=cc
CCFLAGS=-O3 -Werror -Wall -fPIC `pkg-config --cflags glib-2.0`
LDFLAGS=-shared `pkg-config --libs glib-2.0`
ARFLAGS=rcs
 
SRCDIR=src
BINDIR=bin
BUILDDIR=build
 
TARGET_SHARED=$(BINDIR)/libgepollsource.so
TARGET_STATIC=$(BINDIR)/libgepollsource.a
CCOBJSFILE=$(BUILDDIR)/ccobjs
-include $(CCOBJSFILE)
LDOBJS=$(patsubst $(SRCDIR)%.c,$(BUILDDIR)%.o,$(CCOBJS))
 
DEPEND=$(LDOBJS:.o=.dep)
 
all : $(CCOBJSFILE) $(TARGET_SHARED) $(TARGET_STATIC)
	@$(RM) $(CCOBJSFILE)
 
clean : 
	@echo -n "Clean ... " && $(RM) $(BINDIR)/* $(BUILDDIR)/* && echo "OK"
 
$(CCOBJSFILE) : 
	@echo CCOBJS=`ls $(SRCDIR)/*.c` > $(CCOBJSFILE)
 
$(TARGET_SHARED) : $(LDOBJS)
	@echo -n "Linking $^ to $@ ... " && $(CC) -o $@ $^ $(LDFLAGS) && echo "OK"
 
$(TARGET_STATIC) : $(LDOBJS)
	@echo -n "Linking $^ to $@ ... " && $(AR) $(ARFLAGS) $@ $^ && echo "OK"
 
$(BUILDDIR)/%.dep : $(SRCDIR)/%.c
	@$(PP) $(CCFLAGS) -MM -MT $(@:.dep=.o) -o $@ $<
 
$(BUILDDIR)/%.o : $(SRCDIR)/%.c
	@echo -n "Building $< ... " && $(CC) $(CCFLAGS) -c -o $@ $< && echo "OK"
 
-include $(DEPEND)

