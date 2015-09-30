#include "BS_WITHOUT_CIR.h"
#include "../Instruments/Interpolation.h"
#undef max
#define max( a, b ) ((a) > (b) ? (a) : (b))
#undef min
#define min( a, b ) ((a) < (b) ? (a) : (b))

BS_WITHOUT_CIR::BS_WITHOUT_CIR(InterfaceESG0 * &interface_Esg):DIF_MODEL0()
{
	UNREFERENCED_PARAMETER(interface_Esg);
}
BS_WITHOUT_CIR::~BS_WITHOUT_CIR()
{

}




