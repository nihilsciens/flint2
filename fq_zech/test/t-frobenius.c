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

    Copyright (C) 2013 Mike Hansen

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "fq_zech.h"
#include "ulong_extras.h"
#include "long_extras.h"

int
main(void)
{
    int j, i, result;
    flint_rand_t state;
    fq_zech_ctx_t ctx;

    printf("frobenius... ");
    fflush(stdout);

    flint_randinit(state);

    for (j = 0; j < 10; j++)
    {
        fq_zech_ctx_randtest(ctx, state);

        /* Check aliasing: a = frob(a, e) */
        for (i = 0; i < 100; i++)
        {
            fq_zech_t a, b;
            fmpz_t e;

            fq_zech_init(a, ctx);
            fq_zech_init(b, ctx);
            fmpz_init(e);

            fq_zech_randtest(a, state, ctx);
            fmpz_randtest_unsigned(e, state, 6);

            fq_zech_frobenius(b, a, fmpz_get_ui(e), ctx);
            fq_zech_frobenius(a, a, fmpz_get_ui(e), ctx);

            result = (fq_zech_equal(a, b, ctx));
            if (!result)
            {
                printf("FAIL (alias):\n\n");
                printf("a = "), fq_zech_print_pretty(a, ctx), printf("\n");
                printf("b = "), fq_zech_print_pretty(b, ctx), printf("\n");
                abort();
            }

            fq_zech_clear(a, ctx);
            fq_zech_clear(b, ctx);
            fmpz_clear(e);
        }

        /* Compare with exponentiation, for integral values */
        for (i = 0; i < 100; i++)
        {
            fq_zech_t a, b, c;
            fmpz_t e, f;

            fq_zech_init(a, ctx);
            fq_zech_init(b, ctx);
            fq_zech_init(c, ctx);
            fmpz_init(f);
            fmpz_init(e);

            fq_zech_randtest(a, state, ctx);
            fmpz_randtest_unsigned(e, state, 6);

            fq_zech_frobenius(b, a, fmpz_get_ui(e), ctx);

            fmpz_pow_ui(e, fq_zech_ctx_prime(ctx), fmpz_get_ui(e));
            fq_zech_pow(c, a, e, ctx);

            result = (fq_zech_equal(b, c, ctx));
            if (!result)
            {
                printf("FAIL (cmp with pow):\n\n");
                printf("a = "), fq_zech_print_pretty(a, ctx), printf("\n");
                printf("b = "), fq_zech_print_pretty(b, ctx), printf("\n");
                printf("c = "), fq_zech_print_pretty(b, ctx), printf("\n");
                printf("e = "), fmpz_print(e), printf("\n");
                abort();
            }

            fq_zech_clear(a, ctx);
            fq_zech_clear(b, ctx);
            fq_zech_clear(c, ctx);
            fmpz_clear(e);
            fmpz_clear(f);
        }


        fq_zech_ctx_clear(ctx);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}