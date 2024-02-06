# Better Sandwiches

Cafe Emoji is a stunning success, despite the fact that all sandwiches have the
same filling of Mystery Meat™️. In order to offer her diners more variety,
Madame Jellobutter decides to add multiple types of sandwich to her menu.

Currently, your `src/order-confirmation/Item.re` looks something like this:

<<< @/../src/styling-with-css/Item.re

## `sandwich` variant type

To start, add a new variant type `sandwich` and then add that type as the
*argument* for the `Sandwich` constructor of `Item.t`:

<<< Item.re#sandwich-type{1-4,7}

You'll immediately get a compilation error inside the `Item.toPrice` function:

```
File "src/order-confirmation/Item.re", line 13, characters 2-12:
13 |   | Sandwich => 10.
         ^^^^^^^^
Error: The constructor Sandwich expects 1 argument(s),
       but is applied here to 0 argument(s)
```

This is easy to fix by adding a `sandwich` argument in the `Sandwich` branches
for both `Item.toPrice` and `Item.toEmoji`:

```reason
let toPrice =
  fun
  | Sandwich => 10. // [!code --]
  | Sandwich(sandwich) => 10. // [!code ++]
  | Burger => 15.
  | Hotdog => 5.;

let toEmoji =
  fun
  | Sandwich => {js|🥪|js} // [!code --]
  | Sandwich(sandwich) => {js|🥪|js} // [!code ++]
  | Burger => {js|🍔|js}
  | Hotdog => {js|🌭|js};
```

Now there's a different error:

```
File "src/order-confirmation/Item.re", line 13, characters 13-20:
13 |   | Sandwich(sandwich) => 10.
                  ^^^^^^^^
Error (warning 27 [unused-var-strict]): unused variable sandwich.
```

## A valid use of wildcard

OCaml demands that we use variables we declare[^1], unless they begin with
underscore. So we can silence the error by renaming `sandwich` to `_sandwich`:

```reason
| Sandwich(_sandwich) => 10.
```

However, this essentially turns `_sandwich` into a wildcard, which is [generally
a bad idea](/order-confirmation/#wildcard-in-switch-expressions). The reality,
though, is that the warning against using wildcards is more a guideline than a
hard and fast rule. In this case, it's useful to temporarily use wildcards to
silence the compiler errors while we fill in the missing logic for
`Item.toPrice` and `Item.toEmoji`. Still, when the wildcard is meant to be
temporary, we can give it a more attention-grabbing name so it's less likely to
become permanent:

```reason
| Sandwich(_todo_please_use_this_sandwich) => 10.
```

## Update `Item.toPrice` function

The price of a sandwich should vary depending on the type of sandwich received
by `Item.toPrice`[^2]:

<<< Item.re#to-price-nested-switch

Actually, there's no need for the nested switch, as OCaml allows you to pattern
match into nested structures:

<<< Item.re#to-price-flat

## Combining branches in switch

Madame Jellobutter decides to upgrade her portabellos from canned mushrooms to
artisanal mushrooms lovingly grown by mustachioed hipsters on a local farm,
which increases the price of portabello sandwiches to $10. Now portabello and
ham sandwiches are the same price, so you update the `Item.toPrice` function
accordingly:

<<< Item.re#to-price-same-price{3}

We can combine the portabello and ham branches because they return the same
value:

<<< Item.re#to-price-combine{3-4}

Since the outer constructor `Sandwich` is the same in both branches, we can
combine them into a single branch:

<<< Item.re#to-price{3}

The power of pattern matching gives us both clarity and brevity. This is the
OCaml Way!

## Update `Item.toEmoji` function

`Item.toEmoji` should now return a combination of two emojis when it gets a
`Sandwich(_)` argument:

| Sandwich | Emoji |
| -------- | ----- |
| Portabello | 🥪(🍄) |
| Ham | 🥪(🐷) |
| Unicorn | 🥪(🦄) |

We can implement this using `Js.Array.joinWith`:

<<< Item.re#to-emoji-joinwith{3-13}

This touches on the issue of coding style for switch expressions. When possible,
put shorter branches before longer branches:

<<< Item.re#to-emoji-short-branch-first

## `{j||j}` quoted string literal

The approach using `Js.Array.joinWith` works, but readability could be improved
by using a [quoted string literal with the quoted string identifier
`j`](https://melange.re/v2.2.0/communicate-with-javascript/#strings):

<<< Item.re#to-emoji-j-string-literal{5-13}

`{j||j}` quoted string literals are similar to [template literals in
JavaScript](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Template_literals)
except that they can only accept variables, not arbitrary expressions.

Also note that unlike switch expressions, the `fun` syntax does not accept
multi-line expressions in branches unless you add `{}` around them.

## `Printf.sprintf`

The OCaml standard library also provides a type-safe way to do string
interpolation in the form of the
[Printf.sprintf](https://melange.re/v2.2.0/api/re/melange/Stdlib/Printf/index.html#val-sprintf)
function:

<<< Item.re#to-emoji-sprintf

`Printf.sprintf` has a couple of advantages over `{j||j}` quoted string
literals:

- Since it's just a function, you can pass expressions into it
- It supports [conversion
  specifications](https://melange.re/v2.2.0/api/re/melange/Stdlib/Printf/index.html#val-fprintf)
  like `%s`, `%i`, `%d`, etc which concisely handle basic string conversion
  logic for all primitive data types. This can often make your code shorter and
  easier to understand.

## Bundling

An unfortunate downside of the `Printf` module is that using it can add a
surprising amount to your app's production bundle size. Let's generate a bundle
to see just how much of a difference it makes. However, since this is a
[multi-page app](https://vitejs.dev/guide/build#multi-page-app), Vite won't
automatically build bundles for `index.html` files that aren't in the project's
root directory. You must add a little extra configuration to `vite.config.js`:

```javascript{1,12-18}
import { resolve } from 'path'
import { defineConfig } from 'vite'
import { nodeResolve } from '@rollup/plugin-node-resolve'

export default defineConfig({
  plugins: [nodeResolve()],
  server: {
    watch: {
      ignored: ['**/_opam']
    }
  },
  build: {
    rollupOptions: {
      input: {
        order_confirmation: resolve(__dirname, 'src/order-confirmation/index.html'),
      },
    },
  },
});
```

Now run `make bundle` again and you should see some output like this:

```bash{5}
vite v5.0.11 building for production...
✓ 61 modules transformed.
dist/src/order_confirmation/index.html         0.35 kB │ gzip:  0.24 kB
dist/assets/order_confirmation-14ani3dg.css    0.27 kB │ gzip:  0.18 kB
dist/assets/order_confirmation-2_8a7RSk.js   190.13 kB │ gzip: 57.85 kB
✓ built in 1.20s
```

You can see that the JS bundle for the Order Confirmation app is 190 kB. If you
change `Item.toEmoji` back to use a `{j||j}` quoted string literal, the bundle
size will be 144 kB, about 46 kB smaller.

---

Frabjous! The order confirmation widget now supports more types of sandwiches.
In the next chapter, we'll see how to expand the options for burgers.

## Exercises

<b>1.</b> Madame Jellobutter wants to add another type of sandwich and she's
letting you decide what it is. Give it an appropriate price and find the most
suitable emoji(s) for it.

<b>2.</b> Change the logic of `Item.toPrice` so that the new sandwich you just
added is cheaper or more expensive *depending on the day of the week*.

::: details Hint 1

Look at the functions in the
[Js.Date](https://melange.re/v2.2.0/api/re/melange/Js/Date/) module.

:::

::: details Hint 2

Use a `when` guard in the switch expression.

:::

<b>3.</b> The following program uses `{j||j}` quoted string literals for string
interpolation:

```reason
let compute = (a, b) => (a +. 10.) /. b;

// Rewrite using Printf.sprintf, limit result to 3 decimal places
let result = compute(40., 47.) |> string_of_float;
Js.log({j|result to 3 decimal places = $(result)|j});

// Rewrite using Printf.sprintf
let player: Js.t({..}) = {
  "name": "Wilbur",
  "level": 9001234,
  "immortal": false,
};
{
  let name = player##name;
  // bonus: use the flag that makes long numbers more readable
  let level = player##level |> string_of_int;
  let immortal = player##immortal |> string_of_bool;
  Js.log({j|Player: name=$(name), level=$(level), immortal=$(immortal)|j});
};
```

Use `Printf.sprintf` instead and improve the program as suggested by the
comments. You can edit the program interactively in [Melange
Playground](https://melange.re/v2.2.0/playground/?language=Reason&code=bGV0IGNvbXB1dGUgPSAoYSwgYikgPT4gKGEgKy4gMTAuKSAvLiBiOwoKLy8gUmV3cml0ZSB1c2luZyBQcmludGYuc3ByaW50ZiwgbGltaXQgcmVzdWx0IHRvIDMgZGVjaW1hbCBwbGFjZXMKbGV0IHJlc3VsdCA9IGNvbXB1dGUoNDAuLCA0Ny4pIHw%2BIHN0cmluZ19vZl9mbG9hdDsKSnMubG9nKHtqfHJlc3VsdCB0byAzIGRlY2ltYWwgcGxhY2VzID0gJChyZXN1bHQpfGp9KTsKCi8vIFJld3JpdGUgdXNpbmcgUHJpbnRmLnNwcmludGYKbGV0IHBsYXllcjogSnMudCh7Li59KSA9IHsKICAibmFtZSI6ICJXaWxidXIiLAogICJsZXZlbCI6IDkwMDEyMzQsCiAgImltbW9ydGFsIjogZmFsc2UsCn07CnsKICBsZXQgbmFtZSA9IHBsYXllciMjbmFtZTsKICAvLyBib251czogdXNlIHRoZSBmbGFnIHRoYXQgbWFrZXMgbG9uZyBudW1iZXJzIG1vcmUgcmVhZGFibGUKICBsZXQgbGV2ZWwgPSBwbGF5ZXIjI2xldmVsIHw%2BIHN0cmluZ19vZl9pbnQ7IAogIGxldCBpbW1vcnRhbCA9IHBsYXllciMjaW1tb3J0YWwgfD4gc3RyaW5nX29mX2Jvb2w7CiAgSnMubG9nKHtqfFBsYXllcjogbmFtZT0kKG5hbWUpLCBsZXZlbD0kKGxldmVsKSwgaW1tb3J0YWw9JChpbW1vcnRhbCl8an0pOwp9Owo%3D&live=off).

::: details Hint

Consult the [conversion specification
documentation](https://melange.re/v2.2.0/api/re/melange/Stdlib/Printf/index.html#val-fprintf).

:::

## Overview

- Variant constructors can contain arguments
- OCaml's pattern matching syntax makes your code more readable and concise
  - You can match to any level of nesting: `| A(B(C)) => 42`
  - There are many options for combining branches in switch expressions, e.g.
    - `| A => 42 | B => 42` becomes `| A | B => 42`
    - `| A(B) => 42 | A(C) => 42` becomes `| A(B | C) => 42`
- A valid use of wildcards in switch expressions is when you want to silence
  compiler errors while you fill in the logic for new branches in switch
  expressions
- `{j||j}` quoted string literals are similar to JavaScript's template literals,
  but they are not type safe and should be avoided in production code
- `Print.sprintf` is a type-safe function for string interpolation
- For Vite multi-page apps, you must add a little extra configuration to build
  bundles for `index.html` files not in the project's root directory

## Solutions

<b>1.</b> Add a new `Turducken` constructor to variant type `sandwich` and update
the `Item.toPrice` and `Item.toEmoji` functions accordingly:

<<< Item.re#item-turducken

Of course, you could've chosen a completely different sandwich, and that would be
just fine.

<b>2.</b> In the alternate dimension where the turducken sandwich was added,
Madame Jellobutter wants to do a promotion called "Turducken Tuesdays" where
turducken sandwiches become half-price on Tuesdays. To support this generous
discount, `Item.toPrice` needs to be updated like so:

<<< Item.re#to-price-turducken-tuesdays

Note that the `fun` syntax had to be abandoned, because the body of
`Item.toPrice` now consists of more than just a switch expression.

<b>3.</b> After replacing the `{j||j}` quoted string literals with
`Printf.sprintf` and improving it a bit, you might end up with something [like
this](https://melange.re/v2.2.0/playground/?language=Reason&code=bGV0IGNvbXB1dGUgPSAoYSwgYikgPT4gKGEgKy4gMTAuKSAvLiBiOwoKLy8gUmV3cml0ZSB1c2luZyBQcmludGYuc3ByaW50ZiwgbGltaXQgcmVzdWx0IHRvIDMgZGVjaW1hbCBwbGFjZXMKSnMubG9nKAogIFByaW50Zi5zcHJpbnRmKCJyZXN1bHQgdG8gMyBkZWNpbWFsIHBsYWNlcyA9ICUwLjNmIiwgY29tcHV0ZSg0MC4sIDQ3LikpLAopOwoKLy8gUmV3cml0ZSB1c2luZyBQcmludGYuc3ByaW50ZgpsZXQgcGxheWVyOiBKcy50KHsuLn0pID0gewogICJuYW1lIjogIldpbGJ1ciIsCiAgImxldmVsIjogOTAwMTIzNCwKICAiaW1tb3J0YWwiOiBmYWxzZSwKfTsKSnMubG9nKAogIFByaW50Zi5zcHJpbnRmKAogICAgIlBsYXllcjogbmFtZT0lcywgbGV2ZWw9JSNkLCBpbW1vcnRhbD0lQiIsCiAgICBwbGF5ZXIjI25hbWUsCiAgICBwbGF5ZXIjI2xldmVsLAogICAgcGxheWVyIyNpbW1vcnRhbCwKICApLAopOwo%3D&live=off).

-----

View [source
code](https://github.com/melange-re/melange-for-react-devs/blob/main/src/better-sandwiches/)
and [demo](https://react-book.melange.re/demo/src/better-sandwiches/) for this chapter.

-----

[^1]: Technically, undeclared variables produce a warning and it's possible to
  tell OCaml to not treat them as compilation errors. A common way to do this is
  via the [built-in `warning`
  attribute](https://v2.ocaml.org/manual/attributes.html#ss:builtin-attributes).

[^2]: You may be wondering why a unicorn sandwich costs $80. Well, that is just
  the price of ethically-sourced unicorn meat. All the "guests" at the
  Sanctuarium Unicornis are pampered daily with massages, horn polishings, and
  heirloom carrots. Once a week, a 7 kg chunk of fatty meat is extracted
  (essentially liposuction for unicorns), and the wound is healed almost
  instantly by the farm's resident white mage.
