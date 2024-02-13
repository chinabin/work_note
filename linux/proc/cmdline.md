# 0x00. 导读

# 0x01. 简介

Arguments passed to the Linux kernel at boot time.  Often done via a boot manager such as lilo(8) or grub(8).  Any arguments embedded in the kernel image or initramfs via CONFIG_BOOT_CONFIG will also be displayed.

# 0x02. 

check [启动引导.md](../system/启动引导.md)

```bash
$ cat /proc/cmdline
BOOT_IMAGE=/vmlinuz-3.10.0-1160.6.1.el7.x86_64 root=/dev/mapper/centos-root ro crashkernel=auto rhgb quiet intel_idle.max_cstate=0 processor.max_cstate=0 idle=poll nohz=on mce=ignore_ce nmi_watchdog=0 audit=0 nosoftlockup pcie_aspm=performance intel_pstate=disable transparent_hugepage=never selinux=0 clocksource=tsc rcu_nocb_poll acpi_irq_nobalance ipv6.disable=1 pcie_port_pm=off ipmi_si.force_kipmid=0 tsc=reliable iommu=off intel_iommu=off mitigations=off spectre_v2=off skew_tick=1 nohz=on nohz_full=2-8 rcu_nocbs=2-8 tuned.non_isolcpus=0003fe03 intel_pstate=disable nosoftlockup
```