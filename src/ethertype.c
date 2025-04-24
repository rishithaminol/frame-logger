/*
 * This file is part of frame-logger project.
 * Copyright (C) 2025 Rishitha Minol
 *
 * This file is dual-licensed:
 *
 * - Under the GNU General Public License v3.0 or later (GPLv3),
 *   as published by the Free Software Foundation; and
 *
 * - Under the Business Source License 1.1 (BSL),
 *   as published by MariaDB Corporation.
 *
 * You may choose either license for use, subject to the terms below.
 *
 * GPLv3: See the LICENSE.gpl file in the project root or
 *        <https://www.gnu.org/licenses/gpl-3.0.html>
 *
 * BSL:  See the LICENSE.bsl file in the project root or
 *       <https://mariadb.com/bsl11>
 *
 * Under the BSL, production/commercial use of this file is restricted
 * until April 23, 2028. After that date, this file will be permanently
 * licensed under the terms of GPLv3.
 *
 * For licensing inquiries, contact: rishithaminol@gmail.com
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "ethertype.h"

static fl_eth_type_t ether_type_list[] = {
    {0, "UNKNOWN", "UNKNOWN", 0, 0, 0},
    {0x0800, "Internet Protocol version 4", "IPv4", 0, 0, 0},
    {0x0806, "Address Resolution Protocol", "ARP", 0, 0, 0},
    {0x0842, "Wake-on-LAN[8]", "WOL", 0, 0, 0},
    {0x2000, "Cisco Discovery Protocol", "CDP", 0, 0, 0},
    {0x22EA, "Stream Reservation Protocol", "SRP", 0, 0, 0},
    {0x22F0, "Audio Video Transport Protocol", "AVTP", 0, 0, 0},
    {0x22F3, "IETF TRILL Protocol", "TRILL", 0, 0, 0},
    {0x6002, "DEC MOP RC", "MOP", 0, 0, 0},
    {0x6003, "DECnet Phase IV, DNA Routing", "DECNET", 0, 0, 0},
    {0x6004, "DEC LAT", "DECLAT", 0, 0, 0},
    {0x8035, "Reverse Address Resolution Protocol", "RARP", 0, 0, 0},
    {0x809B, "AppleTalk (EtherTalk)", "AppleTalk", 0, 0, 0},
    {0x80F3, "AppleTalk Address Resolution Protocol", "AARP", 0, 0, 0},
    {0x8100, "VLAN-tagged frame (IEEE 802.1Q) and Shortest Path Bridging IEEE 802.1aq with NNI compatibility[9]", "VLAN", 0, 0, 0},
    {0x8102, "Simple Loop Prevention Protocol", "SLPP", 0, 0, 0},
    {0x8103, "Virtual Link Aggregation Control Protocol", "VLACP", 0, 0, 0},
    {0x8137, "IPX", "IPX", 0, 0, 0},
    {0x8204, "QNX Qnet", "QNX_Qnet", 0, 0, 0},
    {0x86DD, "Internet Protocol Version 6", "IPv6", 0, 0, 0},
    {0x8808, "Ethernet flow control", "ETHFLOW", 0, 0, 0},
    {0x8809, "Ethernet Slow Protocols[10] such as the Link Aggregation Control Protocol", "LACP", 0, 0, 0},
    {0x8819, "CobraNet", "CobraNet", 0, 0, 0},
    {0x8847, "MPLS unicast", "MPLS_UNI", 0, 0, 0},
    {0x8848, "MPLS multicast", "MPLS_MULTI", 0, 0, 0},
    {0x8863, "PPPoE Discovery Stage", "PPPoE", 0, 0, 0},
    {0x8864, "PPPoE Session Stage", "PPPoE", 0, 0, 0},
    {0x887B, "HomePlug 1.0 MME", "HomePlug", 0, 0, 0},
    {0x888E, "EAP over LAN (IEEE 802.1X)", "VAL", 0, 0, 0},
    {0x8892, "PROFINET Protocol", "PROFINET", 0, 0, 0},
    {0x889A, "HyperSCSI (SCSI over Ethernet)", "HyperSCSI", 0, 0, 0},
    {0x88A2, "ATA over Ethernet", "VAL", 0, 0, 0},
    {0x88A4, "EtherCAT Protocol", "VAL", 0, 0, 0},
    {0x88A8, "Service VLAN tag identifier (S-Tag) on Q-in-Q tunnel", "VAL", 0, 0, 0},
    {0x88AB, "Ethernet Powerlink[citation needed]", "VAL", 0, 0, 0},
    {0x88B8, "GOOSE (Generic Object Oriented Substation event)", "VAL", 0, 0, 0},
    {0x88B9, "GSE (Generic Substation Events) Management Services", "VAL", 0, 0, 0},
    {0x88BA, "SV (Sampled Value Transmission)", "VAL", 0, 0, 0},
    {0x88BF, "MikroTik RoMON (unofficial)", "MikroTik", 0, 0, 0},
    {0x88CC, "Link Layer Discovery Protocol", "LLDP", 0, 0, 0},
    {0x88CD, "SERCOS III", "VAL", 0, 0, 0},
    {0x88E1, "HomePlug Green PHY", "VAL", 0, 0, 0},
    {0x88E3, "Media Redundancy Protocol (IEC62439-2)", "VAL", 0, 0, 0},
    {0x88E5, "IEEE 802.1AE MAC security", "MACsec", 0, 0, 0},
    {0x88E7, "Provider Backbone Bridges (IEEE 802.1ah)", "PBB", 0, 0, 0},
    {0x88F7, "Precision Time Protocol over IEEE 802.3 Ethernet", "PTP", 0, 0, 0},
    {0x88F8, "NC-SI", "VAL", 0, 0, 0},
    {0x88FB, "Parallel Redundancy Protocol", "PRP", 0, 0, 0},
    {0x8902, "IEEE 802.1ag Connectivity Fault Management (CFM) Protocol / ITU-T Recommendation Y.1731 (OAM)", "VAL", 0, 0, 0},
    {0x8906, "Fibre Channel over Ethernet", "FCoE", 0, 0, 0},
    {0x8914, "FCoE Initialization Protocol", "VAL", 0, 0, 0},
    {0x8915, "RDMA over Converged Ethernet", "RoCE", 0, 0, 0},
    {0x891D, "TTEthernet Protocol Control Frame", "TTE", 0, 0, 0},
    {0x893a, "1905.1 IEEE Protocol", "VAL", 0, 0, 0},
    {0x892F, "High-availability Seamless Redundancy", "HSR", 0, 0, 0},
    {0x9000, "Ethernet Configuration Testing Protocol[11]", "VAL", 0, 0, 0},
    {0xF1C1, "Redundancy Tag (IEEE 802.1CB Frame Replication and Elimination for Reliability) ", "VAL", 0, 0, 0},
    {0, "Berkeley Trailer encapsulation for IP", "BTEIP", 1, 0x1001, 0x100F}
};

/**
 * @brief Return pointer to \ref fl_eth_type_t type structure which
 *        contains high level data about the given Ethernet Type.
 * 
 * @param[in] eth_type_ Ethernet Type number extracted from packet header
 * @todo Improvise for loop logic and reduce \ref fl_eth_type_t variables.
*/
fl_eth_type_t *get_fl_eth_type(u_int16_t eth_type_)
{
    int i;
    int len = sizeof(ether_type_list) / sizeof(ether_type_list[0]);

    for (i = 0; i < len; i++) {
        if (ether_type_list[i].range != 0) {
            if (ether_type_list[i].type_value_start <= eth_type_ &&
                eth_type_ <= ether_type_list[i].type_value_end)
                return &ether_type_list[i];
        } else {
            if (eth_type_ == ether_type_list[i].type_value)
                return &ether_type_list[i];
        }
    }

    return &ether_type_list[0];
}
