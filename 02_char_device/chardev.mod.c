#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xe8213e80, "_printk" },
	{ 0x96370a3b, "__register_chrdev" },
	{ 0x52b15b3b, "__unregister_chrdev" },
	{ 0x5cb46e6d, "validate_usercopy_range" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0xaa47b76e, "__arch_copy_from_user" },
	{ 0x0e9cab28, "memset" },
	{ 0x9c4ed43a, "alt_cb_patch_nops" },
	{ 0xf64ac983, "__copy_overflow" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0xe54e0a6b, "__fortify_panic" },
	{ 0xaa47b76e, "__arch_copy_to_user" },
	{ 0x9b3f403c, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xe8213e80,
	0x96370a3b,
	0x52b15b3b,
	0x5cb46e6d,
	0xa61fd7aa,
	0xaa47b76e,
	0x0e9cab28,
	0x9c4ed43a,
	0xf64ac983,
	0x90a48d82,
	0xe54e0a6b,
	0xaa47b76e,
	0x9b3f403c,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"_printk\0"
	"__register_chrdev\0"
	"__unregister_chrdev\0"
	"validate_usercopy_range\0"
	"__check_object_size\0"
	"__arch_copy_from_user\0"
	"memset\0"
	"alt_cb_patch_nops\0"
	"__copy_overflow\0"
	"__ubsan_handle_out_of_bounds\0"
	"__fortify_panic\0"
	"__arch_copy_to_user\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "173E1C006E5EC9DC24AD94C");
