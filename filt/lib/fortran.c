#include <string.h>

#include "cfortran.h"

#include "getpar.h"
#include "alloc.h"
#include "error.h"
#include "c99.h"
#include "file.h"
#include "files.h"

/* Fortran functions for command-line arguments - NON-STANDARD
   use #ifdefs if necessary */
PROTOCCALLSFFUN0(INT,IARGC,iargc)
PROTOCCALLSFSUB2(GETARG,getarg,INT,PSTRING)

#define IARGC() CCALLSFFUN0(IARGC,iargc)
#define GETARG(I,N) CCALLSFSUB2(GETARG,getarg,INT,PSTRING,I,N)

void sf_init_f(void)
{
    int i, len, argc1;
    char** argv1, *argvi, arg[256];

    argc1 = 1+IARGC();    
    argv1 = (char**) sf_alloc(argc1+1,sizeof(char*));

    for (i=0; i < argc1; i++) {
	GETARG(i,arg);

	len = strlen(arg)+1;
	argvi = argv1[i] = sf_charalloc(len);
	memcpy(argvi,arg,len);
    }    
    argv1[argc1] = NULL;

    sf_init(argc1,argv1);
}

bool sf_getbool_f (const char* key,/*@out@*/ int* ipar)
{
    bool par;
    
    if (!sf_getbool(key,&par)) return false;
    *ipar = par;
    return true;
}

bool sf_getbools_f (const char* key,/*@out@*/ int* ipar, size_t n)
{
    int i;
    bool *par;

    par = sf_boolalloc(n);
    
    if (!sf_getbools(key,par,n)) {
	free (par);
	return false;
    } else {
	for (i=0; i < n; i++) {
	    ipar[i] = par[i];
	}
	free(par);
	return true;
    }
}

#ifdef f2cFortran /* f2c adds an extra underscore */
#undef fcallsc
#define fcallsc(UN,LN) append_fcallsc(_,_,UN,LN)
#endif 

FCALLSCSUB0(sf_init_f,SF_INIT,sf_init)
FCALLSCSUB0(sf_close,SF_CLOSE,sf_close)
FCALLSCFUN0(STRING,sf_getprog,SF_GETPROG,sf_getprog)
FCALLSCFUN2(LOGICAL,sf_getint,SF_GETINT,sf_getint,STRING,PINT)
FCALLSCFUN3(LOGICAL,sf_getints,SF_GETINTS,sf_getints,STRING,INTV,INT)
FCALLSCFUN2(LOGICAL,sf_getfloat,SF_GETFLOAT,sf_getfloat,STRING,PFLOAT)
FCALLSCFUN3(LOGICAL,sf_getfloats,SF_GETFLOATS,sf_getfloats,STRING,FLOATV,INT)
FCALLSCFUN1(STRING,sf_getstring,SF_GETSTRING,sf_getstring,STRING)

#define sf_getstrings_STRV_A2 NUM_ELEM_ARG(3)
FCALLSCFUN3(LOGICAL,sf_getstrings,SF_GETSTRINGS,sf_getstrings,STRING,PSTRINGV,INT)

FCALLSCFUN2(LOGICAL,sf_getbool_f,SF_GETBOOL,sf_getbool,STRING,PLOGICAL)
FCALLSCFUN3(LOGICAL,sf_getbools_f,SF_GETBOOLS,sf_getbools,STRING,LOGICALV,INT)

FCALLSCFUN1(INT,sf_input,SF_INPUT,sf_input,STRING)
FCALLSCFUN1(INT,sf_output,SF_INPUT,sf_output,STRING)
FCALLSCFUN1(INT,sf_gettype,SF_GETTYPE,sf_gettype,INT)
FCALLSCFUN1(INT,sf_getform,SF_GETFORM,sf_getform,INT)
FCALLSCSUB2(sf_settype,SF_SETTYPE,sf_settype,INT,INT)
FCALLSCSUB2(sf_setformat,SF_SETFORMAT,sf_setformat,INT,STRING)
FCALLSCSUB1(sf_fileclose,SF_FILECLOSE,sf_fileclose,INT)
FCALLSCSUB2(sf_fileflush,SF_FILEFLUSH,sf_fileflush,INT,INT)

FCALLSCFUN3(LOGICAL,sf_histint,SF_HISTINT,sf_histint,INT,STRING,PINT)
FCALLSCFUN4(LOGICAL,sf_histints,SF_HISTINTS,sf_histints,INT,STRING,INTV,INT)
FCALLSCFUN3(LOGICAL,sf_histfloat,SF_HISTFLOAT,sf_histfloat,INT,STRING,PFLOAT)
FCALLSCFUN2(STRING,sf_histstring,SF_HISTSTRING,sf_histstring,INT,STRING)

FCALLSCSUB3(sf_putint,SF_PUTINT,sf_putint,INT,STRING,INT)
FCALLSCSUB4(sf_putints,SF_PUTINTS,sf_putints,INT,STRING,INTV,INT)
FCALLSCSUB3(sf_putfloat,SF_PUTFLOAT,sf_putfloat,INT,STRING,FLOAT)
FCALLSCSUB3(sf_putstring,SF_PUTSTRING,sf_putstring,INT,STRING,STRING)
FCALLSCSUB2(sf_putline,SF_PUTLINE,sf_putline,INT,STRING)

FCALLSCFUN1(LONG,sf_bytes,SF_BYTES,sf_bytes,INT)
FCALLSCFUN1(LONG,sf_tell,SF_TELL,sf_tell,INT)
FCALLSCSUB3(sf_seek,SF_SEEK,sf_seek,INT,LONG,INT)
FCALLSCSUB2(sf_unpipe,SF_UNPIPE,sf_unpipe,INT,LONG)

FCALLSCSUB4(sf_write,SF_WRITE,sf_write,PVOID,INT,INT,INT)
FCALLSCSUB4(sf_read,SF_READ,sf_read,PVOID,INT,INT,INT)

FCALLSCFUN2(INT,sf_filedims,SF_FILEDIMS,sf_filedims,INT,INTV)
FCALLSCFUN1(INT,sf_filesize,SF_FILESIZE,sf_filesize,INT)
FCALLSCFUN2(INT,sf_leftsize,SF_LEFTSIZE,sf_leftsize,INT,INT)

FCALLSCSUB1(sf_error,SF_ERROR,sf_error,STRING)
FCALLSCSUB1(sf_warning,SF_WARNING,sf_warning,STRING)

