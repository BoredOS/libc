#ifndef _DEV_PCSK_H
#define _DEV_PCSK_H

#include <stdint.h>

#define PCSK_IOCTL_BEEP 0x5001

struct pcsk_beep {
    uint32_t freq; /* Hz */
    uint32_t ms;   /* milliseconds */
};

#endif
