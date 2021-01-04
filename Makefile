all: bomber-sdl bomber.iso

kvm: bomber.iso
	qemu-system-x86_64 -cdrom $< -enable-kvm -cpu host -k en-us -serial pty -soundhw pcspk -d guest_errors -m 2048

bomber.iso: stubs/.build/stubs.iso
	ln -s $< $@ || true

bomber-sdl: game/bomber
	ln -s $< $@ || true

stubs/.build/stubs.iso: FORCE
	$(MAKE) -C stubs iso

game/bomber: FORCE
	$(MAKE) -C $(dir $@) $(notdir $@)

FORCE:

.PHONY: all kvm clean
