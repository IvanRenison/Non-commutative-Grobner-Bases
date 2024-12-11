# Computation of non commutative Gröbner basis.

This was my degree thesis project. It consist of a library for computing Gröbner basis in the free algebra.

In tha `ncgb` folder you can find the library, everything is in `.h` files because we use templates to make the code generic.

In the `mains` folder you can find several mains that use the library.

There are two formats for writing the non commutative polynomials in text files, one only with numbers, easy to read from code, and the other nice to read by humans.

The only number format is like this:
```
m
c_1 n_1 x_{1, 1} ⋯ x_{1, n_1}
⋮
c_m n_m x_{m, 1} ⋯ x_{m, n_m}
```

Where c_i are element of the field, and x_{i, j} are the variables, represented as numbers.
