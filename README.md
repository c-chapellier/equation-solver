# equation-solver
Equation solving language

## Usage
```sh
./es equation_system.eqs
```

## Example 1

This equation system
```txt
y = x - 1
2*x + y = 1
```

Will produce this markdown document:

> ## Equations
> $$y = x - 1$$
> $$2 \cdot x + y = 1$$
> ## Solution
> $$y = -0.333333$$
> $$x = 0.666667$$

## Example 2

This equation system
```txt
function square_area(r)
{
    return pi * r^2
}

a = square_area(b)
sqrt(a) = b + c
c / a = b
```

Will produce this markdown document:

> ## Functions
> $$sqrt(x) \rArr \sqrt{x}$$
> $$square_{area}(r) \rArr \pi  \cdot r^{2}$$
> ## Equations
> $$a = square_{area}\left(
> b\right)$$
> $$\sqrt{a} = b + c$$
> $$\frac{c}{a} = b$$
> ## Solution
> $$a = 0.772454$$
> $$b = 0.495863$$
> $$c = 0.383031$$

## Features
* Solve equations systems
* Define functions
* Choose variable initial guess

## Todo
* Add support for SI units in functions
* Add support for prefix units
* Add support for derived units
* Add support for libraries

https://en.wikipedia.org/wiki/International_System_of_Units
