// ICoder.h

#ifndef __ICODER_H
#define __ICODER_H

#include "IStream.h"

// {23170F69-40C1-278A-0000-000200040000}
DEFINE_GUID(IID_ICompressProgressInfo, 
0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x02, 0x00, 0x04, 0x00, 0x00);
MIDL_INTERFACE("23170F69-40C1-278A-0000-000200040000")
ICompressProgressInfo: public IUnknown
{
    STDMETHOD(SetRatioInfo)(const UInt64 *inSize, const UInt64 *outSize) = 0;
};

// {23170F69-40C1-278A-0000-000200050000}
DEFINE_GUID(IID_ICompressCoder, 
0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x02, 0x00, 0x05, 0x00, 0x00);
MIDL_INTERFACE("23170F69-40C1-278A-0000-000200050000")
ICompressCoder: public IUnknown
{
    STDMETHOD(Code)(ISequentialInStream *inStream,
        ISequentialOutStream *outStream, 
        const UInt64 *inSize, 
        const UInt64 *outSize,
        ICompressProgressInfo *progress) = 0;
};

namespace NCoderPropID
{
    enum EEnum
    {
        kDictionarySize = 0x400,
        kUsedMemorySize,
        kOrder,
        kPosStateBits = 0x440,
        kLitContextBits,
        kLitPosBits,
        kNumFastBytes = 0x450,
        kMatchFinder,
        kNumPasses = 0x460, 
        kAlgorithm = 0x470,
        kMultiThread = 0x480,
        kEndMarker = 0x490
    };
}

// {23170F69-40C1-278A-0000-000200200000}
DEFINE_GUID(IID_ICompressSetCoderProperties, 
0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x02, 0x00, 0x20, 0x00, 0x00);
MIDL_INTERFACE("23170F69-40C1-278A-0000-000200200000")
ICompressSetCoderProperties: public IUnknown
{
    STDMETHOD(SetCoderProperties)(const PROPID *propIDs, 
        const PROPVARIANT *properties, UInt32 numProperties) PURE;
};

// {23170F69-40C1-278A-0000-000200270000}
DEFINE_GUID(IID_ICompressSetOutStream, 
0x23170F69, 0x40C1, 0x278A, 0x00, 0x00, 0x00, 0x02, 0x00, 0x27, 0x00, 0x00);
MIDL_INTERFACE("23170F69-40C1-278A-0000-000200270000")
ICompressSetOutStream: public IUnknown
{
    STDMETHOD(SetOutStream)(ISequentialOutStream *outStream) PURE;
    STDMETHOD(ReleaseOutStream)() PURE;
};

#endif
