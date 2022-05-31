#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CMA_BLOCK_SIZE 128

typedef struct
{
	size_t Size;
	void* Data;
} CMA_Memory;

typedef struct
{
	size_t Size;
	size_t AllocateSize;
	CMA_Memory* Mem;
} CMA_MemoryZone;

CMA_MemoryZone CMA_Create()
{
	CMA_MemoryZone Zone;
	Zone.Size = 0;
	Zone.AllocateSize = CMA_BLOCK_SIZE;
	Zone.Mem = (CMA_Memory*)calloc(CMA_BLOCK_SIZE, sizeof(CMA_Memory));
	return Zone;
}

void CMA_Destroy(CMA_MemoryZone* Zone)
{
	for (size_t i = 0; i < Zone->Size; i++)
	{
		free(Zone->Mem[i].Data);
		Zone->Mem[i].Data = NULL;
		Zone->Mem[i].Size = 0;
	}
	free(Zone->Mem);
	Zone->Mem = NULL;
	Zone->Size = 0;
	Zone->AllocateSize = 0;
}

size_t CMA_Push(CMA_MemoryZone* Mem, size_t Size, void* Data)
{
	if (Mem->Size >= Mem->AllocateSize)
	{
		Mem->Mem = (CMA_Memory*)realloc(Mem->Mem, (Mem->AllocateSize + CMA_BLOCK_SIZE) * sizeof(CMA_Memory));
		Mem->AllocateSize += CMA_BLOCK_SIZE;
	}


	for (size_t i = 0; i < Mem->Size; i++)
	{
		if (Mem->Mem[i].Data == NULL && Mem->Mem[i].Size == 0)
		{
			Mem->Mem[i].Size = Size;
			Mem->Mem[i].Data = malloc(Size);
			memcpy(Mem->Mem[i].Data, Data, Size);
			return i;
		}
	}
	Mem->Mem[Mem->Size].Data = malloc(Size);
	memcpy(Mem->Mem[Mem->Size].Data, Data, Size);

	Mem->Size++;
	return Mem->Size - 1;
}

size_t CMA_GetSize(CMA_MemoryZone* Mem)
{
	if (Mem->Mem != NULL)
		return Mem->Size;

	return 0;
}

void* CMA_GetAt(CMA_MemoryZone* Mem, size_t Index)
{
	if (Mem->Mem != NULL && Index <= Mem->Size)//Size - 1?
		return Mem->Mem[Index].Data;

	return NULL;
}

void CMA_Pop(CMA_MemoryZone* Mem, size_t Index)
{
	free(Mem->Mem[Index].Data);
	Mem->Mem[Index].Data = NULL;
	Mem->Mem[Index].Size = 0;

	for (uint32_t i = CMA_GetSize(Mem) - 1; i > 0; i--)
	{
		if (Mem->Mem[i].Data != NULL)
			break;

		free(Mem->Mem[i].Data);
		Mem->Mem[i].Data = NULL;
		Mem->Mem[i].Size = 0;
		Mem->Size--;
	}
}