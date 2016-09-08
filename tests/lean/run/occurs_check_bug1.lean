open nat prod
open decidable

constant modulo1 (x : ℕ) (y : ℕ) : ℕ
infixl `mod`:70 := modulo1

constant gcd_aux : ℕ × ℕ → ℕ

noncomputable definition gcd (x y : ℕ) : ℕ := gcd_aux (x, y)

theorem gcd_def (x y : ℕ) : gcd x y = @ite (y = 0) (nat.has_decidable_eq (pr2 (x, y)) 0) nat x (gcd y (x mod y)) :=
sorry

constant succ_ne_zero (a : nat) : succ a ≠ 0

theorem gcd_succ (m n : ℕ) : gcd m (succ n) = gcd (succ n) (m mod succ n) :=
eq.trans (gcd_def _ _) (if_neg (succ_ne_zero _))
