#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CMA_ZONE_SIZE 128

typedef struct
{
	size_t Size;
	size_t DataSize;
	size_t AllocateSize;
	void* Data;
} CMA_MemoryZone;

char ICMA_IsFree(CMA_MemoryZone* Zone, size_t Index)
{
	char* j = (char*)Zone->Data + Zone->DataSize + ((Zone->DataSize + 1) * Index);
	return *j;
}

void ICMA_SetFree(CMA_MemoryZone* Zone, size_t Index, char Value)
{
	char* j = (char*)Zone->Data + Zone->DataSize + ((Zone->DataSize + 1) * Index);
	memset(j, Value, 1);
}

CMA_MemoryZone CMA_Create(size_t DataSize)
{
	CMA_MemoryZone Zone;
	Zone.Size = 0;
	Zone.DataSize = DataSize;
	Zone.AllocateSize = CMA_ZONE_SIZE;
	Zone.Data = calloc(DataSize + 1, CMA_ZONE_SIZE);
	for (size_t i = 0; i < CMA_ZONE_SIZE; i++)
		ICMA_SetFree(&Zone, i, 1);

	return Zone;
}

void CMA_Destroy(CMA_MemoryZone* Zone)
{
	free(Zone->Data);
}

size_t CMA_Push(CMA_MemoryZone* Zone, void* Data)
{
	if (Zone->Size >= Zone->AllocateSize)
	{
		Zone->AllocateSize += CMA_ZONE_SIZE;
		Zone->Data = realloc(Zone->Data, (Zone->DataSize + 1) * Zone->AllocateSize);

		for (size_t i = Zone->AllocateSize - CMA_ZONE_SIZE; i < Zone->AllocateSize; i++)
			ICMA_SetFree(Zone, i, 1);
	}

	size_t Index = Zone->Size;

	for (size_t i = 0; i < Zone->Size; i++)
	{
		if (ICMA_IsFree(Zone, i))
		{
			Index = i;
			break;
		}
	}

	char* TempData = (char*)Zone->Data + ((Zone->DataSize + 1) * Index);
	memcpy(TempData, Data, Zone->DataSize);
	ICMA_SetFree(Zone, Index, 0);

	Zone->Size++;
	return Zone->Size - 1;
}

void* CMA_GetAt(CMA_MemoryZone* Zone, size_t Index)
{
	char* TempData = (char*)Zone->Data + ((Zone->DataSize + 1) * Index);
	if (ICMA_IsFree(Zone, Index))
		TempData = NULL;
	return TempData;
}

void CMA_Pop(CMA_MemoryZone* Zone, size_t Index)
{
	char* TempData = (char*)Zone->Data + ((Zone->DataSize + 1) * Index);
	memset(TempData, 0, Zone->DataSize);
	ICMA_SetFree(Zone, Index, 1);

	for (uint32_t i = Zone->Size - 1; i > 0; i--)
	{
		if (!ICMA_IsFree(Zone, i))
			break;

		ICMA_SetFree(Zone, i, 1);
		Zone->Size--;
	}
}