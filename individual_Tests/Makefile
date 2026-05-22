# ═══════════════════════════════════════════════════════════════
# KLEE Global Makefile
#
#
# Usage:
#   make run_all               run all 100 tests
#   make run_open              run all open() tests
#   make run_open_01           run open/test_01 only
#   make run_write             run all write() tests
#   make run_write_05          run write/test_05 only
#   make run_read              run all read() tests
#   make run_read_08           run read/test_08 only
#   make run_lseek             run all lseek() tests
#   make run_lseek_07          run lseek/test_07 only
#   make run_chmod             run all chmod() tests
#   make run_chmod_12          run chmod/test_12 only
#   make run_close             run all close() tests
#   make run_close_05          run close/test_05 only
#   make run_dup               run all dup/dup2 tests
#   make run_dup_11            run dup/test_11 only
#   make compile_all           compile everything
#   make compile_open          compile open/ only
#   make clean                 nuke everything
#   make clean_open            clean open/ only
# ═══════════════════════════════════════════════════════════════

RED      = \033[1;31m
GREEN    = \033[1;32m
YELLOW   = \033[1;33m
CYAN     = \033[1;36m
WHITE    = \033[1;37m
DIM      = \033[2m
RESET    = \033[0m
BG_CYAN  = \033[46;1;37m

SUITES = open write read lseek chmod close dup

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
run_close:
	@$(MAKE) -C close run
run_dup:
	@$(MAKE) -C dup run

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
compile_close:
	@$(MAKE) -C close compile
compile_dup:
	@$(MAKE) -C dup compile

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
clean_close:
	@$(MAKE) -C close clean
clean_dup:
	@$(MAKE) -C dup clean

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
run_close_%:
	@$(MAKE) -C close compile
	@$(MAKE) -C close run_$*
run_dup_%:
	@$(MAKE) -C dup compile
	@$(MAKE) -C dup run_$*
