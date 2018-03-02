////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_msRightsDef.h
// 用途描述：   MaxiSense权限位定义
// 作者：		SUN
// 创建时间：	2002/06/05
// 最后修改时间：2006/12/06
// 版权信息：	Copyright (C) 1998 - 2004 PicaSoft
// 备注：
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __MSRIGHTSDEF_H__
#define __MSRIGHTSDEF_H__
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// 用户权限
// 字符串长50位，每位定义，如上图所示
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// 每一位0代表没有此权限，1代表有该权限

#define PVR_USE_COMPEL_RECORD					1				// 强制客户端录音
#define PVR_USE_LISTENING						2				// 监听
#define PVR_USE_CHANGE_PASSWORD					3				// 修改口令
#define PVR_USE_CHANGE_USERPARAM				4				// 修改用户参数
#define PVR_USE_CHANGE_SYSPARAM					5				// 修改系统参数
#define PVR_USE_ADD_USER						6				// 添加用户
#define PVR_USE_DEL_USER						7				// 删除用户
#define PVR_USE_MOD_USER						8				// 修改用户
#define PVR_USE_RELOAD_USERLIST					9				// 重新加载用户表
#define PVR_USE_RELOAD_SYSPARAM					10				// 重新加载系统参数表
#define PVR_USE_DOWNLOAD_FILE					11				// 从服务器下载录音文件
#define PVR_USE_MANAGER_ALL_GROUP 				12				// 能否管理所有的组(Administrator,Supervisor,Agent)

typedef enum __pcsUserRights
{
	pcsUR_USE_MANAGER_ALL_GROUP = 0,						// 能否管理所有的组(Administrator,Supervisor,Agent)
	pcsUR_USE_COMPEL_RECORD = 1,							// 强制客户端录音
	pcsUR_USE_LISTENING	= 2,								// 监听
	pcsUR_USE_CHANGE_PASSWORD =	3,							// 修改口令-User
	pcsUR_USE_CHANGE_USERPARAM = 4,							// 修改用户参数
	pcsUR_USE_CHANGE_SYSPARAM =	5,							// 修改系统参数-Administrator
	pcsUR_USE_ADD_USER = 6,									// 添加用户
	pcsUR_USE_DEL_USER = 7,									// 删除用户
	pcsUR_USE_MOD_USER = 8,									// 修改用户
	pcsUR_USE_RELOAD_USERLIST =	9,							// 重新加载用户表
	pcsUR_USE_RELOAD_SYSPARAM =	10,							// 重新加载系统参数表-Administrator
	pcsUR_USE_DOWNLOAD_FILE = 11,							// 从服务器下载录音文件-User	
	pcsUR_USE_DO_QA	= 12,									// 能否评分
	pcsUR_USE_EXPORT_RECORDFILE = 13,						// 导出录音权限
	pcsUR_USE_MOD_MANA_APS = 14,							// 修改Admin可以查看哪些AP
	pcsUR_USE_ADD_AP = 15,									// 添加AP
	pcsUR_USE_MOD_AP = 16,									// 删除AP
	pcsUR_USE_DEL_AP = 17,									// 修改AP
	pcsUR_USE_RELOAD_APLIST = 18,							// 重新加载AP表

} pcsUserRights;
//------------------------------------------------------------------------
#endif  //End #ifndef #define __MSRIGHTSDEF_H__
//------------------------------------------------------------------------
