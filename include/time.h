// Copyright (c) 2023-2026 Christiaan (chris@boreddev.nl)
// This software is released under the GNU General Public License v3.0. See LICENSE file for details.
// This header needs to maintain in any file it is present in, as per the GPL license terms.
#ifndef BOREDOS_LIBC_TIME_H
#define BOREDOS_LIBC_TIME_H

#include <stddef.h>

typedef long long time_t;
typedef unsigned long long clock_t;

#define CLOCKS_PER_SEC 3000000000ULL

struct tm {
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};

struct timespec {
  time_t tv_sec;
  long tv_nsec;
};

time_t time(time_t *out);
clock_t clock(void);
struct tm *localtime(const time_t *timer);
struct tm *gmtime(const time_t *timer);
size_t strftime(char *s, size_t max, const char *fmt, const struct tm *tm);
time_t mktime(struct tm *tm);

int get_ticks(void);
int rtc_get(int *dt);
int rtc_set(int *dt);

#endif
