# Equation Solver Language

Implementation of a compiler for equations systems.

## Example

Code:
```txt
function newton_to_kg(F)
{
    return F / 9.81 [m s-2]
}

C = 3 [Nm]
P = C * 120[s-1]
F_n = C / 0.2 [m]
F_kg = newton_to_kg(F_n)


x + 2*y + 3*z = 14
3*x - 2*y + 3*z = 8
3*y + 5*z - x = 20
```

Markdown output:

> ## Functions
> $$newton_{to\_kg}(F) {\rArr} \frac{F}{9.81\,{[m.s^{-2}]}}$$
> ## Inferred Equations
> $$\textcolor{green}{C} = 3\,{[Nm]}$$
> $$\textcolor{green}{P} = \textcolor{green}{C} \cdot 120\,{[s^{-1}]}$$
> $$\textcolor{green}{F_{n}} = \frac{\textcolor{green}{C}}{0.2\,{[m]}}$$
> $$\textcolor{green}{F_{kg}} = \textcolor{green}{newton_{to\_kg}}\left(
> \textcolor{green}{F_{n}}\right)$$
> ## Inferred Solutions
> $$\textcolor{green}{C} = 3\,{[Nm]}$$
> $$\textcolor{green}{F_{kg}} = 1.5291\,{[kg]}$$
> $$\textcolor{green}{F_{n}} = 15\,{[N]}$$
> $$\textcolor{green}{P} = 360\,{[W]}$$
> ## System of Equations
> $$\textcolor{red}{x} + 2 \cdot \textcolor{red}{y} + 3 \cdot \textcolor{red}{z} = 14$$
> $$3 \cdot \textcolor{red}{x} - 2 \cdot \textcolor{red}{y} + 3 \cdot \textcolor{red}{z} = 8$$
> $$3 \cdot \textcolor{red}{y} + 5 \cdot \textcolor{red}{z} - \textcolor{red}{x} = 20$$
> ## Solutions
> $$\textcolor{red}{x} = 1$$
> $$\textcolor{red}{y} = 2$$
> $$\textcolor{red}{z} = 3$$

## Usage
```sh
./es system_of_equations.eqs
```

## Features
- Linear and non-linear equations systems
- Equations inferrence
- SI units, derived units, units inferration
- Functions
- Guesses (for better convergence)

## Todo
- Add support for SI units in functions
- Add support for prefix units
- Add support for derived units
- Add support for libraries

## References
- GNU/GSL root finding: https://www.gnu.org/software/gsl/doc/html/multiroots.html
- Units: https://en.wikipedia.org/wiki/International_System_of_Units
