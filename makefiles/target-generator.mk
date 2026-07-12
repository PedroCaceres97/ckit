# =====================================
# Makefile template v0.6
# =====================================
# EDIT WITH HIGH PRECAUTION.
# =====================================

define DEFINE_TARGET

$(1)_DIR 		?= src
$(1)_TYPE 		?= executable
$(1)_LIBS 		?=
$(1)_LIBS_PATH 	?=

$(1)_SRC_FILES := $$(call rwildcard,$$($(1)_DIR),*.c)
$(1)_OBJ_FILES := $$(patsubst %.c,$$(OBJ_PATH)/$(1)/%$$(OBJEXT),$$(patsubst $$($(1)_DIR)/%,%,$$($(1)_SRC_FILES)))
$(1)_LDFLAGS := $$(call expand_libs_path,$$($(1)_LIBS_PATH))$$(call expand_libs,$$($(1)_LIBS))

ifeq ($$($(1)_TYPE),executable)

$(1): $$(BIN_PATH)/$(1)$$(EXEEXT)

$$(BIN_PATH)/$(1)$$(EXEEXT): $$($(1)_OBJ_FILES)
	@$$(call MKDIR,$$(dir $$@))
	@$$(PRE_BUILD)
	@echo $$(CC) $$@
	@$$(ECHO_BLANK)
	@$$(call LINK.exe,$$($(1)_OBJ_FILES),$$($(1)_LDFLAGS))
	@$$(POST_BUILD)

endif

ifeq ($$($(1)_TYPE),static)

$(1): $$(LIB_PATH)/$$(STAPRE)$(1)$$(STAEXT)

$$(LIB_PATH)/$$(STAPRE)$(1)$$(STAEXT): $$($(1)_OBJ_FILES)
	@$$(call MKDIR,$$(dir $$@))
	@$$(PRE_BUILD)
	@echo $$(AR) $$@
	@$$(ECHO_BLANK)
	@$$(call LINK.static,$$($(1)_OBJ_FILES),$$($(1)_LDFLAGS))
	@$$(POST_BUILD)

endif

$$(OBJ_PATH)/$(1)/%$$(OBJEXT): $$($(1)_DIR)/%.c
	@$$(call MKDIR,$$(dir $$@))
	@echo $$(CC) $$<
	@$$(call COMPILE.c,$$<)

-include $$(patsubst %$$(OBJEXT),%$$(DEPEXT),$$($(1)_OBJ_FILES))

endef