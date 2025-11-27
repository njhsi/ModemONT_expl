/*  
 *  getshell.c - The simplest kernel module to load shell script.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/kmod.h>
#include <linux/delay.h>


MODULE_LICENSE("GPL");

// in kernel: replace conntrack_mt_init with init_module before build
// must have 'void' or [-Werror=strict-prototypes]

//extern int conntrack_mt_init(void);
extern int __epon_module_init(void);

int __init getshell_init(void)
{
	char * argv[4];
	char * envp[4]; 
	argv[0] = "/bin/sh";
	argv[1] = "-c";
	argv[2] = "/mnt/jffs2/hw/getshell.sh";
	argv[3] = NULL;

	envp[0] = "HOME=/";
	envp[1] = "TERM=linux";
	envp[2] = "PATH=/sbin:/usr/sbin:/bin:/usr/bin";
	envp[3] = NULL;

	call_usermodehelper(argv[0], argv, envp, UMH_NO_WAIT);
	msleep(1000);
	printk(KERN_ERR "getshell: /mnt/jffs2/hw/getshell.sh script run successfully !.\n");

	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */
	return __epon_module_init();
}

//module_init(getshell_init);
//module_exit(getshell_exit);

