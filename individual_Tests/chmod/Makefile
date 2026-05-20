# ═══════════════════════════════════════════════════════════════
# KLEE chmod() Test Suite — Makefile
#
# Uses --sym-files 1 10 (1 file, 10 bytes)
# ═══════════════════════════════════════════════════════════════

CC       = clang
CFLAGS   = -emit-llvm -c -g -O0 -Xclang -disable-O0-optnone -I..
KLEE     = klee
KLEEOPTS = --posix-runtime --libc=uclibc
SYMFILES = --sym-files 1 10

HEADER   = ../klee_test_helper.h

TESTS = test_01 test_02 test_03 test_04 test_05 test_06 \
        test_07 test_08 test_09 test_10 test_11 test_12 \
        test_13 test_14 test_15 test_16

BCFILES = $(addsuffix .bc, $(TESTS))

RED    = \033[1;31m
GREEN  = \033[1;32m
YELLOW = \033[1;33m
CYAN   = \033[1;36m
WHITE  = \033[1;37m
DIM    = \033[2m
RESET  = \033[0m
BG_GREEN = \033[42;1;37m
BG_RED   = \033[41;1;37m

.PHONY: compile run clean

compile: $(BCFILES)
	@echo ""
	@echo "$(CYAN)═══════════════════════════════════════════════════════════$(RESET)"
	@echo "$(WHITE)  Compilation complete — $(words $(BCFILES)) bitcode files ready$(RESET)"
	@echo "$(CYAN)═══════════════════════════════════════════════════════════$(RESET)"

%.bc: %.c $(HEADER)
	@echo "$(DIM)  compiling $< ...$(RESET)"
	@$(CC) $(CFLAGS) $< -o $@

run: compile
	@echo ""
	@echo "$(CYAN)═══════════════════════════════════════════════════════════$(RESET)"
	@echo "$(WHITE)  KLEE chmod() Test Suite — $(words $(TESTS)) tests$(RESET)"
	@echo "$(CYAN)═══════════════════════════════════════════════════════════$(RESET)"
	@passed=0; failed=0; total=0; failed_list=""; \
	for t in $(TESTS); do \
		total=$$((total+1)); \
		outdir="klee-out-$$t"; \
		logfile="$$outdir.log"; \
		rm -rf "$$outdir" "$$logfile"; \
		echo ""; \
		echo "$(YELLOW)───────────────────────────────────────────────────────────$(RESET)"; \
		echo "$(WHITE)  ▶ $$t $(DIM)($(SYMFILES))$(RESET)"; \
		echo "$(YELLOW)───────────────────────────────────────────────────────────$(RESET)"; \
		$(KLEE) $(KLEEOPTS) --output-dir="$$outdir" $$t.bc $(SYMFILES) 2>&1 | tee "$$logfile"; \
		if grep -q "^KLEE: done: completed paths = 0" "$$logfile"; then \
			echo ""; \
			echo "  $(BG_RED) FAIL $(RESET) $$t (0 completed paths)"; \
			failed=$$((failed+1)); \
			failed_list="$$failed_list $$t"; \
		elif grep -Fq "[PASS]" "$$logfile"; then \
			echo ""; \
			echo "  $(BG_GREEN) PASS $(RESET) $$t"; \
			passed=$$((passed+1)); \
		else \
			echo ""; \
			echo "  $(BG_RED) FAIL $(RESET) $$t (no PASS marker)"; \
			failed=$$((failed+1)); \
			failed_list="$$failed_list $$t"; \
		fi; \
	done; \
	echo ""; \
	echo "$(CYAN)═══════════════════════════════════════════════════════════$(RESET)"; \
	echo "$(WHITE)  RESULTS$(RESET)"; \
	echo "$(CYAN)═══════════════════════════════════════════════════════════$(RESET)"; \
	echo ""; \
	echo "  $(WHITE)Total:  $$total$(RESET)"; \
	echo "  $(GREEN)Passed: $$passed$(RESET)"; \
	echo "  $(RED)Failed: $$failed$(RESET)"; \
	echo ""; \
	if [ $$failed -eq 0 ]; then \
		echo "  $(BG_GREEN) ALL TESTS PASSED $(RESET)"; \
	else \
		echo "  $(BG_RED) $$failed TEST(S) FAILED $(RESET)"; \
		echo ""; \
		echo "  $(RED)Failed tests:$$failed_list$(RESET)"; \
	fi; \
	echo ""; \
	echo "$(CYAN)═══════════════════════════════════════════════════════════$(RESET)"

run_%: test_%.bc
	@outdir="klee-out-test_$*"; \
	logfile="$$outdir.log"; \
	rm -rf "$$outdir" "$$logfile"; \
	echo ""; \
	echo "$(YELLOW)───────────────────────────────────────────────────────────$(RESET)"; \
	echo "$(WHITE)  ▶ test_$* $(DIM)($(SYMFILES))$(RESET)"; \
	echo "$(YELLOW)───────────────────────────────────────────────────────────$(RESET)"; \
	$(KLEE) $(KLEEOPTS) --output-dir="$$outdir" $< $(SYMFILES) 2>&1 | tee "$$logfile"; \
	if grep -q "^KLEE: done: completed paths = 0" "$$logfile"; then \
		echo ""; \
		echo "  $(BG_RED) FAIL $(RESET) test_$* (0 completed paths)"; \
	elif grep -Fq "[PASS]" "$$logfile"; then \
		echo ""; \
		echo "  $(BG_GREEN) PASS $(RESET) test_$*"; \
	else \
		echo ""; \
		echo "  $(BG_RED) FAIL $(RESET) test_$* (no PASS marker)"; \
	fi

clean:
	@echo ""
	@echo "$(CYAN)═══════════════════════════════════════════════════════════$(RESET)"
	@echo "$(WHITE)  Cleaning build artifacts$(RESET)"
	@echo "$(CYAN)═══════════════════════════════════════════════════════════$(RESET)"
	@rm -f *.bc
	@rm -rf klee-out-* klee-last *.log
	@echo "  $(DIM)done$(RESET)"
