
#include "defs.h"
#include "fdefs.h"
#include <malloc.h>
#include <stdlib.h>

void
openmanifest(job)
     char job[MAXCOMM] ;
{
  char command[MAXCOMM] ;
  int nread;
  int i;
  
  if (binaryopen) {
    printf("<sorry, binary file %s already open, %s>\n",
	   binaryfile.name,title) ;
    return;
  } 

  if (sscanf(job,"%s %s",command,binaryfile.name) != 2) {
    input_error(command) ;
    return;
  }

  if( (binaryfile.ptr = fopen(binaryfile.name,"r")) 
      == NULL) {
      printf("<sorry, binary file %s does not exist, %s>\n",
	     binaryfile.name,title) ;
      return;
  } 

  nread = fread(&manifest_length, sizeof(int), 1, binaryfile.ptr);
  if (nread != 1 || manifest_length <= 0) {
    printf("<sorry, failed to read manifest %s, %s>\n", 
	   binaryfile.name, title);
    return;
  }

  if (manifest != NULL) free(manifest);
  manifest = (struct manifest_entry *) 
    malloc(manifest_length * sizeof(struct manifest_entry));
  if (manifest == NULL) {
    printf("<sorry, no memory for manifest, %s>\n", title);
    return;
  }

  nread = fread((void *)manifest, sizeof(struct manifest_entry), 
		manifest_length, binaryfile.ptr);
  if (nread != manifest_length) {
    printf("<sorry, short read of manifest: %d vs. %d, %s>\n", 
	   nread, manifest_length, title);
    free(manifest);
    return;
  }

  for (i=0; i<manifest_length; i++) {
    printf("%s   time = %g\n", manifest[i].name, manifest[i].time);
  }
  printf("%d files total.\n", manifest_length);


  /* success */
  ismanifest = TRUE ;
  binaryopen = OPEN ;
  cool_loaded = NO ;
  visc_loaded = NO ;
  form_loaded = NO ;
  eps_loaded = NO ;
  epsgas_loaded = NO ;
  lum_loaded = NO ;
  array_size = 0;

}
