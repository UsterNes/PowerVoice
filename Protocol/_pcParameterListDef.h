////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_pcParameterListDef.h
// 用途描述：   PowerCall参数名称定义
// 作者：		SUN
// 创建时间：	2006/01/08
// 最后修改时间：2006/01/08
// 版权信息：	Copyright (C) 1998 - 2006 PicaSoft
// 备注：
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PCPARAMETERLISTDEF_H__
#define __PCPARAMETERLISTDEF_H__
//------------------------------------------------------------------------

// 参数：最少在线座席数，当登录座席数低于此值后就不再允许在线座席登出
// 注意：此参数通常用于7 * 24小时服务情况，或者管理员需要手工调整参数值（如下班时，否则座席无法全部登出）
// 缺省值：0 - 不限制
#ifndef PCS_SYSPARAM_NAME_MIN_OL_AGENTS
#define PCS_SYSPARAM_NAME_MIN_OL_AGENTS				"MinLoginAgents"
#endif

// 参数：国家代码
// 缺省值：'86' - 中国
#ifndef PCS_SYSPARAM_NAME_COUNTRYCODE
#define PCS_SYSPARAM_NAME_COUNTRYCODE				"Country_Code"
#endif

// 参数：本地电话的区号
// 缺省值：'21' - 上海
#ifndef PCS_SYSPARAM_NAME_LOCALCITYCODE
#define PCS_SYSPARAM_NAME_LOCALCITYCODE				"LocalCity_Code"
#endif

// 参数：本地电话号码长度
// 缺省值：'8' - 8位
#ifndef PCS_SYSPARAM_NAME_LOCALNUMLEN
#define PCS_SYSPARAM_NAME_LOCALNUMLEN				"LocalNum_Length"
#endif

// 参数：市话拨号前缀
// 缺省值：'9'
#ifndef PCS_SYSPARAM_NAME_OBPREFIX_LOCAL
#define PCS_SYSPARAM_NAME_OBPREFIX_LOCAL			"LOCAL"
#endif

// 参数：国内长途拨号前缀
// 缺省值：'9'
#ifndef PCS_SYSPARAM_NAME_OBPREFIX_NATIONAL
#define PCS_SYSPARAM_NAME_OBPREFIX_NATIONAL			"NATIONAL"
#endif

// 参数：国际长途拨号前缀
// 缺省值：'9'
#ifndef PCS_SYSPARAM_NAME_OBPREFIX_INTER
#define PCS_SYSPARAM_NAME_OBPREFIX_INTER			"INTERNATIONAL"
#endif

// 参数：话务员统计数据复位时间，每天这个时刻对统计数据进行复位
// 缺省值：'00:00:30'
#ifndef PCS_SYSPARAM_NAME_WORKDATA_RESET_TIME
#define PCS_SYSPARAM_NAME_WORKDATA_RESET_TIME		"WorkDataResetTime"
#endif

// Sun added 2012-08-10
// 参数：摘机自动拨号号码
// 缺省值：空
#define PCS_SYSPARAM_NAME_AGT_OffHookAutoDial		"OffHookAutoDial"

//------------------------------------------------------------------------
#endif  //End #ifndef #define __PCPARAMETERLISTDEF_H__
//------------------------------------------------------------------------
