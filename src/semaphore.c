// Copyright (c) 2026 Quoc Trung (https://github.com/thisisaname1928)
// This software is released under the GNU General Public License v3.0. See
// LICENSE file for details. This header needs to maintain in any file it is
// present in, as per the GPL license terms.
#include <errno.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <syscall.h>

// POSIX sempahore won't care if programmer make an double init or using after
// destroy, so I remove the SEM_INIT_MAGIC
int sem_init(sem_t *sem, int pshared, unsigned int val) {
  if (!sem) {
    errno = EINVAL;
    return -1;
  }

  // I'll not implement this...
  if (pshared == 1) {
    errno = ENOSYS;
    return -1;
  }

  if (val > SEM_VALUE_MAX) {
    errno = EINVAL;
    return -1;
  }

  atomic_store(&sem->value, val);
  sem->waiter = 0;
  sem->pshared = pshared;

  return 0;
}

int sem_destroy(sem_t *sem) {
  if (!sem) {
    errno = EINVAL;
    return -1;
  }

  // POSIX implementation dont care if caller destroy a running semaphore

  return 0;
}

int sem_wait(sem_t *sem) {
  if (!sem) {
    errno = EINVAL;
    return -1;
  }

  // wait for resource
  while (true) {
    uint32_t cur_sem_val =
        atomic_load_explicit(&sem->value, memory_order_acquire);

    if (cur_sem_val > 0) {
      if (atomic_compare_exchange_strong_explicit(
              &sem->value, &cur_sem_val, cur_sem_val - 1, memory_order_acquire,
              memory_order_relaxed)) // check if cur_sem_val is = sem->value and
                                     // decrease it

        return 0;
      else
        continue; // if they aren't the same, someone stole it, just try get it
                  // again
    }

    cur_sem_val = atomic_load_explicit(&sem->value, memory_order_acquire);
    if (cur_sem_val > 0) { // double check to make sure that no sem_post() sneak
                           // in while registering
      continue;
    }

    // got some sleep
    atomic_fetch_add_explicit(&sem->waiter, 1, memory_order_relaxed);

    int result = (int)syscall3(SYS_FUTEX, (uint64_t)&sem->value, (uint64_t)FUTEX_WAIT, (uint64_t)0);
    if (result != 0) {
      atomic_fetch_sub_explicit(&sem->waiter, 1, memory_order_relaxed);

      if (errno == EAGAIN) // only retry if futex retrun EAGAIN
        continue;

      return -1; // leave the same errno from kernel for caller
    }

    atomic_fetch_sub_explicit(&sem->waiter, 1, memory_order_relaxed);
  }

  return 0;
}

int sem_trywait(sem_t *sem) {
  if (!sem) {
    errno = EINVAL;
    return -1;
  }

  uint32_t cur_sem_val =
      atomic_load_explicit(&sem->value, memory_order_acquire);
  while (cur_sem_val >
         0) { // run a loop to make sure it won't give up when sem->value > 0
    if (atomic_compare_exchange_strong_explicit(
            &sem->value, &cur_sem_val, cur_sem_val - 1, memory_order_acquire,
            memory_order_relaxed)) {
      return 0;
    }

    // atomic_compare_exchange_strong refreshed cur_sem_val for us
  }

  errno = EAGAIN;
  return -1;
}

int sem_post(sem_t *sem) {
  if (!sem) {
    errno = EINVAL;
    return -1;
  }

  uint32_t cur_sem_val =
      atomic_load_explicit(&sem->value, memory_order_relaxed);

  while (cur_sem_val < SEM_VALUE_MAX) {
    if (atomic_compare_exchange_strong_explicit(
            &sem->value, &cur_sem_val, cur_sem_val + 1, memory_order_release,
            memory_order_relaxed)) {

      if (atomic_load_explicit(&sem->waiter, memory_order_relaxed) >
          0) // only wake when waiter > 0

        syscall3(SYS_FUTEX, (uint64_t)&sem->value, (uint64_t)FUTEX_WAKE, (uint64_t)1);

      return 0;
    }

    // cur_sem_val changed while we was comparing so try again
  }

  errno = EOVERFLOW;
  return -1;
}

// get the sem_t's current value
int sem_getvalue(sem_t *restrict sem, int *v) {
  if (!sem || !v) {
    errno = EINVAL;
    return -1;
  }

  *v = (int)atomic_load_explicit(&sem->value, memory_order_acquire);

  return 0;
}

// not supported functions

sem_t *sem_open(const char *name, int oflag, ...) {
  errno = ENOSYS;
  return SEM_FAILED;
}

int sem_timedwait(sem_t *restrict sem, const struct timespec *ts) {
  errno = ENOSYS;
  return -1;
}

int sem_unlink(const char *name) {
  errno = ENOSYS;
  return -1;
}

int sem_close(sem_t *sem) {
  errno = ENOSYS;
  return -1;
}