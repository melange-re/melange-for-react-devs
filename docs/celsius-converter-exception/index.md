# Celsius Converter

Our second widget will be one that takes a temperature value in Celsius and
converts it to Fahrenheit. Create a new file called `CelsiusConverter.re`:

<<< Snippets.re#celsius-converter-v1

## `Js.t` object

Inside the `input`'s `onChange` handler, we get the event target using
`ReactEvent.Form.target`, which has the type `ReactEvent.Form.t => {_.. }`. What
is `{_.. }`? It's shorthand for the `Js.t({..})` type[^1], which consists of two
parts:

- `Js.t` refers to JavaScript objects in Melange
- `{..}` refers to polymorphic objects which can contain any fields or methods

Once we have a `Js.t` object, we can use the `##` operator to access its fields.
But beware, because the compiler knows nothing about the types of those fields.
For example, we could write `ReactEvent.Form.target(evt)##value + 1`, treating
it as if it were an integer, and the compiler wouldn't complain.

## Wrap functions that return `Js.t`

Instead of trusting ourselves to always use `ReactEvent.Form.target(evt)##value`
correctly, it's a good idea to wrap functions that return polymorphic objects
into type-annotated helper functions. For example:

<<< Snippets.re#get-value-from-event{1-2,14}

The `: string` after the argument list tells the compiler that this function
must return a `string`. Using the `getValueFromEvent` function ensures that the
`value` field can't be used as anything other than a string.

Another thing to note about `onChange` is that after the `evt` argument, the
body of the callback function is surrounded by braces (`{}`). OCaml functions
are like JavaScript's arrow functions---if they contain more than one line, they
need to be enclosed by braces.

## Apply pipe last (`|>`)

Let's change the render logic to update the Fahrenheit display based on the
value of celsius:

<<< Snippets.re#pipes{4}

The pipe last operator (`|>`) is very handy here, allowing us to convert a
string to float, then convert that float to another float (Celsius ->
Fahrenheit), convert back to string, and finally convert the string to
`React.element`, all in one line.

## String concatenation (`++`)

We should probably put °F after the Fahrenheit value so that it's clear to the
user what unit of measure they're seeing. We can do so using the string
concatenation operator (`++`):

<<< Snippets.re#string-concat{4-6}

## Catch exception with switch

However, there's a bug in this code: it will crash if you enter anything into
the input that can't be converted to a float. We can remedy this by catching the
exception using a switch expression:

<<< Snippets.re#catch-exception{4-10}

The `| exception _` branch will execute if there is any exception. The
underscore (`_`) is a wildcard, meaning it will match any exception. If we
wanted to be specific about which exception we want to catch, we could instead
write

<<< Snippets.re#catch-failure{6}

## Ternary expression

Right now it correctly renders "error" when you enter an invalid value, but it
also renders "error" if the input is blank. It might be bit more user-friendly
to instead show "?°F" like before. We can do that by wrapping the switch
expression in a ternary expression:

<<< Snippets.re#ternary{5-12}

## If-else expression

The ternary expression (`condition ? a : b`) works the same as in JavaScript.
But in OCaml, it's also shorthand for an if-else expression (`if (condition) {
a; } else { b; }`). So we could rewrite it as this:

<<< Snippets.re#if-else{5-12}

Unlike in JavaScript, the if-else construct is an expression and always yields a
value. Both branches must return a value of the same type or you'll get a
compilation error. In practice, if-else expressions aren't very common in OCaml
code because in simple cases you can use ternary, and in more complex cases you
can use switch. But it's a nice, familiar fallback you can rely on when you
haven't quite gotten used to OCaml syntax yet.

## Labeled argument

If we enter a value with a lot of decimals in it, e.g. `21.1223456`, we'll
get a Fahrenheit value with a lot of decimals in it as well. We can limit the
number of decimals in the converted value using
[Js.Float.toFixedWithPrecision](https://melange.re/v2.2.0/api/re/melange/Js/Float/index.html#val-toFixedWithPrecision):

<<< Snippets.re#fixed-precision{8,11-12}

`Js.Float.toFixedWithPrecision` is a function that has one positional argument
and one [labeled
argument](https://melange.re/v2.2.0/communicate-with-javascript/#labeled-arguments).
In this case, the labeled argument is named `digits` and it's receiving a value
of `2`. It's not possible to pass in the value of a labeled argument without
using the `~label=value` syntax. We'll see more of labeled arguments in the
following chapters after we [introduce props](/order-confirmation/#item-make).

## Partial application

You might have noticed that the function chain feeding the switch expression got
a bit shorter, from

```reason
celsius |> float_of_string |> convert |> string_of_float
```

to

```reason
celsius |> float_of_string |> convert
```

This happened because `string_of_float`, which takes a single argument, was
replaced by `Js.Float.toFixedWithPrecision`, which takes two arguments, and
functions chained using `|>` can only take a single argument. But this
one-argument restriction actually doesn't prevent us from putting
`Js.Float.toFixedWithPrecision` in the chain! We can take advantage of OCaml's
[partial
application](https://reasonml.github.io/docs/en/function#partial-application)
feature to create a one-argument function by writing
`Js.Float.toFixedWithPrecision(~digits=2)`. Then our switch expression becomes

<<< Snippets.re#partial-application{12,15}

-----

Nice, we have a working component now, but catching exceptions isn't The OCaml
Way! In the next chapter, you'll see how to rewrite the logic using `option`.

## Exercises

<b>1.</b> Try changing `{js|°C = |js}` to `"°C = "`. What happens?

<b>2.</b> It's possible to rewrite the `onChange` callback so that it contains a
single expression:

<<< Snippets.re#single-expression

This compiles, but it now contains a hidden bug. Do you know what silent error
might occur?

::: details Hint

The `getValueFromEvent` function is not being invoked in the same scope as
before.

:::

<b>3.</b> It's possible to use partial application with most functions in OCaml,
even operators. Take a look at the following program:

```reason
let addFive = (+)(5);
Js.log(addFive(2));
Js.log(addFive(7));
Js.log(addFive(10));
```

What do you think it outputs? Run it in [Melange
Playground](https://melange.re/v2.2.0/playground) to confirm your hypothesis.

<b>4.</b> Use the pipe last operator (`|>`) and partial application to write a
function that takes an integer argument `x`, subtracts `x` from 10, and converts
that result to binary.

::: details Hint

Use the
[Js.Int.toStringWithRadix](https://melange.re/v2.2.0/api/re/melange/Js/Int/#val-toStringWithRadix)
function.

:::

## Overview

- `Js.t` objects (with the type `Js.t({..})`) can have fields of any name and
  type.
  - You access fields of a `Js.t` object using the `##` operator.
  - You can use type annotations to make the use of such objects safer.
- Concatenate strings using the `++` operator.
- Switch expressions can be used to catch exceptions.
- Ternary expressions have the same syntax as in JS. Unlike in JS, they are also
  shorthand for if-else expressions.
- The two branches of if-else expressions must return values of the same type.
- Besides positional arguments, OCaml functions can also have labeled arguments.
- If a function takes two arguments, we can supply one of them and get a
  function that takes only one argument. This is called partial application.

## Solutions

<b>1.</b> Changing it to `"°C = "` will result in a bit of gibberish being
rendered: "Â°C". We can't rely on OCaml strings to [deal with Unicode
correctly](https://melange.re/v2.2.0/communicate-with-javascript/#strings), so
any string that contains non-ASCII text must be delimited using `{js||js}`.

::: tip

Note that `{js||js}` quoted string literals are specific to Melange and are not
available in native OCaml.

:::

<b>2.</b> Rewriting `onChange` the handler to use a single expression creates a
potential problem with stale values coming from the event object:

<<< Snippets.re#single-expression-annotated{5-6}

Inside of `onChange`, we can expect the function `getValueFromEvent(evt)` to
return the latest value of the `input`. However, we are now calling
`getValueFromEvent(evt)` from a different function---the callback we pass to
`setCelsius`! By the time that `setCelsius`'s callback is invoked, the `evt`
object might have been recycled and no longer have the same value as when
`onChange` was initially invoked. For more details about this, see [Using Event
Values with
useState](https://reasonml.github.io/reason-react/docs/en/usestate-event-value)
in the [ReasonReact](https://reasonml.github.io/reason-react/) docs.

<b>3.</b> Playground: [Define an addFive function using partial application](https://melange.re/v2.2.0/playground/?language=Reason&code=bGV0IGFkZEZpdmUgPSAoKykoNSk7CkpzLmxvZyhhZGRGaXZlKDIpKTsKSnMubG9nKGFkZEZpdmUoNykpOwpKcy5sb2coYWRkRml2ZSgxMCkpOw%3D%3D&live=off)

<b>4.</b> Playground: [Define a function that subtracts from 10 and converts to binary](https://melange.re/v2.2.0/playground/?language=Reason&code=bGV0IGNvb2xGdW5jdGlvbiA9IHggPT4geCB8PiAoKC0pKDEwKSkgfD4gSnMuSW50LnRvU3RyaW5nV2l0aFJhZGl4KH5yYWRpeD0yKTsKSnMubG9nKGNvb2xGdW5jdGlvbigxKSk7CkpzLmxvZyhjb29sRnVuY3Rpb24oNSkpOw%3D%3D&live=off)

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/celsius-converter-exception/)
and [demo](https://react-book.melange.re/demo/src/celsius-converter-exception/) for this chapter.

-----

[^1]:
    See [Using Js.t
    objects](https://melange.re/v2.2.0/communicate-with-javascript/#using-jst-objects) for more
    details.
