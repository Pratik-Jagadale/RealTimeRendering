#include <windows.h>

/* Pure virtual funstions = abstract base class = Interface */
class ISum :public IUnknown
{
public:
	/* Isum specific method declration */
	virtual HRESULT __stdcall SumOfTwiIntegers(int, int, int*) = 0;//pure virtual
};

class ISubstract : public IUnknown
{
public:
	/* ISubstarct specific method declrations */
	virtual HRESULT __stdcall SubstarctionOfTwoIntegers(int, int, int*) = 0; //pre virtual
};

/* CLSID of SumSubtract component {3D786182-221D-4AE3-9E0F-CA000A61AE16} */
const CLSID CLSID_SumSubtract = { 
	0x3d786182, 0x221d, 0x4ae3, { 0x9e, 0xf, 0xca, 0x0, 0xa, 0x61, 0xae, 0x16 } };

/* CLSID of ISum Interface {C2AA7501-FB63-4262-BFD3-6FA9DD760A50} */
const IID IID_ISum = { 
	0xc2aa7501, 0xfb63, 0x4262, { 0xbf, 0xd3, 0x6f, 0xa9, 0xdd, 0x76, 0xa, 0x50 } };

/* IID of ISubstarct Interface {129B36AB-12B5-4CBE-A6A8-3771FFA5841D} */
const IID IID_ISubtract = { 
	0x129b36ab, 0x12b5, 0x4cbe, { 0xa6, 0xa8, 0x37, 0x71, 0xff, 0xa5, 0x84, 0x1d } };
