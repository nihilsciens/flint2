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

    Copyright (C) 2011 Sebastian Pancratz
    Copyright (C) 2009 William Hart

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_mod_poly.h"
#include "ulong_extras.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    printf("mullow....");
    fflush(stdout);

    flint_randinit(state);

    /* Compare with truncated product of a and b */
    for (i = 0; i < 2000; i++)
    {
        fmpz_t p;
        fmpz_mod_poly_t a, b, c;
        slong trunc;

        fmpz_init(p);
        fmpz_randtest_unsigned(p, state, 2 * FLINT_BITS);
        fmpz_add_ui(p, p, 2);

        fmpz_mod_poly_init(a, p);
        fmpz_mod_poly_init(b, p);
        fmpz_mod_poly_init(c, p);
        trunc = n_randint(state, 50);
        fmpz_mod_poly_randtest(b, state, trunc);
        fmpz_mod_poly_randtest(c, state, trunc);

        fmpz_mod_poly_mullow(a, b, c, trunc);
        fmpz_mod_poly_mul(b, b, c);
        fmpz_mod_poly_truncate(b, trunc);

        result = (fmpz_mod_poly_equal(a, b));
        if (!result)
        {
            printf("FAIL:\n");
            fmpz_mod_poly_print(a), printf("\n\n");
            fmpz_mod_poly_print(b), printf("\n\n");
            abort();
        }

        fmpz_mod_poly_clear(a);
        fmpz_mod_poly_clear(b);
        fmpz_mod_poly_clear(c);
        fmpz_clear(p);
    }

    flint_randclear(state);
    flint_cleanup();
    printf("PASS\n");
    return 0;
}

