/*
Copyright (c) 2003-2010, Mark Borgerding

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the author nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "FFT.h"
/* The guts header contains all the multiplication and addition macros that are defined for
 fixed or floating point complex numbers.  It also delares the kf_ internal functions.
 */

static void kf_bfly2(
        kiss_fft_cpx * Fout,
        const size_t fstride,
        const kiss_fft_cfg st,
        int m
        )
{
    kiss_fft_cpx * Fout2;
    kiss_fft_cpx * tw1 = st->twiddles;
    kiss_fft_cpx t;
    Fout2 = Fout + m;
    do{
        C_FIXDIV(*Fout,2); C_FIXDIV(*Fout2,2);

        C_MUL (t,  *Fout2 , *tw1);
        tw1 += fstride;
        C_SUB( *Fout2 ,  *Fout , t );
        C_ADDTO( *Fout ,  t );
        ++Fout2;
        ++Fout;
    }while (--m);
}

static void kf_bfly4(
        kiss_fft_cpx * Fout,
        const size_t fstride,
        const kiss_fft_cfg st,
        const size_t m
        )
{
    kiss_fft_cpx *tw1,*tw2,*tw3;
    kiss_fft_cpx scratch[6];
    size_t k=m;
    const size_t m2=2*m;
    const size_t m3=3*m;


    tw3 = tw2 = tw1 = st->twiddles;

    do {
        C_FIXDIV(*Fout,4); C_FIXDIV(Fout[m],4); C_FIXDIV(Fout[m2],4); C_FIXDIV(Fout[m3],4);

        C_MUL(scratch[0],Fout[m] , *tw1 );
        C_MUL(scratch[1],Fout[m2] , *tw2 );
        C_MUL(scratch[2],Fout[m3] , *tw3 );

        C_SUB( scratch[5] , *Fout, scratch[1] );
        C_ADDTO(*Fout, scratch[1]);
        C_ADD( scratch[3] , scratch[0] , scratch[2] );
        C_SUB( scratch[4] , scratch[0] , scratch[2] );
        C_SUB( Fout[m2], *Fout, scratch[3] );
        tw1 += fstride;
        tw2 += fstride*2;
        tw3 += fstride*3;
        C_ADDTO( *Fout , scratch[3] );

        if(st->inverse) {
            Fout[m].r = scratch[5].r - scratch[4].i;
            Fout[m].i = scratch[5].i + scratch[4].r;
            Fout[m3].r = scratch[5].r + scratch[4].i;
            Fout[m3].i = scratch[5].i - scratch[4].r;
        }else{
            Fout[m].r = scratch[5].r + scratch[4].i;
            Fout[m].i = scratch[5].i - scratch[4].r;
            Fout[m3].r = scratch[5].r - scratch[4].i;
            Fout[m3].i = scratch[5].i + scratch[4].r;
        }
        ++Fout;
    }while(--k);
}

static void kf_bfly3(
         kiss_fft_cpx * Fout,
         const size_t fstride,
         const kiss_fft_cfg st,
         size_t m
         )
{
     size_t k=m;
     const size_t m2 = 2*m;
     kiss_fft_cpx *tw1,*tw2;
     kiss_fft_cpx scratch[5];
     kiss_fft_cpx epi3;
     epi3 = st->twiddles[fstride*m];

     tw1=tw2=st->twiddles;

     do{
         C_FIXDIV(*Fout,3); C_FIXDIV(Fout[m],3); C_FIXDIV(Fout[m2],3);

         C_MUL(scratch[1],Fout[m] , *tw1);
         C_MUL(scratch[2],Fout[m2] , *tw2);

         C_ADD(scratch[3],scratch[1],scratch[2]);
         C_SUB(scratch[0],scratch[1],scratch[2]);
         tw1 += fstride;
         tw2 += fstride*2;

         Fout[m].r = Fout->r - HALF_OF(scratch[3].r);
         Fout[m].i = Fout->i - HALF_OF(scratch[3].i);

         C_MULBYSCALAR( scratch[0] , epi3.i );

         C_ADDTO(*Fout,scratch[3]);

         Fout[m2].r = Fout[m].r + scratch[0].i;
         Fout[m2].i = Fout[m].i - scratch[0].r;

         Fout[m].r -= scratch[0].i;
         Fout[m].i += scratch[0].r;

         ++Fout;
     }while(--k);
}

static void kf_bfly5(
        kiss_fft_cpx * Fout,
        const size_t fstride,
        const kiss_fft_cfg st,
        int m
        )
{
    kiss_fft_cpx *Fout0,*Fout1,*Fout2,*Fout3,*Fout4;
    int u;
    kiss_fft_cpx scratch[13];
    kiss_fft_cpx * twiddles = st->twiddles;
    kiss_fft_cpx *tw;
    kiss_fft_cpx ya,yb;
    ya = twiddles[fstride*m];
    yb = twiddles[fstride*2*m];

    Fout0=Fout;
    Fout1=Fout0+m;
    Fout2=Fout0+2*m;
    Fout3=Fout0+3*m;
    Fout4=Fout0+4*m;

    tw=st->twiddles;
    for ( u=0; u<m; ++u ) {
        C_FIXDIV( *Fout0,5); C_FIXDIV( *Fout1,5); C_FIXDIV( *Fout2,5); C_FIXDIV( *Fout3,5); C_FIXDIV( *Fout4,5);
        scratch[0] = *Fout0;

        C_MUL(scratch[1] ,*Fout1, tw[u*fstride]);
        C_MUL(scratch[2] ,*Fout2, tw[2*u*fstride]);
        C_MUL(scratch[3] ,*Fout3, tw[3*u*fstride]);
        C_MUL(scratch[4] ,*Fout4, tw[4*u*fstride]);

        C_ADD( scratch[7],scratch[1],scratch[4]);
        C_SUB( scratch[10],scratch[1],scratch[4]);
        C_ADD( scratch[8],scratch[2],scratch[3]);
        C_SUB( scratch[9],scratch[2],scratch[3]);

        Fout0->r += scratch[7].r + scratch[8].r;
        Fout0->i += scratch[7].i + scratch[8].i;

        scratch[5].r = scratch[0].r + S_MUL(scratch[7].r,ya.r) + S_MUL(scratch[8].r,yb.r);
        scratch[5].i = scratch[0].i + S_MUL(scratch[7].i,ya.r) + S_MUL(scratch[8].i,yb.r);

        scratch[6].r =  S_MUL(scratch[10].i,ya.i) + S_MUL(scratch[9].i,yb.i);
        scratch[6].i = -S_MUL(scratch[10].r,ya.i) - S_MUL(scratch[9].r,yb.i);

        C_SUB(*Fout1,scratch[5],scratch[6]);
        C_ADD(*Fout4,scratch[5],scratch[6]);

        scratch[11].r = scratch[0].r + S_MUL(scratch[7].r,yb.r) + S_MUL(scratch[8].r,ya.r);
        scratch[11].i = scratch[0].i + S_MUL(scratch[7].i,yb.r) + S_MUL(scratch[8].i,ya.r);
        scratch[12].r = - S_MUL(scratch[10].i,yb.i) + S_MUL(scratch[9].i,ya.i);
        scratch[12].i = S_MUL(scratch[10].r,yb.i) - S_MUL(scratch[9].r,ya.i);

        C_ADD(*Fout2,scratch[11],scratch[12]);
        C_SUB(*Fout3,scratch[11],scratch[12]);

        ++Fout0;++Fout1;++Fout2;++Fout3;++Fout4;
    }
}

/* perform the butterfly for one stage of a mixed radix FFT */
static void kf_bfly_generic(
        kiss_fft_cpx * Fout,
        const size_t fstride,
        const kiss_fft_cfg st,
        int m,
        int p
        )
{
    int u,k,q1,q;
    kiss_fft_cpx * twiddles = st->twiddles;
    kiss_fft_cpx t;
    int Norig = st->nfft;

    kiss_fft_cpx * scratch = (kiss_fft_cpx*)KISS_FFT_TMP_ALLOC(sizeof(kiss_fft_cpx)*p);

    for ( u=0; u<m; ++u ) {
        k=u;
        for ( q1=0 ; q1<p ; ++q1 ) {
            scratch[q1] = Fout[ k  ];
            C_FIXDIV(scratch[q1],p);
            k += m;
        }

        k=u;
        for ( q1=0 ; q1<p ; ++q1 ) {
            int twidx=0;
            Fout[ k ] = scratch[0];
            for (q=1;q<p;++q ) {
                twidx += fstride * k;
                if (twidx>=Norig) twidx-=Norig;
                C_MUL(t,scratch[q] , twiddles[twidx] );
                C_ADDTO( Fout[ k ] ,t);
            }
            k += m;
        }
    }
    KISS_FFT_TMP_FREE(scratch);
}

static
void kf_work(
        kiss_fft_cpx * Fout,
        const kiss_fft_cpx * f,
        const size_t fstride,
        int in_stride,
        int * factors,
        const kiss_fft_cfg st
        )
{
    kiss_fft_cpx * Fout_beg=Fout;
    const int p=*factors++; /* the radix  */
    const int m=*factors++; /* stage's fft length/p */
    const kiss_fft_cpx * Fout_end = Fout + p*m;

    if (m==1) {
        do{
            *Fout = *f;
            f += fstride*in_stride;
        }while(++Fout != Fout_end );
    }else{
        do{
            // recursive call:
            // DFT of size m*p performed by doing
            // p instances of smaller DFTs of size m, 
            // each one takes a decimated version of the input
            printf("%X\n", factors);
            kf_work( Fout , f, fstride*p, in_stride, factors,st);
            f += fstride*in_stride;
        }while( (Fout += m) != Fout_end );
    }

    Fout=Fout_beg;

    // recombine the p smaller DFTs 
    switch (p) {
        case 2: kf_bfly2(Fout,fstride,st,m); break;
        case 3: kf_bfly3(Fout,fstride,st,m); break; 
        case 4: kf_bfly4(Fout,fstride,st,m); break;
        case 5: kf_bfly5(Fout,fstride,st,m); break; 
        default: kf_bfly_generic(Fout,fstride,st,m,p); break;
    }
}

/*  facbuf is populated by p1,m1,p2,m2, ...
    where 
    p[i] * m[i] = m[i-1]
    m0 = n                  */
static 
void kf_factor(int n,int * facbuf)
{
    int p=4;
    double floor_sqrt;
    floor_sqrt = floor( sqrt((double)n) );

    /*factor out powers of 4, powers of 2, then any remaining primes */
    do {
        while (n % p) {
            switch (p) {
                case 4: p = 2; break;
                case 2: p = 3; break;
                default: p += 2; break;
            }
            if (p > floor_sqrt)
                p = n;          /* no more factors, skip to end */
        }
        n /= p;
        *facbuf++ = p;
        *facbuf++ = n;
    } while (n > 1);
}

/*
 *
 * User-callable function to allocate all necessary storage space for the fft.
 *
 * The return value is a contiguous block of memory, allocated with malloc.  As such,
 * It can be freed with free(), rather than a kiss_fft-specific function.
 * */
kiss_fft_cfg kiss_fft_alloc(int nfft,int inverse_fft,void * mem,size_t * lenmem )
{
    kiss_fft_cfg st=NULL;
    size_t memneeded = sizeof(struct kiss_fft_state)
        + sizeof(kiss_fft_cpx)*(nfft-1); /* twiddle factors*/

    if ( lenmem==NULL ) {
        st = ( kiss_fft_cfg)KISS_FFT_MALLOC( memneeded );
    }else{
        if (mem != NULL && *lenmem >= memneeded)
            st = (kiss_fft_cfg)mem;
        *lenmem = memneeded;
    }
    if (st) {
        int i;
        st->nfft=nfft;
        st->inverse = inverse_fft;

        for (i=0;i<nfft;++i) {
            const double pi=3.141592653589793238462643383279502884197169399375105820974944;
            double phase = -2*pi*i / nfft;
            if (st->inverse)
                phase *= -1;
            kf_cexp(st->twiddles+i, phase );
        }

        kf_factor(nfft,st->factors);
    }
    return st;
}


void kiss_fft_stride(kiss_fft_cfg st,const kiss_fft_cpx *fin,kiss_fft_cpx *fout,int in_stride)
{
    if (fin == fout) {
        //NOTE: this is not really an in-place FFT algorithm.
        //It just performs an out-of-place FFT into a temp buffer
        kiss_fft_cpx * tmpbuf = (kiss_fft_cpx*)KISS_FFT_TMP_ALLOC( sizeof(kiss_fft_cpx)*st->nfft);
        kf_work(tmpbuf,fin,1,in_stride, st->factors,st);
        /* memcpy(fout,tmpbuf,sizeof(kiss_fft_cpx)*st->nfft); */
        KISS_FFT_TMP_FREE(tmpbuf);
    }else{
        kf_work( fout, fin, 1,in_stride, st->factors,st );
    }
}

void kiss_fft(kiss_fft_cfg cfg,const kiss_fft_cpx *fin,kiss_fft_cpx *fout)
{
    kiss_fft_stride(cfg,fin,fout,1);
}


void kiss_fft_cleanup(void)
{
    // nothing needed any more
}

int kiss_fft_next_fast_size(int n)
{
    while(1) {
        int m=n;
        while ( (m%2) == 0 ) m/=2;
        while ( (m%3) == 0 ) m/=3;
        while ( (m%5) == 0 ) m/=5;
        if (m<=1)
            break; /* n is completely factorable by twos, threes, and fives */
        n++;
    }
    return n;
}



struct kiss_fftr_state{
    kiss_fft_cfg substate;
    kiss_fft_cpx * tmpbuf;
    kiss_fft_cpx * super_twiddles;
#ifdef USE_SIMD
    void * pad;
#endif
};

kiss_fftr_cfg kiss_fftr_alloc(int nfft,int inverse_fft,void * mem,size_t * lenmem)
{
    int i;
    kiss_fftr_cfg st = NULL;
    size_t subsize, memneeded;

    if (nfft & 1) {
        fprintf(stderr,"Real FFT optimization must be even.\n");
        return NULL;
    }
    nfft >>= 1;

    kiss_fft_alloc (nfft, inverse_fft, NULL, &subsize);
    memneeded = sizeof(struct kiss_fftr_state) + subsize + sizeof(kiss_fft_cpx) * ( nfft * 3 / 2);

    if (lenmem == NULL) {
        st = (kiss_fftr_cfg) KISS_FFT_MALLOC (memneeded);
    } else {
        if (*lenmem >= memneeded)
            st = (kiss_fftr_cfg) mem;
        *lenmem = memneeded;
    }
    if (!st)
        return NULL;

    st->substate = (kiss_fft_cfg) (st + 1); /*just beyond kiss_fftr_state struct */
    st->tmpbuf = (kiss_fft_cpx *) (((char *) st->substate) + subsize);
    st->super_twiddles = st->tmpbuf + nfft;
    kiss_fft_alloc(nfft, inverse_fft, st->substate, &subsize);

    for (i = 0; i < nfft/2; ++i) {
        double phase =
            -3.14159265358979323846264338327 * ((double) (i+1) / nfft + .5);
        if (inverse_fft)
            phase *= -1;
        kf_cexp (st->super_twiddles+i,phase);
    }
    return st;
}

void kiss_fftr(kiss_fftr_cfg st,const kiss_fft_scalar *timedata,kiss_fft_cpx *freqdata)
{
    /* input buffer timedata is stored row-wise */
    int k,ncfft;
    kiss_fft_cpx fpnk,fpk,f1k,f2k,tw,tdc;

    if ( st->substate->inverse) {
        fprintf(stderr,"kiss fft usage error: improper alloc\n");
        return; /* exit(1); */
    }

    ncfft = st->substate->nfft;

    /*perform the parallel fft of two real signals packed in real,imag*/
    kiss_fft( st->substate , (const kiss_fft_cpx*)timedata, st->tmpbuf );
    /* The real part of the DC element of the frequency spectrum in st->tmpbuf
     * contains the sum of the even-numbered elements of the input time sequence
     * The imag part is the sum of the odd-numbered elements
     *
     * The sum of tdc.r and tdc.i is the sum of the input time sequence. 
     *      yielding DC of input time sequence
     * The difference of tdc.r - tdc.i is the sum of the input (dot product) [1,-1,1,-1... 
     *      yielding Nyquist bin of input time sequence
     */
 
    tdc.r = st->tmpbuf[0].r;
    tdc.i = st->tmpbuf[0].i;
    C_FIXDIV(tdc,2);
    CHECK_OVERFLOW_OP(tdc.r ,+, tdc.i);
    CHECK_OVERFLOW_OP(tdc.r ,-, tdc.i);
    freqdata[0].r = tdc.r + tdc.i;
    freqdata[ncfft].r = tdc.r - tdc.i;
#ifdef USE_SIMD    
    freqdata[ncfft].i = freqdata[0].i = _mm_set1_ps(0);
#else
    freqdata[ncfft].i = freqdata[0].i = 0;
#endif

    for ( k=1;k <= ncfft/2 ; ++k ) {
        fpk    = st->tmpbuf[k]; 
        fpnk.r =   st->tmpbuf[ncfft-k].r;
        fpnk.i = - st->tmpbuf[ncfft-k].i;
        C_FIXDIV(fpk,2);
        C_FIXDIV(fpnk,2);

        C_ADD( f1k, fpk , fpnk );
        C_SUB( f2k, fpk , fpnk );
        C_MUL( tw , f2k , st->super_twiddles[k-1]);

        freqdata[k].r = HALF_OF(f1k.r + tw.r);
        freqdata[k].i = HALF_OF(f1k.i + tw.i);
        freqdata[ncfft-k].r = HALF_OF(f1k.r - tw.r);
        freqdata[ncfft-k].i = HALF_OF(tw.i - f1k.i);
    }
}

void kiss_fftri(kiss_fftr_cfg st,const kiss_fft_cpx *freqdata,kiss_fft_scalar *timedata)
{
    /* input buffer timedata is stored row-wise */
    int k, ncfft;

    if (st->substate->inverse == 0) {
        fprintf (stderr, "kiss fft usage error: improper alloc\n");
        return; /* exit (1); */
    }

    ncfft = st->substate->nfft;

    st->tmpbuf[0].r = freqdata[0].r + freqdata[ncfft].r;
    st->tmpbuf[0].i = freqdata[0].r - freqdata[ncfft].r;
    C_FIXDIV(st->tmpbuf[0],2);

    for (k = 1; k <= ncfft / 2; ++k) {
        kiss_fft_cpx fk, fnkc, fek, fok, tmp;
        fk = freqdata[k];
        fnkc.r = freqdata[ncfft - k].r;
        fnkc.i = -freqdata[ncfft - k].i;
        C_FIXDIV( fk , 2 );
        C_FIXDIV( fnkc , 2 );

        C_ADD (fek, fk, fnkc);
        C_SUB (tmp, fk, fnkc);
        C_MUL (fok, tmp, st->super_twiddles[k-1]);
        C_ADD (st->tmpbuf[k],     fek, fok);
        C_SUB (st->tmpbuf[ncfft - k], fek, fok);
#ifdef USE_SIMD        
        st->tmpbuf[ncfft - k].i *= _mm_set1_ps(-1.0);
#else
        st->tmpbuf[ncfft - k].i *= -1;
#endif
    }
    kiss_fft (st->substate, st->tmpbuf, (kiss_fft_cpx *) timedata);
}


void fft_test(void){
    void * mem_void;
    float fft_test_var[] = {2122.0413,
-1500.8131,0.2474,-0.6662,0.0469,-0.5530,-1.1508,-3.0970,-21.2059,112.5379,
-116.3196,24.6289,3.3810,1.1573,0.5008,0.1204,0.3067,0.0116,0.2022,-0.0999,
0.1731,-0.0434,0.0447,-0.0342,0.0682,-0.1098,0.1564,-0.0341,-0.0111,-0.1101,
0.2007,-0.0711,0.0093,-0.0427,0.0334,0.0875,-0.1668,0.1667,-0.1253,-0.0302,
0.2570,-0.3047,0.2215,-0.1534,0.0830,0.0253,-0.1199,0.0293,0.0356,0.0579,
-0.0251,-0.0944,0.0795,0.0102,0.0895,-0.2657,0.4527,-0.4486,0.3200,-0.5518,
0.6536,-0.3497,0.1744,-0.2611,0.2901,-0.0638,-0.0384,-0.0491,0.0187,0.0633,
-0.0370,-0.0399,0.0624,-0.1123,0.1694,-0.1692,0.1723,-0.2049,0.2025,-0.1848,
0.1906,-0.0909,-0.0346,0.0317,0.0267,-0.0772,0.0503,0.1266,-0.1129,-0.1238,
0.0976,0.1351,-0.2573,0.2059,-0.1317,0.1295,-0.1563,0.0823,0.0647,-0.1136,
0.0978,-0.0425,-0.0043,0.0584,-0.0781,0.0304,0.0812,-0.2004,0.1076,0.0677,
-0.0243,-0.1015,0.0296,-0.1005,0.3741,-0.5703,0.6355,-0.4307,0.3661,-0.5394,
0.4918,-0.2553,0.0580,-0.0801,0.1945,-0.1095,-0.0419,0.0835,-0.1031,0.0548,
0.0891,-0.1235,0.0931,-0.1392,0.1702,-0.1538,0.2188,-0.3080,0.2119,-0.1219,
0.1922,-0.1313,-0.1091,0.2143,-0.1231,-0.0647,0.1537,-0.0893,0.0735,-0.0186,
-0.0405,0.1760,-0.3031,0.1299,-0.0405,0.1834,-0.1847,0.0328,0.0299,-0.0164,
-0.0166,-0.0506,0.1289,-0.0639,0.0387,-0.0325,0.1201,-0.1202,-0.1496,0.1907,
0.1155,-0.1354,0.0383,-0.1095,1.4643,-3.7336,2.8235,-0.5818,0.3027,-0.2922,
0.2246,-0.2472,0.1264,-0.0644,0.0578,0.0336,-0.0160,-0.0742,-0.1401,0.2303,
0.0423,-0.1929,0.1559,-0.2083,0.2867,-0.2715,0.3429,-0.3178,0.0604,0.0581,
-0.0017,0.0026,-0.0370,0.0593,0.0352,-0.1804,0.0515,0.0975,-0.1207,0.1098,
-0.0801,0.2162,-0.3100,0.2145,-0.1952,0.2700,-0.1862,0.0512,0.0048,-0.0014,
0.1184,-0.3803,0.3169,-0.0209,-0.1641,0.1548,-0.0160,0.0408,-0.2279,0.0979,
0.2432,-0.2804,0.2722,-0.2923,0.2574,-0.2452,0.1107,-0.1080,0.2136,-0.1726,
0.1412,-0.1343,0.0755,-0.0439,-0.0180,0.1113,0.0077,-0.0114,-0.1996,0.1572,
0.1306,-0.3080,0.2234,-0.2124,0.2628,-0.2286,0.2004,-0.0593,-0.1287,0.0889,
0.1080,-0.1790,0.1162,-0.0030,-0.0004,0.0195,-0.0539,-0.0061,-0.0527,0.0959,
-0.1148,0.1962,-0.2368,0.1865,-0.1752,0.1608,0.0101,-0.1475,0.1006,-0.0785,
0.2672,-0.2726,0.1167,-0.0172,-0.1489,0.0762,0.1273,-0.0113,-0.1241,-0.0338,
0.1765,-0.2115,0.1169,0.0096,0.0721,-0.0074,-0.0588,-0.1432,0.1974,-0.1215,
0.1608,-0.1836,0.1088,0.0164,-0.2960,0.3359,-0.0620,0.0264,-0.0686,-0.0309,
0.2311,-0.3271,0.2444,-0.2636,0.2544,-0.1050,0.0843,-0.1223,0.0809,-0.0294,
-0.0273,-0.0381,0.1105,-0.0955,0.0416,0.0112,0.0642,-0.1083,-0.0195,0.2389,
-0.3578,0.4141,-0.4220,0.3131,-0.2091,0.0639,0.1997,-0.2974,0.0378,0.0159,
0.1688,-0.1568,0.0839,0.0002,-0.1129,0.0350,0.1529,-0.0992,0.0217,-0.0776,
0.1655,-0.1173,-0.1435,0.1081,-0.0418,0.2678,-0.1214,-0.2134,0.0787,0.1428,
-0.0621,0.0389,-0.0879,0.2600,-0.4441,0.1944,0.0083,0.0233,0.0104,-0.1534,
0.2873,-0.2119,0.1059,-0.1754,0.2378,-0.1424,0.1098,-0.0862,0.0227,0.0296,
-0.0925,0.0219,0.0895,-0.1088,-0.0063,0.1132,-0.0931,
0.1389,-0.3760,0.5096,-0.3728,0.1920,-0.0697,0.0134,-0.0260,
0.0293,0.1358,-0.2061,0.0523,0.0253,-0.0683,0.0994,-0.0705,
0.1153,-0.2021,0.1327,-0.0885,0.1582,-0.1199,0.2138,-0.4184,
0.4291,-0.2558,-0.0199,0.0847,-0.0511,0.2117,-0.2799,0.0622,
0.1221,-0.1016,-0.0138,0.0677,0.0399,-0.0223,-0.0783,0.1288,
-0.1793,0.1643,-0.2146,0.3038,-0.2727,0.2083,-0.2087,0.1576,
-0.0825,0.0299,-0.0072,-0.0109,0.1240,-0.1129,0.0127,-0.0038,
-0.0086,-0.0516,0.1679,-0.1735,0.1747,-0.2119,0.2029,-0.1893,
0.1407,-0.0181,-0.0277,-0.1021,0.0856,0.1168,-0.1565,0.0446,
0.0578,-0.0748,0.1079,-0.1769,0.1535,-0.0893,0.1739,-0.3041,
0.2033,-0.2191,0.4223,-0.3822,0.2136,-0.0163,-0.0814,-0.0246,
-0.0110,0.1724,-0.1134,-0.0575,0.1270,-0.1517,0.1099,-0.0819,
0.0787,-0.0485,0.0570,-0.0376,-0.0882,0.2235,-0.2463,0.2131,
-0.1387,0.1017,-0.0740,0.0066,0.0410,-0.0757,0.0143,-0.0076,
0.1102,-0.1264,0.0851,-0.1396,0.1907,-0.1579,0.2128,-0.2536,
0.2154,-0.3094,0.4221,-0.3290,0.1727,-0.0588,0.0133,-0.0347,
-0.0362,0.1369,-0.0596,-0.1471,0.2067,-0.1450,0.2218,-0.3582,
0.2538,-0.0947,0.2086,-0.2866,0.2158,-0.2833,0.3682,-0.2717,
0.0643,0.0785,-0.0131,-0.0777,-0.0156,0.1106,-0.0604,0.0314,
0.0349,-0.1370,0.1239,-0.0815,0.0541,-0.0180,-0.0054,0.0659,
-0.1550,0.1199,-0.1137,0.1327,-0.0233,-0.1109,0.1058,-0.0459,
0.1483,-0.1393,0.0332,-0.0563,0.0971,-0.0571,0.0141,-0.0784,
0.0750,-0.0434,0.1306,-0.1862,0.1524,-0.1531,0.1721,-0.1387,
-0.0640,0.2947,-0.1866,0.0460,-0.0735,0.0423,0.0437,-0.0809,
0.0682,-0.1131,0.3076,-0.4784,0.3513,-0.2617,0.3865,-0.3790,
0.2450,-0.1947,0.2320,-0.1265,-0.0901,0.0648,0.1114,-0.1023,
0.0204,-0.0531,0.0300,-0.0209,0.1431,-0.2231,0.1693,-0.0812,
0.0590,0.0385,-0.1218,0.0807,0.0082,-0.0529,0.0197,-0.0185,
0.0403,0.0095,-0.1184,0.0067,0.1963,-0.1452,0.0058,0.0204,
0.0172,-0.0164,0.0469,-0.1153,0.1657,-0.2442,0.3332,-0.3083,
0.1645,-0.0482,0.0495,-0.0334,-0.1423,0.1217,0.0326,0.0392,
-0.0935,-0.0065,0.1241,-0.1442,0.0923,-0.1328,0.4411,-0.5657,
0.3565,-0.4661,0.6565,-0.5105,0.1682,0.0305,0.1350,-0.1448,
-0.1541,0.2271,-0.0925,0.0296,0.0847,-0.1749,0.1243,-0.1189,
0.1364,-0.1076,0.1105,-0.1603,0.1009,0.0502,-0.0974,0.0013,
0.0195,0.0825,-0.1232,0.0540,0.1311,-0.0967,-0.0454,-0.0754,
0.1364,-0.0050,-0.1679,0.1801,-0.0273,-0.0508,-0.0867,0.2906,
-0.1761,-0.1004,0.1943,-0.0982,-0.0088,0.0475,-0.0190,-0.0296,
0.0417,-0.1261,0.1208,0.0426,-0.0652,-0.1145,0.2276,-0.2643,
0.3393,-0.5607,0.7642,-0.5622,0.3784,-0.4649,0.5087,-0.3979,
0.1199,0.0175,0.0557,-0.0321,-0.0716,0.0285,0.0614,-0.0375,
0.0629,-0.1428,0.1511,-0.0901,0.0759,-0.0548,0.0631,-0.1743,
0.1212,0.0505,-0.0769,0.0445,-0.1099,0.1100,-0.0357,-0.1134,
0.2037,-0.0220,-0.0308,-0.0016,-0.0746,0.1477,-0.2390,0.1028,
0.1253,-0.0422,-0.0689,0.0255,-0.0786,0.0601,0.0762,-0.1584,
0.1599,-0.0849,0.0204,0.0369,0.0061,-0.0913,-0.0430,0.1191,
0.1234,-0.3442,0.2984,-0.3467,0.1308,1.1248,-1.3778,0.1103,
0.4323,-0.2770,0.1386,-0.0608,-0.0135,0.0351,0.0505,-0.0104,
-0.0969,0.0234,-0.0483,0.1585,-0.0109,-0.2184,0.2486,-0.1977,
0.2238,-0.2083,0.2241,-0.1886,-0.0744,0.2507,-0.1425,0.0406,
-0.0417,0.0263,0.0609,-0.1668,0.0579,0.0965,-0.1235,0.1178,
-0.0440,0.0351,-0.1393,0.1162,-0.0314,0.1425,-0.1769,0.0023,
0.1205,-0.1233,0.1279,-0.2025,0.1834,-0.0799,-0.0369,0.1227,
-0.0221,-0.0586,-0.1326,0.1053,0.3123,-0.5380,0.4814,-0.3095,
0.3148,-0.4255,0.2345,-0.1320,0.2497,-0.2849,0.2117,-0.0973,
0.0627,-0.0503,-0.0961,0.2718,-0.1649,-0.0018,-0.0502,0.0946,
0.0043,-0.2367,0.3830,-0.3292,0.1709,-0.0395,0.1048,-0.1578,
0.0135,0.0567,0.0113,-0.1060,0.1322,0.0018,-0.0483,0.0485,
-0.1126,0.0720,-0.0149,0.0011,-0.0185,0.1053,-0.1648,0.0598,
0.0046,0.0023,0.1144,-0.2554,0.2259,-0.0257,0.0377,-0.1258,
0.0298,0.0351,-0.1800,0.2734,-0.0085,-0.1325,-0.0202,-0.0305,
0.2484,-0.4468,0.3852,-0.1510,0.2134,-0.2235,0.0041,0.0482,
0.0467,-0.0725,0.0193,0.0246,-0.0429,0.0532,-0.2599,0.4202,
-0.1688,-0.0665,0.0729,-0.0640,0.1423,-0.2403,0.2746,-0.2545,
0.2280,-0.1512,0.0633,-0.1168,0.1663,-0.1213,0.0620,-0.0673,
0.0662,-0.0255,0.0621,-0.0954,0.0172,0.0829,-0.1270,0.1882,
-0.1832,0.1406,-0.1076,0.0374,0.0302,-0.1183,0.2118,-0.2851,
0.1813,-0.0090,-0.0323,0.0206,-0.0336,0.0894,-0.1723,0.1342,
0.0905,-0.2678,0.2853,-0.1900,0.9077,-1.5180,0.5390,0.0381,
0.2143,-0.0823,0.0141,-0.2254,0.2705,-0.0802,-0.0368,0.0880,
-0.0954,0.1065,-0.2056,0.0871,0.1482,-0.2193,0.1730,-0.1698,
0.2308,-0.2399,0.2453,-0.2650,0.2305,-0.1334,0.1047,-0.0666,
-0.0043,0.0434,-0.0502,-0.0398,0.1146,-0.1043,0.0255,0.1292,
-0.2435,0.1994,-0.2775,0.3939,-0.3081,0.1691,-0.0168,-0.0313,
-0.0219,0.0497,0.0497,-0.0999,0.0409,-0.0200,-0.0729,0.1121,
-0.0690,0.1463,-0.2073,0.0953,0.0151,-0.0896,0.0477,0.1170,
-0.0749,0.0647,-0.1186,-0.0828,0.1629,-0.0690,0.0758,-0.1267,
0.0472,0.1837,-0.3036,0.1640,0.0067,0.0699,-0.0802,-0.0628,
0.1105,-0.1272,0.1755,-0.2413,0.2752,-0.3084,0.2643,-0.1829,
0.1317,-0.0247,-0.0640,0.0688,-0.0250};


    size_t i;

extern kiss_fft_cpx cx_in[512];
extern kiss_fft_cpx cx_out[258];
extern kiss_fft_cpx *p_in;
extern kiss_fft_cpx *p_out;

    p_in = &cx_in;
    p_out = &cx_out;

    for (i=0; i<512; i++){
        cx_in[i].r = fft_test_var[i];
    }
    free(fft_test_var);
    kiss_fftr_cfg cfg = kiss_fftr_alloc(512, 0, NULL, NULL);
    kiss_fft(cfg, p_in, p_out);

    free(cfg);


}