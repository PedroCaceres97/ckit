# =====================================
# Makefile template v0.6
# =====================================
# EDIT WITH HIGH PRECAUTION.
# =====================================

define DEFINE_TEST

$$(BIN_PATH)/$$(TESTS_FOLDER)/$(1)$(EXEEXT): $$(OBJ_PATH)/$$(TESTS_FOLDER)/$(1)$$(OBJEXT) $$(TARGETS)
	@$$(call MKDIR,$$(dir $$@))
	@echo $$(CC) $$@
	@$$(call LINK.exe,$$(OBJ_PATH)/$$(TESTS_FOLDER)/$(1)$$(OBJEXT),$$(TESTS_LDFLAGS))

$$(OBJ_PATH)/$$(TESTS_FOLDER)/$(1)$$(OBJEXT): $$(TESTS_FOLDER)/$(1).c
	@$$(call MKDIR,$$(dir $$@))
	@$$(call COMPILE.c,$$<)

endef