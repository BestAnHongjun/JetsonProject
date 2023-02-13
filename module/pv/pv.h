#ifndef __PV_H__
#define __PV_H__

#include "global_config.h"

#include <semaphore.h>

typedef sem_t PV;
#define P(sem_name) sem_wait(&sem_name)
#define V(sem_name) sem_post(&sem_name)
#define PV_INIT(sem_name, n) sem_init(&sem_name, 0, n)

#endif // __PV_H__