# ═══════════════════════════════════════════════════════════════
# KLEE Global Test Suite — Makefile
#
# Directory structure:
#   ./klee_test_helper.h
#   ./Makefile              <- this file
#   ./open/test_01.c ... test_20.c + Makefile
#   ./write/test_01.c ... test_13.c + Makefile
#   ./read/test_01.c ... test_12.c + Makefile
#   ./lseek/test_01.c ... test_13.c + Makefile
#
# Usage:
#   make run_all               run everything
#   make run_open              run all open() tests
#   make run_open_01           run open/test_01 only
#   make run_write             run all write() tests
#   make run_read              run all read() tests
#   make run_lseek             run all lseek() tests
#   make run_lseek_08          run lseek/test_08 only
#   make compile_all           compile everything
#   make clean                 nuke everything
# ═══════════════════════════════════════════════════════════════

RED      = \033[1;31m
GREEN    = \033[1;32m
YELLOW   = \033[1;33m
CYAN     = \033[1;36m
WHITE    = \033[1;37m
DIM      = \033[2m
RESET    = \033[0m
BG_GREEN = \033[42;1;37m
BG_RED   = \033[41;1;37m
BG_CYAN  = \033[46;1;37m

SUITES = open write read lseek

.PHONY: run_all compile_all clean $(addprefix run_,$(SUITES)) \
        $(addprefix compile_,$(SUITES)) $(addprefix clean_,$(SUITES))

run_all:
	@echo ""
	@echo "$(BG_CYAN) KLEE FULL TEST SUITE $(RESET)"
	@echo ""
	@for suite in $(SUITES); do \
		echo "$(CYAN)═══════════════════════════════════════════════════════════$(RESET)"; \
		echo "$(WHITE)  Suite: $$suite$(RESET)"; \
		echo "$(CYAN)═══════════════════════════════════════════════════════════$(RESET)"; \
		$(MAKE) -C $$suite run; \
		echo ""; \
	done

compile_all:
	@for suite in $(SUITES); do \
		echo "$(CYAN)  Compiling $$suite ...$(RESET)"; \
		$(MAKE) -C $$suite compile; \
	done

clean:
	@echo ""
	@echo "$(CYAN)═══════════════════════════════════════════════════════════$(RESET)"
	@echo "$(WHITE)  Cleaning all suites$(RESET)"
	@echo "$(CYAN)═══════════════════════════════════════════════════════════$(RESET)"
	@for suite in $(SUITES); do \
		$(MAKE) -C $$suite clean; \
	done

run_open:
	@$(MAKE) -C open run
run_write:
	@$(MAKE) -C write run
run_read:
	@$(MAKE) -C read run
run_lseek:
	@$(MAKE) -C lseek run

compile_open:
	@$(MAKE) -C open compile
compile_write:
	@$(MAKE) -C write compile
compile_read:
	@$(MAKE) -C read compile
compile_lseek:
	@$(MAKE) -C lseek compile

clean_open:
	@$(MAKE) -C open clean
clean_write:
	@$(MAKE) -C write clean
clean_read:
	@$(MAKE) -C read clean
clean_lseek:
	@$(MAKE) -C lseek clean

run_open_%:
	@$(MAKE) -C open compile
	@$(MAKE) -C open run_$*

run_write_%:
	@$(MAKE) -C write compile
	@$(MAKE) -C write run_$*

run_read_%:
	@$(MAKE) -C read compile
	@$(MAKE) -C read run_$*

run_lseek_%:
	@$(MAKE) -C lseek compile
	@$(MAKE) -C lseek run_$*
