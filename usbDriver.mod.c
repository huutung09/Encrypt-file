#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
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

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x8d4844b8, "usb_deregister_dev" },
	{ 0x21d81531, "usb_deregister" },
	{ 0x40fe35a3, "usb_register_dev" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0xad6d045f, "kmalloc_caches" },
	{ 0x850e6a88, "kmalloc_trace" },
	{ 0x88139c8d, "filp_open" },
	{ 0xc9778d0d, "vfs_llseek" },
	{ 0x469dcca3, "kernel_read" },
	{ 0x6c083267, "crypto_alloc_base" },
	{ 0xb7d00b90, "crypto_cipher_setkey" },
	{ 0xad970318, "crypto_cipher_decrypt_one" },
	{ 0xbf957606, "filp_close" },
	{ 0xa916b694, "strnlen" },
	{ 0x979d2877, "kernel_write" },
	{ 0x3fae7a0b, "crypto_destroy_tfm" },
	{ 0x37a0cba, "kfree" },
	{ 0x63960650, "crypto_cipher_encrypt_one" },
	{ 0x7682ba4e, "__copy_overflow" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xa19b956, "__stack_chk_fail" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x914ccc00, "usb_register_driver" },
	{ 0x122c3a7e, "_printk" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x453e7dc, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("usb:v0483p3748d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "615DA5024AD20F685C94F90");
