# Computation of non commutative Gröbner basis.

This was my degree thesis project. It consist of a library for computing Gröbner basis in the free algebra.

In tha `ncgb` folder you can find the library, everything is in `.hpp` files because we use templates to make the code generic.

In the `mains` folder you can find several mains that use the library.

## Reading polynomials

There are two formats for writing the non commutative polynomials in text files, one only with numbers, easy to read from code, and the other nice to read by humans.

The only number format is like this:
```
m
c_1 n_1 x_{1, 1} ⋯ x_{1, n_1}
⋮
c_m n_m x_{m, 1} ⋯ x_{m, n_m}
```

Where c_i are element of the field, and x_{i, j} are the variables, represented as numbers.

## Generic code

Most of the code if generic taking two argument, the field `K` and a monomial ordering `ord` witch is set by default to `DegLexOrd`.

The field `K` must have all field operations defined, including constructors that work with `0` and `1`.

## Compiling

See the `Makefile` to get an idea of how to compile. No libraries are strictly required for the `hpp` files, but for modular arithmetic it's recommended to have `givaro`, and for rational numbers `gmp` and `flint`. Also, por parallelization you will need `openmp`.
