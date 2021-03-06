/*
    Copyright (C) 2016 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <gmp.h>
#include <stdlib.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_mpoly.h"

int fmpz_mpoly_is_gen(const fmpz_mpoly_t poly,
                                           slong k, const fmpz_mpoly_ctx_t ctx)
{
   int ret = 1;
   slong i;
   ulong * user_exp, total = 0;

   TMP_INIT;

   if (poly->length != 1)
      return 0;

   if (!fmpz_is_one(poly->coeffs + 0))
      return 0;


    TMP_START;
    user_exp = (ulong *) TMP_ALLOC((ctx->minfo->nvars)*sizeof(ulong));
    mpoly_get_monomial_ui(user_exp, poly->exps, poly->bits, ctx->minfo);

    for (i = 0; i < ctx->minfo->nvars; i++)
    {
        total += user_exp[i];
        ret &= total < 2;
    }

    if (k >= 0)
        ret &= (user_exp[k] == 1);
    else
        ret &= (total == 1);

    TMP_END
    return ret;
}
