/*******************************************************
 *                      t.c file                        *
 *******************************************************/
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define TRK 18
#define CYL 36
#define BLK 1024

#include "ext2.h"
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;
GD    *gp;
INODE *ip;
DIR   *dp;

char buf1[BLK], buf2[BLK];
int color = 0x0A;
u8 ino;


int prints( char *s )
{
  while ( *s != 0 ) {
    putc( *s++ );
  }
}

int gets( char *s ) 
{
  while ( ( *s = getc() ) != '\r' ) {
    putc( *s++ );
  }
  *s = 0;
}

int getblk( u16 blk, char *buf )
{
  readfd( ( 2*blk )/CYL, ( ( 2*blk )%CYL )/TRK, ( ( 2*blk )%CYL )%TRK, buf);
}

DIR * getDIR( INODE * root, char * fname ) {
  int i = 0, totalLen = 0;
  char temp[64];

  for ( i = 0; i < 12; ++i ) {
    u16 curBlk = root->i_block[i];
    getblk( curBlk, buf2 );

    while ( totalLen < BLK ) {
      dp = ( DIR * )(  ( (char * ) buf2 ) + totalLen );
      strncpy( temp, dp->name, dp->name_len );
      temp[ dp->name_len ] = 0;
      if ( strcmp( temp, fname ) == 0 ) {
	return dp;
      }
      totalLen += dp->rec_len;       
    }
  }
  return 0;
}

INODE * getINODE( u16 ino, u16 iblk, char * buf ) {
  getblk( iblk + (u16)(ino-1)/(BLK/sizeof(INODE)), buf );
  return ( INODE * ) buf + (u16)(ino-1)%(BLK/sizeof(INODE));
}
main()
{ 
  u16    i, iblk, totalLen = 0;
  u32 * single_indir_blk_ptr;
  char   c, temp[64];
  DIR * curDir;
  
  getblk(2, buf1);
  gp = ( GD * )buf1;

  iblk = gp->bg_inode_table;

  // get inode table
  getblk( iblk, buf1 );
  
  curDir = getDIR( getINODE( 2, iblk, buf1), "boot" );  
  curDir = getDIR( getINODE( (u16)curDir->inode, (u16)iblk, buf1 ), "mtx" );
  ip = getINODE( (u16)curDir->inode, (u16)iblk, buf1 ); // disk img inode
  getblk( (u16)ip->i_block[12], buf2 );
  single_indir_blk_ptr = buf2;
  
  setes( 0x1000 );

  for (i = 0; i < 12; ++i) {
    getblk( (u16)ip->i_block[i], 0 );
    inces();
  }

  while ( *single_indir_blk_ptr != 0 ) {
    getblk( (u16)*single_indir_blk_ptr, 0 );
    single_indir_blk_ptr++;
    inces();
  }
}  
