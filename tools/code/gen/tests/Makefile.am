include $(top_srcdir)/common.mk

sizeof_test_CPPFLAGS = '-I../../analysis/gather_structs/'
sizeof_test_CPPFLAGS += '-I../'
sizeof_test_CPPFLAGS += '-I../bootstrap'
sizeof_test_CPPFLAGS += '-I../../'
jschema_test_CPPFLAGS = ${sizeof_test_CPPFLAGS}

noinst_PROGRAMS = sizeof_test jschema_test

sizeof_test_SOURCES = ../../analysis/gather_structs/structs_gathering.c \
  sizeof_test.c \
  ../codegen_utils.c \
  ../formats/codegen_text.c \
  ../formats/codegen_json.c \
  ../formats/codegen_dejson.c \
  ../formats/codegen_ebml.c \
  ../formats/codegen_helper.c \
  ../formats/codegen_sizeof.c \
  ../formats/codegen_jschema.c \
  ../bootstrap/codegen_bootstrap_utils.c \
  ../bootstrap/bootstrapped.c

jschema_test_SOURCES = ../../analysis/gather_structs/structs_gathering.c \
  jschema_test.c \
  ../codegen_utils.c \
  ../formats/codegen_text.c \
  ../formats/codegen_json.c \
  ../formats/codegen_dejson.c \
  ../formats/codegen_ebml.c \
  ../formats/codegen_helper.c \
  ../formats/codegen_sizeof.c \
  ../formats/codegen_jschema.c \
  ../bootstrap/codegen_bootstrap_utils.c \
  ../bootstrap/bootstrapped.c