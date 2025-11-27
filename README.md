# ModemONT_expl
research on some ONT Modem

thanks to https://github.com/0neday/OptiXstarHS8145X6
and .. csdddn

external ko built without proper .config and modsyms would not certainly working, as 0neday found out and gave up.

alternately, let's try to inject our homework into a ko of the modem, especially a ko in the writable directory such as h!_gpon.ko in /mnt/jffs2.

~$ objdump  -t /lib/modules/h!_gpon.ko  | grep init
000046ec g     F .text	0000004c h!_gpon_module_init

now use 0neday's method to prepare the kernel building, but.. substitte "conntrack_mt_init" to  "h!_gpon_module_init" before the make step

in 0neday's getshell folder, remove module_init/exit, added 'extern h!_gpon_module_init()' and call h!_gpon_module_init in the getshell_init if you like. and make this getshell.ko as stub.

finally, do the inject to get the new h!_gpon.so
~$ arm-none-eabi-ld -r h!_gpon.ko getshell.ko -o h!_gpon.ko_new # LINK
~$ ./modsym h!_gpon.ko_new h!_gpon_module_init getshell_init    # MODIFY/inject

