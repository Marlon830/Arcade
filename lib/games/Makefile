##
## EPITECH PROJECT, 2024
## B-OOP-400-MPL-4-1-arcade-marlon.pegahi
## File description:
## Makefile
##

SUBDIRS := $(wildcard */.)

.PHONY: all clean fclean

all: $(SUBDIRS)
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean: $(SUBDIRS)
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

fclean: $(SUBDIRS)
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir fclean; \
	done

re: fclean all
