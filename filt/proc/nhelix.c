/* Non-stationary helical filter */

#include <rsf.h>

#include "nhelix.h"

#include "helix.h"
/*^*/

#ifndef _nhelix_h

typedef struct nhelixfilter {
    int np;    
    filter* hlx;
    bool* mis;
    int *pch;
} *nfilter;
/*^*/

#endif

nfilter nallocate(int np   /* number of patches */, 
		  int nd   /* data size */, 
		  int *nh  /* filter size [np] */, 
		  int *pch /* patching [nd] */) 
/*< allocate >*/
{
    nfilter aa;
    int ip, id;
    
    aa = (nfilter) sf_alloc(1,sizeof(*aa));

    aa->np = np;
    aa->hlx = (filter*) sf_alloc(np,sizeof(filter));

    for (ip=0; ip < np; ip++) {
	aa->hlx[ip] = allocatehelix(nh[ip]);
    }
    
    aa->pch = sf_intalloc(nd);
    for (id=0; id < nd; id++) {
	aa->pch[id] = pch[id];
    }

    aa->mis = NULL;
    return aa;
}

void ndeallocate(nfilter aa)
/*< free >*/ 
{
    int ip;
    
    for (ip=0; ip < aa->np; ip++) {
	deallocatehelix(aa->hlx[ip]);
    }
    free (aa->hlx);
    free (aa->pch);
    if (NULL != aa->mis) free (aa->mis);
    free (aa);
}

