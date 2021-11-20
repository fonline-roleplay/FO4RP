//////////////////////////////////////////////////////////////////////
// CDATFile Class
//////////////////////////////////////////////////////////////////////

//#include "macros.h"
#include "datio.h"
#include <memory.h>
#include <stdio.h>

extern FILE *log;


CDATFile::CDATFile(char *dat_filename)
{
	m_bError=true;
	m_pDirectoryBuffer=NULL;
	m_pInBuf=NULL;
	unsigned long i;

	memset(&m_DATName, 0, sizeof(m_DATName));
	strcpy(m_DATName, dat_filename);
	log=fopen("mapper.log", "a+b");
	fprintf(log, "\n-- CDATFile \"%s\" initialization begin --\n", m_DATName);
	fclose(log);
	
	// Open DAT file
	m_hDat=CreateFile(dat_filename,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
    if (m_hDat==INVALID_HANDLE_VALUE) {
		log=fopen("mapper.log", "a+b");
		fprintf (log, "  # ERROR CDATFile \"%s\": open DAT failed\n", m_DATName);
		fclose(log);
		return;
	}

	// Seek to TreeSize and DataSize
	i=SetFilePointer (m_hDat,-8,NULL,FILE_END);
	if (i==0xFFFFFFFF) {
		log=fopen("mapper.log", "a+b");
		fprintf (log, "  # ERROR CDATFile \"%s\": SetFilePointer (m_hDat,-8,NULL,FILE_END) failed\n", m_DATName);
		fclose(log);
		return;
	}

	// Read Treesize
	if(::ReadFile(m_hDat,&TreeSize,4,&i,NULL)==0) {
		log=fopen("mapper.log", "a+b");
		fprintf (log, "  # ERROR CDATFile \"%s\": ::ReadFile(m_hDat,&TreeSize,4,&i,NULL) failed\n", m_DATName);
		fclose(log);
		return;
	}

	// Read Datasize
	if(::ReadFile(m_hDat,&DataSize,4,&i,NULL)==0) {
		log=fopen("mapper.log", "a+b");
		fprintf (log, "  # ERROR CDATFile \"%s\": ::ReadFile(m_hDat,&TreeSize,4,&i,NULL) failed\n", m_DATName);
		fclose(log);
		return;
	}

	// Validate DAT file length
	if(GetFileSize(m_hDat,NULL)!=DataSize) {
		log=fopen("mapper.log", "a+b");
		fprintf (log, "  # ERROR CDATFile \"%s\": GetFileSize(m_hDat,NULL) does not match DataSize\n", m_DATName);
		fclose(log);
		return;
	}

	// Seek to FilesTotal then set TreeSize to real value (that is, -=4)
	i=SetFilePointer (m_hDat,-(TreeSize+8),NULL,FILE_END);
	if (i==0xFFFFFFFF) {
		log=fopen("mapper.log", "a+b");
		fprintf (log, "  # ERROR CDATFile \"%s\": SetFilePointer (m_hDat,-(TreeSize+8),NULL,FILE_END) failed\n", m_DATName);
		fclose(log);
		return;
	}
	
	TreeSize-=4;
			
	// Read FilesTotal
	if(::ReadFile(m_hDat,&FilesTotal,4,&i,NULL)==0) {
		log=fopen("mapper.log", "a+b");
		fprintf (log, "  # ERROR CDATFile \"%s\": ::ReadFile(m_hDat,&FilesTotal,4,&i,NULL) failed\n", m_DATName);
		fclose(log);
		return;
	}

	// Allocate memory for DAT directory
	m_pDirectoryBuffer=new BYTE [TreeSize];
	if(!m_pDirectoryBuffer) {
		log=fopen("mapper.log", "a+b");
		fprintf (log, "  # ERROR CDATFile \"%s\": cannot allocate 0x%x bytes for DAT directory\n", m_DATName, TreeSize);
		fclose(log);
		return;
	}

	// Read DAT directory into buffer
	if(::ReadFile(m_hDat,m_pDirectoryBuffer,TreeSize,&i,NULL)==0) {
		delete[] m_pDirectoryBuffer;
		m_pDirectoryBuffer=NULL;
		log=fopen("mapper.log", "a+b");
		fprintf (log, "  # ERROR CDATFile \"%s\": ReadFile(m_hDat,m_pDirectoryBuffer,TreeSize,&i,NULL) failed\n", m_DATName);
		fclose(log);
		return;
	}

	if (i!=TreeSize) {
		delete[] m_pDirectoryBuffer;
		m_pDirectoryBuffer=NULL;
		log=fopen("mapper.log", "a+b");
		fprintf (log, "  # ERROR CDATFile \"%s\": ReadFile(m_hDat,m_pDirectoryBuffer,TreeSize,&i,NULL) failed\n", m_DATName);
		fclose(log);
		return;
	}

	// Allocate internal buffer for ZLIB usage
	m_pInBuf = new BYTE [ZLIB_BUFF_SIZE];
	if (!m_pInBuf) {
		delete[] m_pDirectoryBuffer;
		m_pDirectoryBuffer=NULL;
		log=fopen("mapper.log", "a+b");
		fprintf (log, "  # ERROR CDATFile \"%s\": cannot allocate %i bytes for zlib buffer\n", m_DATName, ZLIB_BUFF_SIZE);
		fclose(log);
		return;
	}


m_bError=false;
log=fopen("mapper.log", "a+b");
	fprintf(log, "DataSize=%i\nTreeSize=%i\nFilesTotal=%i\nm_pDirectoryBuffer=0x%x\nm_pInBuf=0x%x\n", DataSize, TreeSize, FilesTotal, m_pDirectoryBuffer, m_pInBuf);
	fprintf(log, "-- CDATFile \"%s\" initialization end --\n\n", m_DATName);
fclose(log);
}

CDATFile::~CDATFile()
{
	log=fopen("mapper.log", "a+b");
	fprintf(log, "\n-- CDATFile \"%s\" destruction begin --\n", m_DATName);
	if (m_pDirectoryBuffer!=NULL) {
		fprintf(log, "m_pDirectoryBuffer deleted\n");
		delete [] m_pDirectoryBuffer;
	} else
		fprintf(log, "  # WARNING: m_pDirectoryBuffer already NULL\n");

	if (m_pInBuf!=NULL) {
		fprintf(log, "m_pInBuf deleted\n");
		delete [] m_pInBuf;
	} else 
		fprintf(log, "  # WARNING: m_pInBuf already NULL\n");

	if (m_hDat!=INVALID_HANDLE_VALUE) {
		::CloseHandle(m_hDat);
		fprintf(log, "DAT file handle closed\n");
	} else
		fprintf(log, "  # WARNING: m_hDat already INVALID_HANDLE_VALUE\n");

fprintf(log, "-- CDATFile \"%s\" destruction end --\n\n", m_DATName);
		fclose(log);
}

// In: filename and ptr to buffer. 
// Out: number of bytes actually read or 0.
UINT CDATFile::ReadFile(char *fname, BYTE *pBuffer)
{

	UINT i=0;
	DATFILEDESCRIPTOR zz;

	if(!FindByName(fname, &zz)) {
		log=fopen("mapper.log", "a+b");
		fprintf (log, "  # ERROR CDATFile \"%s\": ReadFile(): FindByName() failed for %s\n", m_DATName, fname);
		fclose(log);
		return 0;			// if we cannot find requested file 
	}

	i=SetFilePointer (m_hDat,zz.Offset,NULL,FILE_BEGIN);
	if (i==0xFFFFFFFF) {
		log=fopen("mapper.log", "a+b");
		fprintf (log, "  # ERROR CDATFile \"%s\": SetFilePointer (m_hDat, 0x%x, NULL,FILE_BEGIN) failed for %s\n", m_DATName, zz.Offset, fname);
		fclose(log);
		return 0;
	}

	// INFLATE
	z_stream stream;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.next_in = Z_NULL;
	stream.avail_in = 0;
	
	if (::inflateInit (&stream)!=Z_OK) {
		log=fopen("mapper.log", "a+b");
		fprintf (log, "  # ERROR CDATFile \"%s\": ReadFile(): ::inflateInit() failed for %s\n", m_DATName, fname);
		fclose(log);
		return 0;
	}

	stream.next_out = pBuffer;
	stream.avail_out = ZLIB_BUFF_SIZE;
	
	int res = Z_OK;

	while ( res == Z_OK ) {
		if (stream.avail_in == 0)
		{
			stream.next_in = m_pInBuf;
			if(::ReadFile(m_hDat,m_pInBuf,ZLIB_BUFF_SIZE,(unsigned long*)&stream.avail_in,NULL)==0) {
				log=fopen("mapper.log", "a+b");
				fprintf (log, "  # ERROR CDATFile \"%s\": ReadFile(): reading into zlib buffer failed for %s\n", m_DATName, fname);
				fclose(log);
				return 0;
			}
		}

		// INFLATING BUFFER
		res = ::inflate (&stream, Z_NO_FLUSH);

		if (stream.avail_out == 0 || res == Z_STREAM_END) {
			stream.avail_out = ZLIB_BUFF_SIZE;
			stream.next_out = pBuffer+ZLIB_BUFF_SIZE;
		}
	}

	::inflateEnd (&stream);
	return stream.total_out;
}


// Searches single descriptor pointed by pDescriptor for specified name
// if search fails, returns -1
UINT CDATFile::IsNameMatches(BYTE *pDescriptor, char *fname)
{
	
	// Get FilenameSize
	BYTE z=*pDescriptor;
	pDescriptor+=4;

	// Get filename into string
	memcpy(&current_name, pDescriptor, z);
	// Do it ASCIIZ string
	current_name[z]=0;

	// skip filename
	pDescriptor+=z;
	// Compare is name matches? 

	if(_memicmp(&current_name, fname, z)==0)
	{
		//skip compress flag, RealSize and PackedSize
		pDescriptor+=9;
		//returns in-DAT file offset
		return *((DWORD*)pDescriptor);
	}
return -1;

}


// In: in-dat filename
// out: size of requested path-file or 0.
UINT CDATFile::GetRealSize(char * fname)
{
	DATFILEDESCRIPTOR zz;
	if(!FindByName(fname, &zz)) {
		log=fopen("mapper.log", "a+b");
		fprintf (log, "  # ERROR CDATFile \"%s\": GetRealSize() failed for %s\n", m_DATName, fname);
		fclose(log);
		return 0;
	}
	return zz.RealSize;

}

// in: filename and ptr to user-provided structure
// out: filled-out struct and true or false.

bool CDATFile::FindByName(char *fname, DATFILEDESCRIPTOR *pFileDescriptor)
{
	UINT i;
	UINT offset=-1;
	BYTE z, *pDescriptor=m_pDirectoryBuffer;

	for(i=0;i<FilesTotal;i++)
	{
		// current descriptor comparision
		offset=IsNameMatches(pDescriptor, fname);

		// skip to after-name part
		z=*pDescriptor;
		pDescriptor+=z+4;

		if(offset!=-1) {
			pFileDescriptor->Type=*pDescriptor++;
			pFileDescriptor->RealSize=*((DWORD*)pDescriptor++);
			pFileDescriptor->PackedSize=*((DWORD*)pDescriptor);
			pFileDescriptor->Offset=offset;
			return true;
		}

		// Else go to next descriptor
		pDescriptor+=13;
	}
	log=fopen("mapper.log", "a+b");
fprintf (log, "  # ERROR CDATFile \"%s\": FindByName() failed for %s\n", m_DATName, fname);
		fclose(log);
return 0;	// нет такого пути-имени
}


