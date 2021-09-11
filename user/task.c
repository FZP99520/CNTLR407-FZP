//#include "task.h"
//#include "os.h"
//#include "led.h"

//#define  START_TASK_PRIO		3
//#define  START_STK_SIZE 		512
//OS_TCB   StartTaskTCB;
//CPU_STK  START_TASK_STK[START_STK_SIZE];

//#define  LED_TASK_PRIO		10
//#define  LED_STK_SIZE 		256
//OS_TCB   LEDTaskTCB;
//CPU_STK  LED_TASK_STK[LED_STK_SIZE];

//void START_(void)
//{
//	OS_ERR err;
//	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		
//				 (CPU_CHAR	* )"start task", 		
//                 (OS_TASK_PTR )Start_Task, 	
//                 (void		* )0,					
//                 (OS_PRIO	  )START_TASK_PRIO,     
//                 (CPU_STK   * )&START_TASK_STK[0],	
//                 (CPU_STK_SIZE)START_STK_SIZE/10,	
//                 (CPU_STK_SIZE)START_STK_SIZE,		
//                 (OS_MSG_QTY  )0,					
//                 (OS_TICK	  )0,					
//                 (void   	* )0,				
//                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
//                 (OS_ERR 	* )&err);		
//}
//void Start_Task(void* p_arg)
//{
//	OS_ERR err;
//	CPU_SR_ALLOC();
//	p_arg = p_arg;

//	CPU_Init();
//#if OS_CFG_STAT_TASK_EN > 0u
//   OSStatTaskCPUUsageInit(&err);               
//#endif
//	
//#ifdef CPU_CFG_INT_DIS_MEAS_EN		
//    CPU_IntDisMeasMaxCurReset();	
//#endif

//#if	OS_CFG_SCHED_ROUND_ROBIN_EN  
//	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
//#endif		
//	OS_CRITICAL_ENTER();	
//  
//	OSTaskCreate((OS_TCB 	* )&LEDTaskTCB,		
//				 (CPU_CHAR	* )"led task", 		
//                 (OS_TASK_PTR )LED_Task, 			
//                 (void		* )0,					
//                 (OS_PRIO	  )LED_TASK_PRIO,     
//                 (CPU_STK   * )&LED_TASK_STK[0],	
//                 (CPU_STK_SIZE)LED_STK_SIZE/10,	
//                 (CPU_STK_SIZE)LED_STK_SIZE,		
//                 (OS_MSG_QTY  )0,					
//                 (OS_TICK	  )0,					
//                 (void   	* )0,					
//                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
//                 (OS_ERR 	* )&err);	
//   OS_CRITICAL_EXIT();
//	 OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);
//}
//void LED_Task(void* p_arg)
//{
//	OS_ERR err;
//	CPU_SR cpu_sr=0;
//	p_arg = p_arg;
//	while(1)
//	{
//		LED1_ON;
//		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); 
//		LED1_OFF;
//		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); 
//	}
//}