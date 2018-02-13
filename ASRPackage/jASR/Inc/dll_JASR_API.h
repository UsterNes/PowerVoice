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

//ʶ����������
#define DEF_RunStatus_Result 4
//ʶ��ǿ�ƽ���
#define DEF_RunStatus_ForceOver 32

#define DEF_ChEngEngine 1000

//--------------------------------------------
//! ���ݸ�ʽ�ֶ�
#define DEF_DataFormat_Index 0 

//! ���������ֶ�
#define DEF_Language_Index 1 

//!�����ļ�ͷ�ֶ�
#define DEF_DataFileHead_Index 2 

//!�����˵����ֶ�
#define DEF_P_PCMVAD_OnOff_Index 4 

//!��ʼ�˵㳤���ֶ�
#define DEF_P_PCM_voiceStart_Index 5 

//!�����˵㳤���ֶ�
#define DEF_P_PCM_silenceEnd_Index 6 

//!�������������ֶ�
#define DEF_P_PCM_voiceMax_Index 7 

//--------------------------------------------
// ���ݸ�ʽ�Լ�wave�ļ�ͷ����
//! DEF_PCM��ʽ����ͷ��ʼƫ�Ƶ�44���ֽ�
#define DEF_PCM 201

//! DEF_A_Law��ʽ����ͷ��ʼƫ�Ƶ�58���ֽ�
#define DEF_A_Law 202

//! DEF_U_Law��ʽ����ͷ��ʼƫ�Ƶ�58���ֽ�
#define DEF_U_Law 203

//! DEF_ADPCM��ʽ����ͷ��ʼƫ�Ƶ�60���ֽ�
#define DEF_ADPCM 204

//--------------------------------------------
// ������Ӣ��
//! ����
#define DEF_OnlyChinese 300 

//! Ӣ��
#define DEF_OnlyEnglish 310 

//!��Ӣ�Ļ��
#define DEF_BothChnEng 320 
