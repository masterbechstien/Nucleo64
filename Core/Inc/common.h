/*
 * common.h
 *
 *  Created on: Jun 6, 2026
 *      Author: maste
 */

#ifndef INC_COMMON_H_
#define INC_COMMON_H_

#include "FreeRTOS.h"
#include "task.h"
#include "stdbool.h"

#define BYTES_TO_WORD_SIZE(x) (x/4)  // For 32-bit architecture convert bytes to words: 4bytes = 1word

#define MAIN_TASK_STACK_SIZE   BYTES_TO_WORD_SIZE(1024)           // For 32bit architecture: 4bytes=1Word, then 256 words = 256words*4bytes/1word = 1024 bytes

#define DELAY_ONE_SECOND pdMS_TO_TICKS(1000)

#define MAIN_TASK_PRIORITY tskIDLE_PRIORITY


#endif /* INC_COMMON_H_ */
