SPRITES := $(wildcard img/*.png)
STUBS := stubs
ISOBUILD := $(STUBS)/.build/stubs.iso
INITRD := $(STUBS)/initrd

all: bomber-sdl

bomber-sdl: game/bomber
	ln -s $< $@ || true

bomber-iso: $(ISOBUILD)
	ln -s $< $@ || true

kvm: $(ISOBUILD)
	qemu-system-x86_64 -cdrom $< -enable-kvm -cpu host -k en-us -serial pty -soundhw pcspk -d guest_errors -m 2048

$(INITRD)/%: %
	@mkdir -p $(dir $@)
	optipng -zc1-9 -zm8-9 -zs0-3 -nx --dir $(dir $@) $^

$(ISOBUILD): $(STUBS)/Makefile $(addprefix $(INITRD)/,$(SPRITES)) FORCE
	$(MAKE) INITRD_FREE=4194304 -C $(STUBS) iso

game/bomber: game/Makefile FORCE
	$(MAKE) -C $(dir $@) $(notdir $@)

FORCE:

.PHONY: all kvm clean
