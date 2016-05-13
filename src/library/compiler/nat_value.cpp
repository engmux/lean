/*
Copyright (c) 2016 Microsoft Corporation. All rights reserved.
Released under Apache 2.0 license as described in the file LICENSE.

Author: Leonardo de Moura
*/
#include "util/numerics/mpz.h"
#include "kernel/expr.h"
#include "library/constants.h"
#include "library/num.h"
#include "library/compiler/compiler_step_visitor.h"

namespace lean {
static expr * g_nat        = nullptr;
static name * g_nat_macro  = nullptr;

/** \brief Auxiliary macro used during compilation */
class nat_value_macro : public macro_definition_cell {
    mpz m_value;
public:
    nat_value_macro(mpz const & v):m_value(v) {}
    virtual bool lt(macro_definition_cell const & d) const {
        return m_value < static_cast<nat_value_macro const &>(d).m_value;
    }
    virtual name get_name() const { return *g_nat_macro; }
    virtual expr check_type(expr const &, abstract_type_context &, bool) const {
        return *g_nat;
    }
    virtual optional<expr> expand(expr const &, abstract_type_context &) const {
        return none_expr();
    }
    virtual unsigned trust_level() const { return 0; }
    virtual bool operator==(macro_definition_cell const & other) const {
        nat_value_macro const * other_ptr = dynamic_cast<nat_value_macro const *>(&other);
        return other_ptr && m_value == other_ptr->m_value;
    }
    virtual void display(std::ostream & out) const {
        out << m_value;
    }
    virtual bool is_atomic_pp(bool, bool) const { return true; }
    virtual unsigned hash() const { return m_value.hash(); }
    virtual void write(serializer &) const { lean_unreachable(); }
    mpz const & get_value() const { return m_value; }
};

expr mk_nat_value(mpz const & v) {
    return mk_macro(macro_definition(new nat_value_macro(v)));
}

bool is_nat_value(expr const & e) {
    return is_macro(e) && dynamic_cast<nat_value_macro const *>(macro_def(e).raw()) != nullptr;
}

mpz const & get_nat_value_value(expr const & e) {
    lean_assert(is_nat_value(e));
    return static_cast<nat_value_macro const *>(macro_def(e).raw())->get_value();
}

class find_nat_values_fn : public compiler_step_visitor {
    expr visit_app(expr const & e) override {
        if (optional<mpz> v = to_num(e)) {
            expr type = ctx().whnf(ctx().infer(e));
            if (is_constant(type, get_nat_name())) {
                return mk_nat_value(*v);
            }
        }
        return compiler_step_visitor::visit_app(e);
    }
public:
    find_nat_values_fn(environment const & env):compiler_step_visitor(env) {}
};

expr find_nat_values(environment const & env, expr const & e) {
    return find_nat_values_fn(env)(e);
}

void initialize_nat_value() {
    g_nat_macro  = new name("nat_value_macro");
    g_nat        = new expr(Const(get_nat_name()));
}

void finalize_nat_value() {
    delete g_nat_macro;
    delete g_nat;
}
}