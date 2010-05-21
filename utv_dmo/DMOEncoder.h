/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

// DMOEncoder.h : CDMOEncoder �̐錾

#pragma once
#include "resource.h"       // ���C�� �V���{��

#include "utv_dmo.h"
#include <vfw.h>
#include "Codec.h"
#include "ClsID.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "DCOM �̊��S�T�|�[�g���܂�ł��Ȃ� Windows Mobile �v���b�g�t�H�[���̂悤�� Windows CE �v���b�g�t�H�[���ł́A�P��X���b�h COM �I�u�W�F�N�g�͐������T�|�[�g����Ă��܂���BATL ���P��X���b�h COM �I�u�W�F�N�g�̍쐬���T�|�[�g���邱�ƁA����т��̒P��X���b�h COM �I�u�W�F�N�g�̎����̎g�p�������邱�Ƃ���������ɂ́A_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ���`���Ă��������B���g�p�� rgs �t�@�C���̃X���b�h ���f���� 'Free' �ɐݒ肳��Ă���ADCOM Windows CE �ȊO�̃v���b�g�t�H�[���ŃT�|�[�g�����B��̃X���b�h ���f���Ɛݒ肳��Ă��܂����B"
#endif



// CDMOEncoder
// 02EB5C68-6991-460D-840B-C1C6497457EF

class ATL_NO_VTABLE CDMOEncoder :
	public CComObjectRootEx<CComMultiThreadModel>,
	public IMediaObjectImpl<CDMOEncoder, 1, 1>
{
private:
	CCodec *m_pCodec;
	IMediaBuffer *m_pInputBuffer;
	BOOL m_bInputKeyFrame;
	BOOL m_bInputTimestampValid;
	BOOL m_bInputTimelengthValid;
	REFERENCE_TIME m_rtInputTimestamp;
	REFERENCE_TIME m_rtInputTimelength;

public:
	CDMOEncoder(DWORD fcc);
	virtual ~CDMOEncoder();

	static HRESULT WINAPI UpdateRegistry(DWORD fcc, REFCLSID clsid, BOOL bRegister);

	BEGIN_COM_MAP(CDMOEncoder)
		COM_INTERFACE_ENTRY(IMediaObject)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	HRESULT InternalGetInputStreamInfo(DWORD dwInputStreamIndex, DWORD *pdwFlags);
	HRESULT InternalGetOutputStreamInfo(DWORD dwOutputStreamIndex, DWORD *pdwFlags);
	HRESULT InternalCheckInputType(DWORD dwInputStreamIndex, const DMO_MEDIA_TYPE *pmt);
	HRESULT InternalCheckOutputType(DWORD dwOutputStreamIndex, const DMO_MEDIA_TYPE *pmt);
	HRESULT InternalGetInputType(DWORD dwInputStreamIndex, DWORD dwTypeIndex, DMO_MEDIA_TYPE *pmt);
	HRESULT InternalGetOutputType(DWORD dwOutputStreamIndex, DWORD dwTypeIndex, DMO_MEDIA_TYPE *pmt);
	HRESULT InternalGetInputSizeInfo(DWORD dwInputStreamIndex, DWORD *pcbSize, DWORD *pcbMaxLookahead, DWORD *pcbAlignment);
	HRESULT InternalGetOutputSizeInfo(DWORD dwOutputStreamIndex, DWORD *pcbSize, DWORD *pcbAlignment);
	HRESULT InternalGetInputMaxLatency(DWORD dwInputStreamIndex, REFERENCE_TIME *prtMaxLatency);
	HRESULT InternalSetInputMaxLatency(DWORD dwInputStreamIndex, REFERENCE_TIME rtMaxLatency);
	HRESULT InternalFlush();
	HRESULT InternalDiscontinuity(DWORD dwInputStreamIndex);
	HRESULT InternalAllocateStreamingResources();
	HRESULT InternalFreeStreamingResources();
	HRESULT InternalProcessInput(DWORD dwInputStreamIndex, IMediaBuffer *pBuffer, DWORD dwFlags, REFERENCE_TIME rtTimestamp, REFERENCE_TIME rtTimelength);
	HRESULT InternalProcessOutput(DWORD dwFlags, DWORD cOutputBufferCount, DMO_OUTPUT_DATA_BUFFER *pOutputBuffers, DWORD *pdwStatus);
	HRESULT InternalAcceptingInput(DWORD dwInputStreamIndex);
};


template<DWORD fcc, const CLSID *pclsid>
class ATL_NO_VTABLE CSpecializedDMOEncoder :
	public CDMOEncoder,
	public CComCoClass<CSpecializedDMOEncoder<fcc, pclsid>, pclsid>
{
public:
	CSpecializedDMOEncoder() : CDMOEncoder(fcc) {}

	static HRESULT WINAPI UpdateRegistry(BOOL bRegister)
	{
		return CDMOEncoder::UpdateRegistry(fcc, *pclsid, bRegister);
	}
};

#define DMOENCODER_ENTRY_AUTO(fcc, fccname) \
	typedef CSpecializedDMOEncoder<fcc, &CLSID_##fccname##DMOEncoder> C##fccname##DMOEncoder; \
	OBJECT_ENTRY_AUTO(CLSID_##fccname##DMOEncoder, C##fccname##DMOEncoder)

//DMOENCODER_ENTRY_AUTO(FCC('ULRA'), ULRA);
DMOENCODER_ENTRY_AUTO(FCC('ULRG'), ULRG);
DMOENCODER_ENTRY_AUTO(FCC('ULY0'), ULY0);
DMOENCODER_ENTRY_AUTO(FCC('ULY2'), ULY2);
