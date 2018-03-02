////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_pcRightsDef.h
// 用途描述：   PowerCall权限位定义
// 作者：		SUN
// 创建时间：	2002/06/05
// 最后修改时间：2003/09/16
// 版权信息：	Copyright (C) 1998 - 2003 Proactive Cyber Space
// 备注：
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PCRIGHTSDEF_H__
#define __PCRIGHTSDEF_H__
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// 权限
// 字符串长50位，每位定义，如上图所示
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// 每一位0代表没有此权限，1代表有该权限

// 座席权限
#define PCS_AGENT_CONFIG_POWERBAR				2				// 配置客户端参数
#define PCS_AGENT_CHANGEPASSWORD				26				// 修改口令
#define PCS_AGENT_CALLOUT						27				// 外拨
#define PCS_AGENT_TRANSFER						28				// 电话转接
#define PCS_AGENT_CONFERENCE					29				// 会议
#define PCS_AGENT_SINGLESTEPTRANSFER			30				// 单步转接
#define PCS_AGENT_PARKING						31				// 电话驻留
#define PCS_AGENT_FETCHING						32				// 驻留电话取回
#define PCS_AGENT_MONITOR_BREAK					44				// 班长强拆
#define PCS_AGENT_MONITOR_HOLDUP				45				// 班长拦截
#define PCS_AGENT_MONITOR_INSERT				46				// 班长强插
#define PCS_AGENT_MONITOR_LOGOUT				47				// 班长强制注销
#define PCS_AGENT_PRESET_MESSAGE				48				// 定制消息
#define PCS_AGENT_AUTOSEND_MESSAGE				49				// 定时消息设定

// 话务员权限
// 子系统(1 - 19)
#define PCS_USE_POWERBAR						1				// 使用客户端
#define PCS_USE_CONFIG_POWERBAR					2				// 配置客户端参数
#define PCS_USE_AGENTMANAGER					3				// 使用AgentManager
#define PCS_USE_POWERVOICE						4				// 使用PowerVoice
#define PCS_USE_CONFIG_POWERVOICE				5				// 配置PowerVoice
#define PCS_USE_ROUTEMANAGE						6				// 使用RouteManager
#define PCS_USE_CONFIG_ROUTEMANAGE				7				// 配置RouteManager
#define PCS_USE_POWERANALYSIS					8				// 使用报表工具
#define PCS_USE_CONFIG_POWERANALYSIS			9				// 配置报表工具
#define PCS_USE_POWERMONITOR					10				// 使用监控工具
#define PCS_USE_CONFIG_POWERMONITOR				11				// 配置监控工具
#define PCS_USE_OBSERVICE						12				// 使用OB Service
#define PCS_USE_CONFIG_OBSERVICE				13				// 配置OB Service
#define PCS_USE_RECORDCLIENT					14				// 使用录音客户端
#define PCS_USE_CONFIG_RECORDCLIENT				15				// 配置录音客户端
#define PCS_USE_RECORDADMIN						16				// 使用录音管理员
#define PCS_USE_CONFIG_RECORDADMIN				17				// 配置录音管理员
#define PCS_USE_CHECK_VOICEMAIL					18				// 检查语音信箱
#define PCS_USE_DELETE_VOICEMAIL				19				// 清理语音信箱

// Agent 相关(20 - 30)
#define PCS_USE_RECEIVECHATMSG					20				// 接收文字消息
#define PCS_USE_SENDTOLED						21				// 发送消息到LED
#define PCS_USE_SENDALLMESSAGE					22				// 发送消息到全体座席
#define PCS_USE_SENDGROUPMESSAGE				23				// 发送消息到座席组
#define PCS_USE_SENDTEAMMESSAGE					24				// 发送消息到座席小组
#define PCS_USE_SENDSINGLEMESSAGE				25				// 发送消息到单个座席
#define PCS_USE_CHANGEPASSWORD					26				// 修改口令
#define PCS_USE_CALLOUT							27				// 外拨
#define PCS_USE_TRANSFER						28				// 电话转接
#define PCS_USE_CONFERENCE						29				// 会议
#define PCS_USE_SINGLESTEPTRANSFER				30				// 单步转接

// OB 相关( 31 - 40)
#define PCS_USE_VIEWALL_TASK					31				// 查看所有任务
#define PCS_USE_VIEWGROUP_TASK					32				// 查看本组任务
#define PCS_USE_VIEWTEAM_TASK					33				// 查看本小组任务
#define PCS_USE_VIEWSELF_TASK					34				// 查看本人任务

// Monitor 相关(41 - 60)
#define PCS_USE_MONITOR_ALL						41				// 监控所有座席
#define PCS_USE_MONITOR_GROUP					42				// 监控本组座席
#define PCS_USE_MONITOR_TEAM					43				// 监控本小组座席
#define PCS_USE_MONITOR_BREAK					44				// 班长强拆
#define PCS_USE_MONITOR_HOLDUP					45				// 班长拦截
#define PCS_USE_MONITOR_INSERT					46				// 班长强插
#define PCS_USE_MONITOR_LOGOUT					47				// 班长强制注销
#define PCS_USE_PRESET_MESSAGE					48				// 定制消息
#define PCS_USE_AUTOSEND_MESSAGE				49				// 定时消息设定
#define PCS_USE_MODIFY_FLOOR_PLAN				50				// 修改布局
#define PCS_USE_CONFIG_LOCAL_ALERT				51				// 修改本地报警
#define PCS_USE_SET_CELL_FORMAT					52				// 定制单元格格式
#define PCS_USE_SET_BAR_FORMAT					53				// 定制状态条
#define PCS_USE_CONFIG_VIEWS					54				// 定制视图
#define PCS_USE_SELECT_VIEWS					55				// 选择视图
#define PCS_USE_SET_LEAVE_THRESHOLD				56				// 休息人数门限
#define PCS_USE_SET_BULLETIN_MESSAGE			57				// 设置公告消息
#define PCS_USE_MONITOR_CHG_AGTST				58				// 班长强制修改座席状态

//------------------------------------------------------------------------
#endif  //End #ifndef #define __PCRIGHTSDEF_H__
//------------------------------------------------------------------------
