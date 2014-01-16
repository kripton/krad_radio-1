include $(top_srcdir)/common.mk

codegen_bootstrap_CPPFLAGS = 
codegen_bootstrap_CPPFLAGS += '-I../'

codegen_cfg_CPPFLAGS = '-I../analysis/gather_structs/'
codegen_cfg_CPPFLAGS += '-Ibootstrap/'
codegen_cfg_CPPFLAGS += '-I../'
codegen_auto_CPPFLAGS = ${codegen_cfg_CPPFLAGS}

noinst_PROGRAMS = codegen_pre codegen_cfg codegen_auto

codegen_pre$(EXEEXT): ; test -s bootstrap/bootstrapped.c && test -s bootstrap/bootstrapped.h || ./bootstrap/codegen_bootstrap ../../../lib "" "" bootstrap/

codegen_auto_SOURCES = ../analysis/gather_structs/structs_gathering.c \
  codegen_utils.c \
  formats/codegen_text.c \
  formats/codegen_json.c \
  formats/codegen_dejson.c \
  formats/codegen_ebml.c \
  formats/codegen_helper.c \
  formats/codegen_sizeof.c \
  formats/codegen_jschema.c \
  bootstrap/codegen_bootstrap_utils.c \
  bootstrap/bootstrapped.c \
  codegen_auto.c 

codegen_cfg_SOURCES = ../analysis/gather_structs/structs_gathering.c \
  codegen_cfg.c
