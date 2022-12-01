#ifndef VENDOR_FATBUS_FAT_BUS_H_
#define VENDOR_FATBUS_FAT_BUS_H_

#include "ff.h"

namespace FatBus {

#define MAX_HANDLES_COUNT 10
#define ID_EMPTY (MAX_HANDLES_COUNT + 1)

typedef int ID;
typedef FRESULT ErrorCode;
typedef BYTE FLAGS;

struct _FileHandle {
	ID id;
	FIL fil;
};

enum BusSuccess {
	FAILURE = 0,
	SUCCESS
};

class FBus {
protected:
	FBus();

public:
    FBus(FBus& other) = delete;
    void operator=(const FBus&) = delete;

    static FBus& Get() { static FBus _instance; return _instance; }

	BusSuccess Init(ErrorCode& out_Error);
	BusSuccess FOpen(const char* path, FLAGS flags, ID& out_Id, ErrorCode& out_Error);
	void Close(ID id);
	void CloseAll();
	BusSuccess FWrite(const char* data, ID id, ErrorCode& out_Error, int& out_BytesWritten);
	BusSuccess FRead(ID id, int count, char* out_Data);
	BusSuccess FFlush(ID id);

private:
	~FBus();

	bool idExists(ID id);
	_FileHandle _handles[MAX_HANDLES_COUNT];
	size_t _numHandles;
};

}

#endif /* VENDOR_FATBUS_FAT_BUS_H_ */
