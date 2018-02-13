
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RTPRECOGNIZEDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RTPRECOGNIZEDLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef RTPRECOGNIZEDLL_EXPORTS
#define RTPRECOGNIZEDLL_API __declspec(dllexport)
#else
#define RTPRECOGNIZEDLL_API __declspec(dllimport)
#endif

// This class is exported from the RtpRecognizeDLL.dll
class RTPRECOGNIZEDLL_API CRtpRecognizeDLL {
public:
	CRtpRecognizeDLL(void);
	// TODO: add your methods here.
};

//设置识别引擎
#define LANG_CN					300
#define LANG_EN					310
#define LANG_FIX				320	

//要识别的音频格式
#define AUDIO_PCM				201
#define AUDIO_ALAW				202
#define AUDIO_ULAW				203


extern RTPRECOGNIZEDLL_API int nRtpRecognizeDLL;

extern RTPRECOGNIZEDLL_API int fnRtpRecognizeDLL(void);
extern RTPRECOGNIZEDLL_API int jASRRtpInit();
extern RTPRECOGNIZEDLL_API void * jASRRtpCreate(char *GrammarName , const int Lang, const int EndLen, unsigned short localport);
extern RTPRECOGNIZEDLL_API int jASRRtpStart(void * pSt , int audioType) ;
extern RTPRECOGNIZEDLL_API int jASRRtpRecognize( void * pSt, char* szResult, unsigned int &nNumResult);
extern RTPRECOGNIZEDLL_API int jASRRtpReset(void * pSt);
extern RTPRECOGNIZEDLL_API int jASRRtpEnd(void * pSt);
extern RTPRECOGNIZEDLL_API int jASRRtpDestory();
//extern RTPRECOGNIZEDLL_API int  Init_Log(char * filename ,int loglevel) ;
//extern RTPRECOGNIZEDLL_API void th_log( int level, char *format, ... ) ;
