/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2010 Sebastian Pancratz

******************************************************************************/

#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_vec.h"
#include "fmpq_poly.h"

void fmpq_poly_scalar_div_si(fmpq_poly_t rop, const fmpq_poly_t op, long c)
{
    if (c == 1L)
        fmpq_poly_set(rop, op);
    else if (c == -1L)
        fmpq_poly_neg(rop, op);
    else
    {
        fmpz_t d, fc;
        fmpq_poly_fit_length(rop, op->length);
        fmpz_init(d);
        fmpz_init(fc);
        _fmpz_poly_vec_content(d, op->coeffs, op->length);
        fmpz_set_si(fc, c);
        fmpz_gcd(d, d, fc);
        if (*d == 1)
        {
            if (rop != op)
                _fmpz_vec_copy(rop->coeffs, op->coeffs, op->length);
            fmpz_mul(rop->den, op->den, fc);
        }
        else
        {
            _fmpz_vec_divexact(rop->coeffs, op->coeffs, op->length, d);
            fmpz_divexact(d, fc, d);
            fmpz_mul(rop->den, op->den, d);
        }
        fmpz_clear(d);
        fmpz_clear(fc);
        _fmpq_poly_set_length(rop, op->length);
        _fmpq_poly_normalise(rop);
    }
}

