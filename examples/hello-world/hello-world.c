/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
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
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"

#include <stdio.h> /* For printf() */
#include "dev/leds.h"/*For leds_ctl()*/
#include "dev/battery-sensor.h"
#include "net/netstack.h"
#include "sicslowpan.h"
#include "app.h"
#include "sys/stack-check.h"
#include "net/routing/routing.h"
#include "batmon-sensor.h"
#define CC26XX_WEB_DEMO_CONVERTED_LEN        12
typedef struct cc26xx_web_demo_sensor_reading {
  struct cc26xx_web_demo_sensor_reading *next;
  int raw;
  int last;
  const char *descr;
  const char *xml_element;
  const char *form_field;
  char *units;
  uint8_t type;
  uint8_t publish;
  uint8_t changed;
  char converted[CC26XX_WEB_DEMO_CONVERTED_LEN];
} cc26xx_web_demo_sensor_reading_t;
/*---------------------------------------------------------------------------*/
/* A cache of sensor values. Updated periodically or upon key press */
LIST(sensor_list);
/*---------------------------------------------------------------------------*/
/* The objects representing sensors used in this demo */
#define DEMO_SENSOR(name, type, descr, xml_element, form_field, units) \
  cc26xx_web_demo_sensor_reading_t name##_reading = \
  { NULL, 0, 0, descr, xml_element, form_field, units, type, 1, 1 }
#define CC26XX_WEB_DEMO_SENSOR_BATMON_TEMP   0
#define CC26XX_WEB_DEMO_SENSOR_BATMON_VOLT   1
#define CC26XX_WEB_DEMO_UNIT_TEMP     "C"
#define CC26XX_WEB_DEMO_UNIT_VOLT     "mV"
/* CC26xx sensors */
DEMO_SENSOR(batmon_temp, CC26XX_WEB_DEMO_SENSOR_BATMON_TEMP,
            "Battery Temp", "battery-temp", "batmon_temp",
            CC26XX_WEB_DEMO_UNIT_TEMP);
DEMO_SENSOR(batmon_volt, CC26XX_WEB_DEMO_SENSOR_BATMON_VOLT,
            "Battery Volt", "battery-volt", "batmon_volt",
            CC26XX_WEB_DEMO_UNIT_VOLT);
static void
init_sensors(void)
{

  list_add(sensor_list, &batmon_temp_reading);
  list_add(sensor_list, &batmon_volt_reading);

#if CC26XX_WEB_DEMO_ADC_DEMO
  list_add(sensor_list, &adc_dio23_reading);
#endif

  SENSORS_ACTIVATE(batmon_sensor);

#if BOARD_SENSORTAG
  list_add(sensor_list, &bmp_pres_reading);
  list_add(sensor_list, &bmp_temp_reading);

  list_add(sensor_list, &tmp_obj_reading);
  list_add(sensor_list, &tmp_amb_reading);

  list_add(sensor_list, &opt_reading);

  list_add(sensor_list, &hdc_hum_reading);
  list_add(sensor_list, &hdc_temp_reading);

  list_add(sensor_list, &mpu_acc_x_reading);
  list_add(sensor_list, &mpu_acc_y_reading);
  list_add(sensor_list, &mpu_acc_z_reading);
  list_add(sensor_list, &mpu_gyro_x_reading);
  list_add(sensor_list, &mpu_gyro_y_reading);
  list_add(sensor_list, &mpu_gyro_z_reading);
#endif
}
static void
get_sync_sensor_readings(void)
{
  int value;

  printf("-----------------------------------------\n");

  value = batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP);
  printf("Bat: Temp=%d C\n", value);

  value = batmon_sensor.value(BATMON_SENSOR_TYPE_VOLT);
  printf("Bat: Volt=%d mV\n", (value * 125) >> 5);
  return;
}
/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();
  init_sensors();

  /* Setup a periodic timer that expires after 10 seconds. */
  etimer_set(&timer, CLOCK_SECOND * 10);

  while(1) {

	get_sync_sensor_readings();
    printf("Hello, world\n");

    /* Wait for the periodic timer to expire and then restart the timer. */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    etimer_reset(&timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
