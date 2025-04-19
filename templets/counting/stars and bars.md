# Problem: Integer Solutions with Parity Restriction

We are asked to find the number of ordered tuples (a, b, c, d, e) of **positive integers** such that:

    a + b + c + d + e = 26

with the additional condition that **exactly 4 of the variables are odd**.

---

## Step 1: Stars and Bars Review

The classic "stars and bars" method is used to count how many ways we can write:

    x1 + x2 + ... + xn = k

### Cases:
- If variables are **non-negative integers** (can be 0), the number of solutions is:

      C(k + n - 1, n - 1)

- If variables are **positive integers** (≥ 1), then:

      C(k - 1, n - 1)

---

## Step 2: Applying Parity Constraint

We want **exactly 4 of the 5 variables** to be **odd**.

To force a variable to be odd, write it as:

    odd = 2x + 1
    even = 2x

Let’s assume variables a, b, c, d are odd, and e is even. We substitute:

    a = 2a' + 1
    b = 2b' + 1
    c = 2c' + 1
    d = 2d' + 1
    e = 2e'

Substitute into the original equation:

    (2a'+1) + (2b'+1) + (2c'+1) + (2d'+1) + (2e') = 26

Simplify:

    2(a' + b' + c' + d' + e') + 4 = 26
    => a' + b' + c' + d' + e' = 11

---

## Step 3: Count Solutions to the Transformed Equation

We now solve:

    a' + b' + c' + d' + e' = 11

with all variables ≥ 0 (non-negative integers).

Using stars and bars:

    C(11 + 5 - 1, 5 - 1) = C(15, 4) = 1365

---

## Step 4: Account for All Possible Odd Variable Choices

We assumed a, b, c, d are the odd ones. But there are:

    C(5, 4) = 5

ways to choose which 4 variables are odd.

---

## Final Answer:

    5 * 1365 = 6825

So, there are **6825** such ordered tuples.
