#include <linux/input.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/*
 * Finding files. Look in /proc/bus/input/devices

I: Bus=0003 Vendor=1532 Product=0101 Version=0100
N: Name="Razer Razer Copperhead Laser Mouse"
P: Phys=usb-0000:00:14.0-3.2/input0
S: Sysfs=/devices/pci0000:00/0000:00:14.0/usb1/1-3/1-3.2/1-3.2:1.0/input/input2
U: Uniq=
H: Handlers=mouse0 event2 
B: PROP=0
B: EV=17
B: KEY=ff0000 0 0 0 0
B: REL=103
B: MSC=10

I: Bus=0003 Vendor=1532 Product=0101 Version=0001
N: Name="Razer Razer Copperhead Laser Mouse"
P: Phys=usb-0000:00:14.0-3.2/input1
S: Sysfs=/devices/pci0000:00/0000:00:14.0/usb1/1-3/1-3.2/1-3.2:1.1/input/input3
U: Uniq=
H: Handlers=sysrq kbd event3 
B: PROP=0
B: EV=100013
B: KEY=1000000000007 ff9f207ac14057ff febeffdfffefffff fffffffffffffffe
B: MSC=10

*/

int main()
{
  FILE *st;
  struct input_event ev;
  const char *code = "X";
  const char *type;

  st = fopen("/dev/input/event2", "r");
  if (!st) {
	perror("fopen");
	exit(1);
  }

  for (;;) {
	assert(1 == fread(&ev, sizeof(struct input_event), 1, st));

	switch (ev.type) {
#define X(Y) case Y: type = #Y; break;
	  X(EV_SYN);
	  X(EV_KEY);
	  X(EV_REL);
	  X(EV_ABS);
	  X(EV_MSC);
	  X(EV_SW);
	  X(EV_LED);
	  X(EV_SND);
	  X(EV_REP);
	  X(EV_FF);
	  X(EV_PWR);
	  X(EV_FF_STATUS);
	  X(EV_MAX);
	  X(EV_CNT);
	default:
	  type = "EV_UNKNOWN";
	  break;
	}

	switch (ev.code) {
#define BTN_CODE(B) case B: code = #B; break;
#include "btn_codes.h"
	default:
	  code = "BTN_UNKNOWN";
	  break;
	}


	printf("%ld,%ld %s %s %d\n",
		   ev.time.tv_sec, ev.time.tv_usec,
		   type, code, ev.value);
  }

  return 0;
}
