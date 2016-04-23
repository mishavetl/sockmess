FLAGS = -std=c11
DEBUG_FLAGS = -Wall -g
LDFLAGS = -lncurses
BINNAME = sockmess

PREHOOK = mkdir -p bin
POSTHOOK =

KERNEL_OUT = -o bin/$(BINNAME) -lm
SENDER_OUT = -o bin/$(BINNAME)_sender
GETTER_OUT = -o bin/$(BINNAME)_getter
VIEW_OUT = -o bin/$(BINNAME)_view

include src/utils/uniutils/uniutils.mk
include src/utils/subprocutils/subprocutils.mk

include src/services/kernel/kernel.mk
include src/services/sender/sender.mk
include src/services/getter/getter.mk
include src/services/view/view.mk

all:
	$(PREHOOK)
	cc $(DEBUG_FLAGS) $(FLAGS) $(KERNEL_F) $(KERNEL_OUT)
	cc $(DEBUG_FLAGS) $(FLAGS) $(SENDER_F) $(SENDER_OUT)
	cc $(DEBUG_FLAGS) $(FLAGS) $(GETTER_F) $(GETTER_OUT)
	cc $(DEBUG_FLAGS) $(FLAGS) $(VIEW_F) $(VIEW_OUT)
	$(POSTHOOK)

prod:
	$(PREHOOK)
	cc $(FLAGS) $(KERNEL_F) $(KERNEL_OUT)
	cc $(FLAGS) $(SENDER_F) $(SENDER_OUT)
	cc $(FLAGS) $(GETTER_F) $(GETTER_OUT)
	cc $(FLAGS) $(VIEW_F) $(VIEW_OUT)
	$(POSTHOOK)

clean:
	rm -rf bin
