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
  printf( "TSCH status:\n");
//  NETSTACK_ROUTING.root_start();
//  tsch_set_coordinator(1);  
  //add your code
  ctimer_set(&timer_ctimer, CLOCK_SECOND, ctimer_callback, NULL);
  etimer_set(&et,CLOCK_SECOND * 60);//set a timer 5s
  while(1){
    PROCESS_WAIT_EVENT();
    if(etimer_expired(&et)){
    last_rssi=sicslowpan_get_last_rssi();
    printf("[%ld]last rssi=%ddbm,packet_rssi=%ddbm\n",sys_time,last_rssi,packet_rssi);
    etimer_restart(&et);
    }
}  
  PROCESS_END();
}


