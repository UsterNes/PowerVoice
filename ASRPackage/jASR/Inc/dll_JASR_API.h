#ifdef __cplusplus
extern "C" {
#endif
__declspec( dllimport ) _stdcall long jASR_Init(char*);
__declspec( dllimport ) _stdcall long jASR_CreateInstance();
__declspec( dllimport ) _stdcall long jASR_LoadGrammar(const char* const,const int);
__declspec( dllimport ) _stdcall long jASR_SetActiveGramma(const char* );
__declspec( dllimport ) _stdcall long jASR_ActiveEngine(int pID);
__declspec( dllimport ) _stdcall long jASR_SetParams(short*);
__declspec( dllimport ) _stdcall long jASR_RunASR(char* ,long );
__declspec( dllimport ) _stdcall char jASR_GetResult(char*,int*);
__declspec( dllimport ) _stdcall long jASR_Quit(int flag);
__declspec( dllimport ) _stdcall long jASR_LoadMultiGrammar(const char* pMultiGrammar,const int nMGNum);
__declspec( dllimport ) _stdcall long jASR_SetActiveMultiGramma(const char* pMultiGrammar,const int nMGNum, const int* pFlagpass , const int nFlag);
__declspec( dllimport ) _stdcall long jASR_SetDeActiveMultiGramma(const char* szGrammarFileName, const int nFlag);
#ifdef __cplusplus
}
#endif

//识别正常结束
#define DEF_RunStatus_Result 4
//识别被强制结束
#define DEF_RunStatus_ForceOver 32

#define DEF_ChEngEngine 1000

//--------------------------------------------
//! 数据格式字段
#define DEF_DataFormat_Index 0 

//! 语言类型字段
#define DEF_Language_Index 1 

//!数据文件头字段
#define DEF_DataFileHead_Index 2 

//!语音端点检测字段
#define DEF_P_PCMVAD_OnOff_Index 4 

//!起始端点长度字段
#define DEF_P_PCM_voiceStart_Index 5 

//!结束端点长度字段
#define DEF_P_PCM_silenceEnd_Index 6 

//!过长语音结束字段
#define DEF_P_PCM_voiceMax_Index 7 

//--------------------------------------------
// 数据格式以及wave文件头设置
//! DEF_PCM格式，从头开始偏移掉44个字节
#define DEF_PCM 201

//! DEF_A_Law格式，从头开始偏移掉58个字节
#define DEF_A_Law 202

//! DEF_U_Law格式，从头开始偏移掉58个字节
#define DEF_U_Law 203

//! DEF_ADPCM格式，从头开始偏移掉60个字节
#define DEF_ADPCM 204

//--------------------------------------------
// 设置中英文
//! 中文
#define DEF_OnlyChinese 300 

//! 英文
#define DEF_OnlyEnglish 310 

//!中英文混合
#define DEF_BothChnEng 320 
