#pragma once

#include "include_generic.h"

#ifdef __CYGWIN__
typedef __UINT16_TYPE__ PXCH_UINT16;
typedef __INT32_TYPE__ PXCH_INT32;
typedef __UINT32_TYPE__ PXCH_UINT32;
#else
typedef unsigned __int16 PXCH_UINT16;
typedef __int32 PXCH_INT32;
typedef unsigned __int32 PXCH_UINT32;
#endif


#ifdef __CYGWIN__
#define WPRS L"%s"
#else
#define WPRS L"%S"
#endif

#ifdef _LP64
#define PRIdword  "u"
#define PRIudword "u"
#else
#define PRIdword  "lu"
#define PRIudword "lu"
#endif

#define _PREFIX_L(s) L ## s
#define PREFIX_L(s) _PREFIX_L(s)

#define WPRDW L"%" PREFIX_L(PRIdword)

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

// In characters -- start
#define MAX_DLL_PATH_BUFSIZE 512
#define MAX_CONFIG_FILE_PATH_BUFSIZE 512
#define MAX_DLL_FILE_NAME_BUFSIZE 64
#define MAX_DLL_FUNC_NAME_BUFSIZE 64
#define MAX_IPC_PIPE_NAME_BUFSIZE 128
#define MAX_COMMAND_EXEC_PATH_BUFSIZE 512
#define MAX_COMMAND_LINE_BUFSIZE 1024
#define MAX_HOSTNAME_BUFSIZE 256
#define MAX_USERNAME_BUFSIZE 256
#define MAX_PASSWORD_BUFSIZE 256
#define MAX_PROXY_NUM 5
#define MAX_FILEMAPPING_BUFSIZE 256
// In characters -- end

#define PXCH_PROXY_TYPE_MASK    0x000000FF
#define PXCH_PROXY_TYPE_INVALID 0x00000000
#define PXCH_PROXY_TYPE_SOCKS5  0x00000001

#define PXCH_PROXY_STATE_MASK      0x0000FF00
#define PXCH_PROXY_STATE_INVALID   0x00000000
#define PXCH_PROXY_STATE_UNUSABLE  0x0000F000
#define PXCH_PROXY_STATE_BLOCK     0x0000F100
#define PXCH_PROXY_STATE_IDLE      0x00000100

#define ProxyInit(x) *((PXCH_UINT32*)(&x)) = 0

#define ProxyIsType(type, x) (((*(PXCH_UINT32*)(&x)) & PXCH_PROXY_TYPE_MASK) == PXCH_PROXY_TYPE_##type)
#define ProxyIsState(type, x) (((*(PXCH_UINT32*)(&x)) & PXCH_PROXY_STATE_MASK) == PXCH_PROXY_STATE_##type)
#define ProxyIsInvalid(x) ((((*(PXCH_UINT32*)(&x)) & PXCH_PROXY_TYPE_MASK) == PXCH_PROXY_TYPE_INVALID) || (((*(PXCH_UINT32*)(&x)) & PXCH_PROXY_STATE_MASK) == PXCH_PROXY_STATE_INVALID))

#define SetProxyType(type, x) *((PXCH_UINT32*)(&x)) = (*((PXCH_UINT32*)(&x)) & ~PXCH_PROXY_TYPE_MASK) | PXCH_PROXY_TYPE_##type
#define SetProxyState(type, x) *((PXCH_UINT32*)(&x)) = (*((PXCH_UINT32*)(&x)) & ~PXCH_PROXY_STATE_MASK) | PXCH_PROXY_STATE_##type


#define PXCH_RULE_TYPE_DOMAIN_KEYWORD   0x00000001
#define PXCH_RULE_TYPE_DOMAIN_SUFFIX    0x00000002
#define PXCH_RULE_TYPE_DOMAIN_FULL      0x00000003
#define PXCH_RULE_TYPE_DOMAIN           0x00000003
#define PXCH_RULE_TYPE_IP_CIDR          0x00000004
#define PXCH_RULE_TYPE_PORT             0x00000005
#define PXCH_RULE_TYPE_FINAL            0x00000006
#define PXCH_RULE_TYPE_INVALID          0x00000000

#define RuleInit(x) *((PXCH_UINT32*)(&x)) = PXCH_RULE_TYPE_INVALID

#define RuleIsType(type, x) ((*(PXCH_UINT32*)(&x)) == PXCH_RULE_TYPE_##type)
#define RuleIsInvalid(x) RuleIsType(INVALID, x)

#define SetRuleType(type, x) *((PXCH_UINT32*)(&x)) = PXCH_RULE_TYPE_##type


#define PXCH_HOST_TYPE_INVALID   0x0000
#define PXCH_HOST_TYPE_HOSTNAME  0x0001
#define PXCH_HOST_TYPE_IP        0x0010
#define PXCH_HOST_TYPE_IPV4      0x0010
#define PXCH_HOST_TYPE_IPV6      0x0011

#define HostInit(x) *((PXCH_UINT16*)(&x)) = 0

#define HostIsType(type, x) ((*(PXCH_UINT16*)(&x)) == PXCH_HOST_TYPE_##type)
#define HostIsIp(x) ((*(PXCH_UINT16*)(&x)) & PXCH_HOST_TYPE_IP)
#define HostIsInvalid(x) HostIsType(INVALID, x)

#define SetHostType(type, x) *((PXCH_UINT16*)(&x)) = PXCH_HOST_TYPE_##type


#ifdef PXCHDLL_EXPORTS
#define PXCHDLL_API __declspec(dllexport)	// Cygwin gcc also recognizes this
#else
#define PXCHDLL_API __declspec(dllimport)
#endif


#ifdef __CYGWIN__
#define IF_CYGWIN_EXIT(code) do {exit(code);} while(0)
#define IF_WIN32_EXIT(code) do {} while(0)
#else
#define IF_CYGWIN_EXIT(code) do {} while(0)
#define IF_WIN32_EXIT(code) do {exit(code);} while(0)
#endif


// Consistent with sockaddr
typedef struct _pxch_sockaddr {
	PXCH_UINT16 sa_family;
	char sa_data[14];
} pxch_sockaddr;

// port must be zero
typedef pxch_sockaddr PXCH_IP_ADDRESS;

typedef char PXCH_HOSTNAME_VALUE[MAX_HOSTNAME_BUFSIZE];
typedef char PXCH_USERNAME[MAX_USERNAME_BUFSIZE];
typedef char PXCH_PASSWORD[MAX_USERNAME_BUFSIZE];


typedef union _PXCH_HOSTNAME {
	PXCH_UINT16 wTag;
	PXCH_HOSTNAME_VALUE szValue;
} PXCH_HOSTNAME;


typedef union _PXCH_HOST {
	PXCH_HOSTNAME Hostname;
	PXCH_IP_ADDRESS Ip;
} PXCH_HOST;


typedef struct _PXCH_PROXY_SOCKS5_DATA {
	PXCH_UINT32 dwTag;

	PXCH_HOST Host;
	PXCH_UINT16 wPort;	// host byte order
	PXCH_USERNAME szUsername;
	PXCH_PASSWORD szPassword;
} PXCH_PROXY_SOCKS5_DATA;


typedef union _PXCH_PROXY_DATA {
	PXCH_PROXY_SOCKS5_DATA Socks5;
} PXCH_PROXY_DATA;


typedef struct _PXCH_RULE {
	PXCH_UINT32 dwTag;

	PXCH_HOST HostAddress;
	PXCH_UINT32 dwCidrPrefixLength;
	
	PXCH_UINT32 bWillProxy;
} PXCH_RULE;


#define PXCHCONFIG_EXTRA_SIZE(pPxchConfig) ((sizeof(PXCH_RULE) * (pPxchConfig)->dwRuleNum) + (sizeof(PXCH_PROXY_DATA) * (pPxchConfig)->dwProxyNum))
#define PXCHCONFIG_EXTRA_SIZE_G PXCHCONFIG_EXTRA_SIZE(g_pPxchConfig)
#define PXCHCONFIG_EXTRA_SIZE_BY_N(ruleNum, proxyNum) ((sizeof(PXCH_RULE) * ruleNum) + (sizeof(PXCH_PROXY_DATA) * proxyNum))

#define PXCHCONFIG_PROXY_ARR(pPxchConfig) ((PXCH_PROXY_DATA*)((char*)(pPxchConfig) + pPxchConfig->cbProxyListOffset))
#define PXCHCONFIG_RULE_ARR(pPxchConfig) ((PXCH_RULE*)((char*)(pPxchConfig) + pPxchConfig->cbRuleListOffset))

#define PXCHCONFIG_PROXY_ARR_G PXCHCONFIG_PROXY_ARR(g_pPxchConfig)
#define PXCHCONFIG_RULE_ARR_G PXCHCONFIG_RULE_ARR(g_pPxchConfig)

typedef struct _PROXYCHAINS_CONFIG {
	PXCH_UINT32 dwMasterProcessId;
	PXCH_INT32 iIsQuiet;
	wchar_t szIpcPipeName[MAX_IPC_PIPE_NAME_BUFSIZE];
	wchar_t szConfigPath[MAX_CONFIG_FILE_PATH_BUFSIZE];
	wchar_t szHookDllPath[MAX_DLL_PATH_BUFSIZE];
	wchar_t szMinHookDllPath[MAX_DLL_PATH_BUFSIZE];
	wchar_t szCommandLine[MAX_COMMAND_LINE_BUFSIZE];
	
	PXCH_UINT32 cbProxyListOffset;
	PXCH_UINT32 dwProxyNum;

	PXCH_UINT32 cbRuleListOffset;
	PXCH_UINT32 dwRuleNum;
} PROXYCHAINS_CONFIG;

static const wchar_t g_szChildDataSavingFileMappingPrefix[] = L"Local\\proxychains_child_data_";

#ifdef __CYGWIN__
static const wchar_t g_szHookDllFileName[] = L"cygproxychains_hook.dll";
#else
static const wchar_t g_szHookDllFileName[] = L"proxychains_hook.dll";
#endif

#if defined(_M_X64) || defined(__x86_64__)
static const wchar_t g_szMinHookDllFileName[] = L"MinHook.x64.dll";
#else
static const wchar_t g_szMinHookDllFileName[] = L"MinHook.x86.dll";
#endif