#include "fat_bus.h"
#include <string.h>

namespace FatBus {
static FATFS FatFs; 	//Fatfs handle

FBus::FBus():
_numHandles(0)
{
	for (int i = 0; i < MAX_HANDLES_COUNT; i++)
	{
		_handles[i].id = ID_EMPTY;
	}
}

FBus::~FBus()
{
	CloseAll();
}

bool FBus::idExists(ID id)
{
	for (int i = 0; i < MAX_HANDLES_COUNT; i++)
	{
		if (_handles[i].id != ID_EMPTY)
		{
			return true;
		}
	}

	return false;
}

BusSuccess FBus::Init(ErrorCode& out_Error)
{
  //Open the file system
  out_Error = f_mount(&FatFs, "", 1); //1=mount now
  if (out_Error != FR_OK) {
	  return FAILURE;
  }

  //Let's get some statistics from the SD card
//  DWORD free_clusters, free_sectors, total_sectors;
//
//  FATFS* getFreeFs;
//
//  out_Error = f_getfree("", &free_clusters, &getFreeFs);
//  if (out_Error != FR_OK) {
//	  return FAILURE;
//  }
//
//  //Formula comes from ChaN's documentation
//  total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
//  free_sectors = free_clusters * getFreeFs->csize;
//
////  myprintf("SD card stats:\r\n%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", total_sectors / 2, free_sectors / 2);

	return SUCCESS;
 }

BusSuccess FBus::FOpen(const char* path, FLAGS flags, ID& out_Id, ErrorCode& out_Error)
{
	if (_numHandles >= MAX_HANDLES_COUNT)
	{
		return FAILURE;
	}

	for (int i = 0; i < MAX_HANDLES_COUNT; i++)
	{
		if (_handles[i].id == ID_EMPTY)
		{
			out_Id = i;
			_handles[out_Id].id = out_Id;

			FIL* fil = &(_handles[out_Id].fil);

			out_Error = f_open(fil, path, flags);
			if (out_Error != FR_OK) {
				_handles[out_Id].id = ID_EMPTY;
				return FAILURE;
			}

			_numHandles++;
			return SUCCESS;
		}
	}

	return FAILURE;
}

void FBus::Close(ID id)
{
	if (id < 0 || id > MAX_HANDLES_COUNT || _handles[id].id == ID_EMPTY)
	{
		return;
	}

	f_close(&_handles[id].fil);
	_handles[id].id = ID_EMPTY;
	_numHandles--;
}

void FBus::CloseAll()
{
	for (size_t i = 0; i < MAX_HANDLES_COUNT; i++)
	{
		ID id = _handles[i].id;
		if (id != ID_EMPTY)
		{
			Close(id);
		}
	}
}

BusSuccess FBus::FRead(ID id, int count, char* out_Data)
{
	if (!idExists(id))
	{
		return FAILURE;
	}

	FIL* fil = &(_handles[id].fil);

	TCHAR* rres = f_gets((TCHAR*)out_Data, count, fil);
	if (rres == 0)
	{
		return FAILURE;
	}

	return SUCCESS;
}

BusSuccess FBus::FWrite(const char* data, ID id, ErrorCode& out_Error, int& out_BytesWritten)
{
	if (!idExists(id))
	{
		out_Error = (ErrorCode)99;
		return FAILURE;
	}

	FIL* fil = &(_handles[id].fil);

	UINT bytesWritten = 0;
	out_Error = FR_OK;
	out_Error = f_write(fil, data, strlen(data), &bytesWritten);
	out_BytesWritten = bytesWritten;
	if (out_Error != FR_OK)
	{
		return FAILURE;
	}

	return SUCCESS;
}

BusSuccess FBus::FFlush(ID id)
{
	if (!idExists(id))
	{
		return FAILURE;
	}

	FIL* fil = &(_handles[id].fil);
	f_sync(fil);
}

}
