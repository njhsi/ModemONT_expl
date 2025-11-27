// stub.c
#include <linux/module.h>

//extern int conntrack_mt_init(void);
extern int hi_gpon_module_init();

int aaaa = 123;
module_param(aaaa, uint, 0444);
MODULE_PARM_DESC(aaaa, "......");

int __init test_stub(void)
{
	printk("i am a stub, and my value is %d\n", aaaa);
	return hi_gpon_module_init();
}

MODULE_LICENSE("GPL");

