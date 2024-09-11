# Order with Promo

Now that you have a `Promo` component, you can add it to your `Order` component.
With that in place, customers can finally enter promo codes and enjoy discounts
on their orders.

## Add `Promo` to `Order`

Add the `Promo` component to the `Order` component:

<<< Order.re#make{3,5,18-25,28}

A breakdown:

- Create a new state variable called `discount` (along with its attendant
  `setDiscount` function)
- Rename the `total` variable to `subtotal`
- Add a new table row with label "Subtotal" to display the value of `subtotal`
- Add a new table row with label "Promo" to render the `Promo` component
- Set the value of `discount` through `Promo`'s `onApply` callback prop (we'll
  add this prop in the next step)
- Subtract `discount` from `subtotal` when rendering the total price of the
  order

## Add `onApply` prop to `Promo`

Add an `onApply` prop to `Promo`, which will be invoked when a promo code is
successfully submitted and results in a discount:

<<< Promo.re#add-on-apply{3}

::: tip

You don't have to type annotate all your component props, but it's a good idea
to at least type annotate your component's callback props as a form of
documentation.

:::

Add a switch expression to the `form` element's `onSubmit` callback which
invokes `onApply`:

<<< Promo.re#switch-in-on-submit{9-20}

The structure of this switch expression closely mirrors the structure of the
switch expression used to define the `discount` derived variable:

<<< Promo.re#discount-derived-variable

The branches in the two switch expressions are exactly the same with only the
return values being different. You wonder if there's some way to reduce the
redundancy. There is! You can replace the definition of `discount` with a
`getDiscount` function:

<<< Promo.re#get-discount

Now use `getDiscount` inside the `onSubmit` callback:

<<< Promo.re#on-submit-using-get-discount{5-10}

And also use it in the render logic's switch expression:

<<< Promo.re#get-discount-in-render{1}

Take a little time to verify that `Order` is able to accept promo codes.

- Type `FREE` into the input and press Enter. It should deduct the price of
  every other burger (ordered by price descending).
- Type `HALF` into the input and press Enter. It should show an error saying
  "Expired promo code".

## Add datasets to `App`

It was OK to keep our `App` component inside of `Index.re` because it was always
very small. But if we want to add more to it, it makes sense for `App` to be
inside its own file. Create a new file called `App.re` and move the contents of
`Index.App` into it (deleting `Index.App` in the process).

Promo behavior tightly depends on what items are in an order, so it would be
nice to render multiple `Order` components containing different sets of items.
Add a `datasets` variable to `App`:

<<< App.re#datasets

Since the `burgers` value is only used in the "5 burgers" expression, we can
move it inside that expression:

<<< App.re#burger-expression{2-9}

Note that we need to add curly braces (`{}`) around the expression because
multi-line expressions must be surrounded by curly braces.

::: tip

OCaml makes it easy to  move variable definitions closer to where they are
actually used. Unlike in JavaScript, you can use `let` anywhere, even inside an
expression.

:::

Now we can refactor `App` to render a different `Order` for each collection of
items:

<<< App.re#refactor

You can delete the now-unused `App.items` variable.

Enter the promo code "HALF" into different `Order`s to see different messages:

- In the "No burgers" order, you'll get the error message "Buy a burger with
  every topping to enjoy this promotion"
- In the "1 burger with at least one of every topping" order, you'll get a
  discount of 15.97.

## Add `DateInput` component

Currently, the date in `App` is static, but it would be very convenient to be
able to change it interactively in order to see the behavior of `Promo` on
different dates. Let's add a new file `DateInput.re`:

<<< DateInput.re#initial{9,18}

A few notes:

- This invocation of `Printf.sprintf` contains [float conversion
    specifications](https://melange.re/v4.0.0/api/re/melange/Stdlib/Printf/index.html#val-fprintf)
  which can convert the float components of a Date[^1] into strings:
  - The float conversion specification `%04.0f` sets a minimum width of 4,
    left-padded with 0, with 0 numbers after the decimal. It converts the year
    part of the given date.
- The `type` prop of `input` has been renamed to `type_`, because in OCaml,
  `type` is a reserved keyword and can't be used as an identifier.

::: tip

Even though the prop `type` has been renamed to `type_` in OCaml, the [generated
JS output will say
`type`](https://melange.re/v4.0.0/playground/?language=Reason&code=W0ByZWFjdC5jb21wb25lbnRdCmxldCBtYWtlID0gKCkgPT4gPGlucHV0IHR5cGVfPSJkYXRlIiAvPjs%3D&live=off).

Some other prop names that clash with OCaml reserved keywords and must also be
suffixed with an underscore: `as_`, `open_`, `begin_`, `end_`, `in_`, `to_`.

:::

Add `DateInput` to `App.make`:

<<< App.re#add-date-input{3-4,8}

Try changing the date to November 3 and enter the promo code "HALF" into the
"All sandwiches" order. You should see a discount of 65.00.

## Add styling for promo code row

The styling is a little bit off. Add the following value to `Order.Style`:

<<< Order.re#promo-class

Then set the `className` of the promo code row to `Style.promo`:

<<< Order.re#set-promo-class{1}

---

Hot diggity! You've added the promo codes to your order confirmation widget,
just in time for Madame Jellobutter's International Burger Day promotions. In
the next chapter, we'll learn how to test our components.

## Overview

- When a nested module starts to get big, it's a good idea to put it into its
  own file
- Some component props have names that aren't legal as identifiers in OCaml, and
  we must add an underscore after them, e.g. `type`, which must be rewritten as
  `type_`.
- You can use `let` inside expressions, which allows you to define variables
  closer to where they're used
- Float conversion specifications, in conjunction with `Printf.sprintf`, gives
  you fine-grained control over how to convert a `float` value to a string.


## Exercises

<b>1.</b> Change the render logic so that a `DateInput` is rendered above each
`Order`. Changing the date on a `DateInput` changes the date for the `Order`
below it, but doesn't affect the dates of the other `Order`s.

::: details Hint

Define a `DateAndOrder` helper component inside `App`.

:::

::: details Solution

Add `App.DateAndOrder` subcomponent:

<<< App.re#date-and-order

Then refactor `App.make` to use the new component:

<<< App.re#make{6}

:::

<b>2.</b> The `DateInput` component has a pretty annoying bug. Focus on
`DateInput`'s `input` element and press the `0` key. The `input` element will go
into an "invalid date" state. Refactor `DateInput` so that the user can no
longer enter invalid dates.

::: details Hint 1

Use
[Js.Date.valueOf](https://melange.re/v4.0.0/api/re/melange/Js/Date/index.html#val-valueOf)
and
[Js.Float.isNaN](https://melange.re/v4.0.0/api/re/melange/Js/Float/#val-isNaN)
to check the validity of a date.

:::

::: details Hint 2

Use [Option.iter](https://melange.re/v4.0.0/api/re/melange/Stdlib/Option/#val-iter).

:::

::: details Solution

First, modify `DateInput.stringToDate` so that it returns `Some(date)` when the date
is valid and `None` otherwise:

<<< DateInput.re#string-to-date-option{4}

Then modify the `input` element's `onChange` callback so that `onChange` is only
invoked when the return value of `stringToDate` is `Some(date)`:

<<< DateInput.re#on-change-switch

A shorter but equivalent way to write the `onChange` callback is to use the
[Option.iter](https://melange.re/v4.0.0/api/re/melange/Stdlib/Option/#val-iter)
function:

<<< DateInput.re#on-change-option-iter

:::

<b>3.</b> Improve `Discount.getSandwichHalfOff` so that it returns a list of
missing sandwich types when the discount can't be applied. Start by changing the
"Not all sandwiches, return Error" test in `DiscountTests` to this:

<<< DiscountTests.re#not-all-sandwiches{6,8}

Note that the `` `MissingSandwichTypes`` variant tag now contains an argument of
`list(string)`. Make this test pass, and fix any incidental compilation errors.
(Don't worry about changing the render logic, that's a subject for the next
chapter.)


::: details Hint 1

To create a list of strings from the `tracker` record, try this:

```reason
[
  tracker.portabello ? "portabello" : "",
  //...
]
```

:::

::: details Hint 2

Use
[List.filter](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/#val-filter)
to filter out empty strings.

:::

::: details Solution

Change the switch expression inside `Discount.getSandwichHalfOff` so that when
there are missing sandwich types, they are collected in a list and returned as
the argument of `` `MissingSandwichTypes`` error tag:

<<< Discount.re#missing-sandwich-types{9-18}

Note that instead of using partial application in

```reason
|> List.filter((!=)(""));
```

We could have instead written

```reason
|> List.filter(s => s != "")
```

which is a little more verbose but arguably easier to understand.

In order to fix the compilation error in `Promo`, add a wildcard variable to the
`` `MissingSandwichTypes`` branch:

<<< Promo.re#missing-sandwich-types-wildcard{6}

:::

<b>4.</b> Add a new helper function `ListSafe.humanize`, which takes a list of
strings and returns a human-readable string. The logic should look something
like this:

```reason
/** Take a list of strings and return a human-readable string */
let humanize =
  fun
  | [] => ""
  | [x] => x
  | [x, y] => x ++ " and " ++ y
  | [first, ...rest] =>
    // fill in this part
```

To test this function, add a new file `ListSafeTests.re`:

<<< @/../src/order-with-promo/ListSafeTests.re

Also add a test command to `tests.t`:

```text
ListSafe tests
  $ node ./output/src/order-confirmation/ListSafeTests.mjs | sed '/duration_ms/d'
```

Recall that you can run all your tests by running `npm run test`.

::: details Hint 1

You may want to use
[List.fold_left](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/#val-fold_left),
[List.mapi](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/#val-mapi), and
[List.rev](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/#val-mapi).

:::

::: details Hint 2

Use `List.rev` in conjunction with `List.mapi` to prefix the last element with
"and ".

:::

::: details Hint 3

Use `List.fold_left` to add ", " in front of each element of `rest`.

:::

::: details Solution

<<< ListSafe.re#humanize{8-12}

An alternate solution would have been to add the commas in the callback to
`List.mapi`, which simplifies the call to `List.fold_left`:

<<< ListSafe.re#alternate{3,5}

:::

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/order-with-promo/)
and [demo](https://react-book.melange.re/demo/src/order-with-promo/) for this chapter.

-----

[^1]: It might be a little confusing that `Js.Date.get*` functions all return
    `float` instead of `int`. The reason is that these functions [must return
    `NaN` if the input Date is
    invalid](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/getFullYear#return_value),
    and in OCaml, only `float` is capable of representing
    [`NaN`](https://melange.re/v4.0.0/api/re/melange/Js/Float/#val-_NaN).
