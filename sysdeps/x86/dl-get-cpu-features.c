/* Initialize CPU feature data via IFUNC relocation.
   Copyright (C) 2015-2024 Free Software Foundation, Inc.

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

#include <ldsodefs.h>

#ifdef SHARED
# include <cpu-features.c>

/* NB: Normally, DL_PLATFORM_INIT calls init_cpu_features to initialize
   CPU features in dynamic executable.  But when loading ld.so inside of
   static executable, DL_PLATFORM_INIT isn't called and IFUNC relocation
   is used to call init_cpu_features.  In static executable, it is called
   once by IFUNC relocation.  In dynamic executable, it is called twice
   by DL_PLATFORM_INIT and by IFUNC relocation.  */

static void
_dl_x86_init_cpu_features (void)
{
  struct cpu_features *cpu_features = &GLRO(dl_x86_cpu_features);
  if (cpu_features->basic.kind == arch_kind_unknown)
    {
      init_cpu_features (cpu_features);
    }
}

#endif

attribute_hidden
const struct cpu_features *
__get_cpu_features (void)
{
  _dl_x86_init_cpu_features();

  return &GLRO(dl_x86_cpu_features);
}

extern int dprintf(int fd, const char *format, ...);
#define _dl_printf(...) dprintf(1, __VA_ARGS__)
#define _dl_diagnostics_cpu __print_cpu_diagnostics

static void
_dl_diagnostics_print_labeled_value (const char *label, uint64_t value)
{
  if (sizeof (value) == sizeof (unsigned long int))
    /* _dl_printf can print 64-bit values directly.  */
    _dl_printf ("%s=0x%lx\n", label, (unsigned long int) value);
  else
    {
      uint32_t high = value >> 32;
      uint32_t low = value;
      if (high == 0)
        _dl_printf ("%s=0x%x\n", label, low);
      else
        _dl_printf ("%s=0x%x%0*x\n", label, high, 8, low);
    }
}

static void
print_cpu_features_value (const char *label, uint64_t value)
{
  _dl_printf ("x86.cpu_features.");
  _dl_diagnostics_print_labeled_value (label, value);
}

static void
print_cpu_feature_internal (unsigned int index, const char *kind,
                            unsigned int reg, uint32_t value)
{
  _dl_printf ("x86.cpu_features.features[0x%x].%s[0x%x]=0x%x\n",
              index, kind, reg, value);
}

static void
print_cpu_feature_preferred (const char *label, unsigned int flag)
{
  _dl_printf("x86.cpu_features.preferred.%s=0x%x\n", label, flag);
}

attribute_hidden
void
_dl_diagnostics_cpu (void)
{
  const struct cpu_features *cpu_features = __get_cpu_features ();

  print_cpu_features_value ("basic.kind", cpu_features->basic.kind);
  print_cpu_features_value ("basic.max_cpuid", cpu_features->basic.max_cpuid);
  print_cpu_features_value ("basic.family", cpu_features->basic.family);
  print_cpu_features_value ("basic.model", cpu_features->basic.model);
  print_cpu_features_value ("basic.stepping", cpu_features->basic.stepping);

  for (unsigned int index = 0; index < CPUID_INDEX_MAX; ++index)
    {
      /* The index values are part of the ABI via
         <sys/platform/x86.h>, so translating them to strings is not
         necessary.  */
      for (unsigned int reg = 0; reg < 4; ++reg)
        print_cpu_feature_internal
          (index, "cpuid", reg,
           cpu_features->features[index].cpuid_array[reg]);
      for (unsigned int reg = 0; reg < 4; ++reg)
        print_cpu_feature_internal
          (index, "active", reg,
           cpu_features->features[index].active_array[reg]);
    }

  /* The preferred indicators are not part of the ABI and need to be
     translated.  */
#define BIT(x) \
  print_cpu_feature_preferred (#x, CPU_FEATURE_PREFERRED_P (cpu_features, x));
#include "cpu-features-preferred_feature_index_1.def"
#undef BIT

  print_cpu_features_value ("isa_1", cpu_features->isa_1);
  print_cpu_features_value ("xsave_state_size",
                            cpu_features->xsave_state_size);
  print_cpu_features_value ("xsave_state_full_size",
                            cpu_features->xsave_state_full_size);
  print_cpu_features_value ("data_cache_size", cpu_features->data_cache_size);
  print_cpu_features_value ("shared_cache_size",
                            cpu_features->shared_cache_size);
  print_cpu_features_value ("non_temporal_threshold",
                            cpu_features->non_temporal_threshold);
  print_cpu_features_value ("rep_movsb_threshold",
                            cpu_features->rep_movsb_threshold);
  print_cpu_features_value ("rep_movsb_stop_threshold",
                            cpu_features->rep_movsb_stop_threshold);
  print_cpu_features_value ("rep_stosb_threshold",
                            cpu_features->rep_stosb_threshold);
  print_cpu_features_value ("level1_icache_size",
                            cpu_features->level1_icache_size);
  print_cpu_features_value ("level1_icache_linesize",
                            cpu_features->level1_icache_linesize);
  print_cpu_features_value ("level1_dcache_size",
                            cpu_features->level1_dcache_size);
  print_cpu_features_value ("level1_dcache_assoc",
                            cpu_features->level1_dcache_assoc);
  print_cpu_features_value ("level1_dcache_linesize",
                            cpu_features->level1_dcache_linesize);
  print_cpu_features_value ("level2_cache_size",
                            cpu_features->level2_cache_size);
  print_cpu_features_value ("level2_cache_assoc",
                            cpu_features->level2_cache_assoc);
  print_cpu_features_value ("level2_cache_linesize",
                            cpu_features->level2_cache_linesize);
  print_cpu_features_value ("level3_cache_size",
                            cpu_features->level3_cache_size);
  print_cpu_features_value ("level3_cache_assoc",
                            cpu_features->level3_cache_assoc);
  print_cpu_features_value ("level3_cache_linesize",
                            cpu_features->level3_cache_linesize);
  print_cpu_features_value ("level4_cache_size",
                            cpu_features->level4_cache_size);
  print_cpu_features_value ("cachesize_non_temporal_divisor",
			    cpu_features->cachesize_non_temporal_divisor);
}
