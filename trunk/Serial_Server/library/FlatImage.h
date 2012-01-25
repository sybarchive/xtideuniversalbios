//======================================================================
//
// Project:     XTIDE Universal BIOS, Serial Port Server
//
// File:        FlatImage.h - Header file for basic flat disk image support
//

#include "library.h"

class FlatImage : public Image
{
private:
	class FileAccess fp;

public:
	FlatImage( char *name, int p_readOnly, int p_drive, int p_create, unsigned long p_cyl, unsigned long p_head, unsigned long p_sect, int p_useCHS )   :   Image( name, p_readOnly, p_drive, p_create, p_cyl, p_head, p_sect, p_useCHS )
	{
		long filesize;

		if( p_create )
		{
			char buff[512];
			unsigned long size;
			double sizef;
			FileAccess cf;

			size = (unsigned long) p_cyl * (unsigned long) p_sect * (unsigned long) p_head;
			if( size > cf.MaxSectors )
				log( -1, "'%s', can't create flat file with size greater than %lu 512-byte sectors", name, cf.MaxSectors );
			sizef = size / 2048.0;   // 512 byte sectors -> MB

			cf.Create( name );

			memset( &buff[0], 0, 512 );
			while( size-- )
				cf.Write( &buff[0], 512 );
		
			if( p_cyl > 1024 )
				log( 0, "Created file '%s', size %.1lf MB", name, sizef );
			else
				log( 0, "Created file '%s', geometry %u:%u:%u, size %.1lf MB", name, p_cyl, p_sect, p_head, sizef );

			cf.Close();
		}

		fp.Open( name );

		totallba = fp.SizeSectors();

		init( name, p_readOnly, p_drive, p_cyl, p_head, p_sect, p_useCHS );
	}

	FlatImage::~FlatImage()
	{
		fp.Close();
	}

	void seekSector( unsigned long lba )
	{
		fp.SeekSectors( lba );
	}

	void writeSector( void *buff )
	{
		fp.Write( buff, 512 );
	}

	void readSector( void *buff )
	{
		fp.Read( buff, 512 );
	}
};

