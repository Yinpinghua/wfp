#pragma once

#define WFP_TAG	'wfpT'

#pragma pack(push)
#pragma pack(1)
typedef struct _tagWfp_NetInfo
{
	USHORT      m_uSrcPort;	//Դ�˿�
	USHORT      m_uRemotePort;	//Ŀ��˿�
	ULONG       m_ulSrcIPAddr; //Դ��ַ
	ULONG       m_ulRemoteIPAddr; //Ŀ���ַ
	ULONG       m_ulNetWorkType; //Э��
	USHORT		m_uDirection;//���ݰ��ķ���0��ʾ���ͣ�1��ʾ����

} ST_WFP_NETINFO, *PST_WFP_NETINFO;

typedef struct _tagWfp_NetInfoList
{
	LIST_ENTRY		m_linkPointer;
	ST_WFP_NETINFO	m_stWfpNetInfo;

}ST_WFP_NETINFOLIST,*PST_WFP_NETINFOLIST;

#pragma pack(pop)


BOOLEAN InitRuleInfo();

BOOLEAN UninitRuleInfo();

BOOLEAN AddNetRuleInfo(PVOID pRuleInfo,ULONG uLen);

BOOLEAN IsHitRule(ULONG uRemotePort);