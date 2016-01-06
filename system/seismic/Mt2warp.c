/* Time-squared warping. */
/*
  Copyright (C) 2004 University of Texas at Austin

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <rsf.h>
#include "stretch4.h"

int main(int argc, char* argv[])
{
    map4 mo;
    bool inv, adj;
    int n1, i2, n2, i3, n3;
    float o1, d1, o2, d2, eps, t;
    float *trace, *t2, *trace2;
    sf_file in, out;

    sf_init(argc,argv);
    in = sf_input("in");
    out = sf_output("out");

    if (!sf_getbool("inv",&inv)) inv=false;
    /* inversion flag */

    if (!sf_getbool("adj",&adj)) adj=false;
    /* adjoint flag */

    if ((inv && !adj) || (!inv && adj)) {
	if (!sf_histint(in,"n1",&n2)) sf_error("No n1= in input");
	if (!sf_histfloat(in,"d1",&d2)) d2=1.;
	if (!sf_histfloat(in,"o1",&o2)) o2=0.;

	if (!sf_histint(in,"n1_t2warp",&n1)) n1=n2;

	o1 = sqrtf(o2);
	d1 = o2+(n2-1)*d2;
	d1 = (sqrtf(d1)-o1)/(n1-1);
	
	sf_putint(out,"n1",n1);
	sf_putfloat(out,"d1",d1);
	sf_putfloat(out,"o1",o1);
    } else {
	if (!sf_histint(in,"n1",&n1)) sf_error("No n1= in input");
	if (!sf_getint("pad",&n2)) n2=n1; /* output time samples */

	if (!sf_histfloat(in,"d1",&d1)) d1=1.;
	if (!sf_histfloat(in,"o1",&o1)) o1=0.;

	o2 = o1*o1;
	d2 = o1+(n1-1)*d1;
	d2 = (d2*d2 - o2)/(n2-1);

	sf_putint(out,"n1",n2);
	sf_putfloat(out,"d1",d2);
	sf_putfloat(out,"o1",o2);

	sf_putint(out,"n1_t2warp",n1);
    } 

    n3 = sf_leftsize(in,1);

    if (!sf_getfloat("eps",&eps)) eps=0.01;
    /* stretch regularization */

    trace = sf_floatalloc(n2);
    t2 = sf_floatalloc(n2);
    trace2 = sf_floatalloc(n1);

    mo = stretch4_init (n1, o1, d1, n2, eps);

    for (i2=0; i2 < n2; i2++) {
	t = o2+i2*d2;
	t2[i2] = sqrtf(t);
    }    

    stretch4_define (mo,t2);
    
    for (i3=0; i3 < n3; i3++) {
	if (inv) {
	    if (adj) {
		sf_floatread(trace2,n1,in);
		stretch4_apply_adj (mo,trace,trace2);
		sf_floatwrite (trace,n2,out); 
	    } else {
		sf_floatread(trace,n2,in);
		stretch4_apply (mo,trace,trace2);
		sf_floatwrite (trace2,n1,out);
	    } 
	} else {
	    if (adj) {
		sf_floatread(trace,n2,in);
		stretch4_invert_adj (mo,trace,trace2);
		sf_floatwrite (trace2,n1,out);
	    } else {
		sf_floatread(trace2,n1,in);
		stretch4_invert (mo,trace,trace2);
		sf_floatwrite (trace,n2,out);
	    }
	}
    }

    exit(0);
}
