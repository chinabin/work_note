```bash
#!/bin/bash
#
# pmcipc - measure and summarize instructions per cycle. Uses Linux perf.
#
# This deliberately measures IPC only, for instances where measuring more than
# a few PMCs leads to bogus measurements. Cross-check with pmcarch and
# perf stat.
#
# FROM: https://github.com/brendangregg/pmc-cloud-tools
#
# USAGE: pmcipc {-C CPU | -p PID | -c CMD} [interval [duration]]
#
# PMCs are performance monitoring counters. This tool reports:
#
# - K_CYCLES: CPU Cycles x 1000
# - K_INSTR: CPU Instructions x 1000
# - IPC: Instructions-Per-Cycle
#
# Copyright 2019 Netflix, Inc.
# Licensed under the Apache License, Version 2.0 (the "License")
#
# 04-Oct-2019	Brendan Gregg	Created this.

function usage {
	cat <<-END >&2
	USAGE: pmcipc {-C CPU | -p PID | -c CMD} [interval [duration]]
	                 -C CPU         # measure this CPU only
	                 -p PID         # measure this PID only
	                 -c 'CMD'       # measure this command only (quote it)
	                 interval       # output interval in secs (default 1)
	                 duration       # total seconds (default infinityish)
	  eg,
	       pmcipc                   # show stats across all CPUs
	       pmcipc 5                 # show stats every 5 seconds
	       pmcipc -C 0              # measure CPU 0 only
	       pmcipc -p 181            # measure PID 181 only
	       pmcipc -c 'cksum /boot/*'  # measure run and measure this cmd
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
# order of PMCs attempts to workaround a 3.13 issue.
# cycles are twice as a workaround for an issue.
# only three (<=4) PMCs are used to workaround another issue.
# the raw counters are from the architectural set, so should be stable.
perf stat -e cycles -e cycles -e instructions \
	-I $(( secs * 1000 )) $target 2>&1 | awk -v hlines=$hlines '
	BEGIN {
		htxt = sprintf("%-10s %-10s %5s", "K_CYCLES", "K_INSTR", "IPC");
		print htxt
		header = hlines
	}
	/invalid/ { print $0 }	# unsupported event
	{ gsub(/,/, ""); }
	$3 == "cycles" { cycles = $2; }
	$3 == "instructions" {
		if (--header == 0) {
			print htxt
			header = hlines
		}
		ins = $2
		if (llcref == 0) { llcref = 1; llcmiss = 0 }
		if (cycles == 0) { cycles = 1 }	 # PMCs are broken, or no events

		printf("%-10d %-10d %5.2f\n",
			cycles / 1000, ins / 1000, ins / cycles)
	}
'
```