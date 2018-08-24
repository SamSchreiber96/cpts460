/*********  t.c file *********************/
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


main()
{
  char ans[64];
  while(1) {
    prints("What's your name?\n\r> ");
    gets(ans);
    prints( "\n\r" );
    if ( ans[0]==0 ){
      prints("return to assembly and hang\n\r");
      return;
    }
    
    prints("Welcome, ");
    prints(ans);
    prints("!\n\r");
  }
}
