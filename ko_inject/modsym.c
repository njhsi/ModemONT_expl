// modsym.c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <elf.h>

int main(int argc, char **argv)
{
	int fd;
	char *mod;
	char *orig_func_name, *stub_func_name;
	unsigned int size, i, j, shn, n;
	Elf32_Sym *syms, *sym, *init, *hook, *dup;
	Elf32_Shdr *shdrs, *shdr;
	Elf32_Ehdr *ehdr;
	const char *strtab;

	init = hook = dup = NULL;
	orig_func_name = argv[2];
	stub_func_name = argv[3];

	fd = open(argv[1], O_RDWR);
	size = lseek(fd, 0L, SEEK_END);
	mod = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	ehdr = (Elf32_Ehdr *)mod;
	shdrs = (Elf32_Shdr *)(mod + ehdr->e_shoff);
	shn = ehdr->e_shnum == 0 ? shdrs[0].sh_size : ehdr->e_shnum;

	for (i = 0; i < shn; i++) {
		shdr = &shdrs[i];
		  printf("%d\n",shdr->sh_type);
		if (shdr->sh_type == SHT_SYMTAB || shdr->sh_type == SHT_DYNSYM)
		  {
		  printf("%d\n",shdr->sh_type);
			syms = (Elf32_Sym *)(mod + shdr->sh_offset);
			strtab = mod + shdrs[shdr->sh_link].sh_offset;
			n = shdr->sh_size / shdr->sh_entsize;
			for (j = 0; j < n; j++) {
				char stype;

				sym = &syms[j];
				stype = ELF32_ST_TYPE(sym->st_info);
				if (stype == STT_FUNC || stype == STT_NOTYPE) {
					if (!strcmp(strtab + sym->st_name, "init_module")) {
						init = sym;
					}
					if (!strcmp(strtab + sym->st_name, stub_func_name)) {
						hook = sym;
					}
				}
				if (stype == STT_NOTYPE) {
					if (!strcmp(strtab + sym->st_name, orig_func_name)) {
						dup = sym;
					}
				}
			}
			if (init && hook) {
				break;
			}
		}
	}
	if (init && hook) {
		if (dup) {
			// 清理掉已经无用的extern符号
			memcpy(dup, init, sizeof(Elf32_Sym));
		}
		printf("   @@@@@@@@ init func  :%s  %d  %d\n", strtab + init->st_name, ELF32_ST_BIND(init->st_info), STB_GLOBAL);
		init->st_value = hook->st_value;
	}
	munmap(mod, size);
}

