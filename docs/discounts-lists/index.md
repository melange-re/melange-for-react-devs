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
you've come to realize that The OCaml Way should be rewrite the function so that
it **cannot** have side effects.

## Intro to lists

A [list](https://reasonml.github.io/docs/en/basic-structures#list) is a
sequential data structure that can often be used in place of an array. In OCaml,
lists are implemented as [singly-linked
lists](https://en.wikipedia.org/wiki/Linked_list#Singly_linked_list). The main
limitation is that they don't allow constant-time access to any element except
the first one (also known as the *head* of a list). Most operations on lists go
through the
[Stdlib.List](https://melange.re/v3.0.0/api/re/melange/Stdlib/List/), which has
many functions that are equivalent to the ones you've already used in
[Js.Array](https://melange.re/v3.0.0/api/re/melange/Js/Array/).

## Refactor `Discount.getFreeBurger`

Let's refactor `Discount.getFreeBurger` to accept `list(Item.t)` instead of
`array(Item.t)`:

<<< Discount.re#get-free-burger

There are a lot of things to talk about in this piece of code--let's go through
them one by one.

## Documentation comment

You might have noticed that the comment at the beginning of the function has a
different format than before:

```reason
/** Buy 2 burgers, get 1 free */
```

This is a **documentation comments**, an enhanced comment that is attached to
the function it appears above. Go to `DiscountTests` and hover over an
invocation of `getFreeBurger`---the editor will show both the type signature and
documentation comment of this function.

::: info

By "editor", we mean an instance of Visual Studio Code that has the [OCaml
Platform
extension](https://marketplace.visualstudio.com/items?itemName=ocamllabs.ocaml-platform)
installed. Other editors that have OCaml support probably have similar features
to the ones we describe in this chapter, but we don't guarantee it.

:::

Documentation comments can also be attached to modules, types, and variables.
Besides showing up in editor popups, they are also consumed by documentation
generators like [odoc](https://ocaml.github.io/odoc/).

## `List` functions

We replaced these functions with their counterparts in the [Stdlib.List
module](https://melange.re/v3.0.0/api/re/melange/Stdlib/List/):

- `Js.Array.filter` →
  [List.filter](https://melange.re/v3.0.0/api/re/melange/Stdlib/List/#val-filter).
  Note that `List.filter` doesn’t accept the labeled argument `~f`, because the
  functions inside `List` don't use labeled arguments.
- `Js.Array.map` →
  [List.map](https://melange.re/v3.0.0/api/re/melange/Stdlib/List/#val-map).
  `List.map` also doesn’t accept the labeled argument `~f`.
- `Js.Array.sortInPlaceWith` →
  [List.sort](https://melange.re/v3.0.0/api/re/melange/Stdlib/List/#val-sort).
  `List.sort` returns a brand new list, because, unlike
`Js.Array.sortInPlaceWith`, it doesn’t modify its argument (and it can’t, since
  lists are immutable).

## Pattern match on lists

The switch expression in `Discount.getFreeBurgers` accepts the entire `prices`
list. Unlike with arrays, we can pattern match on lists even if we don’t know
the length of the list.

Inside the first branch of the switch expression, we see this:

```reason
| [_, cheaperPrice, ..._] => Some(cheaperPrice)
```

This pattern will match on lists that have at least two elements. The first
element is ignored via `_`. The second element is bound to the name
`cheaperPrice` and used in the body of the branch. We use *list spread syntax*
(`...`) to bind the sublist containing the remaining elements to a name, but
here we bind it to `_`, effectively ignoring it.

## List spread syntax

The spread operator (`...`) is also used to add elements to the front of a list:

```reason
let list = [1, 2, 3];
Js.log([0, ...list]); // [0, 1, 2, 3]
Js.log([-1, 0, ...list]); // [-1, 0, 1, 2, 3]
```

When pattern matching, the spread operator allows you to bind the **tail** of
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
you're writing a [custom list function](/todo).

::: warning

The syntax for list literals is identical to that for arrays in JavaScript, but
keep in mind that

```reason
[1, 2, 3]
```

is really just a shortcut for

```reason
[1, ...[2, ...[3, ...[]]]]
```

:::

## Runtime representation of lists

If you run the code snippets above in the [Melange
Playground](https://melange.re/v3.0.0/playground/?language=Reason&code=bGV0IGxpc3QgPSBbMSwgMiwgM107CkpzLmxvZyhbMCwgLi4ubGlzdF0pOyAvLyBbMCwgMSwgMiwgM10KSnMubG9nKFstMSwgMCwgLi4ubGlzdF0pOyAvLyBbLTEsIDAsIDEsIDIsIDNdCgpzd2l0Y2ggKFsxLCAyLCAzXSkgewp8IFsxLCAuLi50YWlsXSA9PiBKcy5sb2codGFpbCkgLy8gWzIsIDNdCnwgXyA9PiAoKQp9OwoKc3dpdGNoIChbMSwgMiwgM10pIHsKfCBbMSwgMiwgMywgLi4udGFpbF0gPT4gSnMubG9nKHRhaWwpIC8vIFtdCnwgXyA9PiAoKQp9Owo%3D&live=off),
you'll see output like this:

```text
{"hd":0,"tl":{"hd":1,"tl":{"hd":2,"tl":{"hd":3,"tl":0}}}}
{"hd":-1,"tl":{"hd":0,"tl":{"hd":1,"tl":{"hd":2,"tl":{"hd":3,"tl":0}}}}}
{"hd":2,"tl":{"hd":3,"tl":0}}
0
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

The "Input array isn't changed" test doesn't need to be changed to use a list.
It can simply be deleted, because lists are immutable and therefore
`Discount.getBurger` can't change the list.

## Refactor `Discount.getHalfOff`

Let's now refactor `Discount.getHalfOff` to use lists:

<<< Discount.re#get-half-off

All that was done was to swap out array functions for list functions:

- `Js.Array.some` →
  [List.exists](https://melange.re/v3.0.0/api/re/melange/Stdlib/List/#val-exists).
  Annoyingly, many of the functions in `List` have different names than their
  counterparts in `Js.Array`.
- `Js.Array.reduce` →
  [List.fold_left](https://melange.re/v3.0.0/api/re/melange/Stdlib/List/#val-fold_left).
  Despite its name, `fold_left`[^1] has the same meaning as `reduce`.

Remember to fix the `Discount.getHalfOff` tests inside `DiscountTests`.

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

The
[Stdlib.ListLabels](https://melange.re/v1.0.0/api/re/melange/Stdlib/ListLabels/)
module has all the functions found in `Stdlib.List`, but many of them take
labeled arguments instead of positional arguments.

---

Summary

## Overview

- tbd

## Exercises

<b>1.</b> tbd

<b>2.</b> tbd

<b>3.</b> tbd

<b>4.</b> tbd

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/discounts-lists/)
and [demo](https://react-book.melange.re/demo/src/discounts-lists/) for this chapter.

-----

[^1]: Inside `Stdlib.List`, there are `fold_left` and `fold_right` functions.
    "Fold left" means to apply the given fold function starting from the first
    element and work towards the end of the list, while "fold right" starts from
    the last element and works backwards through the list.
