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


# Problem: Number of Ways to Get a Sum of 10 with Two Dice Rolls

We are asked to find the number of ways we can roll a **standard 6-sided die twice** such that the sum of the two rolls equals **10**. We will solve this using the **Stars and Bars** method along with **Inclusion-Exclusion**.

### To solve this, let the two dice rolls be denoted as `x_1` and `x_2`, where: 
`x_1 + x_2 = 10`
   
and

`1 <= x_1, x_2 <= 6  `


This implies we are looking for the number of integer solutions to the equation above, with the constraints that both `x_1` and `x_2` are between 1 and 6.

---

### Step 1: Total Number of Solutions (Without Constraints)

First, let's consider the number of solutions to the equation `x_1 + x_2 = 10`, where both `x_1` and `x_2` are greater than or equal to 1. To handle this, we can transform the variables as follows:

`y_1 = x_1 - 1 and y_2 = x_2 - 1` 


Now, `y_1, y_2 >= 0`, and the equation becomes:


The number of solutions to this equation is `8 + 1 = 9`, which corresponds to the following valid pairs:

- (1, 9)
- (2, 8)
- (3, 7)
- (4, 6)
- (5, 5)
- (6, 4)
- (7, 3)
- (8, 2)
- (9, 1)

---

### Step 2: Applying Inclusion-Exclusion

Next, we need to subtract the cases where either `x_1 > 6` or `x_2 > 6` because both dice rolls must fall between 1 and 6.

- **Case 1: When `x_1 > 6`**  
  If `x_1 > 6`, then `x_1 >= 7`, and thus `x_2 = 10 - x_1` must be less than or equal to 3. The valid pairs are:

  - (7, 3)
  - (8, 2)
  - (9, 1)

  This gives 3 solutions.

- **Case 2: When `x_2 > 6`**  
  Similarly, if `x_2 > 6`, the valid pairs are:

  - (3, 7)
  - (2, 8)
  - (1, 9)

  This also gives 3 solutions.

---

### Step 3: Final Count

Now, we subtract the 6 invalid cases from the total number of solutions:

9 - 6 = 3
Thus, the number of valid solutions is 3.

The valid pairs (where the sum is 10) are:

- (4, 6)
- (5, 5)
- (6, 4)

---

### Conclusion

There are **3 ways** to roll a sum of 10 with two dice rolls




