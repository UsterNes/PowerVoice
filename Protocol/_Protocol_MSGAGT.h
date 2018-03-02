////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：			_Protocol_MSGAGT.h
// 用途描述：		消息客户端通信协议
// 作者：			Jerry, Tony
// 创建时间：		2004/11/23
// 最后修改时间：	2010/02/08
// 版权信息：		Copyright (C) 1999 - 2010 PCSG & PicaSoft
// 备注：
//   2010-02-08 [Tony]: 增加“分组事由控制协议”
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_MSGAGENT_H__
#define __PROTOCOL_MSGAGENT_H__
//------------------------------------------------------------------------

//--------------------------
// 常量定义 - 2
//------------
#ifndef MAXHEADLEN
	#define MAXHEADLEN			50			// 消息标题最大长度
#endif

#ifndef MAXMSGLEN
	#define MAXMSGLEN			200			// 消息内容最大长度
#endif

//------------------------------------------
// 常量定义 - 4  - MAX
//-----------------------------------------
#define	MSG_MAX_USER					16384			// 最大用户数
#define	MSG_MAX_TEAM					512				// 最大小组数
#define	MSG_MAX_GROUP					256				// 最大大组数
#define	MSG_MAX_TEAMGROUP				4096			// 最大大小组组合数
//------------
//--------------------------

//------------------------------------------
// 权限起始位置 -- 权限总位数  -- 2
//-----------------------------------------
#define	RIGHTSTART			20			// 
#define	RIGHTCOUNT			6			// 

// Agent 相关(16 - 30)
#define MSG_USE_RECEIVECHATMSG					1				// 接收文字消息
#define MSG_USE_SENDTOLED						2				// 发送消息到LED
#define MSG_USE_SENDALLMESSAGE					3				// 发送消息到全体座席
#define MSG_USE_SENDGROUPMESSAGE				4				// 发送消息到座席组
#define MSG_USE_SENDTEAMMESSAGE					5				// 发送消息到座席小组
#define MSG_USE_SENDSINGLEMESSAGE				6				// 发送消息到单个座席
//------------
//--------------------------

//------------------------------------------
// 消息接受/发送类型 - 4  - TargetType
//-----------------------------------------
#define	MSG_TARGET_ALL					10			// 全体坐席
#define	MSG_TARGET_GROUP				3			// 座席大组
#define	MSG_TARGET_TEAM					2			// 坐席小组
#define	MSG_TARGET_USER					1			// 单个座席
#define	MSG_TARGET_LED					4			// LED
//------------
//--------------------------

//------------------------------------------
// 消息级别 - 5 -- MessageLevel
//-----------------------------------------
#define	MSG_LEVEL_1			1			// 级别最高
#define	MSG_LEVEL_2			2			// 
#define	MSG_LEVEL_3			3			// 
#define	MSG_LEVEL_4			4			// 
#define	MSG_LEVEL_5			5			// 级别最低
//参考信息、一般通知、重要通知、紧急通知
//重要性-高、较高、一般、较低、低
//------------
//--------------------------

//------------------------------------------
// 消息类型 - 3 -- MessageType
//-----------------------------------------
#define	MSG_TYPE_TEXT			1			// 文字消息
#define	MSG_TYPE_ACD			2			// ACD对列消息
#define	MSG_TYPE_SQL			3			// SQL语句
//------------
//--------------------------

//--------------------------
// 消息客户端注册 - Total 4
//--------------------------

//--------------------------
// 注册 - 2
//------------
#define TCP_AGT2MSG_REGISTER				1			// (0x0001) 注册消息
struct S_TCP_AGT2MSG_REGISTER
{
	char	strUserName[20];				// 用户名
	char	strPassword[20];				// 用户口令
	UC		Version;						// 版本号
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// 注册码
};
//------------

//------------
#define TCP_MSG2AGT_REGISTER_ACK			1001		// (0x03E9) 注册响应消息
struct S_TCP_MSG2AGT_REGISTER_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
	char	strDisplayName[50];				// 用户显示名字
	char	strRights[RIGHTCOUNT];			// 权限
	US		TeamID;							// 该用户所属的小组
	UC		InGroupCount;					// 该用户属于多少个组
	US		GroupList[64];					// 组列表
};
//------------
//--------------------------

//--------------------------
// 取消注册 - 2
//------------
#define TCP_AGT2MSG_UNREGISTER				2			// (0x0002) 取消注册消息
//------------

//------------
#define TCP_MSG2AGT_UNREGISTER_ACK			1002		// (0x03EA) 取消注册响应消息
//------------
//--------------------------

//--------------------------
// 状态改变消息 - 1
//------------
#define TCP_MSG2AGT_STATUS_CHANGE		1018		// (0x03FA) 位置状态改变消息
struct S_TCP_MSG2AGT_STATUS_CHANGE
{
	char						UserID[20];					// 座席ID
	_pcC_stMsgClientStatus		Status;					// 位置状态，参照_pcC_stPOS
};
//------------

//--------------------------
// 发送消息 - 3
//------------
#define TCP_AGT2MSG_SENDMESSAGE				3001		// (0x0BB9) 发送文字消息
// 通信消息包结构定义
struct S_TCP_AGT2MSG_SENDMESSAGE
{
	UC				MessageType;		    	// 消息类型
	UC				MessageLevel;				// 消息级别
	UL				ResourceID;					// 发送源编号
	char			ResourceName[50];			// 发送源
	UC				TargetType;					// 接收范围
	UL				TargetID;					// 接收源编号
	char			TargetName[50];				// 接收源名称
	char			SendTime[15];				// 消息发送时间
    char			Title[MAXHEADLEN];			// 标题
	char			Content[MAXMSGLEN];			// 内容
};

#define TCP_MSG2AGT_SENDMESSAGE				3002		// (0x0BBA) 发送文字消息

#define TCP_MSG2AGT_SENDACD					3003		// (0x0BBB) 发送ACD消息
//------------
//--------------------------

//--------------------------
// 状态查询 - 2
//------------
#define TCP_AGT2MSG_STATUS_QUERY			11			// (0x000B) 查询状态消息
//------------

//------------
#define TCP_MSG2AGT_STATUS_QUERY_ACK		1011		// (0x03F3) 查询状态响应消息
struct S_TCP_MSG2AGT_STATUS_QUERY_ACK
{
	UC		ErrorCode;					// 返回码，参见“返回码定义”：
	UC		Status;							// 状态 Ready Or Not Ready
	UC		LeaveReason;					// 离席原因，自定义
};
//------------
//--------------------------

//--------------------------
// 修改状态 - 2
//------------
#define TCP_AGT2MSG_STATUS_CHANGE			14			// (0x000E) 修改状态消息
struct S_TCP_AGT2MSG_STATUS_CHANGE
{
	UC		Status;							// 状态 Ready Or Not Ready
	UC		LeaveReason;					// 离席原因，自定义
};
//------------

//------------
#define TCP_MSG2AGT_STATUS_CHANGE_ACK		1014		// (0x03F6) 修改状态响应消息
struct S_TCP_MSG2AGT_STATUS_CHANGE_ACK
{
	UC		ErrorCode;					// 返回码，参见“返回码定义”：
	UC		Status;							// 状态 Ready Or Not Ready
	UC		LeaveReason;					// 离席原因，自定义
};
//------------
//--------------------------

//--------------------------
// 获取成员树 - 6
//------------
#define TCP_AGT2MSG_TREEVIEW_QUERY			500				// (0x01F6) 查询话务员消息列表响应，后续消息为成员树
//------------
#define TCP_MSG2AGT_TREEVIEW_DATA			1500			// (0x0466) 成员树消息
struct S_TCP_MSG2AGT_TREEVIEW_DATA
{
	US		TotalItemCount;									// 成员数目
	US		ItemIndex;										// 成员序号
	UC		ItemCount;										// 本条消息包括的成员数量
	UC		*pSubTree;										// 指向成员子树
};
//------------
#define TCP_MSG2AGT_TREEVIEW_GROUP_DATA			1501		// (0x0466) 成员树-GROUP消息

#define TCP_MSG2AGT_TREEVIEW_TEAMGROUP_DATA		1502		// (0x0466) 成员树-TEAMGROUp消息

#define TCP_MSG2AGT_TREEVIEW_TEAM_DATA			1503		// (0x0466) 成员树-TEAM消息

#define TCP_MSG2AGT_TREEVIEW_USER_DATA			1504		// (0x0466) 成员树-USER消息
//------------
//--------------------------

//--------------------------
// 获取全部状态 - 2
//------------
#define TCP_AGT2MSG_USERSTATUS_QUERY			1505		// (0x0466) 查询全体坐席状态消息
#define TCP_MSG2AGT_USERSTATUS_DATA				1506		// (0x0466) 成员状态消息
struct S_TCP_MSG2AGT_USERSTATUS_DATA
{
	US		TotalItemCount;									// 成员数目
	US		ItemIndex;										// 成员序号
	UC		ItemCount;										// 本条消息包括的成员数量
	UC		*pSubTree;										// 指向成员子树
};
//------------
//--------------------------

//--------------------------
// 结构体 - 1 - For User Status
//------------
struct SUserStatusItem
{
	char					UserID[20];				// User ID
	_pcC_stMsgClientStatus	Status;					
};  
//------------
//--------------------------

//--------------------------
// 结构体 - 4 - For Tree Item
//------------
struct SGroupMemberTreeItem
{
	US		GroupID;						// Group ID
	char	GroupName[50];					// Group Name
};

struct STeamGroupMemberTreeItem
{
	US		GroupID;						// Group ID
	US		TeamID;							// Team ID
};

struct STeamMemberTreeItem
{
	US		TeamID;							// Team ID
	char	TeamName[50];					// Team Name	
};

struct SUserMemberTreeItem
{					
	char					UserID[20];		// User ID
	char					UserName[50];	// User Name
	US						UserType;				
	US						TeamID;		
	_pcC_stMsgClientStatus	Status;
};
//------------
//--------------------------

//--------------------------
// Sun added 2010-02-08
// 分组事由控制协议 - 4
//------------
#define TCP_AGT2MSG_CHANGE_STATUS_REQ		3021		// (0x0BCD) 修改状态审核请求消息
struct S_TCP_AGT2MSG_CHANGE_STATUS_REQ
{
	UL		InvokeID;						// Invoke ID
	UC		PositionStatus;					// 位置状态，参照_pcC_stPOS
	UC		LeaveReason;					// 离席原因，自定义
	char	Token[MAXHEADLEN];				// Token
	UL		Reserved1;						// 保留1
	UL		Reserved2;						// 保留2
};

#define TCP_MSG2AGT_CHANGE_STATUS_ACK		3022		// (0x0BCE) 修改状态审核响应消息
struct S_TCP_MSG2AGT_CHANGE_STATUS_ACK
{
	UL		InvokeID;						// Invoke ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_REJECTED
											// PCS_DEF_RETCODE_NOTSUPPORT
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

#define TCP_AGT2MSG_CHANGE_STATUS_ADMREQ	3023		// (0x0BCF) 向班长请求修改状态
struct S_TCP_AGT2MSG_CHANGE_STATUS_ADMREQ
{
	UL		InvokeID;						// Invoke ID
	UL		MonitorID;						// 监控者ID，如果不指定，则发给所有在线管理员
	UC		PositionStatus;					// 位置状态，参照_pcC_stPOS
	UC		LeaveReason;					// 离席原因，自定义
	char	ShortMessage[MAXHEADLEN];		// 附带的文字消息（请求原因）
	UL		Reserved1;						// 保留1
	UL		Reserved2;						// 保留2
};

#define TCP_MSG2AGT_CHANGE_STATUS_ADMACK		3024		// (0x0BD0) 请求修改状态班长响应
struct S_TCP_MSG2AGT_CHANGE_STATUS_ADMACK
{
	UL		InvokeID;						// Invoke ID
	UL		MonitorID;						// 监控者ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_REJECTED
											// PCS_DEF_RETCODE_NOTSUPPORT
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	char	ShortMessage[MAXHEADLEN];		// 附带的文字消息（结果说明）
	char	Token[MAXHEADLEN];				// Token
	UL		Reserved1;						// 保留1
	UL		Reserved2;						// 保留2
};
//------------
// Henan added 2010-02-25
// 分组事由控制协议 - 1
#define TCP_AGT2MSG_CHANGE_CAUSA_ASTRICT	3025		// (0x0BD1) 向MSG通知区域限制值改变
//------------
//--------------------------


//--------------------------
// Sun added 2010-10-22
// 小休申请消息定义
#define TCP_NRAM_REQUEST					1			// (0x01)座席小休申请
#define TCP_NRAM_RESPONSE_REFUSED			10			// (0x0A)班长拒绝
#define TCP_NRAM_RESPONSE_APPROVED			11			// (0x0B)班长允许
#define TCP_NRAM_NTF_DISABLED				30			// (0x1E)小休申请开关变化通知：关闭
#define TCP_NRAM_NTF_ENABLED				31			// (0x1F)小休申请开关变化通知：开启

//--------------------------


//--------------------------
//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_RECORDAGENT_H__
//------------------------------------------------------------------------
