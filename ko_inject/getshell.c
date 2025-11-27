/*  
 *  getshell.c - The simplest kernel module to load shell script.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/kmod.h>
#include <linux/delay.h>

#define DRV_NAME "getshell"
#define DRV_VERSION "2.0"
#define DRV_COPYRIGHT "compiled by 0neday using 4.4.197 kernel, 2021/06/26"
#define DRV_DESCRIPTION "persist root shell realized by kernel module"


MODULE_DESCRIPTION(DRV_DESCRIPTION);
MODULE_AUTHOR(DRV_COPYRIGHT);
MODULE_LICENSE("GPL");

//int getshell_init(void)
int test_stub(void)
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
	msleep(3000);
	printk(KERN_ERR "getshell script run successfully !.\n");

	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */
	return 0;
}

//extern int conntrack_mt_init(void);
extern int hi_gpon_module_init(void);

int aaaa = 123;
module_param(aaaa, uint, 0444);
MODULE_PARM_DESC(aaaa, "......");

int __init test_stub_(void)
{
	printk("i am a stub, and my value is %d\n", aaaa);
		return hi_gpon_module_init();
		//return conntrack_mt_init();
}

MODULE_LICENSE("GPL");

