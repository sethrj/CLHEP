// -*- C++ -*-
// $Id: Vector.cc,v 1.2 2003/07/18 05:31:48 garren Exp $
// ---------------------------------------------------------------------------
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
// 
// 
// Copyright Cornell University 1993, 1996, All Rights Reserved.
// 
// This software written by Nobu Katayama and Mike Smyth, Cornell University.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice and author attribution, this list of conditions and the
//    following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright
//    notice and author attribution, this list of conditions and the
//    following disclaimer in the documentation and/or other materials
//    provided with the distribution.
// 3. Neither the name of the University nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// Creation of derivative forms of this software for commercial
// utilization may be subject to restriction; written permission may be
// obtained from Cornell University.
// 
// CORNELL MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.  By way
// of example, but not limitation, CORNELL MAKES NO REPRESENTATIONS OR
// WARRANTIES OF MERCANTABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE OR THAT
// THE USE OF THIS SOFTWARE OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS,
// COPYRIGHTS, TRADEMARKS, OR OTHER RIGHTS.  Cornell University shall not be
// held liable for any liability with respect to any claim by the user or any
// other party arising from use of the program.
//
#ifdef GNUPRAGMA
#pragma implementation
#endif

#include "Matrix/defs.h"
#include <string.h>

#include "CLHEP/Random/Random.h"

#include "CLHEP/Vector/ThreeVector.h"

#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Matrix/Matrix.h"

#ifdef HEP_DEBUG_INLINE
#include "CLHEP/Matrix/Vector.icc"
#endif

namespace CLHEP {

// Simple operation for all elements

#define SIMPLE_UOP(OPER)          \
   register double *a=m;            \
   register double *e=m+num_size(); \
   for(;a<e; a++) (*a) OPER t;

#define SIMPLE_BOP(OPER)          \
   register double *a=m;            \
   register double *b=m2.m;         \
   register double *e=m+num_size(); \
   for(;a<e; a++, b++) (*a) OPER (*b);

#define SIMPLE_TOP(OPER)          \
   register double *a=m1.m;            \
   register double *b=m2.m;         \
   register double *t=mret.m;         \
   register double *e=m1.m+m1.num_size(); \
   for( ;a<e; a++, b++, t++) (*t) = (*a) OPER (*b);

#define CHK_DIM_2(r1,r2,c1,c2,fun) \
   if (r1!=r2 || c1!=c2)  { \
     HepGenMatrix::error("Range error in Vector function " #fun "(1)."); \
   }

#define CHK_DIM_1(c1,r2,fun) \
   if (c1!=r2) { \
     HepGenMatrix::error("Range error in Vector function " #fun "(2)."); \
   }

// Constructors. (Default constructors are inlined and in .icc file)

HepVector::HepVector(int p)
   : nrow(p)
{
   m = new_m(nrow);
}

HepVector::HepVector(int p, int init)
   : nrow(p)
{
   m = new_m(nrow);
   switch (init)
   {
   case 0:
      memset(m, 0, p * sizeof(double));
      break;
      
   case 1:
      {
	 double *e = m + nrow;
	 for (double *i=m; i<e; i++) *i = 1.0;
	 break;
      }
      
   default:
      error("Vector: initialization must be either 0 or 1.");
   }
}

HepVector::HepVector(int p, HepRandom &r)
   : nrow(p)
{
   m = new_m(nrow);
   double *a = m;
   double *b = m + nrow;
   for(;a<b;a++) *a = r();
}


//
// Destructor
//
HepVector::~HepVector() {
   delete_m(nrow, m);
}

HepVector::HepVector(const HepVector &m1)
   : nrow(m1.nrow)
{
   m = new_m(nrow);
   memcpy(m, m1.m, nrow*sizeof(double));
}

//
// Copy constructor from the class of other precision
//


HepVector::HepVector(const HepMatrix &m1)
   : nrow(m1.nrow)
{
   if (m1.num_col() != 1)
      error("Vector::Vector(Matrix) : Matrix is not Nx1");
   
   m = new_m(nrow);
   memcpy(m,m1.m,nrow*sizeof(double));
}

// Sub matrix

HepVector HepVector::sub(int min_row, int max_row) const
#ifdef HEP_GNU_OPTIMIZED_RETURN
return vret(max_row-min_row+1);
{
#else
{
  HepVector vret(max_row-min_row+1);
#endif
  if(max_row > num_row())
    error("HepVector::sub: Index out of range");
  double *a = vret.m;
  double *b = m + min_row - 1;
  double *e = vret.m + vret.num_row();
  for(;a<e;) *(a++) = *(b++);
  return vret;
}

#ifdef HEP_CC_NEED_SUB_WITHOUT_CONST
HepVector HepVector::sub(int min_row, int max_row)
{
  HepVector vret(max_row-min_row+1);
  if(max_row > num_row())
    error("HepVector::sub: Index out of range");
  double *a = vret.m;
  double *b = m + min_row - 1;
  double *e = vret.m + vret.num_row();
  for(;a<e;) *(a++) = *(b++);
  return vret;
}
#endif

void HepVector::sub(int row,const HepVector &v1)
{
  if(row <1 || row+v1.num_row()-1 > num_row())
    error("HepVector::sub: Index out of range");
  double *a = v1.m;
  double *b = m + row - 1;
  double *e = v1.m + v1.num_row();
  for(;a<e;) *(b++) = *(a++);
}

//
// Direct sum of two matricies
//

HepVector dsum(const HepVector &m1,
				     const HepVector &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
  return mret(m1.num_row() + m2.num_row(), 0);
{
#else
{
  HepVector mret(m1.num_row() + m2.num_row(),
				       0);
#endif
  mret.sub(1,m1);
  mret.sub(m1.num_row()+1,m2);
  return mret;
}

/* -----------------------------------------------------------------------
   This section contains support routines for matrix.h. This section contains
   The two argument functions +,-. They call the copy constructor and +=,-=.
   ----------------------------------------------------------------------- */
HepVector HepVector::operator- () const 
#ifdef HEP_GNU_OPTIMIZED_RETURN
      return m2(nrow);
{
#else
{
   HepVector m2(nrow);
#endif
   register double *a=m;
   register double *b=m2.m;
   register double *e=m+num_size();
   for(;a<e; a++, b++) (*b) = -(*a);
   return m2;
}

   

HepVector operator+(const HepMatrix &m1,const HepVector &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m2);
{
#else
{
  HepVector mret(m2);
#endif
  CHK_DIM_2(m1.num_row(),m2.num_row(),m1.num_col(),1,+);
  mret += m1;
  return mret;
}

HepVector operator+(const HepVector &m1,const HepMatrix &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1);
{
#else
{
  HepVector mret(m1);
#endif
  CHK_DIM_2(m1.num_row(),m2.num_row(),1,m2.num_col(),+);
  mret += m2;
  return mret;
}

HepVector operator+(const HepVector &m1,const HepVector &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1.num_row());
{
#else
{
  HepVector mret(m1.num_row());
#endif
  CHK_DIM_1(m1.num_row(),m2.num_row(),+);
  SIMPLE_TOP(+)
  return mret;
}

//
// operator -
//

HepVector operator-(const HepMatrix &m1,const HepVector &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret;
{
#else
{
  HepVector mret;
#endif
  CHK_DIM_2(m1.num_row(),m2.num_row(),m1.num_col(),1,-);
  mret = m1;
  mret -= m2;
  return mret;
}

HepVector operator-(const HepVector &m1,const HepMatrix &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1);
{
#else
{
  HepVector mret(m1);
#endif
  CHK_DIM_2(m1.num_row(),m2.num_row(),1,m2.num_col(),-);
  mret -= m2;
  return mret;
}

HepVector operator-(const HepVector &m1,const HepVector &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1.num_row());
{
#else
{
  HepVector mret(m1.num_row());
#endif
  CHK_DIM_1(m1.num_row(),m2.num_row(),-);
  SIMPLE_TOP(-)
  return mret;
}

/* -----------------------------------------------------------------------
   This section contains support routines for matrix.h. This file contains
   The two argument functions *,/. They call copy constructor and then /=,*=.
   ----------------------------------------------------------------------- */

HepVector operator/(
const HepVector &m1,double t)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1);
{
#else
{
  HepVector mret(m1);
#endif
  mret /= t;
  return mret;
}

HepVector operator*(const HepVector &m1,double t)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1);
{
#else
{
  HepVector mret(m1);
#endif
  mret *= t;
  return mret;
}

HepVector operator*(double t,const HepVector &m1)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1);
{
#else
{
  HepVector mret(m1);
#endif
  mret *= t;
  return mret;
}

HepVector operator*(const HepMatrix &m1,const HepVector &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1.num_row());
{
#else
{
  HepVector mret(m1.num_row());
#endif
  CHK_DIM_1(m1.num_col(),m2.num_row(),*);
  double *m1p,*m2p,*m3p,*vp;
  double temp;
  m3p=mret.m;
  for(m1p=m1.m;m1p<m1.m+m1.num_row()*m1.num_col();m1p=m2p)
    {
      temp=0;
      vp=m2.m;
      m2p=m1p;
      while(m2p<m1p+m1.num_col())
	temp+=(*(m2p++))*(*(vp++));
      *(m3p++)=temp;
    }
  return mret;
}

HepMatrix operator*(const HepVector &m1,const HepMatrix &m2)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return mret(m1.num_row(),m2.num_col());
{
#else
{
  HepMatrix mret(m1.num_row(),m2.num_col());
#endif
  CHK_DIM_1(1,m2.num_row(),*);
  double *m1p;
  double *m2p;
  double *mrp=mret.m;
  for(m1p=m1.m;m1p<m1.m+m1.num_row();m1p++)
    for(m2p=m2.m;m2p<m2.m+m2.num_col();m2p++)
      *(mrp++)=*m1p*(*m2p);
  return mret;
}

/* -----------------------------------------------------------------------
   This section contains the assignment and inplace operators =,+=,-=,*=,/=.
   ----------------------------------------------------------------------- */

HepMatrix & HepMatrix::operator+=(const HepVector &m2)
{
  CHK_DIM_2(num_row(),m2.num_row(),num_col(),1,+=);
  SIMPLE_BOP(+=)
  return (*this);
}

HepVector & HepVector::operator+=(const HepMatrix &m2)
{
  CHK_DIM_2(num_row(),m2.num_row(),1,m2.num_col(),+=);
  SIMPLE_BOP(+=)
  return (*this);
}

HepVector & HepVector::operator+=(const HepVector &m2)
{
  CHK_DIM_1(num_row(),m2.num_row(),+=);
  SIMPLE_BOP(+=)
  return (*this);
}

HepMatrix &  HepMatrix::operator-=(const HepVector &m2)
{
  CHK_DIM_2(num_row(),m2.num_row(),num_col(),1,-=);
  SIMPLE_BOP(-=)
  return (*this);
}

HepVector & HepVector::operator-=(const HepMatrix &m2)
{
  CHK_DIM_2(num_row(),m2.num_row(),1,m2.num_col(),-=);
  SIMPLE_BOP(-=)
  return (*this);
}

HepVector & HepVector::operator-=(const HepVector &m2)
{
  CHK_DIM_1(num_row(),m2.num_row(),-=);
  SIMPLE_BOP(-=)
  return (*this);
}

HepVector & HepVector::operator/=(double t)
{
  SIMPLE_UOP(/=)
  return (*this);
}

HepVector & HepVector::operator*=(double t)
{
  SIMPLE_UOP(*=)
  return (*this);
}

HepMatrix & HepMatrix::operator=(const HepVector &m1)
{
   if(m1.nrow != size)
   {
      delete_m(size,m);
      size = m1.nrow;
      m = new_m(size);
   }
   nrow = m1.nrow;
   ncol = 1;
   memcpy(m,m1.m,size*sizeof(double));
   return (*this);
}

HepVector & HepVector::operator=(const HepVector &m1)
{
   if(m1.nrow != nrow)
   {
      delete_m(nrow, m);
      nrow = m1.nrow;
      m = new_m(nrow);
   }
   memcpy(m,m1.m,num_size()*sizeof(double));
   return (*this);
}

HepVector & HepVector::operator=(const HepMatrix &m1)
{
   if (m1.num_col() != 1)
      error("Vector::operator=(Matrix) : Matrix is not Nx1");
   
   if(m1.nrow != nrow)
   {
      delete_m(nrow, m);
      nrow = m1.nrow;
      m = new_m(nrow);
   }
   memcpy(m,m1.m,num_size()*sizeof(double));
   return (*this);
}

HepVector & HepVector::operator=(const Hep3Vector &v)
{
   if(nrow != 3)
   {
      delete_m(nrow, m);
      nrow = 3;
      m = new_m(nrow);
   }
   m[0] = v.x();
   m[1] = v.y();
   m[2] = v.z();
   return (*this);
}

//
// Copy constructor from the class of other precision
//


// Print the Matrix.

std::ostream& operator<<(std::ostream &s, const HepVector &q)
{
  s << std::endl;
/* Fixed format needs 3 extra characters for field, while scientific needs 7 */
  int width;
  if(s.flags() & std::ios::fixed)
    width = s.precision()+3;
  else
    width = s.precision()+7;
  for(int irow = 1; irow<= q.num_row(); irow++)
    {
      s.width(width);
      s << q(irow) << std::endl;
    }
  return s;
}

HepMatrix HepVector::T() const
#ifdef HEP_GNU_OPTIMIZED_RETURN
return mret(1,num_row());
{
#else
{
  HepMatrix mret(1,num_row());
#endif
  memcpy(mret.m,m,num_size()*sizeof(double));
  return mret;
}

double dot(const HepVector &v1,const HepVector &v2)
{
  if(v1.num_row()!=v2.num_row())
     HepGenMatrix::error("v1 and v2 need to be the same size in dot(HepVector, HepVector)");
  double d= 0;
  double *a = v1.m;
  double *b = v2.m;
  double *e = a + v1.num_size();
  for(;a<e;) d += (*(a++)) * (*(b++));
  return d;
}

HepVector HepVector::
apply(double (*f)(double, int)) const
#ifdef HEP_GNU_OPTIMIZED_RETURN
return mret(num_row());
{
#else
{
  HepVector mret(num_row());
#endif
  double *a = m;
  double *b = mret.m;
  for(int ir=1;ir<=num_row();ir++) {
    *(b++) = (*f)(*(a++), ir);
  }
  return mret;
}

void HepVector::invert(int &) {
   error("HepVector::invert: You cannot invert a Vector");
}

HepVector solve(const HepMatrix &a, const HepVector &v)
#ifdef HEP_GNU_OPTIMIZED_RETURN
     return vret(v);
{
#else
{
  HepVector vret(v);
#endif
  static int max_array = 20;
  static int *ir = new int [max_array+1];

  if(a.ncol != a.nrow)
     HepGenMatrix::error("Matrix::solve Matrix is not NxN");
  if(a.ncol != v.nrow)
     HepGenMatrix::error("Matrix::solve Vector has wrong number of rows");

  int n = a.ncol;
  if (n > max_array) {
    delete [] ir;
    max_array = n;
    ir = new int [max_array+1];
  }
  double det;
  HepMatrix mt(a);
  int i = mt.dfact_matrix(det, ir);
  if (i!=0) {
    for (i=1;i<=n;i++) vret(i) = 0;
    return vret;
  }
  double s21, s22;
  int nxch = ir[n];
  if (nxch!=0) {
    for (int mm=1;mm<=nxch;mm++) {
      int ij = ir[mm];
      i = ij >> 12;
      int j = ij%4096;
      double te = vret(i);
      vret(i) = vret(j);
      vret(j) = te;
    }
  }
  vret(1) = mt(1,1) * vret(1);
  if (n!=1) {
    for (i=2;i<=n;i++) {
      s21 = -vret(i);
      for (int j=1;j<i;j++) {
	s21 += mt(i,j) * vret(j); 
      }
      vret(i) = -mt(i,i)*s21;
    }
    for (i=1;i<n;i++) {
      int nmi = n-i;
      s22 = -vret(nmi);
      for (int j=1;j<=i;j++) {
	s22 += mt(nmi,n-j+1) * vret(n-j+1);
      }
      vret(nmi) = -s22;
    }
  }
  return vret;
}

}  // namespace CLHEP
