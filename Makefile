FLAGS = -Wall -g -std=c11
LDFLAGS=-lncurses
BINNAME=sockmess

include src/utils/uniutils/uniutils.mk
include src/utils/subprocutils/subprocutils.mk

include src/services/kernel/kernel.mk
include src/services/sender/sender.mk
include src/services/getter/getter.mk
include src/services/view/view.mk

all:
	mkdir -p bin
	cc $(FLAGS) $(KERNEL_F) -o bin/$(BINNAME) -lm
	cc $(FLAGS) $(SENDER_F) -o bin/$(BINNAME)_sender
	cc $(FLAGS) $(GETTER_F) -o bin/$(BINNAME)_getter
	cc $(FLAGS) $(VIEW_F) -o bin/$(BINNAME)_view

clean:
	rm -rf bin
