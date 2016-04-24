FLAGS = -std=c11
DEBUG_FLAGS = -Wall -g
BINNAME = sockmess

PREHOOK = mkdir -p bin
POSTHOOK =

KERNEL_OUT = -o bin/$(BINNAME) -lm
SENDER_OUT = -o bin/$(BINNAME)_sender
GETTER_OUT = -o bin/$(BINNAME)_getter
VIEW_OUT = -o bin/$(BINNAME)_view
ENCRYPTER_OUT = -o bin/$(BINNAME)_encrypter -lm
DECRYPTER_OUT = -o bin/$(BINNAME)_decrypter -lm

include src/utils/uniutils/uniutils.mk
include src/utils/subprocutils/subprocutils.mk

include src/services/kernel/kernel.mk
include src/services/sender/sender.mk
include src/services/getter/getter.mk
include src/services/view/view.mk
include src/services/encrypter/encrypter.mk
include src/services/decrypter/decrypter.mk

all:
	$(PREHOOK)
	cc $(DEBUG_FLAGS) $(FLAGS) $(KERNEL_F) $(KERNEL_OUT)
	cc $(DEBUG_FLAGS) $(FLAGS) $(SENDER_F) $(SENDER_OUT)
	cc $(DEBUG_FLAGS) $(FLAGS) $(GETTER_F) $(GETTER_OUT)
	cc $(DEBUG_FLAGS) $(FLAGS) $(VIEW_F) $(VIEW_OUT)
	cc $(DEBUG_FLAGS) $(FLAGS) $(ENCRYPTER_F) $(ENCRYPTER_OUT)
	cc $(DEBUG_FLAGS) $(FLAGS) $(DECRYPTER_F) $(DECRYPTER_OUT)
	$(POSTHOOK)

prod:
	$(PREHOOK)
	cc $(FLAGS) $(KERNEL_F) $(KERNEL_OUT)
	cc $(FLAGS) $(SENDER_F) $(SENDER_OUT)
	cc $(FLAGS) $(GETTER_F) $(GETTER_OUT)
	cc $(FLAGS) $(VIEW_F) $(VIEW_OUT)
	cc $(FLAGS) $(ENCRYPTER_F) $(ENCRYPTER_OUT)
	cc $(FLAGS) $(DECRYPTER_F) $(DECRYPTER_OUT)
	$(POSTHOOK)

tar:
	tar -cf ../$(BINNAME).tar ../$(BINNAME)

clean:
	rm -rf bin
