# Promo Codes

As International Burger Day looms ever closer, Madame Jellobutter is eager to
start her burger-related promotions. She passed out flyers at a recent music
festival and she even bought ads on local billboards (she's very old school when
it comes to advertising). Depending on the ad, potential customers will either
see the promo code FREE, which corresponds to the "buy n burgers, get n/2
burgers free" discount, or HALF, which corresponds to the "buy a burger with
everything on it and get half off the entire order" discount.

## `Discount.getDiscountFunction` function

Let's start by adding a function that maps a given promo code to its
corresponding discount function:

<<< Discount.re#simple-get-discount

Instead of
[Js.String.toUpperCase](https://melange.re/v3.0.0/api/re/melange/Js/String/#val-toLowerCase),
we could've used
[String.uppercase_ascii](https://melange.re/v3.0.0/api/re/melange/Stdlib/String/#val-uppercase_ascii),
but its name implies, `String.uppercase_ascii` can only handle strings
containing ASCII characters. This is a common restriction for functions in the
[Stdlib.String](https://melange.re/v3.0.0/api/re/melange/Stdlib/String/) module,
so for most string operations you should prefer the functions in
[Js.String](https://melange.re/v3.0.0/api/re/melange/Js/String/).

Madame Jellobutter informs you that the FREE promotion is only active during the
month of May, so you change `Discount.getDiscountFunction` accordingly:

<<< Discount.re#get-discount-with-date{1,5-6}

## `result` type

The function now takes a `date` argument and returns `None` if the promo code is
FREE and the given date isn't within the month of May. This logic is technically
correct, but there's a slight problem: it leaves no way to inform the user of
the reason for the promo code not working. Whether the user misspelled the promo
code or entered it on June 1, the only feedback they'd get is that it didn't
work, but they wouldn't know why. We can remedy this by returning `result`
instead of `option`:

<<< Discount.re#get-discount-return-result{6,8}

Basically, we replaced `Some` with `Ok` and `None` with `Error`. But when we
return the `Error` constructor, we also supply a specific error message that can
be shown in the UI. We didn't need to type annotate anything in this function
because both the `Ok` and `Error` constructors of the
[Stdlib.result](https://melange.re/v3.0.0/api/re/melange/Stdlib/#type-result)
type are always in scope (since `Stdlib` is opened by default).


---

Summary

## Overview

tbd

## Exercises

<b>1.</b> tbd

<b>2.</b> tbd

<b>3.</b> tbd

<b>4.</b> tbd

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/promo-codes/)
and [demo](https://react-book.melange.re/demo/src/promo-codes/) for this chapter.

-----

footnotes
