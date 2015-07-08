#include "DelayLoadThunkReader.h"

namespace PE
{
	namespace DelayLoad
	{
		DelayLoadThunkReader::DelayLoadThunkReader(PeDecoder& pe, PIMAGE_DELAYLOAD_DESCRIPTOR descriptor)
		{
			Init(pe, descriptor);
		}
		DelayLoadThunkReader::DelayLoadThunkReader()
		{
			Reset();
		}
		void DelayLoadThunkReader::Init(PeDecoder& pe, PIMAGE_DELAYLOAD_DESCRIPTOR descriptor) 
		{
			this->descriptor = descriptor;
			importAddressTable = pe.GetRvaData(descriptor->ImportAddressTableRVA);
			importNameTable = pe.GetRvaData(descriptor->ImportNameTableRVA);
			Reset();
		}

		DelayLoadThunkReader::~DelayLoadThunkReader()
		{
		}
		PVOID DelayLoadThunkReader::CurrentNameTable()
		{
			return currentImportNameTable;
		}
		PVOID DelayLoadThunkReader::CurrentAddressTable()
		{
			return currentImportAddressTable;
		}
		bool DelayLoadThunkReader::Next()
		{
			if (currentImportAddressTable && currentImportNameTable)
			{
				currentImportAddressTable = (HANDLE_PTR*)currentImportAddressTable + 1;
				currentImportNameTable = (HANDLE_PTR*)currentImportNameTable + 1;
				if (*(HANDLE_PTR*)currentImportAddressTable && *(HANDLE_PTR*)currentImportNameTable)
				{
					return true;
				}
			}
			else
			{
				currentImportAddressTable = importAddressTable;
				currentImportNameTable = importNameTable;
				return true;
			}
			
			
			return false;
		}
		void DelayLoadThunkReader::Reset()
		{
			currentImportAddressTable = NULL;
			currentImportNameTable = NULL;
		}
	}
}