#include "contiki.h" /* Main include file for OS-specific modules. */
#include <stdio.h> /* For printf. */
#include "tsch.h"
#include "sicslowpan.h"
#include "sys/energest.h"
#include "ieee-addr.h"
#include "uip.h"
#include "uip-icmp6.h"
#include "packetbuf.h"
PROCESS(test_proc, "Test process");
static struct ctimer timer_ctimer;
unsigned long sys_time=0;
unsigned long join_time=0;
int last_rssi=0;
//AUTOSTART_PROCESSES(&test_proc);
void
ctimer_callback(void *ptr)
{
  /* rearm the ctimer */
  ctimer_reset(&timer_ctimer);
  sys_time++;
  if(tsch_is_associated!=0&&join_time==0){
    join_time = sys_time;
    printf("join_time=%ld\n",join_time);
  }
 // printf("CTimer callback called\n");
}
/*---------------------------------------------------------------------------*/
static inline unsigned long
to_seconds(uint64_t time)
{
  return (unsigned long)(time / ENERGEST_SECOND);
}
PROCESS_THREAD(test_proc, ev, data)
{
  static struct etimer et;
  int16_t packet_rssi=0;
//  uint8_t i=0;
  PROCESS_BEGIN();
  printf("User appcation Add your code here!\n");
  printf("Compile Time = %s %s\n",__DATE__,__TIME__);
  printf("auto:adams\n");
  printf("version:v0.1\n");
//  NETSTACK_ROUTING.root_start();
//  tsch_set_coordinator(1);  
  //add your code
  ctimer_set(&timer_ctimer, CLOCK_SECOND, ctimer_callback, NULL);
  etimer_set(&et,CLOCK_SECOND * 60);//set a timer 5s
  while(1){
    PROCESS_WAIT_EVENT();
    if(etimer_expired(&et)){
    last_rssi=sicslowpan_get_last_rssi();
//    packet_rssi = packetbuf_attr(PACKETBUF_ATTR_RSSI);
    printf("[%ld]last rssi=%ddbm,packet_rssi=%ddbm\n",sys_time,last_rssi,packet_rssi);

/*
     * Update all energest times. Should always be called before energest
     * times are read.
     */
/*    energest_flush();

    printf("\nEnergest:\n");
    printf(" CPU          %4lus LPM      %4lus DEEP LPM %4lus  Total time %lus\n",
           to_seconds(energest_type_time(ENERGEST_TYPE_CPU)),
           to_seconds(energest_type_time(ENERGEST_TYPE_LPM)),
           to_seconds(energest_type_time(ENERGEST_TYPE_DEEP_LPM)),
           to_seconds(ENERGEST_GET_TOTAL_TIME()));
    printf(" Radio LISTEN %4lus TRANSMIT %4lus OFF      %4lus\n",
           to_seconds(energest_type_time(ENERGEST_TYPE_LISTEN)),
           to_seconds(energest_type_time(ENERGEST_TYPE_TRANSMIT)),
           to_seconds(ENERGEST_GET_TOTAL_TIME()
                      - energest_type_time(ENERGEST_TYPE_TRANSMIT)
                      - energest_type_time(ENERGEST_TYPE_LISTEN)));
*/    
//    ieee_addr_cpy_to(addr,10);
//    addr[0]=0xfd;
//    printf("ieee addr=");

//    printf("assoiiated_time=%ld\n",sys_time);
    etimer_restart(&et);
    }
}  
  PROCESS_END();
}


