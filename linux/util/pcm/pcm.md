# 0x00. 导读

# 0x01. 简介

# 0x02. 

# 0x03. pcm



```bash
$ sudo ./pcm 

 Intel(r) Performance Counter Monitor ($Format:%ci ID=%h$)



=====  Processor information  =====
Linux arch_perfmon flag  : yes
Hybrid processor         : no
IBRS and IBPB supported  : yes
STIBP supported          : yes
Spec arch caps supported : yes
Max CPUID level          : 22
CPU family               : 6
CPU model number         : 85
Number of physical cores: 56
Number of logical cores: 56
Number of online logical cores: 56
Threads (logical cores) per physical core: 1
Num sockets: 2
Physical cores per socket: 28
Last level cache slices per socket: 28
Core PMU (perfmon) version: 4
Number of core PMU generic (programmable) counters: 8
Width of generic (programmable) counters: 48 bits
Number of core PMU fixed counters: 3
Width of fixed counters: 48 bits
Nominal core frequency: 2700000000 Hz
IBRS enabled in the kernel   : no
STIBP enabled in the kernel  : no
The processor is not susceptible to Rogue Data Cache Load: yes
The processor supports enhanced IBRS                     : yes
Package thermal spec power: 205 Watt; Package minimum power: 72 Watt; Package maximum power: 413 Watt;

INFO: Linux perf interface to program uncore PMUs is present
Socket 0: 2 memory controllers detected with total number of 6 channels. 3 UPI ports detected. 2 M2M (mesh to memory)/B2CMI blocks detected. 0 HBM M2M blocks detected. 0 EDC/HBM channels detected. 0 Home Agents detected. 3 M3UPI/B2UPI blocks detected.
Socket 1: 2 memory controllers detected with total number of 6 channels. 3 UPI ports detected. 2 M2M (mesh to memory)/B2CMI blocks detected. 0 HBM M2M blocks detected. 0 EDC/HBM channels detected. 0 Home Agents detected. 3 M3UPI/B2UPI blocks detected.
Socket 0: 1 PCU units detected. 6 IIO units detected. 6 IRP units detected. 28 CHA/CBO units detected. 0 MDF units detected. 1 UBOX units detected. 0 CXL units detected. 0 PCIE_GEN5x16 units detected. 0 PCIE_GEN5x8 units detected.
Socket 1: 1 PCU units detected. 6 IIO units detected. 6 IRP units detected. 28 CHA/CBO units detected. 0 MDF units detected. 1 UBOX units detected. 0 CXL units detected. 0 PCIE_GEN5x16 units detected. 0 PCIE_GEN5x8 units detected.
Initializing RMIDs

 Disabling NMI watchdog since it consumes one hw-PMU counter. To keep NMI watchdog set environment variable PCM_KEEP_NMI_WATCHDOG=1 (this reduces the core metrics set)
 Closed perf event handles
Trying to use Linux perf events...
Successfully programmed on-core PMU using Linux perf
Link 3 is disabled
Link 3 is disabled
Socket 0
Max UPI link 0 speed: 23.3 GBytes/second (10.4 GT/second)
Max UPI link 1 speed: 23.3 GBytes/second (10.4 GT/second)
Socket 1
Max UPI link 0 speed: 23.3 GBytes/second (10.4 GT/second)
Max UPI link 1 speed: 23.3 GBytes/second (10.4 GT/second)

Detected Intel(R) Xeon(R) Gold 6258R CPU @ 2.70GHz "Intel(r) microarchitecture codename Cascade Lake-SP" stepping 7 microcode level 0x5003303

 UTIL  : utlization (same as core C0 state active state residency, the value is in 0..1) 
 IPC   : instructions per CPU cycle
 CFREQ : core frequency in Ghz
 L3MISS: L3 (read) cache misses 
 L2MISS: L2 (read) cache misses (including other core's L2 cache *hits*) 
 L3HIT : L3 (read) cache hit ratio (0.00-1.00)
 L2HIT : L2 cache hit ratio (0.00-1.00)
 L3MPI : number of L3 (read) cache misses per instruction
 L2MPI : number of L2 (read) cache misses per instruction
 READ  : bytes read from main memory controller (in GBytes)
 WRITE : bytes written to main memory controller (in GBytes)
 LOCAL : ratio of local memory requests to memory controller in %
LLCRDMISSLAT: average latency of last level cache miss for reads and prefetches (in ns)
 PMM RD : bytes read from PMM memory (in GBytes)
 PMM WR : bytes written to PMM memory (in GBytes)
 L3OCC : L3 occupancy (in KBytes)
 LMB   : L3 cache external bandwidth satisfied by local memory (in MBytes)
 RMB   : L3 cache external bandwidth satisfied by remote memory (in MBytes)
 TEMP  : Temperature reading in 1 degree Celsius relative to the TjMax temperature (thermal headroom): 0 corresponds to the max temperature
 energy: Energy in Joules


 Core (SKT) | UTIL | IPC  | CFREQ | L3MISS | L2MISS | L3HIT | L2HIT | L3MPI | L2MPI |   L3OCC |   LMB  |   RMB  | TEMP

   0    0     0.60   0.67    3.40      18 K     89 K    0.75    0.98  0.0000  0.0001     1008        0        1     38
   1    1     0.00   0.67    3.40      11 K     93 K    0.87    0.29  0.0017  0.0143        0        0        0     40
   2    0     0.00   0.57    3.40    1177     9269      0.85    0.29  0.0018  0.0141        0        0        0     41
   3    1     0.00   0.38    3.40     223     1152      0.78    0.38  0.0015  0.0076      336        0        0     41
   4    0     0.00   0.33    3.40    1823     5712      0.56    0.46  0.0025  0.0077      224        0        0     41
   5    1     0.00   1.44    3.40    2948       11 K    0.74    0.65  0.0003  0.0013     5488        4        4     39
   6    0     1.00   3.34    3.40    4213       17 K    0.58    0.36  0.0000  0.0000      224        0        0     34
   7    1     0.00   1.26    3.40    1567       10 K    0.84    0.31  0.0004  0.0026        0        0        0     40
   8    0     0.00   0.30    3.40     986     4336      0.64    0.39  0.0018  0.0077        0        0        0     45
   9    1     0.03   0.60    3.40    9490       29 K    0.31    0.92  0.0001  0.0004      112        0        0     40
  10    0     0.00   0.40    3.40     208     2586      0.90    0.33  0.0009  0.0107        0        0        0     42
  11    1     0.00   0.71    3.40     205     1413      0.84    0.31  0.0006  0.0040        0        0        0     40
  12    0     1.00   2.25    3.40     658     9420      0.92    0.51  0.0000  0.0000        0        0        0     36
  13    1     1.00   1.03    3.40      13 K     16 K    0.06    0.99  0.0000  0.0000        0        0        0     32
  14    0     1.00   1.34    3.40    4810       17 K    0.50    0.29  0.0000  0.0000        0        0        0     36
  15    1     0.00   0.45    3.40     496     3113      0.82    0.30  0.0020  0.0127        0        0        0     40
  16    0     0.00   0.29    3.40     505     2068      0.69    0.41  0.0025  0.0102      112        0        0     45
  17    1     0.00   0.36    3.40      86      581      0.84    0.42  0.0011  0.0075        0        0        0     41
  18    0     0.04   0.59    3.40    1805       14 K    0.85    0.93  0.0000  0.0002      224        0        1     43
  19    1     0.00   1.29    3.40    1931     9389      0.76    0.85  0.0002  0.0007      112        0        0     41
  20    0     0.00   0.36    3.40     152     1581      0.88    0.38  0.0010  0.0104        0        0        0     42
  21    1     0.00   0.59    3.40    1398     8574      0.82    0.30  0.0022  0.0136        0        0        0     40
  22    0     0.00   0.70    3.40    2531       53 K    0.94    0.32  0.0005  0.0111        0        0        0     45
  23    1     0.00   0.35    3.40      96      574      0.82    0.43  0.0012  0.0074        0        0        0     41
  24    0     0.03   0.59    3.41    1883       13 K    0.83    0.89  0.0000  0.0002        0        0        0     43
  25    1     0.00   0.64    3.40    2377       14 K    0.82    0.33  0.0021  0.0125        0        0        0     39
  26    0     0.06   0.59    3.40    3791       25 K    0.81    0.93  0.0000  0.0002      448        0        2     42
  27    1     0.00   0.50    3.40     402     2575      0.83    0.47  0.0015  0.0096        0        0        0     40
  28    0     0.02   0.59    3.40    5430       40 K    0.84    0.68  0.0002  0.0011        0        0        0     44
  29    1     0.00   0.57    3.40    1403     7355      0.80    0.53  0.0017  0.0087        0        0        0     41
  30    0     1.00   1.34    3.40     839       21 K    0.90    0.32  0.0000  0.0000        0        0        0     36
  31    1     0.00   0.45    3.40     337     2196      0.83    0.41  0.0017  0.0108        0        0        0     40
  32    0     0.06   0.59    3.40    4866       28 K    0.77    0.92  0.0000  0.0002      112        0        0     43
  33    1     0.01   1.36    3.40    6281       26 K    0.73    0.85  0.0001  0.0006      224        1        0     40
  34    0     0.01   0.56    3.41    2210       17 K    0.86    0.69  0.0001  0.0011      112        0        0     42
  35    1     0.00   0.62    3.40    1381     9499      0.84    0.27  0.0020  0.0139        0        0        0     41
  36    0     0.05   0.60    3.40    4058       26 K    0.82    0.92  0.0000  0.0003      112        0        0     42
  37    1     0.00   0.54    3.40     930     6067      0.83    0.28  0.0020  0.0130        0        0        0     40
  38    0     0.01   1.25    3.40    8470       27 K    0.66    0.69  0.0004  0.0012      336        0        0     43
  39    1     0.00   0.65    3.40    1858       14 K    0.86    0.28  0.0017  0.0135        0        0        0     40
  40    0     0.00   0.61    3.40     996     8522      0.86    0.70  0.0001  0.0009        0        0        0     44
  41    1     0.00   0.55    3.40    1485       10 K    0.84    0.27  0.0021  0.0147        0        0        0     40
  42    0     0.01   0.59    3.41    1120       10 K    0.87    0.81  0.0001  0.0005        0        0        0     42
  43    1     0.00   0.55    3.40    2345     7640      0.66    0.40  0.0022  0.0072        0        0        0     42
  44    0     0.00   0.51    3.40     790     6850      0.87    0.28  0.0015  0.0133        0        0        0     46
  45    1     1.00   3.63    3.40    4921       17 K    0.67    0.79  0.0000  0.0000        0        0        0     28
  46    0     0.00   0.35    3.40     202     1727      0.86    0.32  0.0011  0.0090        0        0        0     44
  47    1     1.00   1.36    3.40     860       11 K    0.87    0.61  0.0000  0.0000        0        0        0     29
  48    0     0.00   0.79    3.40    4515     8258      0.38    0.69  0.0014  0.0026        0        0        0     44
  49    1     1.00   1.09    3.40     796     9193      0.91    0.40  0.0000  0.0000        0        0        0     31
  50    0     0.00   0.40    3.40     110     1149      0.88    0.33  0.0009  0.0094        0        0        0     42
  51    1     1.00   3.38    3.40    6148       36 K    0.82    0.24  0.0000  0.0000      224        1        1     30
  52    0     0.00   0.47    3.40     108     1171      0.89    0.34  0.0008  0.0083        0        0        0     43
  53    1     1.00   3.36    3.40    1408       14 K    0.89    0.35  0.0000  0.0000        0        0        0     30
  54    0     0.00   0.47    3.40    2302     5335      0.54    0.53  0.0016  0.0038    35056        2        0     43
  55    1     0.00   0.39    3.40    1361     4911      0.70    0.48  0.0016  0.0057    30688        0        7     40
---------------------------------------------------------------------------------------------------------------
 SKT    0     0.18   1.80    3.40      79 K    472 K    0.79    0.92  0.0000  0.0000    37968        2        4     34
 SKT    1     0.22   2.29    3.40      77 K    381 K    0.77    0.87  0.0000  0.0000    37184        6       12     28
---------------------------------------------------------------------------------------------------------------
 TOTAL  *     0.20   2.07    3.40     156 K    853 K    0.78    0.90  0.0000  0.0000     N/A     N/A     N/A      N/A

 Instructions retired:   77 G ; Active cycles:   37 G ; Time (TSC): 2698 Mticks;

 Core C-state residencies: C0 (active,non-halted): 19.58 %; C1: 80.42 %; C6: 0.00 %;
 Package C-state residencies:  C0: 100.00 %; C2: 0.00 %; C6: 0.00 %;
                             ┌────────────────────────────────────────────────────────────────────────────────┐
 Core    C-state distribution│00000000000000001111111111111111111111111111111111111111111111111111111111111111│
                             └────────────────────────────────────────────────────────────────────────────────┘
                             ┌────────────────────────────────────────────────────────────────────────────────┐
 Package C-state distribution│00000000000000000000000000000000000000000000000000000000000000000000000000000000│
                             └────────────────────────────────────────────────────────────────────────────────┘
Intel(r) UPI data traffic estimation in bytes (data traffic coming to CPU/socket through UPI links):

               UPI0     UPI1    |  UPI0   UPI1  
---------------------------------------------------------------------------------------------------------------
 SKT    0       12 M     12 M   |    0%     0%   
 SKT    1     8738 K   8471 K   |    0%     0%   
---------------------------------------------------------------------------------------------------------------
Total UPI incoming data traffic:   42 M     UPI data traffic/Memory controller traffic: 0.33

Intel(r) UPI traffic estimation in bytes (data and non-data traffic outgoing from CPU/socket through UPI links):

               UPI0     UPI1    |  UPI0   UPI1  
---------------------------------------------------------------------------------------------------------------
 SKT    0       28 M     28 M   |    0%     0%   
 SKT    1       32 M     31 M   |    0%     0%   
---------------------------------------------------------------------------------------------------------------
Total UPI outgoing data and non-data traffic:  120 M

MEM (GB)->|  READ |  WRITE | LOCAL | PMM RD | PMM WR | CPU energy | DIMM energy | LLCRDMISSLAT (ns)| UncFREQ (Ghz)|
---------------------------------------------------------------------------------------------------------------
 SKT   0     0.04     0.02   48 %      0.00      0.00      87.42       5.31         152.39             2.40
 SKT   1     0.04     0.03   53 %      0.00      0.00     106.81       8.16         138.19             2.40
---------------------------------------------------------------------------------------------------------------
       *     0.08     0.05   50 %      0.00      0.00     194.23      13.47         144.54             2.40

 UTIL  : utlization (same as core C0 state active state residency, the value is in 0..1) 
 IPC   : instructions per CPU cycle
 CFREQ : core frequency in Ghz
 L3MISS: L3 (read) cache misses 
 L2MISS: L2 (read) cache misses (including other core's L2 cache *hits*) 
 L3HIT : L3 (read) cache hit ratio (0.00-1.00)
 L2HIT : L2 cache hit ratio (0.00-1.00)
 L3MPI : number of L3 (read) cache misses per instruction
 L2MPI : number of L2 (read) cache misses per instruction
 READ  : bytes read from main memory controller (in GBytes)
 WRITE : bytes written to main memory controller (in GBytes)
 LOCAL : ratio of local memory requests to memory controller in %
LLCRDMISSLAT: average latency of last level cache miss for reads and prefetches (in ns)
 PMM RD : bytes read from PMM memory (in GBytes)
 PMM WR : bytes written to PMM memory (in GBytes)
 L3OCC : L3 occupancy (in KBytes)
 LMB   : L3 cache external bandwidth satisfied by local memory (in MBytes)
 RMB   : L3 cache external bandwidth satisfied by remote memory (in MBytes)
 TEMP  : Temperature reading in 1 degree Celsius relative to the TjMax temperature (thermal headroom): 0 corresponds to the max temperature
 energy: Energy in Joules


 Core (SKT) | UTIL | IPC  | CFREQ | L3MISS | L2MISS | L3HIT | L2HIT | L3MPI | L2MPI |   L3OCC |   LMB  |   RMB  | TEMP

   0    0     0.65   0.67    3.40      21 K     82 K    0.69    0.98  0.0000  0.0001     1456        1        2     40
   1    1     0.00   0.70    3.40      11 K    109 K    0.89    0.29  0.0015  0.0142        0        0        0     40
   2    0     0.02   0.58    3.41    2236       17 K    0.85    0.93  0.0001  0.0005        0        0        0     41
   3    1     0.00   0.55    3.40     885     4216      0.77    0.41  0.0017  0.0079      336        1        0     41
   4    0     0.02   0.59    3.41     973       13 K    0.92    0.92  0.0000  0.0004      112        0        0     41
   5    1     0.00   0.61    3.40    1588     8207      0.79    0.69  0.0009  0.0047     4592        0        0     39
   6    0     1.00   3.34    3.40    4628       18 K    0.56    0.35  0.0000  0.0000      336        0        1     34
   7    1     0.00   0.71    3.40    1367     8525      0.82    0.32  0.0014  0.0087        0        0        0     40
   8    0     0.00   0.54    3.40     289     4536      0.92    0.38  0.0007  0.0108      224        0        0     45
   9    1     0.00   0.52    3.40     445     2733      0.82    0.46  0.0012  0.0074      112        0        0     41
  10    0     0.00   0.46    3.40     133     2893      0.94    0.36  0.0005  0.0109        0        0        0     42
  11    1     0.00   1.05    3.40      21 K     91 K    0.77    0.36  0.0036  0.0156     1008        0        1     40
  12    0     1.00   2.25    3.40     542     9020      0.93    0.53  0.0000  0.0000        0        0        0     36
  13    1     1.00   1.03    3.40      13 K     16 K    0.04    0.99  0.0000  0.0000        0        0        0     32
  14    0     1.00   1.34    3.40    4824       17 K    0.50    0.29  0.0000  0.0000        0        0        0     37
  15    1     0.00   0.53    3.40     662     4322      0.83    0.44  0.0013  0.0087        0        0        0     40
  16    0     0.00   0.21    3.40     170     2758      0.92    0.45  0.0003  0.0049      112        0        0     45
  17    1     0.00   0.47    3.40     244     1957      0.86    0.50  0.0009  0.0071        0        0        0     42
  18    0     0.00   0.45    3.40     102     1851      0.92    0.40  0.0004  0.0081      336        0        0     43
  19    1     0.00   1.05    3.40    2152       11 K    0.78    0.83  0.0002  0.0009      112        0        0     41
  20    0     0.00   0.46    3.40     239     2471      0.87    0.37  0.0010  0.0105        0        0        0     42
  21    1     0.03   0.60    3.40      10 K     32 K    0.41    0.91  0.0002  0.0005        0        0        0     40
  22    0     0.00   0.71    3.40    4170       58 K    0.91    0.32  0.0009  0.0124        0        0        0     45
  23    1     0.00   0.56    3.40    1425     9132      0.82    0.35  0.0019  0.0122        0        0        0     41
  24    0     0.00   0.48    3.40      89     1897      0.94    0.40  0.0005  0.0105        0        0        0     43
  25    1     0.00   0.63    3.40    1589       12 K    0.85    0.33  0.0016  0.0125        0        0        0     39
  26    0     0.00   0.41    3.40     117     1177      0.87    0.43  0.0007  0.0071      448        0        0     42
  27    1     0.00   0.63    3.40     583     4542      0.85    0.61  0.0007  0.0057        0        0        0     39
  28    0     0.01   0.58    3.40     776     4660      0.80    0.87  0.0001  0.0003        0        0        0     44
  29    1     0.00   0.61    3.40    2158       11 K    0.80    0.50  0.0016  0.0090      112        0        0     41
  30    0     1.00   1.34    3.40     877       22 K    0.89    0.32  0.0000  0.0000        0        0        0     36
  31    1     0.00   0.55    3.40     611     3392      0.80    0.51  0.0012  0.0068        0        0        0     40
  32    0     0.04   0.60    3.41    3036       17 K    0.77    0.93  0.0000  0.0002      224        0        0     43
  33    1     0.01   1.36    3.40    6562       28 K    0.75    0.86  0.0001  0.0006      784        2        0     40
  34    0     0.03   0.58    3.40    2338       14 K    0.81    0.88  0.0000  0.0003      112        0        0     41
  35    1     0.00   0.57    3.40    1191     8853      0.85    0.30  0.0017  0.0129        0        0        0     42
  36    0     0.04   0.60    3.40    3732       18 K    0.75    0.95  0.0000  0.0002     1008        0        1     42
  37    1     0.00   0.58    3.40     807     6790      0.87    0.30  0.0014  0.0121        0        0        0     40
  38    0     0.00   0.61    3.40    3385       28 K    0.86    0.34  0.0010  0.0084      560        0        0     43
  39    1     0.00   0.61    3.40    2201       14 K    0.83    0.30  0.0019  0.0126        0        0        0     40
  40    0     0.00   0.58    3.40    2447       19 K    0.86    0.53  0.0003  0.0023        0        0        0     44
  41    1     0.00   0.56    3.40    1643       11 K    0.84    0.29  0.0021  0.0141        0        0        0     40
  42    0     0.02   0.59    3.40    1516       12 K    0.85    0.83  0.0000  0.0004        0        0        0     42
  43    1     0.00   0.66    3.40    1299     8784      0.84    0.47  0.0010  0.0066        0        0        0     42
  44    0     0.05   0.59    3.40    3423       28 K    0.83    0.89  0.0000  0.0003        0        0        0     47
  45    1     1.00   3.66    3.40    4883       18 K    0.69    0.81  0.0000  0.0000        0        0        0     28
  46    0     0.00   0.58    3.40    1205     9342      0.84    0.69  0.0001  0.0010        0        0        0     44
  47    1     1.00   1.36    3.40     885       11 K    0.87    0.61  0.0000  0.0000        0        0        0     30
  48    0     0.00   0.63    3.40     557     5965      0.89    0.61  0.0001  0.0013        0        0        0     44
  49    1     1.00   1.09    3.40     773     9215      0.91    0.38  0.0000  0.0000        0        0        0     31
  50    0     0.02   0.61    3.41    2134       15 K    0.83    0.90  0.0000  0.0003      336        0        0     41
  51    1     1.00   3.38    3.40    2372       38 K    0.93    0.23  0.0000  0.0000      112        1        0     30
  52    0     0.03   0.60    3.41    2702       18 K    0.83    0.87  0.0000  0.0003        0        0        0     43
  53    1     1.00   3.38    3.40    1368       14 K    0.89    0.36  0.0000  0.0000      224        1        0     29
  54    0     0.02   0.58    3.40    3835       16 K    0.71    0.87  0.0001  0.0004    33488        2        0     43
  55    1     0.00   0.33    3.40     836     2266      0.61    0.55  0.0018  0.0048    29008        0        2     40
---------------------------------------------------------------------------------------------------------------
 SKT    0     0.18   1.79    3.40      72 K    467 K    0.80    0.93  0.0000  0.0000    38752        3        4     34
 SKT    1     0.22   2.30    3.40      95 K    505 K    0.79    0.84  0.0000  0.0000    36400        5        2     28
---------------------------------------------------------------------------------------------------------------
 TOTAL  *     0.20   2.07    3.40     167 K    972 K    0.79    0.90  0.0000  0.0000     N/A     N/A     N/A      N/A

 Instructions retired:   77 G ; Active cycles:   37 G ; Time (TSC): 2694 Mticks;

 Core C-state residencies: C0 (active,non-halted): 19.67 %; C1: 80.33 %; C6: 0.00 %;
 Package C-state residencies:  C0: 100.00 %; C2: 0.00 %; C6: 0.00 %;
                             ┌────────────────────────────────────────────────────────────────────────────────┐
 Core    C-state distribution│00000000000000001111111111111111111111111111111111111111111111111111111111111111│
                             └────────────────────────────────────────────────────────────────────────────────┘
                             ┌────────────────────────────────────────────────────────────────────────────────┐
 Package C-state distribution│00000000000000000000000000000000000000000000000000000000000000000000000000000000│
                             └────────────────────────────────────────────────────────────────────────────────┘
Intel(r) UPI data traffic estimation in bytes (data traffic coming to CPU/socket through UPI links):

               UPI0     UPI1    |  UPI0   UPI1  
---------------------------------------------------------------------------------------------------------------
 SKT    0       11 M     11 M   |    0%     0%   
 SKT    1     8759 K   8534 K   |    0%     0%   
---------------------------------------------------------------------------------------------------------------
Total UPI incoming data traffic:   39 M     UPI data traffic/Memory controller traffic: 0.31

Intel(r) UPI traffic estimation in bytes (data and non-data traffic outgoing from CPU/socket through UPI links):

               UPI0     UPI1    |  UPI0   UPI1  
---------------------------------------------------------------------------------------------------------------
 SKT    0       27 M     27 M   |    0%     0%   
 SKT    1       30 M     29 M   |    0%     0%   
---------------------------------------------------------------------------------------------------------------
Total UPI outgoing data and non-data traffic:  114 M

MEM (GB)->|  READ |  WRITE | LOCAL | PMM RD | PMM WR | CPU energy | DIMM energy | LLCRDMISSLAT (ns)| UncFREQ (Ghz)|
---------------------------------------------------------------------------------------------------------------
 SKT   0     0.04     0.02   56 %      0.00      0.00      87.68       5.30         156.87             2.40
 SKT   1     0.04     0.03   51 %      0.00      0.00     106.64       8.12         151.30             2.40
---------------------------------------------------------------------------------------------------------------
       *     0.08     0.05   52 %      0.00      0.00     194.32      13.41         153.98             2.40

 UTIL  : utlization (same as core C0 state active state residency, the value is in 0..1) 
 IPC   : instructions per CPU cycle
 CFREQ : core frequency in Ghz
 L3MISS: L3 (read) cache misses 
 L2MISS: L2 (read) cache misses (including other core's L2 cache *hits*) 
 L3HIT : L3 (read) cache hit ratio (0.00-1.00)
 L2HIT : L2 cache hit ratio (0.00-1.00)
 L3MPI : number of L3 (read) cache misses per instruction
 L2MPI : number of L2 (read) cache misses per instruction
 READ  : bytes read from main memory controller (in GBytes)
 WRITE : bytes written to main memory controller (in GBytes)
 LOCAL : ratio of local memory requests to memory controller in %
LLCRDMISSLAT: average latency of last level cache miss for reads and prefetches (in ns)
 PMM RD : bytes read from PMM memory (in GBytes)
 PMM WR : bytes written to PMM memory (in GBytes)
 L3OCC : L3 occupancy (in KBytes)
 LMB   : L3 cache external bandwidth satisfied by local memory (in MBytes)
 RMB   : L3 cache external bandwidth satisfied by remote memory (in MBytes)
 TEMP  : Temperature reading in 1 degree Celsius relative to the TjMax temperature (thermal headroom): 0 corresponds to the max temperature
 energy: Energy in Joules


 Core (SKT) | UTIL | IPC  | CFREQ | L3MISS | L2MISS | L3HIT | L2HIT | L3MPI | L2MPI |   L3OCC |   LMB  |   RMB  | TEMP

   0    0     0.62   0.67    3.40      19 K     83 K    0.72    0.98  0.0000  0.0001     2016        1        2     36
   1    1     0.00   0.71    3.40      12 K    111 K    0.88    0.29  0.0016  0.0144        0        0        0     40
   2    0     0.00   0.94    3.40     595     4223      0.82    0.63  0.0002  0.0012      112        0        0     41
   3    1     0.00   0.55    3.40     654     3584      0.81    0.39  0.0014  0.0077      224        0        0     41
   4    0     0.06   0.59    3.41    2862       24 K    0.85    0.96  0.0000  0.0002        0        0        0     41
   5    1     0.00   0.59    3.40    1720     8579      0.79    0.70  0.0010  0.0050     4480        0        0     39
   6    0     1.00   3.34    3.40    4456       17 K    0.56    0.36  0.0000  0.0000      336        0        0     34
   7    1     0.00   0.60    3.40    1248     7343      0.81    0.36  0.0018  0.0103        0        0        0     40
   8    0     0.02   0.59    3.41    1874       15 K    0.85    0.92  0.0000  0.0003      336        0        0     44
   9    1     0.00   0.56    3.40     626     3097      0.77    0.34  0.0020  0.0099      112        0        0     41
  10    0     0.04   0.59    3.40    2355       20 K    0.86    0.92  0.0000  0.0002      112        0        0     42
  11    1     0.00   0.39    3.40      94      550      0.82    0.42  0.0012  0.0071      896        0        0     40
  12    0     1.00   2.25    3.40     522     9213      0.94    0.52  0.0000  0.0000        0        0        0     36
  13    1     1.00   1.03    3.40      13 K     16 K    0.05    0.99  0.0000  0.0000        0        0        0     32
  14    0     1.00   1.34    3.40    4985       17 K    0.49    0.28  0.0000  0.0000        0        0        0     37
  15    1     0.00   0.52    3.40     363     2484      0.84    0.34  0.0016  0.0113        0        0        0     40
  16    0     0.04   0.60    3.41    3005       20 K    0.82    0.92  0.0000  0.0002      448        0        2     45
  17    1     0.00   0.38    3.40     128      450      0.70    0.53  0.0016  0.0058        0        0        0     41
  18    0     0.03   0.58    3.40    2165       18 K    0.86    0.87  0.0000  0.0003      224        1        0     43
  19    1     0.00   1.33    3.40    1802     8967      0.77    0.87  0.0001  0.0006      224        1        0     41
  20    0     0.00   0.45    3.40      84     1904      0.95    0.43  0.0004  0.0097      224        0        0     42
  21    1     0.00   0.48    3.40     201     1223      0.82    0.47  0.0013  0.0081        0        0        0     40
  22    0     0.00   0.69    3.40    4573       56 K    0.90    0.32  0.0010  0.0121        0        0        0     45
  23    1     0.00   0.58    3.40    1491     7752      0.78    0.34  0.0024  0.0127        0        0        0     41
  24    0     0.00   0.45    3.40     116     1962      0.92    0.41  0.0006  0.0095      112        0        0     43
  25    1     0.01   0.59    3.40    6373       13 K    0.35    0.88  0.0004  0.0009        0        0        0     39
  26    0     0.00   0.39    3.40     182     1748      0.87    0.42  0.0009  0.0087      448        0        0     42
  27    1     0.02   0.60    3.40    7908       27 K    0.48    0.89  0.0002  0.0005        0        0        0     39
  28    0     0.00   0.44    3.40     217     2292      0.88    0.47  0.0009  0.0091        0        0        0     45
  29    1     0.00   0.64    3.40    2741       17 K    0.83    0.42  0.0017  0.0112      112        0        0     41
  30    0     1.00   1.34    3.40     813       22 K    0.90    0.32  0.0000  0.0000        0        0        0     36
  31    1     0.00   0.51    3.40     373     2051      0.80    0.45  0.0019  0.0102        0        0        0     40
  32    0     0.02   0.59    3.41    1791       10 K    0.78    0.91  0.0000  0.0003      336        0        1     43
  33    1     0.01   1.39    3.40    5612       23 K    0.74    0.88  0.0001  0.0005      784        1        0     40
  34    0     0.00   0.51    3.40     170     2226      0.91    0.39  0.0007  0.0095      112        0        0     41
  35    1     0.00   0.61    3.40    1431     9290      0.83    0.27  0.0022  0.0142        0        0        0     42
  36    0     0.00   0.47    3.40      88     2002      0.94    0.39  0.0004  0.0090     1008        0        0     42
  37    1     0.00   0.59    3.40     895     6331      0.85    0.28  0.0019  0.0133        0        0        0     40
  38    0     0.02   0.59    3.40    1330       10 K    0.85    0.89  0.0000  0.0003      336        0        0     43
  39    1     0.00   0.69    3.40    1931       15 K    0.86    0.29  0.0016  0.0128        0        0        0     40
  40    0     0.00   0.61    3.40    3818       32 K    0.87    0.31  0.0011  0.0098        0        0        0     44
  41    1     0.00   0.60    3.40    1475       10 K    0.85    0.27  0.0021  0.0148        0        0        0     40
  42    0     0.03   0.58    3.41    2619       15 K    0.80    0.89  0.0000  0.0002      224        0        0     42
  43    1     0.00   0.70    3.40     989     6988      0.84    0.47  0.0009  0.0065        0        0        0     42
  44    0     0.01   0.58    3.40    2233       14 K    0.84    0.70  0.0001  0.0010        0        0        0     46
  45    1     1.00   3.64    3.40    4964       17 K    0.67    0.80  0.0000  0.0000      224        0        0     28
  46    0     0.00   0.52    3.40     886     8217      0.87    0.36  0.0011  0.0100        0        0        0     43
  47    1     1.00   1.36    3.40     975       11 K    0.86    0.60  0.0000  0.0000        0        0        0     29
  48    0     0.00   0.90    3.40     320     4521      0.92    0.33  0.0003  0.0036        0        0        0     44
  49    1     1.00   1.09    3.40     726     9335      0.92    0.34  0.0000  0.0000        0        0        0     31
  50    0     0.02   0.59    3.41    1011       10 K    0.88    0.87  0.0000  0.0003      336        0        0     41
  51    1     1.00   3.40    3.40    2289       39 K    0.94    0.23  0.0000  0.0000      336        1        0     30
  52    0     0.00   1.41    3.40     692     6390      0.87    0.32  0.0002  0.0014        0        0        0     43
  53    1     1.00   3.35    3.40    1435       14 K    0.88    0.36  0.0000  0.0000      224        1        0     29
  54    0     0.00   0.46    3.40    1984     4792      0.52    0.57  0.0014  0.0034    31360        2        1     43
  55    1     0.00   0.34    3.40     603     1296      0.53    0.66  0.0015  0.0032    27216        0        1     39
---------------------------------------------------------------------------------------------------------------
 SKT    0     0.18   1.80    3.40      65 K    440 K    0.81    0.93  0.0000  0.0000    38080        4        5     34
 SKT    1     0.22   2.30    3.40      75 K    399 K    0.78    0.87  0.0000  0.0000    34832        4        1     28
---------------------------------------------------------------------------------------------------------------
 TOTAL  *     0.20   2.07    3.40     140 K    840 K    0.80    0.91  0.0000  0.0000     N/A     N/A     N/A      N/A

 Instructions retired:   77 G ; Active cycles:   37 G ; Time (TSC): 2691 Mticks;

 Core C-state residencies: C0 (active,non-halted): 19.61 %; C1: 80.39 %; C6: 0.00 %;
 Package C-state residencies:  C0: 100.00 %; C2: 0.00 %; C6: 0.00 %;
                             ┌────────────────────────────────────────────────────────────────────────────────┐
 Core    C-state distribution│00000000000000001111111111111111111111111111111111111111111111111111111111111111│
                             └────────────────────────────────────────────────────────────────────────────────┘
                             ┌────────────────────────────────────────────────────────────────────────────────┐
 Package C-state distribution│00000000000000000000000000000000000000000000000000000000000000000000000000000000│
                             └────────────────────────────────────────────────────────────────────────────────┘
Intel(r) UPI data traffic estimation in bytes (data traffic coming to CPU/socket through UPI links):

               UPI0     UPI1    |  UPI0   UPI1  
---------------------------------------------------------------------------------------------------------------
 SKT    0     9753 K   9992 K   |    0%     0%   
 SKT    1     7914 K   7643 K   |    0%     0%   
---------------------------------------------------------------------------------------------------------------
Total UPI incoming data traffic:   35 M     UPI data traffic/Memory controller traffic: 0.30

Intel(r) UPI traffic estimation in bytes (data and non-data traffic outgoing from CPU/socket through UPI links):

               UPI0     UPI1    |  UPI0   UPI1  
---------------------------------------------------------------------------------------------------------------
 SKT    0       24 M     25 M   |    0%     0%   
 SKT    1       26 M     25 M   |    0%     0%   
---------------------------------------------------------------------------------------------------------------
Total UPI outgoing data and non-data traffic:  102 M

MEM (GB)->|  READ |  WRITE | LOCAL | PMM RD | PMM WR | CPU energy | DIMM energy | LLCRDMISSLAT (ns)| UncFREQ (Ghz)|
---------------------------------------------------------------------------------------------------------------
 SKT   0     0.04     0.02   60 %      0.00      0.00      87.20       5.28         156.63             2.40
 SKT   1     0.04     0.02   50 %      0.00      0.00     106.60       8.09         153.82             2.40
---------------------------------------------------------------------------------------------------------------
       *     0.08     0.04   54 %      0.00      0.00     193.80      13.37         155.27             2.40
^CDEBUG: caught signal to interrupt (Interrupt).
Cleaning up
 Closed perf event handles
 Zeroed uncore PMU registers
 Freeing up all RMIDs
 Re-enabling NMI watchdog.
```