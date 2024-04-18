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
