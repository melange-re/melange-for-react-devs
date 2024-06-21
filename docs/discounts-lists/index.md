# Discounts Using Lists

You've implemented the logic for the burger discounts that will be applied on
International Burger Day, but something feels off. The code inside the
`Discount` module is safe, in the sense that it doesn't raise exceptions, so
when it's used in the app, it won't crash. But the current logic in
`Discount.getFreeBurger` is a little fragile, since you have to remember to not
change the order of function invocations in such a way that the function will
[accidentally change the input array](/burger-discounts/#arrays-are-mutable).
You have a test in `DiscountTests` which tests for that possibility, but you
know that tests are only as reliable as the humans that maintain them. By now,
you've come to realize that The OCaml Way should be to rewrite the function so
that it **cannot** have side effects.

## Introduction to lists

A [list](https://reasonml.github.io/docs/en/basic-structures#list) is a
sequential data structure that can often be used in place of an array. In OCaml,
lists are implemented as [singly-linked
lists](https://en.wikipedia.org/wiki/Linked_list#Singly_linked_list). The main
limitation is that they don't allow constant-time access to any element except
the first one (also known as the *head* of a list). Most operations on lists go
through the [Stdlib.List
module](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/), which has many
functions that are equivalent to the ones you've already used in
[Js.Array](https://melange.re/v4.0.0/api/re/melange/Js/Array/).

::: tip

Since `Stdlib` is opened by default, you can access all `Stdlib.List` functions
using just `List`, e.g. `List.map`, `List.filter`, etc. Because of this, we'll
refer to this module as `List` from now on.

:::

## Refactor `Discount.getFreeBurger`

Let's refactor `Discount.getFreeBurger` to accept `list(Item.t)` instead of
`array(Item.t)`:

<<< Discount.re#get-free-burger

There are a lot of things to talk about in this piece of code---let's go through
them one by one.


::: tip

This refactor makes sense because the logic inside `Discount.getFreeBurger`
doesn't need to access random positions within the sequence of items. If that
was the case, then it would be better to keep using arrays.

:::

## Documentation comment

You might have noticed that the comment at the beginning of the function has a
different format than before:

```reason
// Buy 2 burgers, get 1 free // [!code --]
/** Buy 2 burgers, get 1 free */ // [!code ++]
```

This is a **documentation comment**, a special comment that is attached to the
function it appears above. Go to `DiscountTests` and hover over an invocation of
`getFreeBurger`---the editor will display a popup showing both the type
signature and documentation comment of this function:

![Function info popup](function-popup.png)

::: info

By "editor", we mean an instance of Visual Studio Code that has the [OCaml
Platform
extension](https://marketplace.visualstudio.com/items?itemName=ocamllabs.ocaml-platform)
installed. Other editors that have OCaml support probably have the same (or very
similar) features, but we don't guarantee it.

:::

Documentation comments can also be attached to modules, types, and variables.
Besides showing up in editor popups, they are also consumed by documentation
generators like [odoc](https://ocaml.github.io/odoc/).

## `List` functions

We replaced these functions with their counterparts in the [List
module](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/):

- `Js.Array.filter` →
  [List.filter](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/#val-filter).
  Note that `List.filter` doesn’t accept the labeled argument `~f`, because the
  functions inside `List` don't use labeled arguments.
- `Js.Array.map` →
  [List.map](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/#val-map).
  `List.map` also doesn’t accept the labeled argument `~f`.
- `Js.Array.sortInPlaceWith` →
  [List.sort](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/#val-sort).
  `List.sort` returns a brand new list, because, unlike
`Js.Array.sortInPlaceWith`, it doesn’t modify its argument (and it can’t, since
  lists are immutable).

## Pattern match on lists

The switch expression in `Discount.getFreeBurgers` accepts the entire `prices`
list. Unlike with arrays, we can pattern match on lists even if we don’t know
the length of the list.

Inside the "failure" branch of the switch expression, we see this:

```reason
| []
| [_] => None
```

This pattern matches both empty lists and lists with one element, and returns
`None`.

Inside the "success" branch of the switch expression, we have:

```reason
| [_, cheaperPrice, ..._] => Some(cheaperPrice)
```

This pattern will match on lists that have at least two elements. The first
element is ignored via the wildcard pattern `_`. The second element is bound to
the name `cheaperPrice`, which is encased in `Some` and returned. We use *list
spread syntax* (`...`) to indicate that the list can have more than the two
elements we explicitly matched on.

## List spread syntax

The spread operator (`...`) can also be used to create a new list by prepending
elements to an existing list:

```reason
let list = [1, 2, 3];
Js.log([0, ...list]); // [0, 1, 2, 3]
Js.log([-1, 0, ...list]); // [-1, 0, 1, 2, 3]
```

::: tip

```reason
[1, 2, 3]
```

is really just a shortcut for

```reason
[1, ...[2, ...[3, ...[]]]]
```

:::

When pattern matching, the spread operator allows you to bind the *tail* of
a list to a name:

```reason
switch ([1, 2, 3]) {
| [_, ...tail] => Js.log(tail) // [2, 3]
| _ => ()
};

switch ([1, 2, 3]) {
| [_, _, _, ...tail] => Js.log(tail) // []
| _ => ()
};
```

The tail of the list is the sublist that remains after you extract the first n
elements from the front of the list. As you can see, the tail might be the empty
list (`[]`). In practice, you don't need to bind the tail to a name unless
you're writing a [custom list function](/todo). Often, you'll just bind the tail
to the wildcard pattern `_`, effectively ignoring it:

```reason{2}
switch (["one", "two", "three"]) {
| [a, b, ..._] => Js.log(a ++ ", " ++ b ++ ", etc") // one, two, etc
| _ => ()
};
```

## Runtime representation of lists

Run this snippet in [Melange
Playground](https://melange.re/v4.0.0/playground/?language=Reason&code=SnMubG9nMyhBcnJheS5vZl9saXN0KFtdKSwgIi0%2BIiwgW10pOwpKcy5sb2czKEFycmF5Lm9mX2xpc3QoWzQyXSksICItPiIsIFs0Ml0pOwpKcy5sb2czKEFycmF5Lm9mX2xpc3QoWzQsIDUsIDZdKSwgIi0%2BIiwgWzQsIDUsIDZdKTsK&live=off):

```reason
Js.log3(Array.of_list([]), "->", []);
Js.log3(Array.of_list([42]), "->", [42]);
Js.log3(Array.of_list([4, 5, 6]), "->", [4, 5, 6]);
```

You'll see this output:

```text
[] -> 0
[42] -> {"hd":42,"tl":0}
[4,5,6] -> {"hd":4,"tl":{"hd":5,"tl":{"hd":6,"tl":0}}}
```

The list `[4, 5, 6]` becomes this object in the JS runtime (pretty printed for
readability):

```json
{
  "hd": 4,
  "tl": {
    "hd": 5,
    "tl": {
      "hd": 6,
      "tl": 0
    }
  }
}
```

An empty list in the JS runtime is represented by `0`. A non-empty list is
represented by an object with fields `hd` (for the head) and `tl` (for the
tail).

## Fix tests for `Discount.getFreeBurger`

You should be getting this compilation error for `DiscountTests`:

```text
File "src/order-confirmation/DiscountTests.re", lines 15-19, characters 32-11:
15 | ................................[|
16 |            Hotdog,
17 |            Sandwich(Ham),
18 |            Sandwich(Turducken),
19 |          |]..
Error: This expression has type 'a array
       but an expression was expected of type Item.t list
```

It's simple to fix---just change the delimiters from `[||]` to `[]`:

<<< DiscountTests.re#first-test

The "Input array isn't changed" test can simply be deleted, because lists are
immutable and therefore `Discount.getBurger` can't change its input list.

## Refactor `Discount.getHalfOff`

Let's now refactor `Discount.getHalfOff` to use lists:

<<< Discount.re#get-half-off

Again, we swap out array functions for list functions:

- `Js.Array.some` →
  [List.exists](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/#val-exists).
  Note that this is one of several functions in `List` that have different names
  than their counterparts in `Js.Array`.
- `Js.Array.reduce` →
  [List.fold_left](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/#val-fold_left).
  Despite its name, `fold_left`[^1] has the same meaning as `reduce`.

Remember to fix the `Discount.getHalfOff` tests inside `DiscountTests`, and then
all your code should be compiling once more.

## `ListLabels` module

The call to `List.fold_left` is not as readable as the previous version using
`Js.Array.reduce`, but its readability can be improved by instead using
[ListLabels.fold_left](https://melange.re/v1.0.0/api/re/melange/Stdlib/ListLabels/#val-fold_left):

```reason
let total =
  items
  |> List.fold_left((total, item) => total +. Item.toPrice(item), 0.0); // [!code --]
  |> ListLabels.fold_left(~init=0.0, ~f=(total, item) => // [!code ++]
           total +. Item.toPrice(item) // [!code ++]
         ); // [!code ++]
Some(total /. 2.0);
```

The [ListLabels](https://melange.re/v1.0.0/api/re/melange/Stdlib/ListLabels/)
module has all the functions found in `List`, but many of them take labeled
arguments instead of positional arguments.

## Refactor `Order` component

Because we've gone all in on lists, we have to migrate the component modules as
well. Next up is `Order`. Start off by changing the type of `Order.t` from
`array(Item.t)` to `list(Item.t)`, then refactor `Order.make` accordingly:

<<< Order.re#make

Again, we’re mostly just replacing array functions with list functions:

- `Js.Array.reduce` → `ListLabels.fold_left`
- `Js.Array.mapi` →
  [List.mapi](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/#val-mapi).
  Note that the order of the callback arguments has been reversed. For
  `Js.Array.mapi` it’s `(item, index)`, but for `List.mapi` it’s `(index,
  item)`.
- We have to add a call to
  [Stdlib.Array.of_list](https://melange.re/v2.2.0/api/re/melange/Stdlib/Array/#val-of_list)
  in between the calls to `List.mapi` and `React.array` to convert the list to
  an array. Whenever we want to render a list of `React.element`s, we must first
  convert it to an array of `React.element`s. To understand why, recall that a
  list is just an object in the JS runtime, and React cannot directly render
  objects.

Because `Stdlib` is automatically opened, normally we can just call
`Array.of_list`, but we have to use the full name `Stdlib.Array.of_list` because
our custom `Array` module takes precedence[^2].

To get all your code compiling again, you must also fix the code in
`Index`---but all you have to do in there is change the array delimiters
(`[||]`) to list delimiters (`[]`).

## `List.nth_opt`

If we peruse the `List` module a bit, we'll find a function that can simplify
the logic in `Discount.getFreeBurger`:
[List.nth](https://melange.re/v1.0.0/api/re/melange/Stdlib/List/#val-nth). It
takes an index `n` that returns the `n`-th element of a list. However, from
previous experience, we don't want to use unsafe functions like this.
Fortunately, there's a similar
[List.nth_opt](https://melange.re/v1.0.0/api/re/melange/Stdlib/List/#val-nth_opt)
function that does the same thing but is safer because it returns `option`
instead of raising an exception. Let's refactor `Discount.getFreeBurger` to use
it:

```reason
let getFreeBurger = (items: list(Item.t)) => {
  items
  |> List.filter(item =>
       switch (item) {
       | Item.Burger(_) => true
       | Sandwich(_)
       | Hotdog => false
       }
     )
  |> List.map(Item.toPrice)
  |> List.sort((x, y) => - compare(x, y))
  |> List.nth_opt(1);
};
```

By using `List.nth_opt`, we can simplify the function to a single expression.
However, we get a compilation error:

```text
File "docs/order-confirmation/Discount.re", line 61, characters 18-19:
61 |   |> List.nth_opt(1);
                       ^
Error: This expression has type int but an expression was expected of type
         'a list
```

## Placeholder operator

This is because the type signature of `List.nth_opt` is

```text
list('a) => int => option('a)
```

That is, it accepts the list as the first argument, not the last. Recall that
the pipe last operator (`|>`) pipes values into the last argument of a function.
However, there's a way to override the placement of the argument:

```reason
|> List.nth_opt(_, 1)
```

Do not confuse the `_` here for wildcard, here it's a placeholder for where the
argument should go. When we put `_` in the first argument position, it overrides
the default behavior of the pipe last operator. Run `npm run test` to confirm
that `Discount.getFreeBurger` works the same as before.

# `List.nth_opt` is unsafe

The problem with using `List.nth_opt` is that it can still raise an
`Invalid_argument` exception if the value of `n` is negative. You can confirm
this by hovering over `nth_opt` and reading the popup or [read its
documentation](https://melange.re/v1.0.0/api/re/melange/Stdlib/List/#val-nth_opt).
While this is unlikely to cause a problem inside `Discount.getFreeBurger`, it's
best to avoid unsafe functions except in special circumstances, for example if a
function needs to be as fast as possible. Of course, it's possible to implement
a completely safe version of `nth_opt`, which is an exercise at the end of this
chapter.

---

Mazel tov! You've implemented the burger discounts in a way that is more
maintainable, and you've also learned a lot about lists along the way. In the
next chapter, we'll finally use the discount logic to reduce the final price of
an order.

## Overview

- Lists are immutable
- You can pattern match on a whole list, even if you don't know its length
- Uses of list spread syntax (`...`):
  - Create new lists by prepending elements to existing lists
  - Pattern match on the tail of a list
- The delimiters for list literals are `[]`
- The `List` module contains most of the functions you'll need for
  dealing with lists
- The `ListLabels` module contains the same functions as in `List`, but they
  have labeled arguments instead of positional arguments
- The names of equivalent functions in `List` and `Js.Array` might not match
- The runtime representation of lists:
  - Empty list → `0`
  - Nonempty list → a JavaScript object with the fields `hd` (for head) and `tl`
    (for tail)
- Documentation comments:
  - Show up in editor hover popups and generated
  documentation pages
  - Can be attached to functions, modules, types, and variables
- The placeholder operator (`_`) can be used to override the position of the
  piped argument when using the pipe last operator

## Exercises

<b>1.</b> There are a couple ways to improve `Discount.getFreeBurger`:

- Instead of `StdLib.compare`, use a type-specific compare function to make the
  code less brittle. Examples of type-specific compare functions are
  `Bool.compare` and `String.compare`.
- Use
  [List.filter_map](https://melange.re/v2.2.0/api/re/melange/Stdlib/List/#val-filter_map)
  in place of `List.filter` and `List.map`.

::: details Solution

<<< Discount.re#get-free-burger-improved

Prefer type-specific compare functions like
[Float.compare](https://melange.re/v1.0.0/api/re/melange/Stdlib/Float/#val-compare)
over polymorphic `Stdlib.compare`. While `Stdlib.compare` can handle any type,
its flexibility comes with drawbacks. It can be slower due to its polymorphic
nature, and might not always offer meaningful comparisons for complex types.
Additionally, it can raise exceptions if used on non-comparable types like
functions.

:::

<b>2.</b> Add a `ListSafe.nth` function which safely returns the nth element of
a list encased in `Some`. If the nth element doesn’t exist, return `None`. Make
sure the function can be used with the pipe last operator without the use of the
placeholder operator. Refactor `Discount.getFreeBurger` to use your new
function.

::: details Solution

Add a new file `ListSafe.re`:

<<< ListSafe.re

`Discount.getFreeBurger` could be refactored to:

<<< Discount.re#get-free-burger-nth{12}

Note that we no longer need the switch expression anymore because
`ListSafe.nth(1)` will automatically return `None` if there are 0 or 1 items in
the given list.

:::

<b>3.</b> Update the logic of `Discount.getFreeBurger` so that for every pair of
burgers purchased, one of them is free. Order the burgers by price (descending),
and then choose every other burger (starting from the second burger) to be free.
Add this new test to `DiscountTests` and make sure it passes:

<<< DiscountTests.re#get-free-burgers-test

::: details Hint 1

Use
[List.filteri](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/#val-filteri)
and [mod operator](https://ocaml.org/manual/5.2/expr.html#ss:expr-operators).

:::

::: details Hint 2

Use
[ListLabels.fold_left](https://melange.re/v4.0.0/api/re/melange/Stdlib/ListLabels/#val-fold_left)
or [List.fold_left](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/#val-fold_left)

:::

::: details Solution

<<< Discount.re#get-free-burgers

A few points about this solution:
- `getFreeBurger` is renamed to `getFreeBurgers` to reflect the fact that
multiple burgers can be free.
- The callback passed to `List.fold_left` is just the float addition operator
  (`+.`).
- The switch expression reappears because it's needed to detect the cases when
  the discount can't be applied (when there are less than two burgers in the
  order).

:::

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/discounts-lists/)
and [demo](https://react-book.melange.re/demo/src/discounts-lists/) for this chapter.

-----

[^1]: Inside `List`, there are `fold_left` and `fold_right` functions. "Fold
    left" means to apply the given fold function starting from the first element
    and work towards the end of the list, while "fold right" starts from the
    last element and works backwards towards the front of the list.

[^2]: A quick fix to allow you to write `Array.of_list` would be to add a
    function alias in `Array`:

    ```reason
    /** Convert list to array */
    let of_list = Stdlib.Array.of_list;
    ```
