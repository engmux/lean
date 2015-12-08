/*
Copyright (c) 2015 Microsoft Corporation. All rights reserved.
Released under Apache 2.0 license as described in the file LICENSE.

Author: Leonardo de Moura
*/
#pragma once
#include "kernel/environment.h"
#include "library/io_state.h"

#ifndef LEAN_ELIM_DEFAULT_PRIORITY
#define LEAN_ELIM_DEFAULT_PRIORITY 1000
#endif

#ifndef LEAN_INTRO_DEFAULT_PRIORITY
#define LEAN_INTRO_DEFAULT_PRIORITY 1000
#endif

namespace lean {
environment add_elim_lemma(environment const & env, io_state const & ios, name const & c, unsigned prio, name const & ns, bool persistent);
environment add_intro_lemma(environment const & env, io_state const & ios, name const & c, unsigned prio, name const & ns, bool persistent);
bool is_elim_lemma(environment const & env, name const & c);
bool is_intro_lemma(environment const & env, name const & c);
void get_elim_lemmas(environment const & env, buffer<name> & r);
void get_intro_lemmas(environment const & env, buffer<name> & r);
void initialize_intro_elim_lemmas();
void finalize_intro_elim_lemmas();
}
