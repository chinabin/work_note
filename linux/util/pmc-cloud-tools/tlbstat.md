```bash
#!/bin/bash
#
# tlbstat - measure and summarize TLBs. Uses Linux perf and PMCs.
#
# FROM: https://github.com/brendangregg/pmc-cloud-tools
#
# TLB: Translation Lookaside Buffer (the MMU page translation cache)
# PMC: Performance Monitoring Counters
#
# USAGE: tlbstat {-C CPU | -p PID | -c CMD} [interval [duration]]
#
# REQUIREMENTS: perf command, TLB PMCs (counters listed below) which may
# only be provided by Linux 4.14-ish and newer.
#
# Columns:
#
# - K_CYCLES: CPU Cycles x 1000
# - K_INSTR: CPU Instructions x 1000
# - IPC: Instructions-Per-Cycle
# - DTLB_WALKS: Data TLB walks (count)
# - ITLB_WALKS: Instruction TLB walks (count)
# - K_DTLBCYC: Cycles at least one PMH is active with data TLB walks x 1000
# - K_ITLBCYC: Cycles at least one PMH is active with instr. TLB walks x 1000
# - DTLB%: Data TLB active cycles as a ratio of total cycles
# - ITLB%: Instruction TLB active cycles as a ratio of total cycles
#
# Copyright 2018 Netflix, Inc.
# Licensed under the Apache License, Version 2.0 (the "License")
#
# 08-Jan-2018	Brendan Gregg	Created this.

function usage {
	cat <<-END >&2
	USAGE: tlbstat {-C CPU | -p PID | -c CMD} [interval [duration]]
	                 -C CPU         # measure this CPU only
	                 -p PID         # measure this PID only
	                 -c 'CMD'       # measure this command only (quote it)
	                 interval       # output interval in secs (default 1)
	                 duration       # total seconds (default infinityish)
	  eg,
	       tlbstat                  # show stats across all CPUs
	       tlbstat 5                # show stats every 5 seconds
	       tlbstat -C 0             # measure CPU 0 only
	       tlbstat -p 181           # measure PID 181 only
	       tlbstat -c 'cksum /boot/*'  # measure run and measure this cmd
END
	exit
}

opt_cpu=0; opt_pid=0; opt_cmd=0; cpu=""; pid=""; cmd=""

while getopts C:p:c:h opt
do
	case $opt in
	C)	opt_cpu=1; cpu=$OPTARG ;;
	p)	opt_pid=1; pid=$OPTARG ;;
	c)	opt_cmd=1; cmd=$OPTARG ;;
	h|?)	usage ;;
	esac
done
shift $(( $OPTIND - 1 ))

if (( opt_cpu + opt_pid + opt_cmd > 1 )); then
	echo >&2 "ERROR: pick one of -C, -p, -c"
	usage
fi
secs=${1-1}			# default 1 second
duration=${2-999999999}		# default semi-infinite seconds
hlines=25			# lines to repeat header
target=-a
(( opt_cpu )) && target="-C $cpu sleep $duration"
(( opt_pid )) && target="-p $pid sleep $duration"
(( opt_cmd )) && target="$cmd"

if (( opt_pid )); then
	if [ ! -d /proc/$pid ]; then
		echo >&2 "ERROR: Can't find PID $pid. Exiting."
		exit
	fi
fi

# note that instructions is last on purpose, it triggers output
# cycles are twice as a workaround for an issue
perf stat -e cycles -e cycles \
	-e dtlb_load_misses.miss_causes_a_walk \
	-e dtlb_store_misses.miss_causes_a_walk \
	-e itlb_misses.miss_causes_a_walk \
	-e dtlb_load_misses.walk_active \
	-e dtlb_store_misses.walk_active \
	-e itlb_misses.walk_active \
	-e instructions \
	-I $(( secs * 1000 )) $target 2>&1 | awk -v hlines=$hlines '
	BEGIN {
		htxt = sprintf("%-10s %-10s %5s %-10s %-10s %-10s %-10s %5s %5s",
		"K_CYCLES", "K_INSTR", "IPC", "DTLB_WALKS", "ITLB_WALKS",
		"K_DTLBCYC", "K_ITLBCYC", "DTLB%", "ITLB%");
		print htxt
		header = hlines
	}
	/invalid/ { print $0 }	# unsupported event
	{ gsub(/,/, ""); }
	$3 == "cycles" { cycles = $2; }
	# counts:
	$3 == "dtlb_load_misses.miss_causes_a_walk" { dtlbl = $2; }
	$3 == "dtlb_store_misses.miss_causes_a_walk" { dtlbs = $2; }
	$3 == "itlb_misses.miss_causes_a_walk" { itlb = $2; }
	# walk active cycles in at least one PMH cycles:
	$3 == "dtlb_load_misses.walk_active" { dtlblwc = $2; }
	$3 == "dtlb_store_misses.walk_active" { dtlbswc = $2; }
	$3 == "itlb_misses.walk_active" { itlbwc = $2; }
	$3 == "instructions" {
		if (--header == 0) {
			print htxt
			header = hlines
		}
		ins = $2
		if (cycles == 0) { cycles = 1 }	 # PMCs are broken, or no events

		printf("%-10d %-10d %5.2f %-10d %-10d %-10d %-10d %5.2f %5.2f\n",
			cycles / 1000, ins / 1000, ins / cycles,
			dtlbl + dtlbs, itlb,
			(dtlblwc + dtlbswc) / 1000, 
			itlbwc / 1000, 
			100 * (dtlblwc + dtlbswc) / cycles,
			100 * (itlbwc) / cycles)
	}
'
```