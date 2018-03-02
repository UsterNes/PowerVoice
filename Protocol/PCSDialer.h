#include "Stdafx.h"
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __PCSDIALER_H__
#define __PCSDIALER_H__


#define MAX_BUF_SIZE 120
#define MAX_CHANS 8


#define dial_handler 3


#define DXCALL 0
#define DXOFF  1
#define DXUSED 2
#define DXERR  3
#define PCS_MSG_FLAG_OBDIALER		0x68			// OB Dialer通信标志，'D'

//发送、接收者定义：
#define	USER_OBSERVICE				0x23			// 外拨服务
#define	USER_OBDIALER				0x24			// 自动拨号器
//注：外拨服务作为控制者与自动拨号器进行通信。


//拨号任务状态定义：
#define	OBD_DIAL_STATUS_STOP		 0x0 				// - 停止
#define	OBD_DIAL_STATUS_READY		 0x1 				// - 就绪
#define	OBD_DIAL_STATUS_DIAL		 0x2 				// - 拨号
#define	OBD_DIAL_STATUS_SUCC		 0x3 				// - 完成
#define	OBD_DIAL_STATUS_FAIL		 0x4	 			// - 失败
#define	OBD_DIAL_STATUS_RETRY		 0x5 				// - 重试

//拨号任务结果定义：
#define	OBD_DIAL_RESULT_SUCC		 0x48 			// - 成功
#define	OBD_DIAL_RESULT_NOANS		 0x49 			// - 无应答
#define	OBD_DIAL_RESULT_BUSY		 0x50 			// - 对方忙
#define	OBD_DIAL_RESULT_ABAND		 0x51 			// - 放弃
#define	OBD_DIAL_RESULT_FAXTONE		 0x52			// - 传真
#define	OBD_DIAL_RESULT_AATONE		 0x53			// - 自动语音
#define	OBD_DIAL_RESULT_WRONGNO		 0x54			// - 号码错
#define	OBD_DIAL_RESULT_POWEROFF	 0x55			// - 关机
#define	OBD_DIAL_RESULT_OUTSERVICE   0x56			// - 不在服务范围
#define	OBD_DIAL_RESULT_STOPSERVICE  0x57			// - 欠费停机
#define	OBD_DIAL_RESULT_OTHERS		 0x79			// - 其他
//消息定义
#define UDP_OBS2OBD_REGISTER		  0x1				//对拨号器进行控制注册	[0..19]控制者IP；[20..21]控制者侦听端口
#define UDP_OBS2OBD_UNREGISTER		  0x2				//取消对拨号器的控制注册	无
			
#define UDP_OBS2OBD_TASK_ADD		  0x11			//增加拨号任务	[0..1]拨号Campaign ID；[2..3]拨号任务ID；[4..23]电话号码；[24..31]拨通后转接号码
#define UDP_OBS2OBD_TASK_REMOVE		  0x12			//删除拨号任务	[0..1]拨号Campaign ID；[2..3]拨号任务ID
#define UDP_OBS2OBD_TASK_REMOVE_ALL	  0x13			//删除所有任务	无
#define UDP_OBS2OBD_TASK_QUERY		  0x14			//查询拨号任务状态	[0..1]拨号Campaign ID；[2..3]拨号任务ID
#define UDP_OBS2OBD_TASK_LIST_QUERY	  0x15			//查询拨号任务列表	无
			
#define UDP_OBS2OBD_ATTACH_DATA		  0x21			//附加数据	
			
			
#define UDP_OBD2OBS_REGISTER_ACK	  0x101			//控制注册应答	[0]成功0/其他失败

#define UDP_OBD2OBS_UNREGISTER_ACK	  0x102			//取消控制注册应答	无
			
#define UDP_OBD2OBS_TAST_ADD_ACK	  0x111			//增加拨号任务应答	[0..1]拨号Campaign ID；[2..3]拨号任务ID；[4]接受0/拒绝1
#define UDP_OBD2OBS_TAST_REMOVE_ACK	  0x112			//删除拨号任务应答	[0..1]拨号Campaign ID；[2..3]拨号任务ID；[4]接受0/拒绝1
#define UDP_OBD2OBS_TAST_REMOVE_ALL_ACK	0x113			//删除所有任务应答	[0..1]删除任务数量
#define UDP_OBD2OBS_TASK_QUERY_ACK	   0x114			//查询拨号任务状态	[0..1]拨号Campaign ID；[2..3]拨号任务ID；[4]任务状态；[5..6]当前状态持续时间
#define UDP_OBD2OBS_TASK_LIST_QUERY_AC	0x115			//查询拨号任务列表	[0..1]当前活动任务总数；[2..3]本日累计完成任务；[4..5]本日累计成功任务；[6]任务平均通道占用时间

#define UDP_OBD2OBS_TASK_RESULT			0x120			//拨号任务执行结果	[0..1]拨号Campaign ID；[2..3]拨号任务ID；[4]任务执行结果；[5..6]呼叫ID


#define PCS_MSGHDR_FLAG0		0x55		// 消息头标志0
#define PCS_MSGHDR_FLAG1		0xAA		// 消息头标志1
#define PCS_MSGHDR_FLAG2		0x7F		// 消息头标志2
#define PCS_MSGHDR_FLAG3		0xFE		// 消息头标志3



#endif