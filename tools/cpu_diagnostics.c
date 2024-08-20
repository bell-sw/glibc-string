#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern void __print_cpu_diagnostics(void);

int main(void)
{
	__print_cpu_diagnostics();

	return 0;
}
