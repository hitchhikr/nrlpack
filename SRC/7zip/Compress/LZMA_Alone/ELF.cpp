// --------------------------------------------
// ELF files parser
// --------------------------------------------

// --------------------------------------------
// Includes
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "ELF.h"

// --------------------------------------------
// Variables
u32 ELF_MAGIC = 0x464c457f;

elf_header_t *eh;
elf_pheader_t *eph;
elf_section_header *shead;
int Nbr_Elf_Sections;

u32 ELF_Base;	 /* Loading address */
u32 ELF_PC;     /* Entry point */
u32 ELF_Size;   /* Final size */

/* The default ELF ident field */
u8 ident[] =
{
    0x7F, 0x45, 0x4C, 0x46, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// --------------------------------------------
int Check_ELF(u8 *Buffer)
{
	eh = (elf_header_t *) Buffer;
	if(*(u32 *) &eh->ident != ELF_MAGIC)
    {
        return(0);
	}
    return(1);
}

// --------------------------------------------
u8 *Parse_ELF(u8 *Buffer, int Size)
{
	u8 *Dest_Buffer;
	elf_section_header *Tmpshead;
	int First_Section;
	int i;

	eh = (elf_header_t *) Buffer;

	// Save entry point
	ELF_PC = eh->entry;

	eph = (elf_pheader_t *) (Buffer + eh->phoff);

	// Save Loading base address
	ELF_Base = eph->vaddr;

	// Maximum original size of the file
	// We're going to copy everything in this block
	// at original offsets
	Dest_Buffer = (u8 *) malloc(Size);
    memset(Dest_Buffer, 0, Size);
	Nbr_Elf_Sections = eh->shnum;
	shead = (elf_section_header *) (Buffer + eh->shoff);
	Tmpshead = shead;
	First_Section = 0;
	ELF_Size = 0;

    if(Nbr_Elf_Sections)
    {
	   // Copy the relevants sections
	    for(i = 0; i < Nbr_Elf_Sections; i++)
        {
		    if(Tmpshead->size && (Tmpshead->type & SHT_PROGBITS) && (Tmpshead->flags & SHF_ALLOC))
            {
			    if(!First_Section)
                {
                    First_Section = Tmpshead->offset;
			    }
                memcpy(Dest_Buffer + Tmpshead->offset - First_Section, Buffer + Tmpshead->offset, Tmpshead->size);
			    ELF_Size = ((Dest_Buffer + Tmpshead->offset - First_Section) + Tmpshead->size) - Dest_Buffer;
		    }
		    Tmpshead++;
        }
    }
    else
    {
        // No sections infos, copy everything but the header
        if(eph->offset)
        {
            ELF_Size = eph->filesz;
            memcpy(Dest_Buffer, Buffer + eph->offset, ELF_Size);
        }
        else
        {
            // Mangled header
            ELF_Size = eph->filesz - (eh->phoff + eh->phentsize);        
            memcpy(Dest_Buffer, Buffer + eh->phoff + eh->phentsize, ELF_Size);
            ELF_Base = ELF_PC;
        }
    }
	return(Dest_Buffer);
}

// --------------------------------------------
u32 Get_ELF_Base_Address(void)
{
    return(ELF_Base);
}

// --------------------------------------------
u32 Get_ELF_Entry_Point(void)
{
	return(ELF_PC);
}

// --------------------------------------------
u32 Get_ELF_Total_Size(void)
{
	return(ELF_Size);
}
