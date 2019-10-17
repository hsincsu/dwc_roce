#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
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
__used
__attribute__((section("__versions"))) = {
	{ 0xe3ff436c, "module_layout" },
	{ 0xec6f6532, "alloc_pages_current" },
	{ 0x3ce4ca6f, "disable_irq" },
	{ 0x2d3385d3, "system_wq" },
	{ 0xb34c7af, "netdev_info" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0x6fddb248, "__put_devmap_managed_page" },
	{ 0x349cba85, "strchr" },
	{ 0x48d41cea, "pcim_enable_device" },
	{ 0x245b17c0, "param_ops_int" },
	{ 0x7b243c3, "napi_disable" },
	{ 0xa10b0ee1, "napi_schedule_prep" },
	{ 0x6d37417, "__napi_schedule_irqoff" },
	{ 0x27bbf221, "disable_irq_nosync" },
	{ 0x5b20db14, "pcim_iomap_table" },
	{ 0xeedf217a, "dma_set_mask" },
	{ 0xc7a4fbed, "rtnl_lock" },
	{ 0x4ea25709, "dql_reset" },
	{ 0xc0a3d105, "find_next_bit" },
	{ 0x8366534b, "dma_direct_sync_single_for_cpu" },
	{ 0xc4cc2419, "__dev_kfree_skb_any" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x9a5ee1f7, "dma_free_attrs" },
	{ 0x274dc2b, "netif_get_num_default_rss_queues" },
	{ 0x51d52ff8, "dma_set_coherent_mask" },
	{ 0x2cd02f8, "netdev_alert" },
	{ 0x439f389a, "netif_napi_del" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xb49e5bcc, "__dynamic_netdev_dbg" },
	{ 0x17de3d5, "nr_cpu_ids" },
	{ 0xf1513640, "pci_set_master" },
	{ 0xbb5e999c, "netif_schedule_queue" },
	{ 0x97934ecf, "del_timer_sync" },
	{ 0x7e526bfa, "__x86_indirect_thunk_r10" },
	{ 0xef4a9b17, "netif_tx_wake_queue" },
	{ 0x71abfecb, "netif_tx_stop_all_queues" },
	{ 0x9a76f11f, "__mutex_init" },
	{ 0xfc0ae855, "ethtool_op_get_link" },
	{ 0xe1537255, "__list_del_entry_valid" },
	{ 0x5a5a2271, "__cpu_online_mask" },
	{ 0xa00aca2a, "dql_completed" },
	{ 0x4c9d28b0, "phys_base" },
	{ 0x9451d403, "free_netdev" },
	{ 0x11247515, "register_netdev" },
	{ 0xdae7d56a, "dma_direct_map_page" },
	{ 0x5792f848, "strlcpy" },
	{ 0x5dabb8c5, "dma_alloc_attrs" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0x69dd3b5b, "crc32_le" },
	{ 0xf1969a8e, "__usecs_to_jiffies" },
	{ 0xd5c08736, "netif_set_real_num_rx_queues" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0x27a09531, "netif_set_real_num_tx_queues" },
	{ 0x17240583, "netif_napi_add" },
	{ 0x37893ee3, "_dev_err" },
	{ 0x78041b8f, "byte_rev_table" },
	{ 0x68f31cbd, "__list_add_valid" },
	{ 0xb358e20e, "pcim_iomap_regions" },
	{ 0xe9b2c53d, "dma_direct_unmap_page" },
	{ 0xef9417b, "napi_gro_receive" },
	{ 0xb601be4c, "__x86_indirect_thunk_rdx" },
	{ 0x12a38747, "usleep_range" },
	{ 0xfe916dc6, "hex_dump_to_buffer" },
	{ 0x2c7a0206, "__napi_schedule" },
	{ 0x19d15bf0, "devm_free_irq" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0x7bc8fd05, "napi_complete_done" },
	{ 0x2ea2c95c, "__x86_indirect_thunk_rax" },
	{ 0x9a664a1b, "eth_type_trans" },
	{ 0xd8b727b3, "pskb_expand_head" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xcb59654e, "netdev_err" },
	{ 0x467df16d, "netdev_rss_key_fill" },
	{ 0xaad90e1b, "pci_unregister_driver" },
	{ 0xfc9ab21d, "netdev_warn" },
	{ 0x61a2b841, "eth_validate_addr" },
	{ 0xfcec0987, "enable_irq" },
	{ 0x37a0cba, "kfree" },
	{ 0x69acdf38, "memcpy" },
	{ 0x84f7f09d, "skb_add_rx_frag" },
	{ 0xff018061, "__pci_register_driver" },
	{ 0xb352177e, "find_first_bit" },
	{ 0x63c4d61f, "__bitmap_weight" },
	{ 0xc3ab3ef0, "unregister_netdev" },
	{ 0x2e0d2f7f, "queue_work_on" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x60083689, "consume_skb" },
	{ 0x4fd92be3, "__napi_alloc_skb" },
	{ 0xb0bce6f1, "skb_put" },
	{ 0xfd113687, "devm_request_threaded_irq" },
	{ 0x6e720ff2, "rtnl_unlock" },
	{ 0x331662a0, "dma_ops" },
	{ 0xe23a9d4c, "__put_page" },
	{ 0x587f22d7, "devmap_managed_key" },
	{ 0x2ea47863, "alloc_etherdev_mqs" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("pci:v000016CAd00007312sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "6660FBF8EE3F2E8B0903477");
