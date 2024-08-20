# SPDX-License-Identifier: LGPL-2.1-or-later
# Copyright (c) 2021-2024, BELLSOFT. All rights reserved.

LIB_NAME := libglibc-string
LIB_DIR  := lib
OBJ_DIR  := obj

CC := gcc
LD := gcc
AR := ar

ARCH=$(shell $(CC) -dumpmachine | cut -d- -f1)

CFLAGS := -Wall -Werror -g -O2 -fPIC \
	-ffreestanding -fno-builtin -fno-tree-loop-distribute-patterns \
	-fno-unwind-tables -fno-asynchronous-unwind-tables \
	-ffunction-sections -fdata-sections \
	-std=gnu99 -DSHARED -U_FORTIFY_SOURCE \
	-fcf-protection=none \
	-Wa,--noexecstack \
	-DHAVE_GCC_IFUNC \
	-DHAVE_CC_NO_STACK_PROTECTOR -DPIC \
	$(CFLAGS)

CPU_FEATURES_INC := -I. -I./compat -I./elf -I./include
STR_INC := -I. -I./compat -I./elf -I./include
CPU_DIAGNOSTICS := tools/cpu_diagnostics

ifeq ($(ARCH),x86_64)
$(info Building: x86_64)
STR_INC += \
	-I./sysdeps/x86_64/multiarch \
	-I./sysdeps/x86_64 \
	-I./sysdeps/x86/include \
	-I./sysdeps/x86 \

STR_DIR := sysdeps/x86_64/multiarch
STR_SINGLE_OBJS :=

SKIP := $(STR_DIR)/memmove-vec-unaligned-erms.S \
	$(STR_DIR)/memset-vec-unaligned-erms.S \
	$(STR_DIR)/memchr-evex-base.S \
	$(STR_DIR)/strchr-evex-base.S \
	$(STR_DIR)/strlen-evex-base.S \
	$(STR_DIR)/strrchr-evex-base.S \
	$(STR_DIR)/strcat-strlen-avx2.h.S \
	$(STR_DIR)/strcat-strlen-evex.h.S \
	$(STR_DIR)/strcmp-sse42.S \
	$(STR_DIR)/strlen-vec.S

# Not used by any processor at the moment
SKIP += $(STR_DIR)/memchr-evex512.S \
	$(STR_DIR)/strchr-evex512.S \
	$(STR_DIR)/strchrnul-evex512.S \
	$(STR_DIR)/strlen-evex512.S \
	$(STR_DIR)/strnlen-evex512.S \
	$(STR_DIR)/strrchr-evex512.S \
	$(STR_DIR)/wcslen-evex512.S

CPU_FEATURES_INC += -I./sysdeps/x86 -I./sysdeps/x86/include
CPU_FEATURES_SRC := sysdeps/x86/dl-get-cpu-features.c
CPU_FEATURES_OBJ := $(OBJ_DIR)/dl-get-cpu-features.o


else ifeq ($(ARCH),aarch64)
$(info Building: aarch64)

STR_INC += \
	-I./sysdeps/aarch64/multiarch \
	-I./sysdeps/aarch64 \
	-I./sysdeps/unix/sysv/linux/aarch64

STR_DIR := sysdeps/aarch64/multiarch
STR_SINGLE_OBJS := \
	$(OBJ_DIR)/single/memcmp.os \
	$(OBJ_DIR)/single/memrchr.os \
	$(OBJ_DIR)/single/stpcpy.os \
	$(OBJ_DIR)/single/strchrnul.os \
	$(OBJ_DIR)/single/strchr.os \
	$(OBJ_DIR)/single/strcmp.os \
	$(OBJ_DIR)/single/strcpy.os \
	$(OBJ_DIR)/single/strncmp.os \
	$(OBJ_DIR)/single/strnlen.os \
	$(OBJ_DIR)/single/strrchr.os

CPU_FEATURES_INC += -I./sysdeps/unix/sysv/linux/aarch64
CPU_FEATURES_SRC := sysdeps/unix/sysv/linux/aarch64/cpu-features.c
CPU_FEATURES_OBJ := $(OBJ_DIR)/cpu-features.o

else
$(error Unsupported arch: $(ARCH))

endif

CPU_FEATURES_INC += -I./sysdeps/generic -include stdbool.h -include sc_level_values.h -include glibc-cmp.h
STR_INC += -I./sysdeps/generic -include glibc-cmp.h

STR_SRC_A := $(filter-out $(SKIP), $(wildcard $(STR_DIR)/*.S))
STR_SRC_C := $(filter-out $(SKIP), $(wildcard $(STR_DIR)/*.c))
STR_OBJS  := $(STR_SRC_A:$(STR_DIR)/%.S=$(OBJ_DIR)/multiarch/%.os) \
	$(STR_SRC_C:$(STR_DIR)/%.c=$(OBJ_DIR)/multiarch/%.o)

all: $(CPU_FEATURES_OBJ) $(STR_OBJS) $(STR_SINGLE_OBJS) $(LIB_NAME)

$(CPU_FEATURES_OBJ): $(CPU_FEATURES_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPU_FEATURES_INC) -c -o $@ $<

$(OBJ_DIR)/multiarch/%.os: $(STR_DIR)/%.S
	@mkdir -p $(OBJ_DIR)/multiarch
	$(CC) $(CFLAGS) $(STR_INC) -c -o $@ $<

$(OBJ_DIR)/single/%.os: $(STR_DIR)/../%.S
	@mkdir -p $(OBJ_DIR)/single
	$(CC) $(CFLAGS) $(STR_INC) -c -o $@ $<

$(OBJ_DIR)/multiarch/%.o: $(STR_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/multiarch
	$(CC) $(CFLAGS) $(STR_INC) -c -o $@ $<

$(LIB_NAME): $(CPU_FEATURES_OBJ) $(CPU_DIAG_OBJ) $(STR_OBJS) $(STR_SINGLE_OBJS)
	@mkdir -p $(LIB_DIR)
	@rm -f $(LIB_DIR)/$(LIB_NAME).a
	$(AR) rc $(LIB_DIR)/$(LIB_NAME).a $(CPU_FEATURES_OBJ) $(STR_OBJS) $(STR_SINGLE_OBJS)

shared: $(CPU_FEATURES_OBJ) $(STR_OBJS)
	@mkdir -p $(LIB_DIR)
	@rm -f $(LIB_DIR)/$(LIB_NAME).so
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $(LIB_DIR)/$(LIB_NAME).so $(CPU_FEATURES_OBJ) $(STR_OBJS) $(STR_SINGLE_OBJS)

tools: $(LIB_NAME)
	$(CC) -Os $(CPU_FEATURES_INC) -o $(CPU_DIAGNOSTICS) $(CPU_DIAGNOSTICS).c -L $(LIB_DIR) -l:$(LIB_NAME).a

.PHONY: all clean

clean:
	rm -f $(CPU_FEATURES_OBJ) $(STR_OBJS) $(STR_SINGLE_OBJS) $(CPU_DIAGNOSTICS)
