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

main()
{ 
  u16    i, iblk, totalLen = 0;
  char   c, temp[64];
  prints("read block# 2 (GD)... Press a key to DO IT!!!\n\r");
  getc();
  prints("DO IT!!!\n\r");
  getblk(2, buf1);
  gp = ( GD * )buf1;
// 1. WRITE YOUR CODE to get iblk = bg_inode_table block number
  iblk = gp->bg_inode_table;
  prints("inode_block=");
  putc(iblk+'0'); prints("\n\r"); 

// 2. WRITE YOUR CODE to get root inode
  getblk( iblk, buf1 );
  ip = ( INODE * ) buf1 + 1; // 2nd INODE
  prints("read inodes begin block to get root inode\n\r");

// 3. WRITE YOUR CODE to step through the data block of root inode
   prints("read data block of root DIR\n\r");
   for ( i = 0; i < 12; ++i ) {
     u16 curBlk = ( u16 )ip->i_block[i];
     getblk( curBlk, buf2 );

     // 4. print file names in the root directory /
     while ( totalLen < BLK ) {
       dp = ( DIR * )(  ( (char * ) buf2 ) + totalLen );
       prints("DIR: ");
       strncpy( temp, dp->name, dp->name_len );
       temp[ dp->name_len ] = 0;
       prints( temp );
       
       // Check if boot dir...
       if ( strcmp( temp, "boot" ) == 0 ) {
	 prints( "     BOOT FOUND!" );
       }
       prints( "\n\r" );
       totalLen += dp->rec_len;       
     }
   }
   prints("Done\n\r");
}  

int prints(char *s)
{
  while ( *s != 0 ) {
    putc( *s++ );
  }
}

int gets(char *s) 
{
  while ( ( *s = getc() ) != '\r' ) {
    putc( *s++ );
  }
  *s = 0;
}

int getblk(u16 blk, char *buf)
{
  readfd( ( 2*blk )/CYL, ( ( 2*blk )%CYL )/TRK, ( ( 2*blk )%CYL )%TRK, buf);
}
