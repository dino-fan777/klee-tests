# ═══════════════════════════════════════════════════════════════
# KLEE Global Test Suite — Makefile
#
# Directory structure:
#   ./klee_test_helper.h
#   ./Makefile              <- this file
#   ./open/     20 tests    ./write/   13 tests
#   ./read/     12 tests    ./lseek/   15 tests
#   ./chmod/    16 tests
# ═══════════════════════════════════════════════════════════════

RED      = \033[1;31m
GREEN    = \033[1;32m
YELLOW   = \033[1;33m
CYAN     = \033[1;36m
WHITE    = \033[1;37m
DIM      = \033[2m
RESET    = \033[0m
BG_CYAN  = \033[46;1;37m

SUITES = open write read lseek chmod

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
run_chmod:
	@$(MAKE) -C chmod run

compile_open:
	@$(MAKE) -C open compile
compile_write:
	@$(MAKE) -C write compile
compile_read:
	@$(MAKE) -C read compile
compile_lseek:
	@$(MAKE) -C lseek compile
compile_chmod:
	@$(MAKE) -C chmod compile

clean_open:
	@$(MAKE) -C open clean
clean_write:
	@$(MAKE) -C write clean
clean_read:
	@$(MAKE) -C read clean
clean_lseek:
	@$(MAKE) -C lseek clean
clean_chmod:
	@$(MAKE) -C chmod clean

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
run_chmod_%:
	@$(MAKE) -C chmod compile
	@$(MAKE) -C chmod run_$*
