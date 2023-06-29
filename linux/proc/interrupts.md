```bash
cat /proc/interrupts
           CPU0       CPU1       CPU2       CPU3       
  0:        304          0          0          0   IO-APIC-edge      timer
  1:       1892          0          0          0   IO-APIC-edge      i8042
  6:          3          0          0          0   IO-APIC-edge      floppy
  8:          0          0          0          0   IO-APIC-edge      rtc0
  9:          0          0          0          0   IO-APIC-fasteoi   acpi
 10:          0          0          0          0   IO-APIC-fasteoi   ehci_hcd:usb2, uhci_hcd:usb4, uhci_hcd:usb5, uhci_hcd:usb7, uhci_hcd:usb8, uhci_hcd:usb10
 11:          1          0          0   14006570   IO-APIC-fasteoi   ehci_hcd:usb1, uhci_hcd:usb3, uhci_hcd:usb6, uhci_hcd:usb9, virtio3
 12:         15          0          0          0   IO-APIC-edge      i8042
 14:          0          0          0          0   IO-APIC-edge      ata_piix
 15:          0          0          0          0   IO-APIC-edge      ata_piix
 24:          0          0          0          0   PCI-MSI-edge      virtio0-config
 25:         13          0          0          0   PCI-MSI-edge      virtio0-virtqueues
 26:          0          0          0          0   PCI-MSI-edge      virtio1-config
 27:          0          0          0          0   PCI-MSI-edge      virtio4-config
 28:   13968851          0          0          0   PCI-MSI-edge      virtio1-virtqueues
 29:       4420          0   12627281          0   PCI-MSI-edge      virtio4-req.0
 30:          0          0          0          0   PCI-MSI-edge      virtio2-config
 31:          2          0          0          0   PCI-MSI-edge      virtio2-virtqueues
 32:          0          0          0          0   PCI-MSI-edge      virtio6-config
 33:  519757952          0          0          0   PCI-MSI-edge      virtio6-input.0
 34:       3079          0          0          0   PCI-MSI-edge      virtio6-output.0
 35:          0          0          0          0   PCI-MSI-edge      virtio5-config
 36:    2118159          0          0          0   PCI-MSI-edge      virtio5-req.0
 37:       5237          0          0          0   PCI-MSI-edge      snd_hda_intel:card0
NMI:          0          0          0          0   Non-maskable interrupts
LOC: 1982580867   44381254 1894808960 1428960540   Local timer interrupts
SPU:          0          0          0          0   Spurious interrupts
PMI:          0          0          0          0   Performance monitoring interrupts
IWI:  192884758   74568011   97620689   56598145   IRQ work interrupts
RTR:          0          0          0          0   APIC ICR read retries
RES: 1542116287 1010605382 1461332694  723367353   Rescheduling interrupts
CAL:    3432478    2570460    1350866    1900716   Function call interrupts
TLB:  163720688  136498837  158728814  125010169   TLB shootdowns
TRM:          0          0          0          0   Thermal event interrupts
THR:          0          0          0          0   Threshold APIC interrupts
DFR:          0          0          0          0   Deferred Error APIC interrupts
MCE:          0          0          0          0   Machine check exceptions
MCP:      51251      51251      51251      51251   Machine check polls
ERR:          0
MIS:          0
PIN:          0          0          0          0   Posted-interrupt notification event
NPI:          0          0          0          0   Nested posted-interrupt event
PIW:          0          0          0          0   Posted-interrupt wakeup event
```
第一列：IRQ 号，可以接着 cat /proc/irq/IRQ号/smp_affinity

CPU列：被中断的次数

第六列：中断的类型（例如 edge 或者 level ）

第七列：中断对应的设备/名称