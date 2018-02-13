////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_NodeFunc.h
// ��;������	�ڵ����д���������
// ���ߣ�		SUN
// ����ʱ�䣺	2001/03/21
// ����޸�ʱ�䣺2012/05/07
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2012 Changjiang Xincheng, PicaSoft
// ��ע��
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//------------------------------------------------------------------------
#ifndef __NODEFUNC_H_
#define __NODEFUNC_H_
//------------------------------------------------------------------------

#ifndef MAXSYSNODEID
#define MAXSYSNODEID		255			// ����ϵͳ�ڵ�ID
#endif

// Sun added 2003-04-25
#define MAXLANGUAGECOUNT	10			// ��������������

//------------------------------------------------------------------------
// Sun added 2002-05-22
/// ϵͳ����
#define SysVar_MainMenu						"MainMenu"				// ҵ������
#define SysVar_SubMenu						"SubMenu"				// ҵ������˵�
#define SysVar_ThirdMenu					"ThirdMenu"				// ҵ�������˵�
#define SysVar_Language						"Language"				// �������Դ���
#define SysVar_CallBack						"CallBack"				// �ز����
#define SysVar_FaxBack						"FaxBack"				// FOD���
#define SysVar_EMailBack					"EMailBack"				// EMail���
#define SysVar_SMSBack						"SMSBack"				// SMS���
#define SysVar_MailBack						"MailBack"				// Mail���
#define SysVar_CBExtData					"CBExtData"				// �ز���չ����
#define SysVar_WaitTime						"WaitTime"				// ��ʱ���
#define SysVar_CustomerID					"CustomerID"			// �ͻ����
#define SysVar_Password						"Password"				// Password
#define SysVar_CallInTime					"CallInTime"			// ����ʱ��
#define SysVar_CallRefID					"CallRefID"				// ���б�ʶ
#define SysVar_ANI							"ANI"					// ���к���
#define SysVar_DNIS							"DNIS"					// ���к���
#define SysVar_VisitLog						"VisitLog"				// �ڵ������־
#define SysVar_AppData						"AppData"				// Ӧ������
#define SysVar_UserData						"UserData"				// �û�����
#define SysVar_ExtData						"ExtData"				// ��չ����
#define SysVar_OBTelNo						"OBTelNo"				// �Ⲧ����

// Sun added 2012-03-09
/// ֻ������������ʱ����Ҫ����ռ䣬ֻҪ�����ּ���ʹ��
#define SysVar_ChannelID					"ChannelID"				// IVRͨ����

// Sun added 2007-12-12
#define SysVar_CampaignID					"CampaignID"			// �Ⲧ�ж�ID
#define SysVar_TaskID						"TaskID"				// �Ⲧ����ID

// Sun added 2013-05-20
#define SysVar_GlobalCallRefID				"GlobalCallRefID"			// ȫ�ֺ��б�ʾ��char(24)
#define SysVar_CountryCode					"CountryCode"			// ���Ҵ��룬char(6)
#define SysVar_CityCode						"CityCode"				// ���ţ�char(6)
#define SysVar_PhoneNumber					"PhoneNumber"			// �绰���룬char(20)
#define SysVar_WorkTicketID					"WorkTicketID"			// ����ID��char(50)


//------------------------------------------------------------------------

#define DEF_CallBack_None					0x00
#define DEF_CallBack_Phone					0x01
#define DEF_CallBack_Fax					0x02
#define DEF_CallBack_EMail					0x04
#define DEF_CallBack_SMS					0x08
#define DEF_CallBack_Mail					0x10

// �¼���ṹ
#define DEF_NODE_DATA_SMALL_LEN				13
#define DEF_NODE_DATA_BIG_LEN				64
struct SEventTable{
	int cStatus;							// �ڵ�ID
	UC  Data1[DEF_NODE_DATA_SMALL_LEN];		// ���ݲ���1
	UC	Data2[DEF_NODE_DATA_BIG_LEN];		// ���ݲ���2
	UC	NodeNo;								// �ڵ�����(������)
};

//------------------------------------------------------------------------
// �ڵ�000��ϵͳ�ڵ�
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_000{
	UC		reserved1[10];		// ����
    UC		Languages;          // ��������
    UC		MajorVer;           // ���汾
    UC		MinorVer;           // �ΰ汾
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_000{
	UC		key_repeat;			// �ظ���ǰ�ڵ㰴��
	UC		key_return;			// �ص���һ���ڵ㰴��
	UC		key_root;			// �ص����˵�����
	UC		reserved1;			// ����
    US		ResourceProject;	// ��Դ��ĿID sun 2002-12-03
    US		MainCOM;			// ����COM�����Դ��� sun 2002-12-03
	UC		LogSwitchOff;		// �ڵ���־ȫ�ֿ��ƿ���, sun added 2008-02-19
    UC		reserved2[39];		// ���� sun 2002-12-03
    
	US		nd_parent;			// ���ڵ�ID
	US		nd_root;			// ���˵�(��)ID
	US		nd_SysSendData;		// ϵͳȱʡ�������ݸ�ʽ����ڵ� sun 2004-12-30
	US		nd_BeforeHookOn;	// �һ�ǰת�ڵ㣬Sun added 2012-05-07
	UC		reserved3[8];		// ����
};

//------------------------------------------------------------------------
// �ڵ�001����������
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_001{
	UC		reserved1[11];		// ����
	UC		uservars;			// �û����������, 0-255
    UC      reserved2;
};

//------------------------------------------------------------------------
// �ڵ�002�������嵥
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_002{
	UC		reserved1[13];		// ����
};
/// �û���������ṹ
struct SUserVarDef{
	UC		length;				// ��������0-255
	UC		type;				// ������������
	UC		name[14];			// ��������
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_002{
	SUserVarDef uservar[4];		// �û��������0-3
};

//------------------------------------------------------------------------
// �ڵ�006��������ת��
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_006{
	US		sleep;				// ��ʱ��sleep������λΪ����
	UC		reserved1[11];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_006{
	US		nd_goto;			// ��ת�ڵ�ID
	UC		reserved1[46];		// ����
	US		nd_parent;			// ���ڵ�ID
	UC		reserved2[14];		// ����
};

//------------------------------------------------------------------------
// �ڵ�007�������֤
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_007{
	UC		timeout;			// �ڵ㳬ʱ(��)
	UC		maxuserid;			// �û�������󳤶�
	UC		maxpassword;		// �û�������󳤶�
	UC		key_term;			// ������ֹ��, ����
	UC		maxtrytime;			// ����Դ���
	UC		log;				// ��������־
	UC		var_trytime;		// ��֤������¼(0 - maxtrytime)
	UC		var_result;			// ��֤�����¼
	UC		var_userid;			// �û������¼
	UC		var_password;		// �û������¼
	UC		reserved1[3];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_007{
	US		vox_userid;			// ��ʾ�û�������벥������
	US		vox_password;		// ��ʾ�û�������������
	US		vox_tryagain;		// ��ʾ�û��������벥������
	UC		reserved1[10];		// ����
	US		com_iid;			// COM�ӿ�ID
	UC		reserved2[30];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_succeed;			// �ɹ�ת�ڵ�ID
	US		nd_fail;			// ʧ��ת�ڵ�ID
	UC		reserved3[10];		// ����
};

//------------------------------------------------------------------------
// �ڵ�008���޸Ŀ���
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_008{
	UC		timeout;			// �ڵ㳬ʱ(��)
	UC		reserved1;			// ����
	UC		maxpassword;		// �û�������󳤶�
	UC		key_term;			// ������ֹ��, ����
	UC		maxtrytime;			// ����Դ���
	UC		log;				// ��������־
	UC		var_trytime;		// ���Դ�����¼(0 - maxtrytime)
	UC		var_result;			// ��֤�����¼
	UC		reserved2;			// ����
	UC		var_password;		// �û������¼
	UC		reserved3[3];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_008{
	US		vox_password;		// ��ʾ�û������¿��������
	US		vox_confirm;		// ��ʾ�û��ٴ�ȷ�ϲ�������
	US		vox_tryagain;		// ���β�һ���������벥������
	US		vox_succeed;		// ��ʾ�û��޸ĳɹ���������
	UC		reserved1[8];		// ����
	US		com_iid;			// COM�ӿ�ID
	UC		reserved2[30];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_succeed;			// �ɹ�ת�ڵ�ID
	US		nd_fail;			// ʧ��ת�ڵ�ID
	UC		reserved3[10];		// ����
};

//------------------------------------------------------------------------
// �ڵ�009��ʱ���֧
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_009{
	UC		reserved1[5];		// ����
	UC		log;				// ��������־
	UC		reserved2[7];		// ����
};
/// ʱ��νṹ
struct STimePeriod{
	UC		StartHH;			// ��ʼСʱ
	UC		StartMM;			// ��ʼ����
	UC		EndHH;				// ��ֹСʱ
	UC		EndMM;				// ��ֹ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_009{
	UC		workday;			// �����հ���, Bit����, 0-6λ��Ч,0:��Ϣ��;1:������
	UC		worktime;			// ������ʱ��ΰ���, Bit����, 0-5λ��Ч,0:��Ч;1:��Ч
	STimePeriod timesec[6];		// ʱ���1-6
	UC		reserved1[22];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_sparetime;		// ��Ϣ��ת�ڵ�ID
	US		nd_timesec1;		// ʱ���1ת�ڵ�ID
	US		nd_timesec2;		// ʱ���2ת�ڵ�ID
	US		nd_timesec3;		// ʱ���3ת�ڵ�ID
	US		nd_timesec4;		// ʱ���4ת�ڵ�ID
	US		nd_timesec5;		// ʱ���5ת�ڵ�ID
	US		nd_timesec6;		// ʱ���6ת�ڵ�ID
};

//------------------------------------------------------------------------
// Sun added 2004-04-30
// �ڵ�010���������趨
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_010{
    UC		maincalendar;		// �Ƿ��������������ɼ���ϵͳʶ��
    UC		startyear;			// ��ʼ��ݣ�YY
    UC		startmonth;			// ��ʼ�£�1-12
    UC		monthcount;			// �������£����12���£�
    UC		reserved1;			// ����
    UC		log;				// ��������־
	UC		reserved2[7];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_010{
    UC		daytype[48];		// λ����: 0 - �����գ�1 - �ڼ���
    US		nd_parent;			// ���ڵ�ID
    US		nd_daysec[3];		// ת�ڵ�ID
    UC		reserved1[8];		// ����
};

//------------------------------------------------------------------------
// Sun added 2004-12-30
// �ڵ�016��������֧
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_016{
    UC		reserved1[5];		// ����
    UC		log;                // ��������־
	UC		var_id;				// ������
    UC		logic;              // �߼������
    UC		convert;            // ת����ʽ
    UC		param1;             // ����1
    UC		param2;             // ����2
    UC		reserved2[2];       // ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_016{
    UC		var_value[48];      // ����ֵ
    US		nd_parent;	        // ���ڵ�ID
    US		nd_succ;	        // ��������ת�ƽڵ�ID
    US		nd_fail;			// ����������ת�ƽڵ�ID
    UC		reserved3[10];      // ����
};

/// �߼����������
#define DEF_NODE016_LOGIC_EQUE			0			// =
#define DEF_NODE016_LOGIC_BIGGER		1			// >
#define DEF_NODE016_LOGIC_SMALLER		2			// <
#define DEF_NODE016_LOGIC_EB			3			// >=
#define DEF_NODE016_LOGIC_ES			4			// <=
#define DEF_NODE016_LOGIC_NE			5			// <>
#define DEF_NODE016_LOGIC_LIKE			6			// Like

/// ת����ʽ����
#define DEF_NODE016_CONVERT_NONE		0			// Do Nothing
#define DEF_NODE016_CONVERT_LEFT		1			// Left(n)
#define DEF_NODE016_CONVERT_RIGHT		2			// Right(n)
#define DEF_NODE016_CONVERT_MID			3			// Mid(m, n)
#define DEF_NODE016_CONVERT_LEN			4           // Len()

//------------------------------------------------------------------------
// Sun added 2003-04-24
// �ڵ�017����������ѡ��
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_017{
	UC		timeout;			// �ڵ㳬ʱ(��)
	UC		maxtrytime;			// ����Դ���
	UC		var_lang;			// ��֤�����¼
	UC		reserved1[10];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_017{
	US		vox_play;			// ��������
	UC		lang[MAXLANGUAGECOUNT];			// ���Զ�Ӧ����
	UC		reserved1[36];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_succ;			// �ɹ��ڵ�ID
	US		nd_fail;			// ʧ�ܽڵ�ID
	UC		reserved2[10];		// ����
};

//------------------------------------------------------------------------
// Sun updated 2004-12-30
// �ڵ�018����������
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_018{
    UC		seperator;			// �ָ���
	UC		reserved1[12];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_018{
    UC		typeflags[15];		// ��ǣ�AppData/UserData
    UC		prefix1[15];        // ����ǰ׺1
    UC		prefix2[15];		// ����ǰ׺2
    UC		valueid[15];		// ����ID
	US		nd_parent;			// ���ڵ�ID
	US		nd_child;			// �ӽڵ�ID
};

//------------------------------------------------------------------------
// �ڵ�019���޲���
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_019{
	UC		reserved1[12];		// ����
	UC		leavequeue;			// �Ƿ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_019{
	US		delaytime;			// ��ʱʱ��(ms)
	UC		reserved1[46];		// ����
	US		nd_parent;			// ���ڵ�ID
	UC		reserved2[14];		// ����
};

//------------------------------------------------------------------------
// �ڵ�020�������һ�
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_020{
	UC		reserved1;			// ����
	UC		playclear;			// ������ձ�־: 0 -- �����;1 -- ���
	UC		reserved2[3];		// ����
	UC		log;				// ��������־
	UC		reserved3[7];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_020{
	US		vox_play;			// ��������
	UC		reserved1[46];		// ����
	US		nd_parent;			// ���ڵ�ID
	UC		reserved2[14];		// ����
};

//------------------------------------------------------------------------
// �ڵ�021����������
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_021{
	UC		reserved1;			// ����
	UC		playclear;			// ������ձ�־: 0 -- �����;1 -- ���
	UC		playtype;			// ��������(�����):0����1���2����4��ĸ8����16����
	UC		usevar;				// ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
	UC		breakkey;			// �жϰ���
	UC		log;				// ��������־
	UC		reserved3[7];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_021{
	US		vox_pred;			// ǰ����������
	US		vox_succ;			// ������������
	UC		reserved1[12];		// ����
	US		com_iid;			// COM�ӿ�ID
	UC		reserved2[30];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_child;			// �ӽڵ�ID
	UC		reserved3[12];		// ����
};

//------------------------------------------------------------------------
// �ڵ�022�������ȴ�����
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_022{
	UC		timeout;			// �ڵ㳬ʱ(��)
	UC		playclear;			// ������ձ�־: 0 -- �����;1 -- ���
	UC		getlength;			// �û����볤��
	UC		maxinterval;		// ���������(��)
	UC		breakkey;			// �жϰ���
	UC		log;				// ��������־
	UC		var_key;			// �û�������¼, 1-255����ID
	UC		maxtrytime;			// ����Դ���
	UC		reserved1[5];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_022{
	US		vox_play;			// ��������
	US		vox_nodefail;		// �ڵ�ʧ�ܲ�������[V2]
	UC		reserved1[28];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_key0;			// �ӽڵ�ID: 0
	US		nd_key1;			// �ӽڵ�ID: 1
	US		nd_key2;			// �ӽڵ�ID: 2
	US		nd_key3;			// �ӽڵ�ID: 3
	US		nd_key4;			// �ӽڵ�ID: 4
	US		nd_key5;			// �ӽڵ�ID: 5
	US		nd_key6;			// �ӽڵ�ID: 6
	US		nd_key7;			// �ӽڵ�ID: 7
	US		nd_key8;			// �ӽڵ�ID: 8
	US		nd_key9;			// �ӽڵ�ID: 9
	US		nd_keyA;			// �ӽڵ�ID: *
	US		nd_keyB;			// �ӽڵ�ID: #
	US		nd_nodefail;		// �ӽڵ�ID: ʧ��ת��ڵ�[V2]
	UC		reserved2[4];		// ����
};

//------------------------------------------------------------------------
// �ڵ�023������ת��
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_023{
	UC		timeout;			// �ڵ㳬ʱ(��)
	UC		playclear;			// ������ձ�־: 0 -- �����;1 -- ���
	UC		breakkey;			// �жϰ���
	UC		reserved1[2];		// ����
	UC		log;				// ��������־
	UC		var_play;			// ����, 1-255����ID
	UC		reserved2[6];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_023{
	US		vox_play;			// ��������
	UC		reserved1[46];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_goto;			// ת�ƽڵ�ID
	UC		reserved2[12];		// ����
};

//------------------------------------------------------------------------
// Sun added 2004-12-30
// �ڵ�028��TTS����
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_028{
	UC		timeout;			// �ڵ㳬ʱ(��)
	UC		playclear;			// ������ձ�־: 0 -- �����;1 -- ���
	UC		usevar;				// ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
	UC		breakkey;			// �жϰ���
	UC		reserved1;			// ����
	UC		log;				// ��������־
	UC		reserved2[7];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_028{
	US		vox_string;			// �����ַ�����֧��"%s"�����Ӵ�
	US		vox_alter;			// TTS������ʱ�������
	UC		playtype;			// �������ͣ�
                                /// 0 - �ַ���
                                /// 1 - �ı��ļ���vox_string��ʽ������Ϊ�ļ�·����
                                /// 2 - ExtData
                                /// 3 - UserData
	UC		reserved1[11];		// ����
	US		com_iid;			// COM�ӿ�ID
	UC		reserved2[30];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_succ;			// TTS���ųɹ�ת�ƽڵ�ID
	US		nd_fail;			// TTS����ʧ��ת�ƽڵ�ID
	UC		reserved3[10];		// ����
};

//------------------------------------------------------------------------
// �ڵ�030���Զ�����ʶ��
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_030{
	US		timeout;			// �ڵ㳬ʱ������500-10000��Ĭ�ϣ�5000��
	UC		playclear;			// ������ձ�־: 0 -- �����;1 -- ���
	UC		trytimes;           // ���Դ���
	UC		dataformat;         // �������ݸ�ʽ
	UC		log;				// ��������־
	UC		var_key;			// �û�������¼����, 0 --����¼��1-255����ID
	UC		var_asr;			// ʶ������¼����ID: 0 --����¼��1 -- 255 ����ID
	UC		recordfile;			// ¼���Ƿ񱣴����ļ��У�0 --�����棻1 ���棨�ļ����Զ����ɣ�
	UC		breakkey;			// �жϰ���
	UC		sendbeep;			// ʶ��ǰ�Ƿ񷢳�Beep����0 -- ������1 -- ��
	UC		reserved1[2];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_030{
	US		vox_op;				// ������ʾ����
	US		grammar_id;			// �﷨�ļ�(��ԴID)
	UC		reserved1[24];		// ����
	US		asr_language;		// ASR����-����
	US		asr_vad_onoff;		// ASR����-�˵��⿪�أ�0-�أ�1-80����Ĭ�ϣ�20��
	US		asr_vad_startcut;	// ASR����-��ʼ�˵㳤�ȣ����룬80-800��Ĭ�ϣ�350��
	US		asr_vad_endcut;		// ASR����-�����˵㳤�ȣ����룬200-2000��Ĭ�ϣ�600��
	UC		reserved2[12];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_key_break;		// �����ж�ת�ڵ�ID
	US		nd_asr_succ;		// ASR�ɹ�ת�ڵ�ID
	US		nd_fail;			// ʧ��ת�ڵ�ID
	UC		reserved3[8];		// ����
};

/// ASR����-�������ݸ�ʽ
#define DEF_NODE030_ASR_DATFMT_PCM		1			// PCM, ƫ��44���ֽ�
#define DEF_NODE030_ASR_DATFMT_ALAW		2			// A-Law, ƫ��58���ֽ�
#define DEF_NODE030_ASR_DATFMT_ULAW		3			// U-Law, ƫ��58���ֽ�
#define DEF_NODE030_ASR_DATFMT_ADPCM	4			// ADPCM, ƫ��60���ֽ�

/// ASR����-����
#define DEF_NODE030_ASR_LANG_CHINESE	1			// ������
#define DEF_NODE030_ASR_LANG_ENGLISH	2			// ��Ӣ��
#define DEF_NODE030_ASR_LANG_CHN_ENG	3			// ��Ӣ��

//------------------------------------------------------------------------
// �ڵ�040����������
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_040{
	UC		rectime;			// ¼��ʱ��(��)
	UC		playclear;			// ������ձ�־: 0 -- �����;1 -- ���
	UC		breakkey;			// �жϰ���
	UC		var_agent;			// Agent ID
	UC		var_filename;       // ¼���ļ�����¼����
	UC		log;				// ��������־
	UC		var_appfield[3];	// Ӧ��������
	UC		maxsilencetime;		// �����ʱ��(��)
    UC		vmsclass;           // ���Է���[2007-02-28]
    UC		MinRecLength;       // ���¼��ʱ��(��)[2007-03-20]
    UC		toneoff;			// ������¼����ʼ��ʾ[2009-07-24]��0 - ���ţ�1 - ������
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_040{
	US		vox_op;				// ¼��ǰ��ʾ����
    UC		recfiletype;        // ¼���ļ�����[2007-06-28]: 0 - vox; 1 - wav
    UC		rectime_ho;         // ¼��ʱ����8λ
	UC		var_notifyintvl;	// ��ʾ���(��)[2009-07-24]��0 - ����ʾ
	UC		var_rectime;		// ¼��ʵ��ʱ����¼����(��)[2009-07-24]
	UC		NotifyPL;			// �Ƿ�֪ͨPL¼��ϵͳ[2012-04-18] [A]
	UC		reserved1[41];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_child;			// �ӽڵ�ID
	UC		reserved2[12];		// ����
};

/// ���Է��ඨ��
#define DEF_NODE040_VMSCLASS_UNKNOWN	0			// δ֪
#define DEF_NODE040_VMSCLASS_ALL		1           // ����
#define DEF_NODE040_VMSCLASS_GROUP		2           // ��
#define DEF_NODE040_VMSCLASS_EXT		3           // �ֻ�
#define DEF_NODE040_VMSCLASS_USER		4           // �û�

//------------------------------------------------------------------------
// �ڵ�041���쿴����
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_041{
	UC		timeout;			// �ڵ㳬ʱ(��)
	UC		playclear;			// ������ձ�־: 0 -- �����;1 -- ���
	UC		breakkey;			// �жϰ���
	UC		var_agent;			// Agent ID
	UC		reserved1;			// ����
	UC		log;				// ��������־
    UC		vmstype;			// �쿴��������
    UC		closewhencheck;		// �쿴���Զ���Ϊ�ر�״̬
	UC		reserved2[5];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_041{
	US		vox_predrec;		// ����״̬����ǰ��
	US		vox_succrec;		// ����״̬�������
	US		vox_browse;			// �����ʾ����
	US		vox_op;				// ������ʾ����
	UC		key_first;			// ����һ�����԰���
	UC		key_previous;		// ǰһ�����԰���
	UC		key_next;			// ����һ�����԰���
	UC		key_last;			// �����һ�����԰���
	UC		key_repeat;			// ������ǰ���԰���
	UC		key_delete;			// ɾ����ǰ���԰���
	UC		key_exit;			// �˳����ڵ㰴��
	UC		key_convert;		// ת�����Ͱ���
	UC		reserved1[32];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_child;			// �ӽڵ�ID
	UC		reserved2[12];		// ����
};

/// �쿴�������Ͷ���
#define DEF_NODE041_VMSTYPE_NEW			0			// �����ԣ���ɾ�����浵
#define DEF_NODE041_VMSTYPE_CLOSED		1           // �浵���ԣ���ɾ��
#define DEF_NODE041_VMSTYPE_DELETED		2           // ɾ�����ԣ���ȡ��ɾ����Ϊ�浵

//------------------------------------------------------------------------
/// ���촫���ļ���
#define DEF_NODE050_FAXN_TYPE_AUTO		0			// �Զ�����
#define DEF_NODE050_FAXN_TYPE_RESID		1           // ��ԴID
#define DEF_NODE050_FAXN_TYPE_VAR2RESID	2           // ������Ӧ��ԴID
#define DEF_NODE050_FAXN_TYPE_VAR2NAME	3           // ������Ӧ�ļ���
#define DEF_NODE050_FAXN_TYPE_FORMAT	4           // �����滻��Դ�е�ͨ���

// �ڵ�050���򵥴���
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_050{
	US		timeout;			// �ڵ㳬ʱ(��)
	UC		filenametype;		// �����ļ�������: 
								/// 1 -- ��ԴID
								/// 2 -- ������Ӧ��ԴID
								/// 3 -- ������Ӧ�ļ���
								/// 4 -- �����滻��Դ�е�ͨ���
								/// һ�ζ�����Ͷ���ļ��÷ֺŷָ�����5��
	UC		trytimes;           // ���Դ���
	UC		record_cdr;			// �Ƿ��¼���淢���굥: 0 --����¼��1 -- ��¼
	UC		log;				// ��������־
    UC		var_faxfile;        // �����ļ�ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
	UC		var_fromno;         // ��������ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
	UC		var_tono;			// ���պ���ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
    UC		var_result;         // �����¼����ID: 0 --��ʹ�ã�1 -- 255 ����ID
	UC		var_appfield[3];	// Ӧ��������
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_050{
	US		vox_op;				// ������ʾ����
	US		fax_fileid;			// �����ļ�(��ԴID)
	US		header_id;			// �������(��ԴID)
	UC		reserved1[42];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_succ;			// �ɹ�ת�ڵ�ID
	US		nd_fail;			// ʧ��ת�ڵ�ID
	UC		reserved2[10];		// ����
};

//------------------------------------------------------------------------
// �ڵ�051��TTF����
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_051{
	US		timeout;			// �ڵ㳬ʱ(��)
	UC		reserved1[3];		// ����
	UC		log;				// ��������־
	UC		reserved2[7];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_051{
	US		vox_op;				// ������ʾ����
	US		fax_logo;			// LOGO�ļ�(��ԴID)
	US		fax_format;			// ����ʽ�ļ�(��ԴID)
	US		header_id;			// �������(��ԴID)
	US		from_id;			// ��������(��ԴID)
	UC		reserved1[6];		// ����
	US		com_iid;			// COM�ӿ�ID
	UC		reserved2[30];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_child;			// �ӽڵ�ID
	UC		reserved3[12];		// ����
};

//------------------------------------------------------------------------
// �ڵ�055���������
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_055{
	US		timeout;			// �ڵ㳬ʱ(��)
	UC		filenametype;		// �����ļ�������: 
								/// 0 -- �Զ����ɣ�$Record\Group%n\%Y%m%d%H%M%S_c<CH>.tif��
								/// 1 -- ��ԴID
								/// 2 -- ������Ӧ��ԴID
								/// 3 -- ������Ӧ�ļ���
								/// 4 -- �����滻��Դ�е�ͨ���
	UC		var_faxfile;        // �����ļ�ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
	UC		record_cdr;			// �Ƿ��¼��������굥: 0 --����¼��1 -- ��¼
	UC		log;				// ��������־
	UC		var_fromno;         // [get]���������¼����ID: 0 --��ʹ�ã�1 -- 255 ����ID
	UC		var_tono;			// ���պ����¼����ID: 0 --��ʹ�ã�1 -- 255 ����ID
	UC		var_extno;			// �ֻ������¼����ID: 0 --��ʹ�ã�1 -- 255 ����ID
    UC		var_result;         // �����¼����ID: 0 --��ʹ�ã�1 -- 255 ����ID
	UC		var_appfield[3];	// Ӧ���������¼����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_055{
	US		vox_op;				// ������ʾ����
	US		fax_fileid;			// �����ļ�(��ԴID)
	UC		reserved1[44];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_succ;			// �ɹ�ת�ڵ�ID
	US		nd_fail;			// ʧ��ת�ڵ�ID
	UC		reserved2[10];		// ����
};

//------------------------------------------------------------------------
// �ڵ�060��ת����ϯ
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_060{
	UC		timeout;			// �ڵ㳬ʱ(��)
	UC		switchtype;			// ת�ӷ�ʽ(0���Զ����ӣ�1��ָ����ϯ��2���û�����)
	UC		agentid;			// ָ����ϯID, ��ʽ1ʱ / �жϰ���, ��ʽ2ʱ
	UC		getlength;			// �û����볤��, ת�ӷ�ʽΪ2ʱ��Ч��ת�ӷ�ʽΪ1ʱΪָ����ϯID�ĸ�λ
	UC		looptimes;			// �ȴ�ѭ�����Ŵ���
	UC		log;				// ��������־
	UC		var_key;			// �û�������¼, ת�ӷ�ʽΪ2ʱ��Ч��1-255����ID
	UC		agentinfo;			// �Ƿ�������ϯ��Ϣ, 0 ��������1 ����
	UC		reserved1[5];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_060{
	US		vox_op;				// ������ʾ����
	US		vox_sw;				// ת����ʾ����
	US		vox_wt;				// �ȴ�ѭ����������
	US		vox_nobody;			// û���ϰ���ʾ����
	US		vox_busy;			// ��ϯæ��ʾ����
	US		vox_noanswer;		// ��ϯ��Ӧ����ʾ����
	US		vox_ok;				// ��ϯת�ӳɹ�
	UC		length_agentinfo;	// ��ϯ��Ϣ���磺���ţ�λ����Ĭ��4λ��������ǰ�油0��0����ԭʼ����
	UC		reserved1[33];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_nobody;			// û���ϰ�ת�ڵ�ID
	US		nd_busy;			// ��ϯæת�ڵ�ID
	US		nd_noanswer;		// ��ϯ��Ӧ��ת�ڵ�ID
	US		nd_ok;				// ת�ӳɹ�ת�ڵ�ID
	UC		reserved2[6];		// ����
};

//------------------------------------------------------------------------
// Sun added 2004-12-30
// �ڵ�061��ת����ϯ��
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_061{
    US		maxwait;			// �ȴ���ʱ(��)��0 ��ʾ���޵ȴ�
    UC		toacd;				// ת��ACD����RoutePoint��0 �� RoutePoint��1 �� ACD
	UC		usevar;				// ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
    UC		looptimes;          // �ȴ�ѭ�����Ŵ�����0 ��ʾ����ѭ��
    UC		log;				// ��������־
    UC		agentinfo;			// �Ƿ�������ϯ��Ϣ, 0 ��������1 ����
	UC		readEWT;			// �Ƿ񲥱�Ԥ��ȴ�ʱ��, 0 ��������1 ����-�룻2 ����-ʱ����
	UC		switchtype;         // ת�ӷ�ʽ: 0 - ���ţ�1 - CTI
	UC		waitmethod;         // �ȴ���ʽ: 0 - ����������1 - �첽������
	UC		var_userid;			// ת����ϯID��¼����ID: 0 --����¼��1 -- 255 ����ID
	UC		var_loginid;		// ת����ϯ���ż�¼����ID: 0 --����¼��1 -- 255 ����ID
    UC		waitansto;			// �ȴ���ϯӦ��ʱ(��)��0 ��ʾ���޵ȴ�
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_061{
	US		vox_op;				// ������ʾ����-�ڵ㿪ʼ��ת��ǰ��
	US		vox_sw;				// ת����ʾ����-ת����Ӧ�󣨱�EWT��
	US		vox_wt;				// �ȴ�ѭ����������
	US		vox_nobody;			// û���ϰ���ʾ����-ת����Ӧ��
	US		vox_busy;			// ��ϯ��ȫæ��ʾ����-ת����Ӧ��
	US		vox_noanswer;		// ��ϯ��Ӧ����ʾ����
	US		vox_ok;				// ��ϯ��ת�ӳɹ�
    US		routepointid;		// ת�ӵ�·�ɵ���
    UC		acddn[20];			// ת�ӵ�ACD����
	UC		length_agentinfo;	// ��ϯ��Ϣ���磺���ţ�λ����Ĭ��4λ��������ǰ�油0��0����ԭʼ����
	UC		reserved1[10];		// ����
	UC		waitansto_hi;       // ����ȴ���ϯӦ��ʱ�ĸ�8λ
	US		nd_parent;			// ���ڵ�ID
	US		nd_nobody;			// û���ϰ�ת�ڵ�ID
	US		nd_busy;			// ��ϯ��ȫæת�ڵ�ID
	US		nd_noanswer;		// ��ϯ��Ӧ��ת�ڵ�ID�����Ϊ0������ȴ���
	US		nd_ok;				// ת�ӳɹ�ת�ڵ�ID
	US		nd_wait;			// �ȴ�������ڽڵ��
	UC		reserved2[4];		// ����
};

//------------------------------------------------------------------------
// �ڵ�062��������飬2012-04-18 [B]
/// �ڵ����ݽṹ-Data1(03-0F/13b)
/*
struct SData1_062{
	UC		reserved1[3];		// ����
	UC		usevar;				// ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
	UC		looptimes;			// �ȴ�ѭ�����Ŵ���
	UC		log;				// ��������־
	UC		reserved2[7];		// ����
};
*/
struct SData1_062{
    UC		timeout;			// �ڵ㳬ʱ(��)
    UC		reserved1;			// ����
	UC		usevar;				// ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
    UC		looptimes;          // �ȴ�ѭ�����Ŵ�����0 ��ʾʹ��ϵͳĬ�ϴ���
    UC		switchtype;			// ת�ӷ�ʽ: 0 - ���ţ���TAPI������Ч����1 - CTI
    UC		log;				// ��������־
    UC		waitansto;			// �ȴ���ϯӦ��ʱ(��)��0 ��ʾ���޵ȴ�
	UC		reserved2[6];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
/*
struct SData2_062{
	US		vox_op;				// ������ʾ����
	US		vox_sw;				// ת����ʾ����
	US		vox_wt;				// �ȴ�ѭ����������
	US		vox_nobody;			// û���ϰ���ʾ����
	US		vox_busy;			// ��ϯȫæ��ʾ����
	US		vox_ok;				// ��ϯת�ӳɹ�
	UC		reserved1[36];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_nobody;			// û���ϰ�ת�ڵ�ID
	US		nd_busy;			// ��ϯȫæת�ڵ�ID
	US		nd_ok;				// ת�ӳɹ�ת�ڵ�ID
	UC		reserved2[8];		// ����
};
*/
struct SData2_062{
	US		vox_op;				// ������ʾ����
	US		vox_sw;				// ת����ʾ����
	US		vox_wt;				// �ȴ�ѭ����������
	US		vox_noconf;			// ����ʧ����ʾ����
	US		vox_noans;			// ��Ӧ����ʾ����
	US		vox_ans;			// �ɹ�Ӧ����ʾ����
	US		vox_ok;				// ��ɻ�����ʾ����
	US		vox_syserror;		// ϵͳ������ʾ����
	UC		DialNo[24];			// ������룬Ҳ������:������
	UC		predial[6];			// ����ǰ׺
	UC		reserved1[2];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_noans;			// ��Ӧ��ת�ڵ�ID
	US		nd_failed;			// ����ʧ��ת�ڵ�ID
	US		nd_ok;				// ����ɹ�ת�ڵ�ID
	UC		reserved2[8];		// ����
};

//------------------------------------------------------------------------
// �ڵ�063����ǿת����ϯ��
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_063{
	UC		reserved1[3];		// ����
	UC		usevar;				// ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
	UC		looptimes;			// �ȴ�ѭ�����Ŵ���
	UC		log;				// ��������־
	UC		reserved2[7];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_063{
	US		vox_op;				// ������ʾ����
	US		vox_sw;				// ת����ʾ����
	US		vox_wt;				// �ȴ�ѭ����������
	US		vox_nobody;			// û���ϰ���ʾ����
	US		vox_busy;			// ��ϯȫæ��ʾ����
	US		vox_ok;				// ��ϯת�ӳɹ�
	UC		reserved1[36];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_nobody;			// û���ϰ�ת�ڵ�ID
	US		nd_busy;			// ��ϯȫæת�ڵ�ID
	US		nd_ok;				// ת�ӳɹ�ת�ڵ�ID
	UC		reserved2[8];		// ����
};

//------------------------------------------------------------------------
// �ڵ�066����������
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_066{
	US		maxwait;			// �ȴ���ʱ(��)��0 ��ʾ���޵ȴ�
	UC		waitansto;			// �ȴ���ϯӦ��ʱ(��)��0 ��ʾ���޵ȴ�
	UC		usevar;				// ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
	UC		looptimes;			// �ȴ�ѭ�����Ŵ���
	UC		log;				// ��������־
	UC		switchtype;         // ���鷽ʽ: 0 - ���ţ�1 - CTI
	UC		var_answerdn;		// ����Ӧ������¼����ID: 0 --����¼��1 -- 255 ����ID
	UC		reserved1[5];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_066{
	US		vox_op;				// ������ʾ����
	US		vox_wt;				// �ȴ�ѭ����������
	US		vox_busy;			// Ŀ��æ��ʾ����
	US		vox_noanswer;		// Ŀ����Ӧ����ʾ����
	US		vox_ok;				// Ŀ��ɹ�Ӧ����ʾ����
	UC		reserved1[38];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_busy;			// Ŀ��æת�ڵ�ID
	US		nd_noanswer;		// Ŀ����Ӧ��ת�ڵ�ID
	US		nd_ok;				// ����ɹ�ת�ڵ�ID
	UC		reserved2[8];		// ����
};

//------------------------------------------------------------------------
// �ڵ�069��ת����ֻ�
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_069{
	UC		reserved1[3];		// ����
	UC		usevar;				// ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
	UC		switchtype;         // ת�ӷ�ʽ: 0 - ���ţ�1 - CTI
	UC		log;				// ��������־
	UL		vagency;			// ����ֻ�����
	UC		reserved3[3];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_069{
	US		vox_op;				// ������ʾ����
    US		maxtry;				// ����Դ�����2012-04-18 [C]
    US		tryinterval;		// ���Լ����2012-04-18 [C]
	UC		reserved1[42];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_child;			// �ӽڵ�ID
	UC		reserved2[12];		// ����
};

//------------------------------------------------------------------------
// �ڵ�070����ѯ·�ɵ�
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_070{
    UC		timeout;			// �ڵ㳬ʱ(��)
	UC		reserved1;			// ����
	UC		usevar;				// ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
    UC		paramindex;			// �������
    UC		logic;				// �߼������
    UC		log;				// ��������־
	UC		querytype;          // ��ѯ���ͣ�0 - ·�ɵ㣻1 - ���У�2 - �飻3 - С��
    UC		var_result;			// ��ѯ�����¼����: 0 -- ��ʹ�ã�1 -- 255 ����ID��[2007-04-12]
    UC		reserved2[5];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_070{
    US		routepointid;		// ·�ɵ���
    US		comparedvalue;		// �ο�ֵ
    UC		reserved1[12];		// ����
    US		com_iid;			// COM�ӿ�ID
    UC		reserved2[30];		// ����
    US		nd_parent;			// ���ڵ�ID
    US		nd_yes;				// ��������ת�ڵ�ID
    US		nd_no;				// ����������ת�ڵ�ID
    US		nd_fail;			// ��ѯʧ�ܽڵ�ID
    UC		reserved3[8];		// ����
};

//------------------------------------------------------------------------
// �ڵ�071����ѯ��ϯ״̬
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_071{
    UC		timeout;			// �ڵ㳬ʱ(��)
	UC		usevar;				// ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
    UC		dn_status;			// DN״̬���
	UC		pos_status;			// POS״̬���
    UC		dn_logic;			// DN�߼������
	UC		pos_logic;			// POS�߼������
    UC		log;				// ��������־
	UC		querytype;          // ��ѯ���ͣ�0 - Agent; 1 - User (����ʹ�ñ���ȷ���û���); 2 - ����
	UC		conditions;			// �������μ����������塱
    UC		reserved1[4];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_071{
    UL		agentid;			// ��ϯ���
    UC		reserved1[44];		// ����
    US		nd_parent;			// ���ڵ�ID
    US		nd_yes;				// ��������ת�ڵ�ID
    US		nd_no;				// ����������ת�ڵ�ID
    US		nd_fail;			// ��ѯʧ�ܽڵ�ID
    UC		reserved2[8];		// ����
};

/// ��������
#define DEF_NODE071_CONDITION_NONE		0
#define DEF_NODE071_CONDITION_FIRST		1
#define DEF_NODE071_CONDITION_SECOND	2
#define DEF_NODE071_CONDITION_BOTH		3
#define DEF_NODE071_CONDITION_EITHER	4

//------------------------------------------------------------------------
// �ڵ�080������ACD�Ŷ�
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_080{
	UC		reserved1[3];		// ����
	UC		usevar;				// ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
	UC		reserved2;			// ����
	UC		log;				// ��������־
	US		maxwaittime;		// ��Ŷӵȴ�ʱ��(��)
	UC		reserved3[5];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_080{
	US		vox_wait;			// �Ŷӵȴ�����
	UC		reserved1[14];		// ����
	US		com_iid;			// COM�ӿ�ID
	UC		reserved2[30];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_child;			// �ӽڵ�ID
	UC		reserved3[12];		// ����
};

//------------------------------------------------------------------------
// �ڵ�090���������ߺ���
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_090{
	UC		timeout;			// �Ⲧ��ʱ����
    UC		numbertype;         // ��������: 0 -- �̶��趨��1 -- ������2 -- �Ⲧ��Ϣ��3 -- COM
    UC		dialtype;           // ��������: 0 -- ���������ţ�1 -- CTI����
    UC		connecttype;        // ���Ž�ͨ�б�: 0 -- �������б�1 -- CTI��Ϣ�б�
    UC		trytimes;           // ���Դ���
	UC		log;				// ��������־
    UC		extdelay;           // ���ֻ�ǰ��ʱ����
    UC		usevar;             // ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
    UC		resultvar;          // �����¼����ID: 0 --��ʹ�ã�1 -- 255 ����ID
    UC		resultinform;       // ����Ƿ�֪ͨ�ⲿϵͳ
	UC		explictoffhook;     // �Ƿ�ǿ��ժ����Sun added 2012-01-17
    UC		reserved1[2];       // ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_090{
    UC		predial[14];        // ����ǰ׺
    UC		phoneno[32];        // ���к��룬��������Ϊ0ʱ��Ч
	US		com_iid;			// COM�ӿ�ID
	US		nd_parent;			// ���ڵ�ID
	US		nd_succ;			// �ɹ�ת�ڵ�ID
	US		nd_fail;			// ʧ��ת�ڵ�ID
    US		setANI;				// ��ʽ�������к��루��ԴID����Sun added 2012-06-26
	UC		reserved1[8];		// ����
};

//------------------------------------------------------------------------
// �ڵ�091��Calling Card
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_091{
	UC		timeout;			// �ڵ㳬ʱ����
    UC		talklentype;        // ��ͨ��ʱ��������ʽ: 0 -- ����Ҫ������1 -- ʱ���룻2 -- ���룻3 -- �֣��������룩��4 -- ��
	UC		obgroup;			// �Ⲧ���
	UC		remindminute;		// ͨ��ʱ��ʣ����ٷ�������
	UC		reserved1;			// ����
	UC		log;				// ��������־
    UC		reserved2;			// ����
    UC		var_cardno;			// ����ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
    UC		var_telno;			// �Ⲧ����ʹ�ñ���ID: 0 --��ʹ�ã�1 -- 255 ����ID
    UC		var_connectlength;	// ͨ��ʱ����¼����ID: 0 --����¼��1 -- 255 ����ID
    UC		reserved3[3];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_091{
    US		vox_talklen;		// ������ͨ��ʱ��ǰ��������ϵͳ�Զ�����talklentype������ʱ�䵥λʹ��ϵͳ������Դ��
    US		vox_timeout;		// ͨ����ʱ��������
	US		vox_noservice;		// �������Ч��ʾ����
	UC		reserved1[38];		// ����
    US		com_talklength;		// Ԥ�ȼ����ͨ��ʱ��COM�ӿ�ID������ʱ���룩��������ʽ����0ʱ��Ч
    US		com_billing;		// �Ʒ�COM�ӿ�ID�����ر���ͨ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_child;			// �ӽڵ�ID
	UC		reserved2[12];		// ����
};

//------------------------------------------------------------------------
// �ڵ�096���첽ͨ�Žڵ�
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_096{
    UC		timeout;			// �ȴ���ʱ����
    UC		seperator;			// �ָ���
    UC		extdata;			// ��չ���ݴ���ʽ: 0 -- ������1 -- �����ļ���2 -- ��¼��������3 -- ��¼��AppData��4 -- ��¼��UserData��5 -- TTS��
	UC		extvar;				// extdata��¼����
    UC		reserved1;			// ����
    UC		log;				// ��������־
	UC		carryonasynplay;	// �Ƿ��յ�ͨ����Ӧ�Ƿ�����첽������Ĭ��Ϊ0-������
    UC		reserved2[6];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_096{
    US		command;			// �������
    US		vox_wt;				// �ȴ�ѭ����������
    UC		var_send[10];		// ���ͱ���ID
    UC		var_receive[10];	// ���ձ���ID
	UC		fileprefix[2];		// �ļ���ǰ׺
    UC		reserved1[22];		// ����
    US		nd_parent;			// ���ڵ�ID
    US		nd_child;			// �ӽڵ�ID
    US		nd_timeout;			// ��ʱ�ڵ�ID
    UC		reserved2[10];		// ����
};

//------------------------------------------------------------------------
// �ڵ�100���û�DLL
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_100{
	UC		reserved1[5];		// ����
	UC		log;				// ��������־
	UC		reserved2[7];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_100{
	US		dll_fid;			// DLL�ļ�ID
	UC		reserved1[46];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_child;			// �ӽڵ�ID
	UC		reserved2[12];		// ����
};

//------------------------------------------------------------------------
// �ڵ�101���û�COM
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_101{
	UC		reserved1[5];		// ����
	UC		log;				// ��������־
	UC		reserved2[7];		// ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_101{
	UC		reserved1[16];		// ����
	US		com_iid;			// COM�ӿ�ID
	UC		reserved2[30];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_child;			// �ӽڵ�ID
	UC		reserved3[12];		// ����
};

//------------------------------------------------------------------------
// Sun update 2004-12-30
// �ڵ�102����¼����
/// �ڵ����ݽṹ-Data1(03-0F/13b)
struct SData1_102{
    UC		reserved1[5];		// ����
    UC		log;                // ��������־
    UC		var_chg;	        // ��¼����ID: 0 --����¼��1 -- 255 ����ID
    UC		convert;            // ת����ʽ
    UC		param1;             // ����1
    UC		param2;             // ����2
    UC		reserved2[3];       // ����
};
/// �ڵ����ݽṹ-Data2(30-6F/64b)
struct SData2_102{
	UC		reserved1[16];		// ����
	US		com_iid;			// COM�ӿ�ID
	UC		value[24];			// ����ֵ
	UC		reserved2[6];		// ����
	US		nd_parent;			// ���ڵ�ID
	US		nd_child;			// �ӽڵ�ID
	UC		reserved3[12];		// ����
};

//------------------------------------------------------------------------
// Sun added 2002-05-23
// Ver 2 Mend 1
// Exchange File Header Definition
// 512 Bytes
struct SCFFileHeader{
    UC		P_ID_Name[10];		// �ָ���                  (10)
    UC		P_ID;				// ��ĿID                    (1)
    UC		P_Type_Name[10];	// �ָ���                  (10)
    UC		P_Type;				// ��Ŀ����                  (1)
    UC		P_Name_Name[10];	// �ָ���                  (10)
    UC		P_Name[50];			// ��Ŀ����                  (50)
    UC		P_Description_Name[10];	// �ָ���               (10)
    UC		P_Description[200];	// ��Ŀ��������              (200)
    UC		P_Version_Name[10];	// �ָ���                  (10)
    UC		P_Version[10];		// ��Ŀ�汾��                (10)
    UC		P_Auther_Name[10];	// �ָ���                  (10)
    UC		P_Auther[50];		// ��Ŀ����                  (50)
    UC		P_User_Name[10];	// �ָ���                  (10)
    UC		P_User[50];			// ��Ŀ�û�                  (50)
    UC		P_CreateTime_Name[10];	// �ָ���                (10)
    UC		P_CreateTime[8];	// ��Ŀ����ʱ��              (8)
    UC		P_ModifyTime_Name[10];	// �ָ���                (10)
    UC		P_ModifyTime[8];	// ��Ŀ����޸�ʱ��          (8)
    UC		P_RCN_Name[10];		// �ָ���                  (10)
    US		P_RCN;				// ��¼��                    (2)
	US		P_RCN1;				// ��¼��                    (2)
    UC		P_Reserved_Name[10];	// �ָ���                  (10)
    UC		Reserved[20];		// ��䱣��                  (20)
};

// Exchange File Record Definition
// 128 Bytes
struct SCFFileRecord{
    US		N_ID;				// �ڵ�ID                    (2)
    UC		N_NO;				// �ڵ���                  (1)
    UC		N_Description[32];	// �ڵ���������              (32)
    UC		N_Page;				// �ڵ�����ҳ��              (1)
    US		N_Left;				// �ڵ�������                (2)
    US		N_Top;				// �ڵ�������                (2)
    US		N_Height;			// �ڵ�߶�                  (2)
    US		N_Width;			// �ڵ���                  (2)
    UC		N_Data1[DEF_NODE_DATA_SMALL_LEN];	// �ڵ����ݶ�1               (13)
    UC		N_Data2[DEF_NODE_DATA_BIG_LEN];		// �ڵ����ݶ�2               (64)
    UC		Reserved[7];		// ����                      (7)
};

//------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------