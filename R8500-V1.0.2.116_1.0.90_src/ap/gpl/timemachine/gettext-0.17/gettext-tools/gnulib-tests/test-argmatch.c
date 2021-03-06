/* Test of exact or abbreviated match search.
   Copyright (C) 1990, 1998-1999, 2001-2007 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* Written by Bruno Haible <bruno@clisp.org>, 2007, based on test code
   by David MacKenzie <djm@gnu.ai.mit.edu>.  */

#include <config.h>

#include "argmatch.h"

#include <stdio.h>
#include <stdlib.h>

#include "progname.h"

#define ASSERT(expr) \
  do									     \
    {									     \
      if (!(expr))							     \
        {								     \
          fprintf (stderr, "%s:%d: assertion failed\n", __FILE__, __LINE__); \
          abort ();							     \
        }								     \
    }									     \
  while (0)

enum backup_type
{
  no_backups,
  simple_backups,
  numbered_existing_backups,
  numbered_backups
};

static const char *const backup_args[] =
{
  "no", "none", "off",
  "simple", "never", "single",
  "existing", "nil", "numbered-existing",
  "numbered", "t", "newstyle",
  NULL
};

static const enum backup_type backup_vals[] =
{
  no_backups, no_backups, no_backups,
  simple_backups, simple_backups, simple_backups,
  numbered_existing_backups, numbered_existing_backups, numbered_existing_backups,
  numbered_backups, numbered_backups, numbered_backups
};

int
main (int argc, char *argv[])
{
  set_program_name (argv[0]);

  /* Not found.  */
  ASSERT (ARGMATCH ("klingon", backup_args, backup_vals) == -1);

  /* Exact match.  */
  ASSERT (ARGMATCH ("none", backup_args, backup_vals) == 1);
  ASSERT (ARGMATCH ("nil", backup_args, backup_vals) == 7);

  /* Too long.  */
  ASSERT (ARGMATCH ("nilpotent", backup_args, backup_vals) == -1);

  /* Abbreviated.  */
  ASSERT (ARGMATCH ("simpl", backup_args, backup_vals) == 3);
  ASSERT (ARGMATCH ("simp", backup_args, backup_vals) == 3);
  ASSERT (ARGMATCH ("sim", backup_args, backup_vals) == 3);

  /* Exact match and abbreviated.  */
  ASSERT (ARGMATCH ("numbered", backup_args, backup_vals) == 9);
  ASSERT (ARGMATCH ("numbere", backup_args, backup_vals) == -2);
  ASSERT (ARGMATCH ("number", backup_args, backup_vals) == -2);
  ASSERT (ARGMATCH ("numbe", backup_args, backup_vals) == -2);
  ASSERT (ARGMATCH ("numb", backup_args, backup_vals) == -2);
  ASSERT (ARGMATCH ("num", backup_args, backup_vals) == -2);
  ASSERT (ARGMATCH ("nu", backup_args, backup_vals) == -2);
  ASSERT (ARGMATCH ("n", backup_args, backup_vals) == -2);

  /* Ambiguous abbreviated.  */
  ASSERT (ARGMATCH ("ne", backup_args, backup_vals) == -2);

  /* Ambiguous abbreviated, but same value.  */
  ASSERT (ARGMATCH ("si", backup_args, backup_vals) == 3);
  ASSERT (ARGMATCH ("s", backup_args, backup_vals) == 3);

  return 0;
}
