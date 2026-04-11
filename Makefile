SECURE_DIR    := secure-image
NONSECURE_DIR := nonsecure-image

.PHONY: all
all: secure nonsecure

.PHONY: flash
flash: secure-flash nonsecure-flash

.PHONY: clean
clean:
	$(MAKE) -C $(SECURE_DIR) clean
	$(MAKE) -C $(NONSECURE_DIR) clean

.PHONY: secure
secure:
	$(MAKE) -C $(SECURE_DIR) $(MAKECMDGOALS)

.PHONY: secure-flash
secure-flash:
	$(MAKE) -C $(SECURE_DIR) flash

.PHONY: nonsecure
nonsecure:
	$(MAKE) -C $(NONSECURE_DIR) $(MAKECMDGOALS)

.PHONY: nonsecure-flash
nonsecure-flash:
	$(MAKE) -C $(NONSECURE_DIR) flash

