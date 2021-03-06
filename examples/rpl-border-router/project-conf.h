/*
 * Copyright (c) 2010, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_


/* IEEE802.15.4 PANID */
#define IEEE802154_CONF_PANID 0xcccc
#define IEEE802154_CONF_DEFAULT_CHANNEL 5

#define RPL_CONF_DEFAULT_LIFETIME 3

#define LOG_CONF_LEVEL_RPL                         LOG_LEVEL_INFO

#define RPL_CONF_WITH_DAO_ACK                 0
/*
#define LOG_CONF_LEVEL_RPL                         LOG_LEVEL_INFO
#define LOG_CONF_LEVEL_TCPIP                       LOG_LEVEL_INFO
#define LOG_CONF_LEVEL_IPV6                        LOG_LEVEL_INFO
#define LOG_CONF_LEVEL_6LOWPAN                     LOG_LEVEL_INFO
#define LOG_CONF_LEVEL_MAC                         LOG_LEVEL_INFO
#define LOG_CONF_LEVEL_FRAMER                      LOG_LEVEL_INFO
*/
//#define LOG_CONF_LEVEL_MAIN                      LOG_LEVEL_DBG

//#define RPL_CONF_DEFAULT_LIFETIME_UNIT 5

//

//#define RPL_CONF_DAO_DELAY  (CLOCK_SECOND * 1)
//#define RPL_CONF_DAO_RETRANSMISSION_TIMEOUT (CLOCK_SECOND * 1)
//#define RPL_CONF_TRICKLE_REFRESH_DAO_ROUTES 1
//#define RPL_CONF_DIO_INTERVAL_MIN 0

//#define RPL_CONF_DIS_INTERVAL 10
/*
#define RPL_CONF_DAO_DELAY        (1 * CLOCK_SECOND)

#define RPL_CONF_DELAY_BEFORE_LEAVING  (1 * CLOCK_SECOND)
#define RPL_CONF_DEFAULT_LIFETIME_UNIT 10
#define RPL_CONF_DEFAULT_LIFETIME 5
#define RPL_CONF_DIS_INTERVAL (10 * CLOCK_SECOND)
#define RPL_CONF_PROBING_INTERVAL (10 * CLOCK_SECOND)
*/
#ifndef WEBSERVER_CONF_CFS_CONNS
#define WEBSERVER_CONF_CFS_CONNS 2
#endif

#ifndef BORDER_ROUTER_CONF_WEBSERVER
#define BORDER_ROUTER_CONF_WEBSERVER 1
#endif

#if BORDER_ROUTER_CONF_WEBSERVER
#define UIP_CONF_TCP 1

#endif

/*
#define LOG_CONF_LEVEL_RPL                         LOG_LEVEL_INFO
#define LOG_CONF_LEVEL_TCPIP                       LOG_LEVEL_INFO
#define LOG_CONF_LEVEL_IPV6                        LOG_LEVEL_DBG
#define LOG_CONF_LEVEL_6LOWPAN                     LOG_LEVEL_INFO
#define LOG_CONF_LEVEL_MAC                         LOG_LEVEL_INFO
#define LOG_CONF_LEVEL_FRAMER                      LOG_LEVEL_INFO

*/
#endif /* PROJECT_CONF_H_ */
