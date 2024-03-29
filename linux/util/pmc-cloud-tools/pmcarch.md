```bash
# ./pmcarch -h
USAGE: pmcarch {-C CPU | -p PID | -c CMD} [interval [duration]]
                 -C CPU         # measure this CPU only
                 -p PID         # measure this PID only
                 -c 'CMD'       # measure this command only (quote it)
                 interval       # output interval in secs (default 1)
                 duration       # total seconds (default infinityish)
  eg,
       pmcarch                  # show stats across all CPUs
       pmcarch 5                # show stats every 5 seconds
       pmcarch -C 0             # measure CPU 0 only
       pmcarch -p 181           # measure PID 181 only
       pmcarch -c 'cksum /boot/*'  # measure run and measure this cmd

# ./pmcarch
K_CYCLES   K_INSTR      IPC BR_RETIRED   BR_MISPRED  BMR% LLCREF      LLCMISS     LLC%
96163187   87166313    0.91 19730994925  679187299   3.44 656597454   174313799  73.45
93988372   87205023    0.93 19669256586  724072315   3.68 666041693   169603955  74.54
93863787   86981089    0.93 19548779510  669172769   3.42 649844207   176100680  72.90
93739565   86349653    0.92 19339320671  634063527   3.28 642506778   181385553  71.77
94495981   86815232    0.92 19482504710  648954409   3.33 628548666   180975066  71.21

K_CYCLES: CPU Cycles x 1000
K_INSTR: CPU Instructions x 1000
IPC: Instructions-Per-Cycle
BMR%: Branch Misprediction Ratio, as a percentage
LLC%: Last Level Cache hit ratio, as a percentage
```

```bash
#!/bin/bash
#
# pmcarch - measure and summarize architectural PMCs. Uses Linux perf.
#
# FROM: https://github.com/brendangregg/pmc-cloud-tools
#
# USAGE: pmcarch {-C CPU | -p PID | -c CMD} [interval [duration]]
#
# PMCs are performance monitoring counters. This script uses the architectural
# set to report on:
#
# - K_CYCLES: CPU Cycles x 1000
# - K_INSTR: CPU Instructions x 1000
# - IPC: Instructions-Per-Cycle
# - BMR%: Branch Misprediction Ratio, as a percentage
# - LLC%: Last Level Cache hit ratio, as a percentage
#
# Copyright 2017 Netflix, Inc.
# Licensed under the Apache License, Version 2.0 (the "License")
#
# 07-Apr-2017	Brendan Gregg	Created this.

function usage {
	cat <<-END >&2
	USAGE: pmcarch {-C CPU | -p PID | -c CMD} [interval [duration]]
	                 -C CPU         # measure this CPU only
	                 -p PID         # measure this PID only
	                 -c 'CMD'       # measure this command only (quote it)
	                 interval       # output interval in secs (default 1)
	                 duration       # total seconds (default infinityish)
	  eg,
	       pmcarch                  # show stats across all CPUs
	       pmcarch 5                # show stats every 5 seconds
	       pmcarch -C 0             # measure CPU 0 only
	       pmcarch -p 181           # measure PID 181 only
	       pmcarch -c 'cksum /boot/*'  # measure run and measure this cmd
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
# order of PMCs attempts to workaround a 3.13 issue
# cycles are twice as a workaround for an issue
# the raw counters are from the architectural set, so should be stable
perf stat -e cycles -e cycles \
	-e r4f2e -e r412e \
	-e r00c4 -e r00c5 -e instructions \
	-I $(( secs * 1000 )) $target 2>&1 | awk -v hlines=$hlines '
	BEGIN {
		htxt = sprintf("%-10s %-10s %5s %-12s %-10s %5s %-11s %-10s %5s",
		"K_CYCLES", "K_INSTR", "IPC", "BR_RETIRED", "BR_MISPRED", "BMR%",
		"LLCREF", "LLCMISS", "LLC%");
		print htxt
		header = hlines
	}
	/invalid/ { print $0 }	# unsupported event
	{ gsub(/,/, ""); }
	$3 == "cycles" { cycles = $2; }
	$3 == "r4f2e" { llcref = $2; }
	$3 == "r412e" { llcmiss = $2; }
	$3 == "r00c4" { brretired = $2; }
	$3 == "r00c5" {
		brmiss = $2
		if (brretired > 0) {
			brratio = (100 * brmiss) / brretired;
		} else {
			brratio = 0;
		}
	}
	$3 == "instructions" {
		if (--header == 0) {
			print htxt
			header = hlines
		}
		ins = $2
		if (llcref == 0) { llcref = 1; llcmiss = 0 }
		if (cycles == 0) { cycles = 1 }	 # PMCs are broken, or no events

		printf("%-10d %-10d %5.2f %-12d %-10d %5.2f %-11d %-10d %5.2f\n",
			cycles / 1000, ins / 1000, ins / cycles, brretired, brmiss,
			brratio, llcref, llcmiss, 100 * (llcref - llcmiss) / llcref)
	}
'
```