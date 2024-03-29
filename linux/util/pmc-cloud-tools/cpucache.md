```bash
#!/bin/bash
#
# cpucache - measure and summarize CPU cache references (L1/L2/LLC).
#            Uses Linux perf and PMCs.
#
# FROM: https://github.com/brendangregg/pmc-cloud-tools
#
# USAGE: cpucache {-C CPU | -p PID | -c CMD} [interval [duration]]
#
# Columns:
#
# - CYCLES: CPU Cycles x 1000
# - INSTR: CPU Instructions x 1000
# - IPC: Instructions-Per-Cycle
# - L1DREF: Level 1 data cache loads x 1000
# - L1DMISS: Level 1 data cache load misses x 1000
# - L1D%: Level 1 data cache hit ratio, as a percentage
# - L2REF: Level 2 requests x 1000
# - L2MISS: Level 2 misses x 1000
# - L2%: Level 2 hit ratio, as a percentage
# - LLCREF: Last Level Cache references x 1000
# - LLCMISS: Last Level Cache misses x 1000
# - LLC%: Last Level Cache hit ratio, as a percentage
#
# Copyright 2018 Netflix, Inc.
# Licensed under the Apache License, Version 2.0 (the "License")
#
# 12-Jan-2018	Brendan Gregg	Created this.

function usage {
	cat <<-END >&2
	USAGE: cpucache {-C CPU | -p PID | -c CMD} [interval [duration]]
	                 -C CPU         # measure this CPU only
	                 -p PID         # measure this PID only
	                 -c 'CMD'       # measure this command only (quote it)
	                 interval       # output interval in secs (default 1)
	                 duration       # total seconds (default infinityish)
	  eg,
	       cpucache                 # show stats across all CPUs
	       cpucache 5               # show stats every 5 seconds
	       cpucache -C 0            # measure CPU 0 only
	       cpucache -p 181          # measure PID 181 only
	       cpucache -c 'cksum /boot/*' # measure run and measure this cmd
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
secs=${1:-1}			# default 1 second
duration=${2:-999999999}	# default semi-infinite seconds
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
# the r4f2e and r412e counters are from the architectural set, so should be stable
echo "All counter columns are x 1000"
perf stat -e cycles -e cycles \
	-e L1-dcache-loads -e L1-dcache-load-misses \
	-e l2_rqsts.references -e l2_rqsts.miss \
	-e r4f2e -e r412e -e instructions \
	-I $(( secs * 1000 )) $target 2>&1 | awk -v hlines=$hlines '
	BEGIN {
		htxt = sprintf("%-10s %-10s %5s %-9s %-9s %5s %-8s %-8s %5s %-8s %-8s %5s",
		"CYCLES", "INSTR", "IPC", "L1DREF", "L1DMISS", "L1D%",
		"L2REF", "L2MISS", "L2%", "LLCREF", "LLCMISS", "LLC%");
		print htxt
		header = hlines
	}
	/invalid/ { print $0 }	# unsupported event
	{ gsub(/,/, ""); }
	$3 == "cycles" { cycles = $2 }
	$3 == "r4f2e" { llcref = $2 }
	$3 == "r412e" { llcmiss = $2 }
	$3 == "L1-dcache-loads" { l1dload = $2 }
	$3 == "L1-dcache-load-misses" { l1dloadmiss = $2 }
	$3 == "l2_rqsts.references" { l2ref = $2 }
	$3 == "l2_rqsts.miss" { l2miss = $2 }
	$3 == "instructions" {
		if (--header == 0) {
			print htxt
			header = hlines
		}

		ins = $2
		# if we are missing refs, then also reset hits to zero, to avoid a bogus ratio:
		if (l1dload == 0) { l1dload = 1; l1dloadmiss = 0 }
		if (l2ref == 0) { l2ref = 1; l2miss = 0 }
		if (llcref == 0) { llcref = 1; llcmiss = 0 }
		if (cycles == 0) { cycles = 1; ins = 0 }	 # PMCs are broken, or no events
		l1dratio = 100 * (l1dload - l1dloadmiss) / l1dload
		l2ratio = 100 * (l2ref - l2miss) / l2ref
		llcratio = 100 * (llcref - llcmiss) / llcref

		printf("%-10d %-10d %5.2f %-9d %-9d %5.2f %-8d %-8d %5.2f %-8d %-8d %5.2f\n",
			cycles / 1000, ins / 1000, ins / cycles,
			l1dload / 1000, l1dloadmiss / 1000, l1dratio,
			l2ref / 1000, l2miss / 1000, l2ratio, llcref / 1000, llcmiss / 1000, llcratio)
	}
'
```