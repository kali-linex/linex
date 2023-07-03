// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINEX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		This file implements the various access functions for the
 *		PROC file system.  It is mainly used for debugging and
 *		statistics.
 *
 * Authors:	Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *		Gerald J. Heim, <heim@peanuts.informatik.uni-tuebingen.de>
 *		Fred Baumgarten, <dc6iq@insu1.etec.uni-karlsruhe.de>
 *		Erik Schoenfelder, <schoenfr@ibr.cs.tu-bs.de>
 *
 * Fixes:
 *		Alan Cox	:	UDP sockets show the rxqueue/txqueue
 *					using hint flag for the netinfo.
 *	Pauline Middelink	:	identd support
 *		Alan Cox	:	Make /proc safer.
 *	Erik Schoenfelder	:	/proc/net/snmp
 *		Alan Cox	:	Handle dead sockets properly.
 *	Gerhard Koerting	:	Show both timers
 *		Alan Cox	:	Allow inode to be NULL (kernel socket)
 *	Andi Kleen		:	Add support for open_requests and
 *					split functions for more readibility.
 *	Andi Kleen		:	Add support for /proc/net/netstat
 *	Arnaldo C. Melo		:	Convert to seq_file
 */
#include <linex/types.h>
#include <net/net_namespace.h>
#include <net/icmp.h>
#include <net/protocol.h>
#include <net/tcp.h>
#include <net/mptcp.h>
#include <net/udp.h>
#include <net/udplite.h>
#include <linex/bottom_half.h>
#include <linex/inetdevice.h>
#include <linex/proc_fs.h>
#include <linex/seq_file.h>
#include <linex/export.h>
#include <net/sock.h>
#include <net/raw.h>

#define TCPUDP_MIB_MAX max_t(u32, UDP_MIB_MAX, TCP_MIB_MAX)

/*
 *	Report socket allocation statistics [mea@utu.fi]
 */
static int sockstat_seq_show(struct seq_file *seq, void *v)
{
	struct net *net = seq->private;
	int orphans, sockets;

	orphans = tcp_orphan_count_sum();
	sockets = proto_sockets_allocated_sum_positive(&tcp_prot);

	socket_seq_show(seq);
	seq_printf(seq, "TCP: inuse %d orphan %d tw %d alloc %d mem %ld\n",
		   sock_prot_inuse_get(net, &tcp_prot), orphans,
		   refcount_read(&net->ipv4.tcp_death_row.tw_refcount) - 1,
		   sockets, proto_memory_allocated(&tcp_prot));
	seq_printf(seq, "UDP: inuse %d mem %ld\n",
		   sock_prot_inuse_get(net, &udp_prot),
		   proto_memory_allocated(&udp_prot));
	seq_printf(seq, "UDPLITE: inuse %d\n",
		   sock_prot_inuse_get(net, &udplite_prot));
	seq_printf(seq, "RAW: inuse %d\n",
		   sock_prot_inuse_get(net, &raw_prot));
	seq_printf(seq,  "FRAG: inuse %u memory %lu\n",
		   atomic_read(&net->ipv4.fqdir->rhashtable.nelems),
		   frag_mem_limit(net->ipv4.fqdir));
	return 0;
}

/* snmp items */
static const struct snmp_mib snmp4_ipstats_list[] = {
	SNMP_MIB_ITEM("InReceives", IPSTATS_MIB_INPKTS),
	SNMP_MIB_ITEM("InHdrErrors", IPSTATS_MIB_INHDRERRORS),
	SNMP_MIB_ITEM("InAddrErrors", IPSTATS_MIB_INADDRERRORS),
	SNMP_MIB_ITEM("ForwDatagrams", IPSTATS_MIB_OUTFORWDATAGRAMS),
	SNMP_MIB_ITEM("InUnknownProtos", IPSTATS_MIB_INUNKNOWNPROTOS),
	SNMP_MIB_ITEM("InDiscards", IPSTATS_MIB_INDISCARDS),
	SNMP_MIB_ITEM("InDelivers", IPSTATS_MIB_INDELIVERS),
	SNMP_MIB_ITEM("OutRequests", IPSTATS_MIB_OUTPKTS),
	SNMP_MIB_ITEM("OutDiscards", IPSTATS_MIB_OUTDISCARDS),
	SNMP_MIB_ITEM("OutNoRoutes", IPSTATS_MIB_OUTNOROUTES),
	SNMP_MIB_ITEM("ReasmTimeout", IPSTATS_MIB_REASMTIMEOUT),
	SNMP_MIB_ITEM("ReasmReqds", IPSTATS_MIB_REASMREQDS),
	SNMP_MIB_ITEM("ReasmOKs", IPSTATS_MIB_REASMOKS),
	SNMP_MIB_ITEM("ReasmFails", IPSTATS_MIB_REASMFAILS),
	SNMP_MIB_ITEM("FragOKs", IPSTATS_MIB_FRAGOKS),
	SNMP_MIB_ITEM("FragFails", IPSTATS_MIB_FRAGFAILS),
	SNMP_MIB_ITEM("FragCreates", IPSTATS_MIB_FRAGCREATES),
	SNMP_MIB_SENTINEL
};

/* Following items are displayed in /proc/net/netstat */
static const struct snmp_mib snmp4_ipextstats_list[] = {
	SNMP_MIB_ITEM("InNoRoutes", IPSTATS_MIB_INNOROUTES),
	SNMP_MIB_ITEM("InTruncatedPkts", IPSTATS_MIB_INTRUNCATEDPKTS),
	SNMP_MIB_ITEM("InMcastPkts", IPSTATS_MIB_INMCASTPKTS),
	SNMP_MIB_ITEM("OutMcastPkts", IPSTATS_MIB_OUTMCASTPKTS),
	SNMP_MIB_ITEM("InBcastPkts", IPSTATS_MIB_INBCASTPKTS),
	SNMP_MIB_ITEM("OutBcastPkts", IPSTATS_MIB_OUTBCASTPKTS),
	SNMP_MIB_ITEM("InOctets", IPSTATS_MIB_INOCTETS),
	SNMP_MIB_ITEM("OutOctets", IPSTATS_MIB_OUTOCTETS),
	SNMP_MIB_ITEM("InMcastOctets", IPSTATS_MIB_INMCASTOCTETS),
	SNMP_MIB_ITEM("OutMcastOctets", IPSTATS_MIB_OUTMCASTOCTETS),
	SNMP_MIB_ITEM("InBcastOctets", IPSTATS_MIB_INBCASTOCTETS),
	SNMP_MIB_ITEM("OutBcastOctets", IPSTATS_MIB_OUTBCASTOCTETS),
	/* Non RFC4293 fields */
	SNMP_MIB_ITEM("InCsumErrors", IPSTATS_MIB_CSUMERRORS),
	SNMP_MIB_ITEM("InNoECTPkts", IPSTATS_MIB_NOECTPKTS),
	SNMP_MIB_ITEM("InECT1Pkts", IPSTATS_MIB_ECT1PKTS),
	SNMP_MIB_ITEM("InECT0Pkts", IPSTATS_MIB_ECT0PKTS),
	SNMP_MIB_ITEM("InCEPkts", IPSTATS_MIB_CEPKTS),
	SNMP_MIB_ITEM("ReasmOverlaps", IPSTATS_MIB_REASM_OVERLAPS),
	SNMP_MIB_SENTINEL
};

static const struct {
	const char *name;
	int index;
} icmpmibmap[] = {
	{ "DestUnreachs", ICMP_DEST_UNREACH },
	{ "TimeExcds", ICMP_TIME_EXCEEDED },
	{ "ParmProbs", ICMP_PARAMETERPROB },
	{ "SrcQuenchs", ICMP_SOURCE_QUENCH },
	{ "Redirects", ICMP_REDIRECT },
	{ "Echos", ICMP_ECHO },
	{ "EchoReps", ICMP_ECHOREPLY },
	{ "Timestamps", ICMP_TIMESTAMP },
	{ "TimestampReps", ICMP_TIMESTAMPREPLY },
	{ "AddrMasks", ICMP_ADDRESS },
	{ "AddrMaskReps", ICMP_ADDRESSREPLY },
	{ NULL, 0 }
};


static const struct snmp_mib snmp4_tcp_list[] = {
	SNMP_MIB_ITEM("RtoAlgorithm", TCP_MIB_RTOALGORITHM),
	SNMP_MIB_ITEM("RtoMin", TCP_MIB_RTOMIN),
	SNMP_MIB_ITEM("RtoMax", TCP_MIB_RTOMAX),
	SNMP_MIB_ITEM("MaxConn", TCP_MIB_MAXCONN),
	SNMP_MIB_ITEM("ActiveOpens", TCP_MIB_ACTIVEOPENS),
	SNMP_MIB_ITEM("PassiveOpens", TCP_MIB_PASSIVEOPENS),
	SNMP_MIB_ITEM("AttemptFails", TCP_MIB_ATTEMPTFAILS),
	SNMP_MIB_ITEM("EstabResets", TCP_MIB_ESTABRESETS),
	SNMP_MIB_ITEM("CurrEstab", TCP_MIB_CURRESTAB),
	SNMP_MIB_ITEM("InSegs", TCP_MIB_INSEGS),
	SNMP_MIB_ITEM("OutSegs", TCP_MIB_OUTSEGS),
	SNMP_MIB_ITEM("RetransSegs", TCP_MIB_RETRANSSEGS),
	SNMP_MIB_ITEM("InErrs", TCP_MIB_INERRS),
	SNMP_MIB_ITEM("OutRsts", TCP_MIB_OUTRSTS),
	SNMP_MIB_ITEM("InCsumErrors", TCP_MIB_CSUMERRORS),
	SNMP_MIB_SENTINEL
};

static const struct snmp_mib snmp4_udp_list[] = {
	SNMP_MIB_ITEM("InDatagrams", UDP_MIB_INDATAGRAMS),
	SNMP_MIB_ITEM("NoPorts", UDP_MIB_NOPORTS),
	SNMP_MIB_ITEM("InErrors", UDP_MIB_INERRORS),
	SNMP_MIB_ITEM("OutDatagrams", UDP_MIB_OUTDATAGRAMS),
	SNMP_MIB_ITEM("RcvbufErrors", UDP_MIB_RCVBUFERRORS),
	SNMP_MIB_ITEM("SndbufErrors", UDP_MIB_SNDBUFERRORS),
	SNMP_MIB_ITEM("InCsumErrors", UDP_MIB_CSUMERRORS),
	SNMP_MIB_ITEM("IgnoredMulti", UDP_MIB_IGNOREDMULTI),
	SNMP_MIB_ITEM("MemErrors", UDP_MIB_MEMERRORS),
	SNMP_MIB_SENTINEL
};

static const struct snmp_mib snmp4_net_list[] = {
	SNMP_MIB_ITEM("SyncookiesSent", LINEX_MIB_SYNCOOKIESSENT),
	SNMP_MIB_ITEM("SyncookiesRecv", LINEX_MIB_SYNCOOKIESRECV),
	SNMP_MIB_ITEM("SyncookiesFailed", LINEX_MIB_SYNCOOKIESFAILED),
	SNMP_MIB_ITEM("EmbryonicRsts", LINEX_MIB_EMBRYONICRSTS),
	SNMP_MIB_ITEM("PruneCalled", LINEX_MIB_PRUNECALLED),
	SNMP_MIB_ITEM("RcvPruned", LINEX_MIB_RCVPRUNED),
	SNMP_MIB_ITEM("OfoPruned", LINEX_MIB_OFOPRUNED),
	SNMP_MIB_ITEM("OutOfWindowIcmps", LINEX_MIB_OUTOFWINDOWICMPS),
	SNMP_MIB_ITEM("LockDroppedIcmps", LINEX_MIB_LOCKDROPPEDICMPS),
	SNMP_MIB_ITEM("ArpFilter", LINEX_MIB_ARPFILTER),
	SNMP_MIB_ITEM("TW", LINEX_MIB_TIMEWAITED),
	SNMP_MIB_ITEM("TWRecycled", LINEX_MIB_TIMEWAITRECYCLED),
	SNMP_MIB_ITEM("TWKilled", LINEX_MIB_TIMEWAITKILLED),
	SNMP_MIB_ITEM("PAWSActive", LINEX_MIB_PAWSACTIVEREJECTED),
	SNMP_MIB_ITEM("PAWSEstab", LINEX_MIB_PAWSESTABREJECTED),
	SNMP_MIB_ITEM("DelayedACKs", LINEX_MIB_DELAYEDACKS),
	SNMP_MIB_ITEM("DelayedACKLocked", LINEX_MIB_DELAYEDACKLOCKED),
	SNMP_MIB_ITEM("DelayedACKLost", LINEX_MIB_DELAYEDACKLOST),
	SNMP_MIB_ITEM("ListenOverflows", LINEX_MIB_LISTENOVERFLOWS),
	SNMP_MIB_ITEM("ListenDrops", LINEX_MIB_LISTENDROPS),
	SNMP_MIB_ITEM("TCPHPHits", LINEX_MIB_TCPHPHITS),
	SNMP_MIB_ITEM("TCPPureAcks", LINEX_MIB_TCPPUREACKS),
	SNMP_MIB_ITEM("TCPHPAcks", LINEX_MIB_TCPHPACKS),
	SNMP_MIB_ITEM("TCPRenoRecovery", LINEX_MIB_TCPRENORECOVERY),
	SNMP_MIB_ITEM("TCPSackRecovery", LINEX_MIB_TCPSACKRECOVERY),
	SNMP_MIB_ITEM("TCPSACKReneging", LINEX_MIB_TCPSACKRENEGING),
	SNMP_MIB_ITEM("TCPSACKReorder", LINEX_MIB_TCPSACKREORDER),
	SNMP_MIB_ITEM("TCPRenoReorder", LINEX_MIB_TCPRENOREORDER),
	SNMP_MIB_ITEM("TCPTSReorder", LINEX_MIB_TCPTSREORDER),
	SNMP_MIB_ITEM("TCPFullUndo", LINEX_MIB_TCPFULLUNDO),
	SNMP_MIB_ITEM("TCPPartialUndo", LINEX_MIB_TCPPARTIALUNDO),
	SNMP_MIB_ITEM("TCPDSACKUndo", LINEX_MIB_TCPDSACKUNDO),
	SNMP_MIB_ITEM("TCPLossUndo", LINEX_MIB_TCPLOSSUNDO),
	SNMP_MIB_ITEM("TCPLostRetransmit", LINEX_MIB_TCPLOSTRETRANSMIT),
	SNMP_MIB_ITEM("TCPRenoFailures", LINEX_MIB_TCPRENOFAILURES),
	SNMP_MIB_ITEM("TCPSackFailures", LINEX_MIB_TCPSACKFAILURES),
	SNMP_MIB_ITEM("TCPLossFailures", LINEX_MIB_TCPLOSSFAILURES),
	SNMP_MIB_ITEM("TCPFastRetrans", LINEX_MIB_TCPFASTRETRANS),
	SNMP_MIB_ITEM("TCPSlowStartRetrans", LINEX_MIB_TCPSLOWSTARTRETRANS),
	SNMP_MIB_ITEM("TCPTimeouts", LINEX_MIB_TCPTIMEOUTS),
	SNMP_MIB_ITEM("TCPLossProbes", LINEX_MIB_TCPLOSSPROBES),
	SNMP_MIB_ITEM("TCPLossProbeRecovery", LINEX_MIB_TCPLOSSPROBERECOVERY),
	SNMP_MIB_ITEM("TCPRenoRecoveryFail", LINEX_MIB_TCPRENORECOVERYFAIL),
	SNMP_MIB_ITEM("TCPSackRecoveryFail", LINEX_MIB_TCPSACKRECOVERYFAIL),
	SNMP_MIB_ITEM("TCPRcvCollapsed", LINEX_MIB_TCPRCVCOLLAPSED),
	SNMP_MIB_ITEM("TCPBacklogCoalesce", LINEX_MIB_TCPBACKLOGCOALESCE),
	SNMP_MIB_ITEM("TCPDSACKOldSent", LINEX_MIB_TCPDSACKOLDSENT),
	SNMP_MIB_ITEM("TCPDSACKOfoSent", LINEX_MIB_TCPDSACKOFOSENT),
	SNMP_MIB_ITEM("TCPDSACKRecv", LINEX_MIB_TCPDSACKRECV),
	SNMP_MIB_ITEM("TCPDSACKOfoRecv", LINEX_MIB_TCPDSACKOFORECV),
	SNMP_MIB_ITEM("TCPAbortOnData", LINEX_MIB_TCPABORTONDATA),
	SNMP_MIB_ITEM("TCPAbortOnClose", LINEX_MIB_TCPABORTONCLOSE),
	SNMP_MIB_ITEM("TCPAbortOnMemory", LINEX_MIB_TCPABORTONMEMORY),
	SNMP_MIB_ITEM("TCPAbortOnTimeout", LINEX_MIB_TCPABORTONTIMEOUT),
	SNMP_MIB_ITEM("TCPAbortOnLinger", LINEX_MIB_TCPABORTONLINGER),
	SNMP_MIB_ITEM("TCPAbortFailed", LINEX_MIB_TCPABORTFAILED),
	SNMP_MIB_ITEM("TCPMemoryPressures", LINEX_MIB_TCPMEMORYPRESSURES),
	SNMP_MIB_ITEM("TCPMemoryPressuresChrono", LINEX_MIB_TCPMEMORYPRESSURESCHRONO),
	SNMP_MIB_ITEM("TCPSACKDiscard", LINEX_MIB_TCPSACKDISCARD),
	SNMP_MIB_ITEM("TCPDSACKIgnoredOld", LINEX_MIB_TCPDSACKIGNOREDOLD),
	SNMP_MIB_ITEM("TCPDSACKIgnoredNoUndo", LINEX_MIB_TCPDSACKIGNOREDNOUNDO),
	SNMP_MIB_ITEM("TCPSpuriousRTOs", LINEX_MIB_TCPSPURIOUSRTOS),
	SNMP_MIB_ITEM("TCPMD5NotFound", LINEX_MIB_TCPMD5NOTFOUND),
	SNMP_MIB_ITEM("TCPMD5Unexpected", LINEX_MIB_TCPMD5UNEXPECTED),
	SNMP_MIB_ITEM("TCPMD5Failure", LINEX_MIB_TCPMD5FAILURE),
	SNMP_MIB_ITEM("TCPSackShifted", LINEX_MIB_SACKSHIFTED),
	SNMP_MIB_ITEM("TCPSackMerged", LINEX_MIB_SACKMERGED),
	SNMP_MIB_ITEM("TCPSackShiftFallback", LINEX_MIB_SACKSHIFTFALLBACK),
	SNMP_MIB_ITEM("TCPBacklogDrop", LINEX_MIB_TCPBACKLOGDROP),
	SNMP_MIB_ITEM("PFMemallocDrop", LINEX_MIB_PFMEMALLOCDROP),
	SNMP_MIB_ITEM("TCPMinTTLDrop", LINEX_MIB_TCPMINTTLDROP),
	SNMP_MIB_ITEM("TCPDeferAcceptDrop", LINEX_MIB_TCPDEFERACCEPTDROP),
	SNMP_MIB_ITEM("IPReversePathFilter", LINEX_MIB_IPRPFILTER),
	SNMP_MIB_ITEM("TCPTimeWaitOverflow", LINEX_MIB_TCPTIMEWAITOVERFLOW),
	SNMP_MIB_ITEM("TCPReqQFullDoCookies", LINEX_MIB_TCPREQQFULLDOCOOKIES),
	SNMP_MIB_ITEM("TCPReqQFullDrop", LINEX_MIB_TCPREQQFULLDROP),
	SNMP_MIB_ITEM("TCPRetransFail", LINEX_MIB_TCPRETRANSFAIL),
	SNMP_MIB_ITEM("TCPRcvCoalesce", LINEX_MIB_TCPRCVCOALESCE),
	SNMP_MIB_ITEM("TCPOFOQueue", LINEX_MIB_TCPOFOQUEUE),
	SNMP_MIB_ITEM("TCPOFODrop", LINEX_MIB_TCPOFODROP),
	SNMP_MIB_ITEM("TCPOFOMerge", LINEX_MIB_TCPOFOMERGE),
	SNMP_MIB_ITEM("TCPChallengeACK", LINEX_MIB_TCPCHALLENGEACK),
	SNMP_MIB_ITEM("TCPSYNChallenge", LINEX_MIB_TCPSYNCHALLENGE),
	SNMP_MIB_ITEM("TCPFastOpenActive", LINEX_MIB_TCPFASTOPENACTIVE),
	SNMP_MIB_ITEM("TCPFastOpenActiveFail", LINEX_MIB_TCPFASTOPENACTIVEFAIL),
	SNMP_MIB_ITEM("TCPFastOpenPassive", LINEX_MIB_TCPFASTOPENPASSIVE),
	SNMP_MIB_ITEM("TCPFastOpenPassiveFail", LINEX_MIB_TCPFASTOPENPASSIVEFAIL),
	SNMP_MIB_ITEM("TCPFastOpenListenOverflow", LINEX_MIB_TCPFASTOPENLISTENOVERFLOW),
	SNMP_MIB_ITEM("TCPFastOpenCookieReqd", LINEX_MIB_TCPFASTOPENCOOKIEREQD),
	SNMP_MIB_ITEM("TCPFastOpenBlackhole", LINEX_MIB_TCPFASTOPENBLACKHOLE),
	SNMP_MIB_ITEM("TCPSpuriousRtxHostQueues", LINEX_MIB_TCPSPURIOUS_RTX_HOSTQUEUES),
	SNMP_MIB_ITEM("BusyPollRxPackets", LINEX_MIB_BUSYPOLLRXPACKETS),
	SNMP_MIB_ITEM("TCPAutoCorking", LINEX_MIB_TCPAUTOCORKING),
	SNMP_MIB_ITEM("TCPFromZeroWindowAdv", LINEX_MIB_TCPFROMZEROWINDOWADV),
	SNMP_MIB_ITEM("TCPToZeroWindowAdv", LINEX_MIB_TCPTOZEROWINDOWADV),
	SNMP_MIB_ITEM("TCPWantZeroWindowAdv", LINEX_MIB_TCPWANTZEROWINDOWADV),
	SNMP_MIB_ITEM("TCPSynRetrans", LINEX_MIB_TCPSYNRETRANS),
	SNMP_MIB_ITEM("TCPOrigDataSent", LINEX_MIB_TCPORIGDATASENT),
	SNMP_MIB_ITEM("TCPHystartTrainDetect", LINEX_MIB_TCPHYSTARTTRAINDETECT),
	SNMP_MIB_ITEM("TCPHystartTrainCwnd", LINEX_MIB_TCPHYSTARTTRAINCWND),
	SNMP_MIB_ITEM("TCPHystartDelayDetect", LINEX_MIB_TCPHYSTARTDELAYDETECT),
	SNMP_MIB_ITEM("TCPHystartDelayCwnd", LINEX_MIB_TCPHYSTARTDELAYCWND),
	SNMP_MIB_ITEM("TCPACKSkippedSynRecv", LINEX_MIB_TCPACKSKIPPEDSYNRECV),
	SNMP_MIB_ITEM("TCPACKSkippedPAWS", LINEX_MIB_TCPACKSKIPPEDPAWS),
	SNMP_MIB_ITEM("TCPACKSkippedSeq", LINEX_MIB_TCPACKSKIPPEDSEQ),
	SNMP_MIB_ITEM("TCPACKSkippedFinWait2", LINEX_MIB_TCPACKSKIPPEDFINWAIT2),
	SNMP_MIB_ITEM("TCPACKSkippedTimeWait", LINEX_MIB_TCPACKSKIPPEDTIMEWAIT),
	SNMP_MIB_ITEM("TCPACKSkippedChallenge", LINEX_MIB_TCPACKSKIPPEDCHALLENGE),
	SNMP_MIB_ITEM("TCPWinProbe", LINEX_MIB_TCPWINPROBE),
	SNMP_MIB_ITEM("TCPKeepAlive", LINEX_MIB_TCPKEEPALIVE),
	SNMP_MIB_ITEM("TCPMTUPFail", LINEX_MIB_TCPMTUPFAIL),
	SNMP_MIB_ITEM("TCPMTUPSuccess", LINEX_MIB_TCPMTUPSUCCESS),
	SNMP_MIB_ITEM("TCPDelivered", LINEX_MIB_TCPDELIVERED),
	SNMP_MIB_ITEM("TCPDeliveredCE", LINEX_MIB_TCPDELIVEREDCE),
	SNMP_MIB_ITEM("TCPAckCompressed", LINEX_MIB_TCPACKCOMPRESSED),
	SNMP_MIB_ITEM("TCPZeroWindowDrop", LINEX_MIB_TCPZEROWINDOWDROP),
	SNMP_MIB_ITEM("TCPRcvQDrop", LINEX_MIB_TCPRCVQDROP),
	SNMP_MIB_ITEM("TCPWqueueTooBig", LINEX_MIB_TCPWQUEUETOOBIG),
	SNMP_MIB_ITEM("TCPFastOpenPassiveAltKey", LINEX_MIB_TCPFASTOPENPASSIVEALTKEY),
	SNMP_MIB_ITEM("TcpTimeoutRehash", LINEX_MIB_TCPTIMEOUTREHASH),
	SNMP_MIB_ITEM("TcpDuplicateDataRehash", LINEX_MIB_TCPDUPLICATEDATAREHASH),
	SNMP_MIB_ITEM("TCPDSACKRecvSegs", LINEX_MIB_TCPDSACKRECVSEGS),
	SNMP_MIB_ITEM("TCPDSACKIgnoredDubious", LINEX_MIB_TCPDSACKIGNOREDDUBIOUS),
	SNMP_MIB_ITEM("TCPMigrateReqSuccess", LINEX_MIB_TCPMIGRATEREQSUCCESS),
	SNMP_MIB_ITEM("TCPMigrateReqFailure", LINEX_MIB_TCPMIGRATEREQFAILURE),
	SNMP_MIB_ITEM("TCPPLBRehash", LINEX_MIB_TCPPLBREHASH),
	SNMP_MIB_SENTINEL
};

static void icmpmsg_put_line(struct seq_file *seq, unsigned long *vals,
			     unsigned short *type, int count)
{
	int j;

	if (count) {
		seq_puts(seq, "\nIcmpMsg:");
		for (j = 0; j < count; ++j)
			seq_printf(seq, " %sType%u",
				type[j] & 0x100 ? "Out" : "In",
				type[j] & 0xff);
		seq_puts(seq, "\nIcmpMsg:");
		for (j = 0; j < count; ++j)
			seq_printf(seq, " %lu", vals[j]);
	}
}

static void icmpmsg_put(struct seq_file *seq)
{
#define PERLINE	16

	int i, count;
	unsigned short type[PERLINE];
	unsigned long vals[PERLINE], val;
	struct net *net = seq->private;

	count = 0;
	for (i = 0; i < ICMPMSG_MIB_MAX; i++) {
		val = atomic_long_read(&net->mib.icmpmsg_statistics->mibs[i]);
		if (val) {
			type[count] = i;
			vals[count++] = val;
		}
		if (count == PERLINE) {
			icmpmsg_put_line(seq, vals, type, count);
			count = 0;
		}
	}
	icmpmsg_put_line(seq, vals, type, count);

#undef PERLINE
}

static void icmp_put(struct seq_file *seq)
{
	int i;
	struct net *net = seq->private;
	atomic_long_t *ptr = net->mib.icmpmsg_statistics->mibs;

	seq_puts(seq, "\nIcmp: InMsgs InErrors InCsumErrors");
	for (i = 0; icmpmibmap[i].name; i++)
		seq_printf(seq, " In%s", icmpmibmap[i].name);
	seq_puts(seq, " OutMsgs OutErrors OutRateLimitGlobal OutRateLimitHost");
	for (i = 0; icmpmibmap[i].name; i++)
		seq_printf(seq, " Out%s", icmpmibmap[i].name);
	seq_printf(seq, "\nIcmp: %lu %lu %lu",
		snmp_fold_field(net->mib.icmp_statistics, ICMP_MIB_INMSGS),
		snmp_fold_field(net->mib.icmp_statistics, ICMP_MIB_INERRORS),
		snmp_fold_field(net->mib.icmp_statistics, ICMP_MIB_CSUMERRORS));
	for (i = 0; icmpmibmap[i].name; i++)
		seq_printf(seq, " %lu",
			   atomic_long_read(ptr + icmpmibmap[i].index));
	seq_printf(seq, " %lu %lu %lu %lu",
		snmp_fold_field(net->mib.icmp_statistics, ICMP_MIB_OUTMSGS),
		snmp_fold_field(net->mib.icmp_statistics, ICMP_MIB_OUTERRORS),
		snmp_fold_field(net->mib.icmp_statistics, ICMP_MIB_RATELIMITGLOBAL),
		snmp_fold_field(net->mib.icmp_statistics, ICMP_MIB_RATELIMITHOST));
	for (i = 0; icmpmibmap[i].name; i++)
		seq_printf(seq, " %lu",
			   atomic_long_read(ptr + (icmpmibmap[i].index | 0x100)));
}

/*
 *	Called from the PROCfs module. This outputs /proc/net/snmp.
 */
static int snmp_seq_show_ipstats(struct seq_file *seq, void *v)
{
	struct net *net = seq->private;
	u64 buff64[IPSTATS_MIB_MAX];
	int i;

	memset(buff64, 0, IPSTATS_MIB_MAX * sizeof(u64));

	seq_puts(seq, "Ip: Forwarding DefaultTTL");
	for (i = 0; snmp4_ipstats_list[i].name; i++)
		seq_printf(seq, " %s", snmp4_ipstats_list[i].name);

	seq_printf(seq, "\nIp: %d %d",
		   IPV4_DEVCONF_ALL(net, FORWARDING) ? 1 : 2,
		   READ_ONCE(net->ipv4.sysctl_ip_default_ttl));

	BUILD_BUG_ON(offsetof(struct ipstats_mib, mibs) != 0);
	snmp_get_cpu_field64_batch(buff64, snmp4_ipstats_list,
				   net->mib.ip_statistics,
				   offsetof(struct ipstats_mib, syncp));
	for (i = 0; snmp4_ipstats_list[i].name; i++)
		seq_printf(seq, " %llu", buff64[i]);

	return 0;
}

static int snmp_seq_show_tcp_udp(struct seq_file *seq, void *v)
{
	unsigned long buff[TCPUDP_MIB_MAX];
	struct net *net = seq->private;
	int i;

	memset(buff, 0, TCPUDP_MIB_MAX * sizeof(unsigned long));

	seq_puts(seq, "\nTcp:");
	for (i = 0; snmp4_tcp_list[i].name; i++)
		seq_printf(seq, " %s", snmp4_tcp_list[i].name);

	seq_puts(seq, "\nTcp:");
	snmp_get_cpu_field_batch(buff, snmp4_tcp_list,
				 net->mib.tcp_statistics);
	for (i = 0; snmp4_tcp_list[i].name; i++) {
		/* MaxConn field is signed, RFC 2012 */
		if (snmp4_tcp_list[i].entry == TCP_MIB_MAXCONN)
			seq_printf(seq, " %ld", buff[i]);
		else
			seq_printf(seq, " %lu", buff[i]);
	}

	memset(buff, 0, TCPUDP_MIB_MAX * sizeof(unsigned long));

	snmp_get_cpu_field_batch(buff, snmp4_udp_list,
				 net->mib.udp_statistics);
	seq_puts(seq, "\nUdp:");
	for (i = 0; snmp4_udp_list[i].name; i++)
		seq_printf(seq, " %s", snmp4_udp_list[i].name);
	seq_puts(seq, "\nUdp:");
	for (i = 0; snmp4_udp_list[i].name; i++)
		seq_printf(seq, " %lu", buff[i]);

	memset(buff, 0, TCPUDP_MIB_MAX * sizeof(unsigned long));

	/* the UDP and UDP-Lite MIBs are the same */
	seq_puts(seq, "\nUdpLite:");
	snmp_get_cpu_field_batch(buff, snmp4_udp_list,
				 net->mib.udplite_statistics);
	for (i = 0; snmp4_udp_list[i].name; i++)
		seq_printf(seq, " %s", snmp4_udp_list[i].name);
	seq_puts(seq, "\nUdpLite:");
	for (i = 0; snmp4_udp_list[i].name; i++)
		seq_printf(seq, " %lu", buff[i]);

	seq_putc(seq, '\n');
	return 0;
}

static int snmp_seq_show(struct seq_file *seq, void *v)
{
	snmp_seq_show_ipstats(seq, v);

	icmp_put(seq);	/* RFC 2011 compatibility */
	icmpmsg_put(seq);

	snmp_seq_show_tcp_udp(seq, v);

	return 0;
}

/*
 *	Output /proc/net/netstat
 */
static int netstat_seq_show(struct seq_file *seq, void *v)
{
	const int ip_cnt = ARRAY_SIZE(snmp4_ipextstats_list) - 1;
	const int tcp_cnt = ARRAY_SIZE(snmp4_net_list) - 1;
	struct net *net = seq->private;
	unsigned long *buff;
	int i;

	seq_puts(seq, "TcpExt:");
	for (i = 0; i < tcp_cnt; i++)
		seq_printf(seq, " %s", snmp4_net_list[i].name);

	seq_puts(seq, "\nTcpExt:");
	buff = kzalloc(max(tcp_cnt * sizeof(long), ip_cnt * sizeof(u64)),
		       GFP_KERNEL);
	if (buff) {
		snmp_get_cpu_field_batch(buff, snmp4_net_list,
					 net->mib.net_statistics);
		for (i = 0; i < tcp_cnt; i++)
			seq_printf(seq, " %lu", buff[i]);
	} else {
		for (i = 0; i < tcp_cnt; i++)
			seq_printf(seq, " %lu",
				   snmp_fold_field(net->mib.net_statistics,
						   snmp4_net_list[i].entry));
	}
	seq_puts(seq, "\nIpExt:");
	for (i = 0; i < ip_cnt; i++)
		seq_printf(seq, " %s", snmp4_ipextstats_list[i].name);

	seq_puts(seq, "\nIpExt:");
	if (buff) {
		u64 *buff64 = (u64 *)buff;

		memset(buff64, 0, ip_cnt * sizeof(u64));
		snmp_get_cpu_field64_batch(buff64, snmp4_ipextstats_list,
					   net->mib.ip_statistics,
					   offsetof(struct ipstats_mib, syncp));
		for (i = 0; i < ip_cnt; i++)
			seq_printf(seq, " %llu", buff64[i]);
	} else {
		for (i = 0; i < ip_cnt; i++)
			seq_printf(seq, " %llu",
				   snmp_fold_field64(net->mib.ip_statistics,
						     snmp4_ipextstats_list[i].entry,
						     offsetof(struct ipstats_mib, syncp)));
	}
	kfree(buff);
	seq_putc(seq, '\n');
	mptcp_seq_show(seq);
	return 0;
}

static __net_init int ip_proc_init_net(struct net *net)
{
	if (!proc_create_net_single("sockstat", 0444, net->proc_net,
			sockstat_seq_show, NULL))
		goto out_sockstat;
	if (!proc_create_net_single("netstat", 0444, net->proc_net,
			netstat_seq_show, NULL))
		goto out_netstat;
	if (!proc_create_net_single("snmp", 0444, net->proc_net, snmp_seq_show,
			NULL))
		goto out_snmp;

	return 0;

out_snmp:
	remove_proc_entry("netstat", net->proc_net);
out_netstat:
	remove_proc_entry("sockstat", net->proc_net);
out_sockstat:
	return -ENOMEM;
}

static __net_exit void ip_proc_exit_net(struct net *net)
{
	remove_proc_entry("snmp", net->proc_net);
	remove_proc_entry("netstat", net->proc_net);
	remove_proc_entry("sockstat", net->proc_net);
}

static __net_initdata struct pernet_operations ip_proc_ops = {
	.init = ip_proc_init_net,
	.exit = ip_proc_exit_net,
};

int __init ip_misc_proc_init(void)
{
	return register_pernet_subsys(&ip_proc_ops);
}
