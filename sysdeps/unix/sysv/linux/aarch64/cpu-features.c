/* Initialize CPU feature data.  AArch64 version.
   This file is part of the GNU C Library.
   Copyright (C) 2017-2023 Free Software Foundation, Inc.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include <cpu-features.h>
#include <sys/auxv.h>
#include <sys/prctl.h>
#include <sys/utsname.h>

#define DCZID_DZP_MASK (1 << 4)
#define DCZID_BS_MASK (0xf)

/* The maximal set of permitted tags that the MTE random tag generation
   instruction may use.  We exclude tag 0 because a) we want to reserve
   that for the libc heap structures and b) because it makes it easier
   to see when pointer have been correctly tagged.  */
#define MTE_ALLOWED_TAGS (0xfffe << PR_MTE_TAG_SHIFT)

#if HAVE_TUNABLES
struct cpu_list
{
  const char *name;
  uint64_t midr;
};

static struct cpu_list cpu_list[] = {
      {"thunderxt88",	 0x430F0A10},
      {"thunderx2t99",   0x431F0AF0},
      {"thunderx2t99p1", 0x420F5160},
      {"ares",		 0x411FD0C0},
      {"emag",		 0x503F0001},
      {"kunpeng920", 	 0x481FD010},
      {"a64fx",		 0x460F0010},
      {"generic", 	 0x0}
};

static uint64_t
get_midr_from_mcpu (const char *mcpu)
{
  for (int i = 0; i < sizeof (cpu_list) / sizeof (struct cpu_list); i++)
    if (strcmp (mcpu, cpu_list[i].name) == 0)
      return cpu_list[i].midr;

  return UINT64_MAX;
}
#endif

#if __LINUX_KERNEL_VERSION < 0x060200

/* Return true if we prefer using SVE in string ifuncs.  Old kernels disable
   SVE after every system call which results in unnecessary traps if memcpy
   uses SVE.  This is true for kernels between 4.15.0 and before 6.2.0, except
   for 5.14.0 which was patched.  For these versions return false to avoid using
   SVE ifuncs.
   Parse the kernel version into a 24-bit kernel.major.minor value without
   calling any library functions.  If uname() is not supported or if the version
   format is not recognized, assume the kernel is modern and return true.  */

static inline bool
prefer_sve_ifuncs (void)
{
  struct utsname buf;
  const char *p = &buf.release[0];
  int kernel = 0;
  int val;

  if (uname (&buf) < 0)
    return true;

  for (int shift = 16; shift >= 0; shift -= 8)
    {
      for (val = 0; *p >= '0' && *p <= '9'; p++)
	val = val * 10 + *p - '0';
      kernel |= (val & 255) << shift;
      if (*p++ != '.')
	break;
    }

  if (kernel >= 0x060200 || kernel == 0x050e00)
    return true;
  if (kernel >= 0x040f00)
    return false;
  return true;
}

#endif

static inline void
init_cpu_features (struct cpu_features *cpu_features)
{
  register uint64_t midr = UINT64_MAX;
  unsigned long _dl_hwcap = getauxval(AT_HWCAP);
  unsigned long _dl_hwcap2 = getauxval(AT_HWCAP2);

#if HAVE_TUNABLES
  /* Get the tunable override.  */
  const char *mcpu = TUNABLE_GET (glibc, cpu, name, const char *, NULL);
  if (mcpu != NULL)
    midr = get_midr_from_mcpu (mcpu);
#endif

  /* If there was no useful tunable override, query the MIDR if the kernel
     allows it.  */
  if (midr == UINT64_MAX)
    {
      if (GLRO (dl_hwcap) & HWCAP_CPUID)
	asm volatile ("mrs %0, midr_el1" : "=r"(midr));
      else
	midr = 0;
    }

  cpu_features->midr_el1 = midr;

  /* Check if ZVA is enabled.  */
  unsigned dczid;
  asm volatile ("mrs %0, dczid_el0" : "=r"(dczid));

  if ((dczid & DCZID_DZP_MASK) == 0)
    cpu_features->zva_size = 4 << (dczid & DCZID_BS_MASK);

  /* Check if BTI is supported.  */
  cpu_features->bti = GLRO (dl_hwcap2) & HWCAP2_BTI;

  /* Setup memory tagging support if the HW and kernel support it, and if
     the user has requested it.  */
  cpu_features->mte_state = 0;
  cpu_features->mte_enabled = GLRO(dl_hwcap2) & HWCAP2_MTE;

#ifdef USE_MTAG
# if HAVE_TUNABLES
  int mte_state = TUNABLE_GET (glibc, mem, tagging, unsigned, 0);
  cpu_features->mte_state = (GLRO (dl_hwcap2) & HWCAP2_MTE) ? mte_state : 0;
  /* If we lack the MTE feature, disable the tunable, since it will
     otherwise cause instructions that won't run on this CPU to be used.  */
  TUNABLE_SET (glibc, mem, tagging, cpu_features->mte_state);
# endif

  if (cpu_features->mte_state & 4)
    /* Enable choosing system-preferred faulting mode.  */
    __prctl (PR_SET_TAGGED_ADDR_CTRL,
	     (PR_TAGGED_ADDR_ENABLE | PR_MTE_TCF_SYNC | PR_MTE_TCF_ASYNC
	      | MTE_ALLOWED_TAGS),
	     0, 0, 0);
  else if (cpu_features->mte_state & 2)
    __prctl (PR_SET_TAGGED_ADDR_CTRL,
	     (PR_TAGGED_ADDR_ENABLE | PR_MTE_TCF_SYNC | MTE_ALLOWED_TAGS),
	     0, 0, 0);
  else if (cpu_features->mte_state)
    __prctl (PR_SET_TAGGED_ADDR_CTRL,
	     (PR_TAGGED_ADDR_ENABLE | PR_MTE_TCF_ASYNC | MTE_ALLOWED_TAGS),
	     0, 0, 0);
#endif

  /* Check if SVE is supported.  */
  cpu_features->sve = GLRO (dl_hwcap) & HWCAP_SVE;

  cpu_features->prefer_sve_ifuncs = cpu_features->sve;

#if __LINUX_KERNEL_VERSION < 0x060200
  if (cpu_features->sve)
    cpu_features->prefer_sve_ifuncs = prefer_sve_ifuncs ();
#endif

  /* Check if MOPS is supported.  */
  cpu_features->mops = GLRO (dl_hwcap2) & HWCAP2_MOPS;

  cpu_features->init_done = true;
}

attribute_hidden
struct cpu_features GLRO(dl_aarch64_cpu_features);

static void
_dl_aarch64_init_cpu_features (void)
{
  struct cpu_features *cpu_features = &GLRO(dl_aarch64_cpu_features);
  if (!cpu_features->init_done)
    {
      init_cpu_features (cpu_features);
    }
}

attribute_hidden
const struct cpu_features *
__get_cpu_features (void)
{
  _dl_aarch64_init_cpu_features();

  return &GLRO(dl_aarch64_cpu_features);
}

extern int dprintf(int fd, const char *format, ...);
#define printf(...) dprintf(1, __VA_ARGS__)

static void print_cpu(const char *name)
{
	printf("aarch64.cpu_features.name=\"%s\"\n", name);
}

attribute_hidden
void __print_cpu_diagnostics(void)
{
	const struct cpu_features *cf = __get_cpu_features();

	uint64_t midr = cf->midr_el1;

	if (IS_THUNDERX(midr))
		print_cpu("ThunderX");
	if (IS_THUNDERX2PA(midr))
		print_cpu("ThunderX2PA");
	if (IS_THUNDERX2(midr))
		print_cpu("ThunderX2");
	if (IS_NEOVERSE_N1(midr))
		print_cpu("Neoverse N1");
	if (IS_NEOVERSE_N2(midr))
		print_cpu("Neoverse N2");
	if (IS_NEOVERSE_V1(midr))
		print_cpu("Neoverse V1");
	if (IS_EMAG(midr))
		print_cpu("Ampere eMAG");
	if (IS_KUNPENG920(midr))
		print_cpu("Kunpeng 920");
	if (IS_A64FX(midr))
		print_cpu("A64FX");

	printf("aarch64.cpu_features.midr_el1=0x%lx\n", cf->midr_el1);
	printf("aarch64.cpu_features.zva_size=%u\n", cf->zva_size);
	printf("aarch64.cpu_features.bti=%d\n", cf->bti);
	printf("aarch64.cpu_features.mte_state=0x%x\n", cf->mte_state);
	printf("aarch64.cpu_features.mte_enabled=0x%x\n", cf->mte_enabled);
	printf("aarch64.cpu_features.sve=%d\n", cf->sve);
	printf("aarch64.cpu_features.prefer_sve_ifuncs=%d\n", cf->prefer_sve_ifuncs);
	printf("aarch64.cpu_features.mops=%d\n", cf->mops);
}
