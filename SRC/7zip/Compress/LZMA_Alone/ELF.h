/* The primary ELF header. */
typedef unsigned long int u32;
typedef unsigned short int u16;
typedef unsigned char u8;

typedef struct
{
    u8  ident[16];    /* The first 4 bytes are the ELF magic */
    u16 type;         /* == 2, EXEC (executable file) */
    u16 machine;      /* == 8, MIPS r3000 */
    u32 version;      /* == 1, default ELF value */
    u32 entry;        /* program starting point */
    u32 phoff;        /* program header offset in the file */
    u32 shoff;        /* section header offset in the file, unused for us, so == 0 */
    u32 flags;        /* flags, unused for us. */
    u16 ehsize;       /* this header size ( == 52 ) */
    u16 phentsize;    /* size of a program header ( == 32 ) */
    u16 phnum;        /* number of program headers */
    u16 shentsize;    /* size of a section header, unused here */
    u16 shnum;        /* number of section headers, unused here */
    u16 shstrndx;     /* section index of the string table */
} elf_header_t;

typedef struct
{
    u32 type;         /* == 1, PT_LOAD (that is, this section will get loaded */
    u32 offset;       /* offset in file, on a 4096 bytes boundary */
    u32 vaddr;        /* virtual address where this section is loaded */
    u32 paddr;        /* physical address where this section is loaded */
    u32 filesz;       /* size of that section in the file */
    u32 memsz;        /* size of that section in memory (rest is zero filled) */
    u32 flags;        /* PF_X | PF_W | PF_R, that is executable, writable, readable */
    u32 align;        /* == 0x1000 that is 4096 bytes */
} elf_pheader_t;

typedef struct
{
    u32 entryAddr;
    u32 numSections;
} packed_Header;

typedef struct
{
	u32 name;
	u32 type;
	u32 flags;
	u32 addr;
	u32 offset;
	u32 size;
	u32 link;
	u32 info;
	u32 addralign;
	u32 entsize;
} elf_section_header;

#define PT_LOAD 1
#define PF_X 1
#define PF_W 2
#define PF_R 4

#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_LOPROC 0x70000000
#define SHT_HIPROC 0x7fffffff
#define SHT_LOUSER 0x80000000
#define SHT_HIUSER 0xffffffff

#define SHF_WRITE 0x1
#define SHF_ALLOC 0x2
#define SHF_EXECINSTR 0x4
#define SHF_MASKPROC 0xf0000000

int Check_ELF(u8 *Buffer);
u8 *Parse_ELF(u8 *Buffer, int Size);
u32 Get_ELF_Base_Address(void);
u32 Get_ELF_Entry_Point(void);
u32 Get_ELF_Total_Size(void);
