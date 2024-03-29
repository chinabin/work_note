```bash
#!/bin/bash
#
# icache - measure instruction cache usage.
#            Uses Linux perf and PMCs.
#
# FROM: https://github.com/brendangregg/pmc-cloud-tools
#
# USAGE: icache {-C CPU | -p PID | -c CMD} [interval [duration]]
#
# Columns:
#
# - INS: Instructions x 1000
# - L1IMISS: Level 1 instruction cache misses x 1000
# - L1I%: Level 1 instruction cache hit ratio percent (L1MISS/INS)
# - L2IHIT: Level 2 instruction cache hits x 1000
# - L2IMISS: Level 2 instruction cache misses x 1000
# - L2I%: Level 2 instruction cache hit ratio percent
# - LLCREF: Last Level Cache references x 1000
# - LLCMISS: Last Level Cache misses x 1000
# - LLC%: Last Level Cache hit ratio percent
#
# Copyright 2020 Netflix, Inc.
# Licensed under the Apache License, Version 2.0 (the "License")
#
# 24-Jan-2020	Brendan Gregg	Created this.

function usage {
	cat <<-END >&2
	USAGE: icache {-C CPU | -p PID | -c CMD} [interval [duration]]
	                 -C CPU         # measure this CPU only
	                 -p PID         # measure this PID only
	                 -c 'CMD'       # measure this command only (quote it)
	                 interval       # output interval in secs (default 1)
	                 duration       # total seconds (default infinityish)
	  eg,
	       icache                 # show stats across all CPUs
	       icache 5               # show stats every 5 seconds
	       icache -C 0            # measure CPU 0 only
	       icache -p 181          # measure PID 181 only
	       icache -c 'cksum /boot/*' # measure run and measure this cmd
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
perf stat -e instructions,L1-icache-load-misses,r2424,r4424,cache-references,cache-misses \
	-I $(( secs * 1000 )) $target 2>&1 | awk -v hlines=$hlines '
	BEGIN {
		htxt = sprintf("%-10s %10s %5s %9s %9s %5s %8s %8s %5s",
			"INS", "L1IMISS", "L1I%", "L2IHIT", "L2IMISS", "L2I%",
			"LLCREF", "LLCMISS", "LLC%");
		print htxt
		header = hlines
	}
	/invalid/ { print $0 }	# unsupported event
	{ gsub(/,/, ""); }
	$3 == "instructions" { ins = $2 }
	$3 == "L1-icache-load-misses" { l1im = $2 }
	$3 == "r2424" { l2im = $2 }			# L2_RQSTS.CODE_RD_MISS
	$3 == "r4424" { l2ih = $2 }			# L2_RQSTS.CODE_RD_HIT
	$3 == "cache-references" { llcr = $2 }
	$3 == "cache-misses" {			# last one, trigger output
		llcm = $2
		if (--header == 0) {
			print htxt
			header = hlines
		}

		# if we are missing refs, then also reset hits to zero, to avoid a bogus ratio:
		if (ins == 0) { ins = 1; l1im = 0 }
		if (l2im == 0) { l2im = 1; l2ih = 0 }
		if (llcr == 0) { llcr = 1; llcm = 0 }
		l1iratio = 100 * (ins - l1im) / ins
		l2iratio = 100 * l2ih / (l2ih + l2im)
		llcratio = 100 * (llcr - llcm) / llcr

		printf("%-10d %10d %5.2f %9d %9d %5.2f %8d %8d %5.2f\n",
			ins / 1000, l1im / 1000, l1iratio, l2ih / 1000, l2im / 1000, l2iratio,
			llcr / 1000, llcm / 1000, llcratio);
	}
'

```