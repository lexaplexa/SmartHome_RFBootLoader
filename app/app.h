/*************************************************************************
 * app.h
 *
 * Created: 24.6.2018 10:34:46
 * Revised:
 * Author: LeXa
 * BOARD:
 *
 * ABOUT:
 *
 *************************************************************************/


#ifndef APP_H_
#define APP_H_

#include <core/core.h>

/************************************************************************/
/* EXTERNAL CLASSES                                                     */
/************************************************************************/
extern class RF cRf;                    /* app\tasks\task_startupapp.cpp */
extern RF_DATA RFData;                  /* app\tasks\task_startupapp.cpp */
extern DATASET_RF sDataset;             /* app\tasks\task_startupapp.cpp */
extern DATASET_RF sDatasetEE;           /* app\tasks\task_startupapp.cpp */

/************************************************************************/
/* TASK PROTOTYPES                                                      */
/*----------------------------------------------------------------------*/
/* Every task function is named void task<name of task>();              */
/* For example:                                                         */
/* void taskStartUp();                                                  */
/*                                                                      */
/* File names with source code are named task_<name of task>.cpp        */
/* For example:                                                         */
/* task_startup.cpp                                                     */
/*                                                                      */
/* Every task source code must include core.h                           */
/************************************************************************/
void taskRf();

#endif /* APP_H_ */