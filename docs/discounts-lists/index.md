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

## Rewrite `Discount.getFreeBurger`

Let's rewrite `Discount.getFreeBurger` to accept `list(Item.t)` instead of
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

footnotes
