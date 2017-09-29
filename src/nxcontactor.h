// (C) 2010 Yoshy
// ��������� ������ NxContactor - ���������� ����������� � ��������
// ��������������� ������� � ���������� ����� PhysX.

#ifndef _NXCONTACTOR_H_
#define _NXCONTACTOR_H_

#define WIN32
#define NX_CALL_CONV __cdecl
#define NX32

#include "NxUserContactReport.h"

namespace SpaceArena {

class NxContactor : public NxUserContactReport
{
public: 
	virtual void onContactNotify(NxContactPair& pair, NxU32 events);
};

};
#endif _NXCONTACTOR_H_
