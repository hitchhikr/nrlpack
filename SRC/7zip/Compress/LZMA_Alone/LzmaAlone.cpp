// LzmaAlone.cpp
// v1.1:
//  - Added support for elf files with mangled headers.

// --------------------------------------------
// TODO:
//
// - Convert the lzma depacker again (in hand written asm this time).
// - See if we can create a map file or isolate the code.
// - See to use a disassembler and add a dictionary construction system for the packed exes.
//   (Since there's other stuff than code in .text we may need a code tracer).
//
// --------------------------------------------

#define VERSION "1"
#define REVISION "1"

#include "../../../Common/MyWindows.h"
#include "../../../Common/MyInitGuid.h"
#include <shlwapi.h>

#include <stdio.h>

#if defined(_WIN32) || defined(OS2) || defined(MSDOS)
#include <fcntl.h>
#include <io.h>
#define MY_SET_BINARY_MODE(file) _setmode(_fileno(file),O_BINARY)
#else
#define MY_SET_BINARY_MODE(file)
#endif

#include "../../../Common/CommandLineParser.h"
#include "../../../Common/StringConvert.h"
#include "../../../Common/StringToInt.h"

#include "../../Common/FileStreams.h"

#include "../LZMA/LZMAEncoder.h"
#include "ELF.h"

#define DEPACKER_MEMORY

#ifdef DEPACKER_MEMORY
#include "depacker.h"
#endif

using namespace NCommandLineParser;

char tempName[MAX_PATH + 1];
char tempName2[MAX_PATH + 1];
#ifndef DEPACKER_MEMORY
char depackerName[MAX_PATH + 1];
#endif

static void PrintHelp()
{
    printf("\nUsage: NRLPack <Input file> [Output file]\n");
}

static void PrintHelpAndExit(const char *s)
{
    printf("\n%s\n", s);
    PrintHelp();
    throw -1;
}

static void IncorrectCommand()
{
    PrintHelpAndExit("Incorrect command");
}

static void WriteArgumentsToStringList(int numArguments, const char *arguments[], 
    UStringVector &strings)
{
    for(int i = 1; i < numArguments; i++)
    {
        strings.Add(MultiByteToUnicodeString(arguments[i]));
    }
}

int Get_File_Size(const char *FileName)
{
	FILE *FileHandle = NULL;
	int FileSize = 0;

	FileHandle = fopen(FileName, "rb");
	if(FileHandle)
    {
		fseek(FileHandle, 0, SEEK_END);
		FileSize = ftell(FileHandle);
		fseek(FileHandle, 0, SEEK_SET);
		fclose(FileHandle);
	}
	return(FileSize);
}

int main2(int n, const char *args[])
{
	int i;

	printf("NRLPack v"VERSION"."REVISION" written by hitchhikr of Neural^Rebels\n");

	if(n < 2)
    {
		PrintHelp();
		return 0;
	}

	GetModuleFileName(NULL, tempName, MAX_PATH);
	GetModuleFileName(NULL, tempName2, MAX_PATH);
#ifndef DEPACKER_MEMORY
	GetModuleFileName(NULL, depackerName, MAX_PATH);
#endif
	PathRemoveFileSpec(tempName);
	PathRemoveFileSpec(tempName2);
#ifndef DEPACKER_MEMORY
	PathRemoveFileSpec(depackerName);
#endif
	strcat(tempName, "\\pack.tmp");
	strcat(tempName2, "\\pack2.tmp");
#ifndef DEPACKER_MEMORY
	strcat(depackerName, "\\depacker.bin");
#endif

	UStringVector commandStrings;
	WriteArgumentsToStringList(n, args, commandStrings);
	CParser parser(0);
	try
    {
		parser.ParseStrings(NULL, commandStrings);
	}
	catch(...)
    {
		IncorrectCommand();
	}

	const UStringVector &nonSwitchStrings = parser.NonSwitchStrings;

	int paramIndex = 0;
	if(paramIndex >= nonSwitchStrings.Size())
    {
        IncorrectCommand();
    }
	CMyComPtr<ISequentialInStream> inStream;
	CInFileStream *inStreamSpec = 0;
	if(paramIndex >= nonSwitchStrings.Size())
    {
        IncorrectCommand();
	}
    const UString &inputName = nonSwitchStrings[paramIndex++]; 
	
	// Parse ELF file
	FILE *ELF_File;
	BYTE *RAW_Mem;
	ELF_File = fopen(GetOemString(inputName), "rb");
    if(ELF_File)
    {
		int ELF_File_Size;
		fseek(ELF_File, 0, SEEK_END);
		ELF_File_Size = ftell(ELF_File);
		fseek(ELF_File, 0, SEEK_SET);
		BYTE *ELF_Mem = (BYTE *) malloc(ELF_File_Size);
		if(ELF_Mem)
        {
			fread(ELF_Mem, 1, ELF_File_Size, ELF_File);
			fclose(ELF_File);

			// *** Parse it here ***
			if(!Check_ELF(ELF_Mem))
            {
		        printf("\nInput is not a valid PS2 ELF file\n");
				return 1;
            }
		    RAW_Mem = Parse_ELF(ELF_Mem, ELF_File_Size);
		    if(RAW_Mem == NULL)
            {
			   printf("\nCorrupted ELF file\n");
			   return 1;
		    }
		    ELF_File = fopen(tempName, "wb");			
		    if(ELF_File)
            {
			   // Save the raw datas
			   fwrite(RAW_Mem, 1, Get_ELF_Total_Size(), ELF_File);
			   fclose(ELF_File);
			   free(RAW_Mem);
			   free(ELF_Mem);
		    }
            else
            {
                free(RAW_Mem);
			    free(ELF_Mem);
			    printf("\nCan't open output file %s\n", (const char *) tempName);
			    return 1;
		    }
		}
        else
        {
		    fclose(ELF_File);
			printf("\nNot enough memory\n");
			return 1;
		}
	}
    else
    {
		printf("\nCan't open input file %s\n", (const char *) GetOemString(inputName));
		return 1;
	}

	inStreamSpec = new CInFileStream;
	inStream = inStreamSpec;
	if(!inStreamSpec->Open(tempName))
    {
		printf("\nCan't open input file %s\n", (const char *) tempName);
		return 1;
    }

	CMyComPtr<ISequentialOutStream> outStream;
	UString outputName;
	if(paramIndex >= nonSwitchStrings.Size())
    {
	    outputName = inputName;
	}
    else
    {
		outputName = nonSwitchStrings[paramIndex++]; 
	}
	COutFileStream *outStreamSpec = new COutFileStream;
	outStream = outStreamSpec;
	if(!outStreamSpec->Create(GetSystemString(tempName2), true))
    {
		printf("\nCan't open output file %s\n", (const char *)GetOemString(outputName));
		return 1;
    }

	UInt64 fileSize;

	NCompress::NLZMA::CEncoder *encoderSpec = 
	new NCompress::NLZMA::CEncoder;
	CMyComPtr<ICompressCoder> encoder = encoderSpec;

	UInt32 dictionary = 1 << 23;

	UInt32 posStateBits = 2;
	UInt32 litContextBits = 3;
	UInt32 litPosBits = 0;
	UInt32 algorithm = 2;
	UInt32 numFastBytes = 255;

	PROPID propIDs[] =
    {
		NCoderPropID::kDictionarySize,
		NCoderPropID::kPosStateBits,
		NCoderPropID::kLitContextBits,
		NCoderPropID::kLitPosBits,
		NCoderPropID::kAlgorithm,
		NCoderPropID::kNumFastBytes
    };
	const int kNumProps = sizeof(propIDs) / sizeof(propIDs[0]);

	PROPVARIANT properties[kNumProps];
    for(int p = 0; p < 6; p++)
    {
	    properties[p].vt = VT_UI4;
    }
    properties[0].ulVal = UInt32(dictionary);
    properties[1].ulVal = UInt32(posStateBits);
    properties[2].ulVal = UInt32(litContextBits);
    properties[3].ulVal = UInt32(litPosBits);
    properties[4].ulVal = UInt32(algorithm);
    properties[5].ulVal = UInt32(numFastBytes);
    
	if(encoderSpec->SetCoderProperties(propIDs, properties, kNumProps) != S_OK)
    {
		IncorrectCommand();
    }

	inStreamSpec->File.GetLength(fileSize);

	// Write file size
	for(i = 0; i < 4; i++)
    {
		Byte b = Byte(fileSize >> (8 * i));
		if(outStream->Write(&b, sizeof(b), 0) != S_OK)
        {
			printf("Write error");
			return 1;
		}
    }

	int Input_Size = Get_File_Size(GetSystemString(inputName));

	printf("\nInput: '%s'\n", GetSystemString(inputName));
	printf("Output: '%s'\n", GetSystemString(outputName));

	printf("\nStarting address: 0x%x\n", Get_ELF_Base_Address());
	printf("Entry point: 0x%x\n\n", Get_ELF_Entry_Point());

	printf("Packing file... ");

	HRESULT result = encoder->Code(inStream, outStream, 0, 0, 0);
	if(result == E_OUTOFMEMORY)
    {
		printf("can't allocate memory\n");
		return 1;
    }

	// Close the files
	inStream = NULL;
	outStream = NULL;

	DeleteFile(tempName);

	// Correct the header of the file
	FILE *Post_File;
	elf_header_t Post_Header;
	elf_pheader_t Post_Section_Header;
	BYTE Post_Ident[] =
    {
		0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	char *Pack_ID = "NRLPack v"VERSION"."REVISION;
	BYTE Pad_Byte = 0;
	int Pad_Length;
	int Depacker_File_Size;
	int Depacker_Code_Size;
	int Total_Packed_Datas_Size;
	BYTE *Depacker_Mem;
	DWORD *dwDepacker_Mem;

	// Depacker file structure:
	//
	// - Relative offset to Source address.l
	// - Relative offset to dest address.l
	// - Relative offset to temporary buffer.l
	// - Relative offset to jump address.l
	// - Code.b

#ifndef DEPACKER_MEMORY
	FILE *Depacker_File;

	Depacker_FileDepacker_File = fopen(depackerName, "rb")	
	if(Depacker_FileDepacker_File)
    {
		fseek(Depacker_File, 0, SEEK_END);
		Depacker_File_Size = ftell(Depacker_File);
		fseek(Depacker_File, 0, SEEK_SET);
		Depacker_Code_Size = Depacker_File_Size - 16;
		Depacker_Mem = (BYTE *) malloc(Depacker_File_Size);
		dwDepacker_Mem = (DWORD *) Depacker_Mem;
		if(Depacker_Mem)
        {
			fread(Depacker_Mem, 1, Depacker_File_Size, Depacker_File);
			fclose(Depacker_File);
		}
        else
        {
			fclose(Depacker_File);
			printf("not enough memory\n");
			return 1;	
		}
	}
    else
    {
		printf("can't open depacker.bin file\n");
		return 1;	
	}
#else
	Depacker_File_Size = depacker_size;
	Depacker_Code_Size = Depacker_File_Size - 16;
	Depacker_Mem = depacker_Dats;
	dwDepacker_Mem = (DWORD *) Depacker_Mem;
#endif

	Post_File = fopen(GetOemString(tempName2), "rb");	
	if(Post_File)
    {
		int Post_File_Size;
		fseek(Post_File, 0, SEEK_END);
		Post_File_Size = ftell(Post_File);
		fseek(Post_File, 0, SEEK_SET);

 		Pad_Length = (((Post_File_Size - 1) & 0xfffffffc) + 4) - (Post_File_Size - 1);
		Total_Packed_Datas_Size = Pad_Length + (Post_File_Size - 1);
		if(Pad_Length < 0) Pad_Length = 0;
		BYTE *Post_Mem = (BYTE *) malloc(Post_File_Size);
		if(Post_Mem)
        {
		   fread(Post_Mem, 1, Post_File_Size, Post_File);
            fclose(Post_File);
			DeleteFile(tempName2);

			Post_File = fopen(GetOemString(outputName), "wb");
			if(Post_File)
            {
				memcpy(Post_Header.ident, Post_Ident, sizeof(Post_Ident));

				Post_Section_Header.type = PT_LOAD;
				Post_Section_Header.offset = 0x60;

				// *** Complete size - header (0x60) ***
				Post_Section_Header.filesz = Depacker_Code_Size + Total_Packed_Datas_Size;
				// *** Complete size in memory - header (0x60) rounded to alignment ***
				Post_Section_Header.memsz = ((Post_Section_Header.filesz & 0xfffffff0) + 16);
				Post_Section_Header.flags = 7;
				Post_Section_Header.align = 16;

				// Decrease position from the upper of the 32 megs of memory
                Post_Section_Header.vaddr = 
				Post_Section_Header.paddr = (0x2000000 - (Post_Section_Header.memsz + (26422 * 2))) & 0xfffffff0;

				Post_Header.type = 2;
				Post_Header.machine = 8;
				Post_Header.version = 1;
				// *** Entry point ***
				// (-16 to pass over the 4 pointer of the depacker).
				Post_Header.entry = Post_Section_Header.paddr + Total_Packed_Datas_Size + dwDepacker_Mem[0] - 16;
				Post_Header.phoff = 0x34;
				Post_Header.shoff = 0;
				Post_Header.flags = 0;
				Post_Header.ehsize = 0x34;
				Post_Header.phentsize = 0x20;
				Post_Header.phnum = 1;
				Post_Header.shentsize = 0;
				Post_Header.shnum = 0;
				Post_Header.shstrndx = 0;

				fwrite(&Post_Header, 1, sizeof(elf_header_t), Post_File);
				fwrite(&Post_Section_Header, 1, sizeof(elf_pheader_t), Post_File);
				// Signature
                fwrite(Pack_ID, 1, 12, Post_File);

                // Unpacked size
				fwrite(Post_Mem, 1, 4, Post_File);
				// Code (inverted)
				fwrite(Post_Mem + 8, 1, 1, Post_File);
				fwrite(Post_Mem + 7, 1, 1, Post_File);
				fwrite(Post_Mem + 6, 1, 1, Post_File);
				fwrite(Post_Mem + 5, 1, 1, Post_File);
				// Compressed datas
				fwrite(Post_Mem + 9, 1, Post_File_Size - 9, Post_File);

				for(i = 0; i < Pad_Length; i++)
                {
					fwrite(&Pad_Byte, 1, 1, Post_File);
				}

				// Fix depacker addresses
				
				// Source
				u32 SourceAddress = Post_Section_Header.paddr + 8;
				Depacker_Mem[dwDepacker_Mem[0] + 4] = (BYTE) (SourceAddress & 0xff);
				Depacker_Mem[dwDepacker_Mem[0] + 5] = (BYTE) ((SourceAddress & 0xff00) >> 8);
				Depacker_Mem[dwDepacker_Mem[0]] = (BYTE) ((SourceAddress & 0xff0000) >> 16);
				Depacker_Mem[dwDepacker_Mem[0] + 1] = (BYTE) ((SourceAddress & 0xff000000) >> 24);

				// Dest
				u32 DestAddress = Get_ELF_Base_Address();
				Depacker_Mem[dwDepacker_Mem[1] + 4] = (BYTE) (DestAddress & 0xff);
				Depacker_Mem[dwDepacker_Mem[1] + 5] = (BYTE) ((DestAddress & 0xff00) >> 8);
				Depacker_Mem[dwDepacker_Mem[1]] = (BYTE) ((DestAddress & 0xff0000) >> 16);
				Depacker_Mem[dwDepacker_Mem[1] + 1] = (BYTE) ((DestAddress & 0xff000000) >> 24);

			    u32 TmpAddress = Post_Section_Header.memsz + Post_Section_Header.paddr;
			    Depacker_Mem[dwDepacker_Mem[2] + 4] = (BYTE) (TmpAddress & 0xff);
			    Depacker_Mem[dwDepacker_Mem[2] + 5] = (BYTE) ((TmpAddress & 0xff00) >> 8);
			    Depacker_Mem[dwDepacker_Mem[2]] = (BYTE) ((TmpAddress & 0xff0000) >> 16);
			    Depacker_Mem[dwDepacker_Mem[2] + 1] = (BYTE) ((TmpAddress & 0xff000000) >> 24);

				// JUmp address
				u32 JumpInstruction = (Get_ELF_Entry_Point() >> 2) | 0x8000000;
				Depacker_Mem[dwDepacker_Mem[3]] = (BYTE) (JumpInstruction & 0xff);
				Depacker_Mem[dwDepacker_Mem[3] + 1] = (BYTE) ((JumpInstruction & 0xff00) >> 8);
				Depacker_Mem[dwDepacker_Mem[3] + 2] = (BYTE) ((JumpInstruction & 0xff0000) >> 16);
				Depacker_Mem[dwDepacker_Mem[3] + 3] = (BYTE) ((JumpInstruction & 0xff000000) >> 24);

				// Write depacker
				fwrite(Depacker_Mem + 16, 1, Depacker_Code_Size, Post_File);

				fclose(Post_File);
				free(Post_Mem);
			}
            else
            {
				free(Post_Mem);
				printf("can't open output file %s\n", (const char *) GetOemString(outputName));
				return 1;
			}
		}
        else
        {
			fclose(Post_File);
			DeleteFile(tempName2);
			printf("not enough memory\n");
			return 1;
		}
	}
    else
    {
	    printf("can't open output file %s\n", (const char *) GetOemString(outputName));
		return 1;
    }

	printf("done.\n");

	int Output_Size = Get_File_Size(GetSystemString(outputName));
	printf("\n");
	printf("     Input     Output       Gain     %%\n");
	printf("--------------------------------------\n");
	printf("%10d %10d", Input_Size, Output_Size);
	printf(" %10d %.02f\n", Input_Size - Output_Size, (((float) (Input_Size - Output_Size)) * 100.0f) / (float) Input_Size);
	
	return 0;
}

int main(int n, const char *args[])
{
	try
    {
		return main2(n, args);
	}
	catch(const char *s)
    {
		printf("\nError: %s\n", s);
		return 1; 
	}
	catch(...)
    {
		printf("\nException error !\n");
		return 1; 
	}
}
