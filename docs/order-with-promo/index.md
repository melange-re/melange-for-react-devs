# Order with Promo

Now that you have a `Promo` component, you can add it to your `Order` component.
With that in place, customers can finally enter promo codes and enjoy discounts
on their orders.

## Add `discount` type

But first, let's see how to create a normal variant type for the `discount`
derived variable inside `Promo`. We do not have to do this, because it works fine
the way it is now, but the process of creating this new type should give us more
insight into OCaml's type system. Additionally, normal variants are better than
polymorphic variants at "documenting" the types that will be used in your
program, since they must always be explicitly defined before you can use them.

When we hover over the `discount` variable, we see this type expression:

<<< Types.re#inferred-type

The easiest thing to do is to create a new `discount` type and assign it to the
above type expression, then delete the `` ` `` from the top-level variant tags
to turn them into variant constructors[^1]:

<<< Types.re#bad-discount-type

However, this results in a compilation error:

<<< type-error.txt

We'll come back to this error message later. For now, observe that the error
disappears if we simply delete `>`:

<<< Types.re#delete-refinement

This fixes the syntax error so that we now have a correctly-defined variant
type.

## Refactor `discount`

Refactor the `discount` derived variable inside `Promo.make` to use our new
variant type by deleting all occurrences of `` ` ``:

<<< Promo.re#discount-variant

You can likewise refactor the switch expression inside the render logic:

<<< Promo.re#discount-render

## Type constructor and type variable

Change the `discount` type to this:

<<< Types.re#type-variable

Now `discount` is a *type constructor* that takes a *type variable* named `'a`.
A type constructor is not a fixed type---you can think of it as a function that
takes a type and outputs a new type.

The advantage of doing this is that the variant tags inside `DiscountError` are
no longer constrained by our `discount` type. This makes sense because they are
used primarily in the `Discount` module, and if any variant tags are renamed,
added, or deleted, those changes will and should happen in `Discount`.

Using a type variable does not sacrifice type safety, if you hover over the
`discount` variable, you see that its type is:

```reason
discount([> `MissingSandwichTypes
          | `NeedMegaBurger
          | `NeedOneBurger
          | `NeedTwoBurgers ])
```

Based on its usage, OCaml can figure out the exact type of the `discount`
variable and automatically fill in the value of the type variable.

## `>` = "allow more than"

In the type expression above, we once again see `>`, so let's see what it means.
In polymorphic variant type expressions, it means "allow more than". In this
case, it means that tags other than the four that are listed are allowed. For
example, this type would be allowed:

```reason{5-6}
discount([| `MissingSandwichTypes
          | `NeedMegaBurger
          | `NeedOneBurger
          | `NeedTwoBurgers
          | `HoneyButter
          | `KewpieMayo ])
```

When defining your own types, you will most often used *fixed* polymormorphic
variants, i.e. those that don't have `>` in their type expressions. But it is
still useful to know what `>` does, since it appears when the compiler
infers the type of a variable or function that uses polymorphic variants.

::: tip

Fixed polymorphic variants and normal variants are roughly equivalent and can be
used interchangeably.

:::

## Implicit type variable

Let's come back to the question of why the original attempt at a variant type
definition was syntactically invalid:

<<< Types.re#bad-discount-type

The reason is that there's an implicit type variable around the `>`. The
above type expression is equivalent to:

<<< Types.re#explicit-type-var{14}

Now the error message makes a bit more sense:

<<< type-error.txt{7}

The type variable exists, but it's pointless unless it appears as an argument of
the `discount` type constructor. Once it's added, it compiles:

<<< Types.re#add-type-arg{1}

This is somewhat like accidentally using a variable in a function but forgetting
to add that variable to the function's argument list.

## Force `DiscountError` argument to be polymorphic variant

Right now the argument of the `DiscountError` constructor can be any type at
all, but to be explicit, we can force it to be a polymorphic variant:

<<< Types.re#must-be-poly{4}

The `[> ]` type expression means a polymorphic variant that has no tags, but
allows more tags, which basically means any polymorphic variant. Note that
adding this small restriction to the type doesn't make a real difference in this
program---it's just a way to make it clear that `DiscountError`'s argument
should be a polymorphic variant. It's an optional embellishment that you can
feel free to leave out.

## Quick summary

You refactored the `discount` reactive variable to use a normal variant instead
of a polymorphic variant. The code changes were fairly minimal, but to
understand what was happening, it was necessary to learn the basics of type
constructors and type variables. In the next sections, we'll set types and other
theoretical considerations aside and get into the nitty-gritty of the UI changes
you must make to add promo support to the `Order` component.

## Add `DateInput` component

To see different promotions in action, we want to be able to easily change the
date in our demo, so add a new file `DateInput.re`:

<<< DateInput.re{7,16}

A few notes:

- We use `Printf.sprintf` to give us more control over how the `float`
  components of a Date[^2] are converted to strings:
  - The [float conversion
    specification](https://melange.re/v4.0.0/api/re/melange/Stdlib/Printf/index.html#val-fprintf)
    `%4.0f` sets a minimum width of 4 and 0 numbers after the decimal
  - The float conversion specification `%02.0f` sets a minimum width of 2
    (left padded with 0) and 0 numbers after the decimal
- The `type` prop of `input` has been renamed to `type_`, because in OCaml,
  `type` is a reserved keyword and can't be used as an argument name. But don't
  worry, it will still say `type` in the generated JS output.

## Add `Demo` component

Move the contents of `Index.App` into a new file called `Demo.re`. In the
process, add our newly-created `DateInput` component:

<<< Demo.re#initial{16-17,21}

Change `Index` to use the new `Demo` component:

<<< Index.re{7}

## Add `Promo` to `Order`

Add the `Promo` component to the `Order` component:

<<< Order.re#make{3,22-25,28}

A breakdown:

- Create a new state variable called `discount` (along with its attendant
  `setDiscount` function)
- Set the value of `discount` through `Promo`'s `onApply` callback prop (we'll
  add this prop in the next step)
- Subtract `discount` from `subtotal` when rendering the total price of the
  order

## Add `onApply` prop to `Promo`

Add an `onApply` prop to `Promo`, which will be invoked when a promo code is
successfully submitted and results in a discount:

<<< Promo.re#add-on-apply{3}

::: tip

You don't have to type annotate your component props, but it's a good idea to at
least type annotate your component's callback props as a form of documentation.

:::

To invoke `onApply`, we can add a `useEffect` hook that invokes `onApply` when
`discount` has a value of the form `` `Discount(value) ``:

<<< Promo.re#use-effect

Note that when `discount` has an error value, we return
[`()`](https://ocaml.org/docs/basic-data-types#unit) from the switch expression,
which essentially means "do nothing".

## `React.useEffect*` functions

`React.useEffect1` is one of the binding functions for React's [useEffect
hook](https://react.dev/reference/react/useEffect). The number `1` at the end of
the function indicates how many dependencies this function is supposed to take.
Accordingly, we also have `React.useEffect0`, `React.useEffect2`, etc, all the
way up to `React.useEffect7`[^3].

All `React.useEffect*` functions accept a [setup
callback](https://react.dev/reference/react/useEffect#reference) as their first
argument, the type of which is:

```reason
unit => option(unit => unit)
```

The setup callback's return type is `option(unit => unit)`, which allows you to
return a cleanup function encased in `Some`. When the effect doesn't need a
cleanup function, just return `None`.

The second argument for all `React.useEffect*` functions (except
`React.useEffect0`) is for the dependencies. For example, the type of
`React.useEffect2` is:

```reason
(unit => option(unit => unit), ('a, 'b)) => unit
```

And the type of `React.useEffect3` is:

```reason
(unit => option(unit => unit), ('a, 'b, 'c)) => unit
```

::: tip

Every time you add or remove a dependency from your `useEffect` hook, you'll
need to use a different `React.useEffect*` function (the one that corresponds to
how many dependencies you now have).

:::

## Why does `React.useEffect2` accept a tuple?

`React.useEffect2` takes its dependencies as a tuple instead of an array. To
understand why, we need to understand the type properties of tuples and arrays:

- The elements of tuples can have different types, e.g. `(1, "a", 23.5)`
- The elements of arrays must all be of the same type, e.g. `[|1, 2, 3|]`,
  `[|"a", "b", "c"|]`

Therefore, we must use tuples to express the dependencies of `useEffect` hooks,
otherwise our dependencies would all have to be of the same type. This applies
to all `React.useEffect*` functions which take 2 or more dependencies.

Even though we use tuples for dependencies in our OCaml code, they are turned
into JS arrays in the runtime. So the generated code will run the same as in any
ReactJS app.

However, you might have noticed that `React.useEffect1` is the odd man out,
because it accepts an array for its single dependency. The reason is that
one-element OCaml tuples don't become arrays in the JS runtime, they instead
take on the value of their single element. So in this case, `React.useEffect1`
[must take an
array](https://reasonml.github.io/reason-react/docs/en/components#hooks) so that
it respects the API of the underlying `useEffect` function.

## `RR.useEffect1` helper function

`React.useEffect1` taking an array means that you could accidentally pass in an
empty array as the dependency for `React.useEffect1`. You can sidestep this
possibility by adding a helper function to your `RR` module:

<<< RR.re#use-effect-1

Refactor `Promo` to use your new helper function:

<<< Promo.re#use-effect-helper{1,11}

You may be wondering why ReasonReact doesn't provide this helper function for
you. The reason is that its bindings to React functions are supposed to be
zero-cost, without any additional abstractions on top. This is the same reason
that something like our `RR.useStateValue` helper function is also not included
with ReasonReact.

## Add styling for promo code row

Execute `npm run serve` to see your app in action. Verify that it behaves as
expected:

- Type `FREE` into the input and press Enter. It should deduct the price of
  every other burger (ordered by price descending).
- Type `HALF` into the input and press Enter. It should deduct half off the
  entire order.
- Change the date to something other than May 28. It should show an error saying
  "Expired promo code"

However, the styling is a little bit off. Add the following value to
`Order.Style`:

<<< Order.re#promo-class

Then set the `className` of the promo code row to `Style.promo`:

<<< Order.re#set-promo-class{1}

## How often does the Effect run?

Everything seems to be working correctly, but let's see how often our
`useEffect` hook fires by adding a little logging:

<<< Promo.re#log{8}

You see that every time a promo code is successfully applied, it logs twice to
the console. That doesn't seem right, because the value of `discount` only
changes once when you submit a new promo code.

The reason lies in the runtime representation of `discount`---recall that
variant constructors with arguments are turned into objects in the JS runtime.
Because `discount` is a derived variable, it gets recreated on every render, and
even if its contents didn't change, the [hook will always treat it as having
changed because the object is no longer the same one as
before](https://react.dev/reference/react/useEffect#removing-unnecessary-object-dependencies).

The easiest fix is to simply change the dependency to `submittedCode` instead of
`discount`:

<<< Promo.re#submitted-code-dep{13}

This does the trick---the Effect only runs once every time you submit a new
promo code. But wait! Why does it behave differently when `submittedCode` is an
`option`, and `option` is just another variant type?[^4]

Although `option` is a variant type, its [runtime representation is a special
case](../burger-discounts/#runtime-representation-of-option):

- `None` becomes `undefined`
- `Some(value)` becomes `value`

Therefore, an `option` value that wraps a primitive value doesn't ever turn into
an object in the JS runtime, and therefore can be used as a dependency for React
hooks.

## You don't need an Effect

The above discussion about Effects was somewhat academic, because we [don't
actually need Effects to handle user
events](https://react.dev/learn/you-might-not-need-an-effect#how-to-remove-unnecessary-effects).
Let's delete the call to `RR.useEffect1` and start over.

A better place to call `onApply` is from within the `form`'s `onSubmit`
callback. Replace the `discount` derived variable with a `getDiscount` function:

<<< Promo.re#get-discount

Call `getDiscount` within the `onSubmit` callback function:

<<< Promo.re#on-submit

Inside the render logic, change the input of the switch expression from
`discount` to `getDiscount(submittedCode)`:

```reason
{switch (discount) { // [!code --]
{switch (getDiscount(submittedCode)) { // [!code ++]
```

## Add `datasets` to `Demo`

To make it easier to see the different promo-related error messages, you can
create different collections of items. Add a `datasets` variable to `Demo`:

<<< Demo.re#datasets

Since the `burgers` value is only used in a single expression, we can move it
inside that expression:

<<< Demo.re#burger-expression{2-9}

::: tip

OCaml makes it easy to  move variable definitions closer to where they are
actually used. Unlike in JavaScript, you can use `let` anywhere, even inside an
expression.

:::

Now we can refactor `Demo` to render a different `Order` for each collection of
items:

<<< Demo.re#refactor

Remember to delete the now-unused `Demo.items` variable.

---

Hot diggity! You've added the promo codes to your order confirmation widget,
just in time for Madame Jellobutter's International Burger Day promotions. In
the next chapter, we'll further polish the sandwich promotion logic.

## Overview

- A type constructor takes a type and outputs another type
- A type variable is a variable that stands in a for a type and often appears in
  type constructors or type signatures
- In polymorphic variant type expressions, `>` means that the polymorphic
  variant can accept more than the variant tags that are listed
  - You rarely need to use `>` in your own type definitions, but it often
    appears in inferred type definitions (that appear when you hover over
    variables and functions)
  - Inferred type definitions that contain `>` also have an implicit type
    variable
- Some component props have names that aren't legal as function arguments in
  OCaml, and we must add an underscore after them. A common example is `type`,
  which must be rewritten as `type_`[^5].
- ReasonReact has several binding functions for React's `useEffect` hook, e.g.
  `React.useEffect0`, `React.useEffect1`, ...., `React.useEffect7`
  - The number at the end indicates how many dependencies the function takes
  - `React.useEffect1` takes an array for its one dependency
  - `React.useEffect2` and above take tuples for their dependencies
- The elements of a tuple can be different types
- Tuples become arrays in the JavaScript runtime
- The elements of an array must all be the same type
- Be careful about using variants as hook dependencies, because they often get
  turned into objects in the runtime and cause Effects to run more often than
  you want
- It's often safe to use `option` as a hook dependency, because even though it's
  a variant, it's a special case and does not become an object in the JavaScript
  runtime
- You can use `let` inside expressions, which allows you to define variables
  closer to where they're used

## Exercises

<b>1.</b> The following code
([playground](https://melange.re/v4.0.0/playground/?language=Reason&code=bGV0IGdldE5hbWUgPSAoYW5pbWFsOiBbfCBgQ2F0IHwgYERvZyhpbnQpIHwgYFVuaWNvcm4oc3RyaW5nKV0pID0%2BCiAgc3dpdGNoIChhbmltYWwpIHsKICB8IGBDYXQgPT4gIk1yIFdoaXNrZXJzIgogIHwgYERvZyhuKSA9PiAiQmFuZGl0ICIgKysgc3RyaW5nX29mX2ludChuKQogIHwgYFVuaWNvcm4obmFtZSkgPT4gIlNpciAiICsrIG5hbWUKICB9Ow%3D%3D&live=off))
doesn’t compile. Fix it by adding a single character.

```reason
let getName = (animal: [| `Cat | `Dog(int) | `Unicorn(string)]) =>
  switch (animal) {
  | `Cat => "Mr Whiskers"
  | `Dog(n) => "Bandit " ++ string_of_int(n)
  | `Unicorn(name) => "Sir " ++ name
  };
```

::: details Hint

Find a place where you can insert a space.

:::

::: details Solution

```reason{1}
let getName = (animal: [ | `Cat | `Dog(int) | `Unicorn(string)]) =>
  switch (animal) {
  | `Cat => "Mr Whiskers"
  | `Dog(n) => "Bandit " ++ string_of_int(n)
  | `Unicorn(name) => "Sir " ++ name
  };
```

A common mistake when writing polymorphic variant type definitions is forgetting
to put a space between the `[` and the `|` characters. Note that you don't need
to add the implicit type variable in type annotations.

::: warning

In the next version of Melange, polymorphic variant definitions no longer
require a space between `[` and `|`.

:::

<b>2.</b> The following code
([playground](https://melange.re/v4.0.0/playground/?language=Reason&code=bGV0IGdldE5hbWUgPSAoYW5pbWFsOiBbIHwgYENhdCB8IGBEb2coaW50KSB8IGBVbmljb3JuKHN0cmluZyldKSA9PgogIHN3aXRjaCAoYW5pbWFsKSB7CiAgfCBgQ2F0ID0%2BICJNciBXaGlza2VycyIKICB8IGBEb2cobikgPT4gIkJhbmRpdCAiICsrIHN0cmluZ19vZl9pbnQobikKICB8IGBVbmljb3JuKG5hbWUpID0%2BICJTaXIgIiArKyBuYW1lCiAgfCBgRHJhZ29uID0%2BICJQdWZmIHRoZSBNYWdpYyIKICB9Ow%3D%3D&live=off))
doesn’t compile. Fix it by adding a single character.

```reason
let getName = (animal: [| `Cat | `Dog(int) | `Unicorn(string)]) =>
  switch (animal) {
  | `Cat => "Mr Whiskers"
  | `Dog(n) => "Bandit " ++ string_of_int(n)
  | `Unicorn(name) => "Sir " ++ name
  | `Dragon => "Puff the Magic"
  };
```

::: details Solution

```reason{1}
let getName = (animal: [> | `Cat | `Dog(int) | `Unicorn(string)]) =>
  switch (animal) {
  | `Cat => "Mr Whiskers"
  | `Dog(n) => "Bandit " ++ string_of_int(n)
  | `Unicorn(name) => "Sir " ++ name
  | `Dragon => "Puff the Magic"
  };
```

Adding a `>` to the polymorphic variant type definition allows it to accept more
than the listed variant tags.

:::

<b>3.</b> Fix the following code ([playground](https://melange.re/v4.0.0/playground/?language=Reason&code=LyoqIE9ubHkgaW52b2tlIFtmXSB3aGVuIFtvMV0gYW5kIFtvMl0gYXJlIFtTb21lXSAqLwpsZXQgbWFwMjogKG9wdGlvbignYSksIG9wdGlvbignYSksICgnYSwgJ2EpID0%2BICdhKSA9PiBvcHRpb24oJ2EpID0KICAobzEsIG8yLCBmKSA9PgogICAgc3dpdGNoIChvMSwgbzIpIHsKICAgIHwgKE5vbmUsIE5vbmUpCiAgICB8IChOb25lLCBTb21lKF8pKQogICAgfCAoU29tZShfKSwgTm9uZSkgPT4gTm9uZQogICAgfCAoU29tZSh2MSksIFNvbWUodjIpKSA9PiBTb21lKGYodjEsIHYyKSkKICAgIH07CgpKcy5sb2cobWFwMihTb21lKDExKSwgU29tZSgzMyksICgrKSkpOwpKcy5sb2cobWFwMihTb21lKCJBQkMiKSwgU29tZSgxMjMpLCAoYSwgYikgPT4gKGEsIGIpKSk7&live=off)) which fails to compile:

```reason
/** Only invoke [f] when [o1] and [o2] are [Some] */
let map2: (option('a), option('a), ('a, 'a) => 'a) => option('a) =
  (o1, o2, f) =>
    switch (o1, o2) {
    | (None, None)
    | (None, Some(_))
    | (Some(_), None) => None
    | (Some(v1), Some(v2)) => Some(f(v1, v2))
    };

Js.log(map2(Some(11), Some(33), (+)));
Js.log(map2(Some("ABC"), Some(123), (a, b) => (a, b)));
```

::: details Hint 1

Fix the type annotation.

:::

::: details Hint 2

Delete the type annotation.

:::

::: details Solution

```reason
/** Only invoke [f] when [o1] and [o2] are [Some] */
let map2: (option('a), option('b), ('a, 'b) => 'c) => option('c) =
  (o1, o2, f) =>
    switch (o1, o2) {
    | (None, None)
    | (None, Some(_))
    | (Some(_), None) => None
    | (Some(v1), Some(v2)) => Some(f(v1, v2))
    };

Js.log(map2(Some(11), Some(33), (+)));
Js.log(map2(Some("ABC"), Some(123), (a, b) => (a, b)));
```

We have to use different type variables if we expect that the types might be
different. Note that we could have deleted the type annotation and then OCaml's
inferred type would be the same as the type annotation above.

:::


<b>4.</b> Change the render logic so that a `DateInput` is rendered above each
`Order`. Changing the date on a `DateInput` changes the date for the `Order`
below it.

::: details Hint

Define a `DateAndOrder` helper component.

:::

::: details Solution

Add `Demo.DateAndOrder` subcomponent:

<<< Demo.re#date-and-order

Then refactor `Demo.make` to use the new component:

<<< Demo.re#make

:::

<b>5.</b> Make the message for `Discount.getSandwichHalfOff`'s ``
`MissingSandwichTypes`` error more friendly by listing the sandwiches you still
need to buy to fulfill the conditions of the promotion. As a start, change the
"Not all sandwiches, return Error" test in `DiscountTests.SandwichHalfOff`:

<<< DiscountTests.re#not-all-sandwiches{13}

Note that the `` `MissingSandwichTypes`` variant tag now has an argument which
is a list of strings.

::: details Hint 1

Inside `Discount.getSandwichHalfOff`, use
[List.filter](https://melange.re/v4.0.0/api/re/melange/Stdlib/List/#val-filter)
to filter out sandwich types that don't appear in `items`.

:::

::: details Hint 2

In `Promo.make`, use
[Stdlib.Array.of_list](https://melange.re/v4.0.0/api/re/melange/Stdlib/Array/#val-of_list)
and
[Js.Array.join](https://melange.re/v4.0.0/api/re/melange/Js/Array/index.html#val-join)
to create a comma-delimited string.

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

which is a little more verbose and arguably easier to understand.

Then change the render logic inside `Promo.make`'s `` `MissingSandwichTypes``
branch to convert the list of missing sandwich types to a comma-delimited
string:

<<< Promo.re#show-missing-sandwich-types{7-9}

Recall that we have to use `Stdlib.Array.of_list` instead of `Array.of_list`
because our custom `Array` module takes precedence.

:::

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/order-with-promo/)
and [demo](https://react-book.melange.re/demo/src/order-with-promo/) for this chapter.

-----

[^1]: In OCaml terminology, variant tags start with `` ` `` and correspond to
    polymorphic variant types, while variant constructors correspond to normal
    variant types.

[^2]: It might be a little confusing that `Js.Date.get*` functions all return
    `float` instead of `int`. The reason is that these functions [must return
    `NaN` if the input Date is
    invalid](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Date/getFullYear#return_value),
    and in OCaml, only `float` is capable of representing
    [`NaN`](https://melange.re/v4.0.0/api/re/melange/Js/Float/#val-_NaN).

[^3]: If you happen to need more than 7 dependencies, you can define your own
   binding function based on the [current binding
   functions](https://github.com/reasonml/reason-react/blob/713ab6cdb1644fb44e2c0c8fdcbef31007b37b8d/src/React.rei#L248-L255).
   We'll cover bindings in more detail [later](/todo).

[^4]: Recall that variant constructors with arguments also get turned into
    objects in the JS runtime.

[^5]: Some other prop names which cannot be used in their original form are:
    `as_`, `open_`, `begin_`, `end_`, `in_`, and `to_`.
