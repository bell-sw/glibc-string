# SPDX-License-Identifier: LGPL-2.1-or-later
# Copyright (c) 2021, BELLSOFT. All rights reserved.

LIB_NAME := libglibc-string
LIB_DIR  := lib
OBJ_DIR  := obj

CC := gcc
LD := gcc
AR := ar

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

CPU_FEATURES_INC := -I. -I./compat -I./elf -I./include \
	-I./sysdeps/x86 -I./sysdeps/x86/include -I./sysdeps/generic \
	-include stdbool.h -include sc_level_values.h -include glibc-cmp.h

CPU_FEATURES_SRC := sysdeps/x86/dl-get-cpu-features.c
CPU_FEATURES_OBJ := $(OBJ_DIR)/dl-get-cpu-features.o

STR_INC := -I. -I./compat -I./elf -I./include \
	-I./sysdeps/x86_64/multiarch \
	-I./sysdeps/x86_64 \
	-I./sysdeps/x86/include \
	-I./sysdeps/x86 \
	-I./sysdeps/generic \
	-include glibc-cmp.h

STR_DIR   := sysdeps/x86_64/multiarch

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

STR_SRC_A := $(filter-out $(SKIP), $(wildcard $(STR_DIR)/*.S))
STR_SRC_C := $(filter-out $(SKIP), $(wildcard $(STR_DIR)/*.c))
STR_OBJS  := $(STR_SRC_A:$(STR_DIR)/%.S=$(OBJ_DIR)/multiarch/%.os) \
	$(STR_SRC_C:$(STR_DIR)/%.c=$(OBJ_DIR)/multiarch/%.o)

all: $(CPU_FEATURES_OBJ) $(STR_OBJS) $(LIB_NAME)

$(CPU_FEATURES_OBJ): $(CPU_FEATURES_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPU_FEATURES_INC) -c -o $@ $<

$(OBJ_DIR)/multiarch/%.os: $(STR_DIR)/%.S
	@mkdir -p $(OBJ_DIR)/multiarch
	$(CC) $(CFLAGS) $(STR_INC) -c -o $@ $<

$(OBJ_DIR)/multiarch/%.o: $(STR_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/multiarch
	$(CC) $(CFLAGS) $(STR_INC) -c -o $@ $<

$(LIB_NAME): $(CPU_FEATURES_OBJ) $(STR_OBJS)
	@mkdir -p $(LIB_DIR)
	@rm -f $(LIB_DIR)/$(LIB_NAME).a
	$(AR) rc $(LIB_DIR)/$(LIB_NAME).a $(CPU_FEATURES_OBJ) $(STR_OBJS)

shared: $(CPU_FEATURES_OBJ) $(STR_OBJS)
	@mkdir -p $(LIB_DIR)
	@rm -f $(LIB_DIR)/$(LIB_NAME).so
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $(LIB_DIR)/$(LIB_NAME).so $(CPU_FEATURES_OBJ) $(STR_OBJS)

.PHONY: all clean

clean:
	rm -f $(CPU_FEATURES_OBJ) $(STR_OBJS)
