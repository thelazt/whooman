SPRITES := $(wildcard img/*.png)
STUBS := stubs
ISOBUILD := $(STUBS)/.build/stubs.iso
INITRD := $(STUBS)/initrd

bomber.sdl: src/bomber
	ln -fs $< $@

all: bomber.sdl bomber.iso

bomber.iso: $(ISOBUILD)
	ln -fs $< $@

kvm: $(ISOBUILD)
	qemu-system-x86_64 -cdrom $< -enable-kvm -cpu host -k en-us -serial stdio -soundhw pcspk -d guest_errors -m 2048

$(INITRD)/%: %
	@mkdir -p $(dir $@)
	optipng -zc1-9 -zm8-9 -zs0-3 -nx --dir $(dir $@) $^

$(ISOBUILD): $(STUBS)/Makefile $(addprefix $(INITRD)/,$(SPRITES)) FORCE
	$(MAKE) INITRD_FREE=4194304 PROJECT=WHOOMan -C $(STUBS) iso

src/bomber: src/Makefile FORCE
	$(MAKE) -C $(dir $@) $(notdir $@)

FORCE:

.PHONY: all kvm clean
