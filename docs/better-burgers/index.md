# Better Burgers

Cafe Emoji is still the hottest restaurant in town, despite the fact that all
burgers are exactly the same. Madame Jellobutter isn't one to rest on her
laurels, though, so she decides to kick it up a notch by allowing customers to
choose the toppings on their burgers.

Currently, your `Item.t` variant type looks something like this:

<<< Item.re#starting-item-t

## `burger` record type

Add a new `burger` *record* type and then add that new type as an argument for
the `Burger` constructor of `Item.t`:

<<< Item.re#add-burger-type{1-5,9}

The fields in the `burger` record are:

| Name | Type | Meaning |
| ---- | ---- | ------- |
| `lettuce` | `bool` | if `true`, include lettuce |
| `onions` | `int` | the number of onion slices |
| `cheese` | `int` | the number of cheese slices |

Records are similar to the [`Js.t` objects we've seen
before](/celsius-converter-exception/#js-t-object), in that they both group a
collection of values into a single object with named fields. However, there are
a number of syntactic and practical differences between them:

- Record fields must be predefined
- Records use `.` to access fields, while `Js.t` objects use `##`
- Records can be destructured and pattern matched
- Records use [nominal
  typing](https://en.wikipedia.org/wiki/Nominal_type_system), while `Js.t`
  objects use [structural
  typing](https://en.wikipedia.org/wiki/Structural_type_system). This means that
  two record types with exactly the same fields are still considered different
  types.

The runtime representation of a record is a [plain JavaScript
object](https://melange.re/v3.0.0/communicate-with-javascript/#data-types-and-runtime-representation),
the same as for a `Js.t` object.

::: info

It might be better to give more semantic names to the record fields, such as
`hasLettuce` and `onionCount`, but we've kept the names short in order to make
the code listings shorter.

:::

## Update `Item.toEmoji` function

Depending on what toppings were chosen and how many of toppings there are, we
want to show different emoji:

| `item` | `Item.toEmoji(item)` |
| ------ | ------------------------------ |
| `Burger({lettuce: true, onions: 1, cheese: 2})` | <span v-pre>🍔{🥬,🧅×1,🧀×2}</span> |
| `Burger({lettuce: false, onions: 0, cheese: 0})` | <span v-pre>🍔{🧅×0,🧀×0}</span> |

To support this logic, we need to add a `| Burger(burger)` branch to the `fun`
expression inside of `Item.toEmoji`:

<<< Item.re#to-emoji{14-24}

Note the use of `Js.Array.filter` to filter out empty strings in the array.

## Destructuring records

It's not necessary to write `burger.*` repeatedly, OCaml gives us a nice syntax
for destructuring records:

<<< Item.re#destructure-burger{2,6-8}

Even better would be to do the destructuring directly inside the `| Burger(_)`
branch:

<<< Item.re#destructure-burger-branch{1}

There is a little bit of redundancy to the logic for displaying onion and cheese
toppings, which we can remedy by adding a small helper function called
`multiple`:

<<< Item.re#multiple{2-3,9-10}

## `Burger` submodule

The `Item.toEmoji` function is arguably getting a bit large and could stand to
be broken up into smaller functions. The most straightforward way to do it is to
add a new function called `Item.toBurgerEmoji` and call it from within
`Item.toEmoji`. However, in OCaml, we have another choice: create a new
submodule called `Burger` and put all the types and functions related to burgers
inside of it:

<<< Item.re#burger-submodule

Note that we renamed the `burger` type to `t` since, by convention, the
primary type of a module is called `t`. Also, we put the destructuring of the
burger record in the argument list of the `Burger.toEmoji` function.

In order to get everything to compile again, we'll need to also update the
definitions for `Item.t`, `Item.toEmoji`, and `Item.toPrice`:

<<< Item.re#t-and-functions{3,9,29}

## Update `Item.Burger.toPrice`

Let's update `Item.Burger.toPrice` so that burgers with more toppings cost more.
Specifically, each onion slice costs 20 cents and each cheese slice costs 10
cents:

```reason
let toPrice = ({onions, cheese, lettuce}) => {
  15.  // base cost
  +. float_of_int(onions)
  *. 0.2
  +. float_of_int(cheese)
  *. 0.1;
};
```

You get an error from Melange:

```
File "src/better-burgers/Item.re", line 24, characters 34-41:
24 |   let toPrice = ({onions, cheese, lettuce}) => {
                                       ^^^^^^^
Error (warning 27 [unused-var-strict]): unused variable lettuce.
```

Since Madame Jellobutter doesn't want to charge for lettuce, we don't need the
value of the `lettuce` variable and remove it:

```reason
let toPrice = ({onions, cheese}) => {
  15.  // base cost
  +. float_of_int(onions)
  *. 0.2
  +. float_of_int(cheese)
  *. 0.1;
};
```

However, this results in a different error:

```
File "src/better-burgers/Item.re", line 24, characters 17-33:
24 |   let toPrice = ({onions, cheese}) => {
                      ^^^^^^^^^^^^^^^^
Error (warning 9 [missing-record-field-pattern]): the following labels are not bound in this record pattern:
lettuce
Either bind these labels explicitly or add '; _' to the pattern.
```

OCaml doesn't like that you left the `lettuce` field out of the pattern match.
We could take the advice of the error message and add a wildcard to the end of
the record pattern match:

<<< Item.re#record-wildcard

But this makes our code less future-proof. For example, what if you added a
`bacon` field to the `Burger.t` record and it was expected that adding bacon
increases the price? With the wildcard in place, the compiler would not tell you
to update the `Burger.toPrice` function. A better way would be to match on
`lettuce` but put a wildcard for its value:

<<< Item.re#record-lettuce-wildcard

This tells the compiler that you expect a `lettuce` field but you don't intend
to use its value inside the function. Similarly to how it's preferable to match
on all constructors of a variant type, it's also a good idea to match on all
fields of a record type.

## Pattern matching records

Right now, if a customer doesn't add any toppings to a burger,
`Item.Burger.toPrice` will return `🍔{🧅×0, 🧀×0}`. It would be better if it
just returned `🍔` to indicate that it's a burger without embellishments. One
way to handle this is to use a ternary expression:

<<< Item.re#ternary{5-7}

Another approach is to pattern match on the tuple of `(lettuce, onions,
cheese)`:

<<< Item.re#match-tuple{5-7}

However, pattern matching on tuples of more than 2 elements tends to be hard to
read; it can even be error-prone when some of the elements of the tuple are of
the same type. For example, what if you accidentally changed the positions of
the `onions` and `cheese` variables in the second branch:

```reason{1,3}
switch (lettuce, onions, cheese) {
| (false, 0, 0) => {js|🍔|js}
| (lettuce, cheese, onions) =>
```

The compiler wouldn't complain but the ensuing logic would likely be wrong.

The best approach here is to use the record itself as the input to the switch
expression:

<<< Item.re#match-record{6-7}

Note that destructuring of the record has been moved from the argument list to
the branches of the switch expression. Now `Item.Burger.toEmoji` gets the name
`t` for its single argument.

---

Magnifique! The order confirmation widget now supports burgers with different
toppings. In the next chapter, we'll start writing tests for our code.

## Exercises

<b>1.</b> Inside `Item.re`, create another submodule for sandwich-related types
and functions.

<b>2.</b> Add `tomatoes: bool` and `bacon: int` fields to `Item.Burger.t`. Let’s
say that adding tomatoes costs $0.05 and each piece of bacon[^1] costs $0.5.

<b>3.</b> Make `Item.Burger.toPrice` function more readable by writing a helper
function that calculates the cost of a topping by multiplying its price with its
quantity.

```reason
let toPrice = ({onions, cheese, tomato, bacon, lettuce: _}) => {
  let toppingCost = /* write me */;

  15.  // base cost
  +. toppingCost(onions, 0.2)
  +. toppingCost(cheese, 0.1)
  /* some stuff involving tomato and bacon */;
};
```

<b>4.</b> Right now, the `Item.Burger.toEmoji` function shows more emojis than
absolutely necessary. Refactor the `multiple` inner function in `Burger.toEmoji`
so that it exhibits the following behavior:

| `item` | `Item.Burger.toEmoji(item)` |
| ------ | --------------- |
| `Burger({lettuce: true, onions: 1, cheese: 1})` | `🍔{🥬,🧅,🧀}` |
| `Burger({lettuce: true, onions: 0, cheese: 0})` | `🍔{🥬}` |

::: details Hint

Use a switch expression.

:::

## Overview

- Record types are like `Js.t` objects but their fields must be explicitly
  defined
  - Records and `Js.t` objects are both JavaScript objects during runtime
- You can get the fields out of a record using destructuring and pattern
  matching:
  - `let` destructuring:
    ```reason
    let {a, b, c} = record;
    ```
  - Switch expression branch pattern matching:
    ```reason
    switch (record) {
    | {a: "foo", b: "bar", c: 42} => "Magnifique!"
    | {a, b, c} => a ++ b ++ string_of_int(c)
    };
    ```
- It's common practice to group related types and functions into a submodule
- Try not to ignore record fields when pattern matching on records. Instead of
  ```reason
  | {a, b, _} =>
  ```

  Prefer

  ```reason
  | {a, b, c: _, d: _} =>
  ```
  Assuming `c` and `d` aren't used inside the branch.
- Try not to pattern match on tuples of more than 2 elements because it tends to
  be hard to read

## Solutions

<b>1.</b> The `Item.Sandwich` submodule should look something like this:

<<< Item.re#sandwich-module

You also need to refactor `Item.toPrice` and `Item.toEmoji` functions to use the
new functions in `Item.Sandwich`.

<b>2.</b> After adding `tomatoes` and `bacon` fields to `Item.Burger.t`, the
changes to `Item.Burger.toEmoji` are fairly mechanical, so let's focus on
`Item.Burger.toPrice`:

<<< Item.re#to-price-tomato-bacon

Note that we keep `lettuce: _` at the end of the pattern match since the value
of `lettuce` isn't ever used.

<b>3.</b> The `toppingCost` helper function inside `Burger.toPrice` should look
something like this:

<<< Item.re#to-price-topping-cost

<b>4.</b> After refactoring the `multiple` helper function inside
`Item.Burger.toEmoji` to avoid showing unnecessary emojis, it should look
something like this:

<<< Item.re#to-emoji-multiple

Since the body of this function only consists of a switch expression, you can
actually refactor the `multiple` function to use the `fun` syntax:

<<< Item.re#to-emoji-multiple-fun

This might look a little strange, but in OCaml, all functions take exactly one
argument. What looks like a two-argument function is actually a one-argument
function that returns another one-argument function[^2]. Manually adding a type
annotation to the function makes it more clear that it's entirely equivalent to
the version that uses a switch expression:

<<< Item.re#to-emoji-multiple-fun-annotated

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/better-burgers/)
and [demo](https://react-book.melange.re/demo/src/better-burgers/) for this chapter.

-----

[^1]: Of course, it's not just any kind of bacon. It's unicorn bacon. By the
    way, the tomatoes are also magically delicious, because they're grown in a
    special field fertilized by unicorn manure.

[^2]: A simpler example that illustrates how function arguments work in OCaml
    would be:

    ```reason
    let add = (x, y, z) => x + y + z;
    // The above is an easier-to-read version of this:
    let explicitAdd = x => y => z => x + y + z;
    ```

    See this [playground
    snippet](https://melange.re/v3.0.0/playground/?language=Reason&code=Ly8gVXNlIGEgcmFuZG9tIHZhcmlhYmxlIHNvIGZ1bmN0aW9uIGludm9jYXRpb25zIGFyZW4ndCBvcHRpbWl6ZWQgYXdheQpsZXQgYyA9IFJhbmRvbS5pbnQoMTApOwoKbGV0IGFkZCA9ICh4LCB5LCB6KSA9PiB4ICsgeSArIHo7CkpzLmxvZyhhZGQoMSwgMiwgYykpOwoKLy8gQW4gZXF1aXZhbGVudCBkZWZpbml0aW9uIHRoYXQgZXhwbGljaXRseSByZXR1cm5zIGZ1bmN0aW9uczoKbGV0IGV4cGxpY2l0QWRkID0geCA9PiB5ID0%2BIHogPT4geCArIHkgKyB6OwpKcy5sb2coZXhwbGljaXRBZGQoMSwgMiwgYykpOwovLyBDb25jZXB0dWFsbHksIHRoZXJlIGFyZSBtdWx0aXBsZSBmdW5jdGlvbiBpbnZvY2F0aW9ucy4gQnV0IGluIHRoZSBKUyBvdXRwdXQsCi8vIGl0J3MgYSBzaW5nbGUgZnVuY3Rpb24gY2FsbC4KSnMubG9nKGV4cGxpY2l0QWRkKDEpKDIpKGMpKTs%3D&live=off)
    for an extended example and the [official OCaml
    docs](https://ocaml.org/docs/values-and-functions#types-of-functions-of-multiple-parameters)
    for more details.
