# Styling with CSS

The restaurant website's order confirmation widget is far from complete, but
Madame Jellobutter insists that you make the widget less ugly before you do
anything else. In previous chapters, we saw how to add styles using the `style`
prop, but let's see how to do it with good old CSS.

## Update webpack config

In order to use CSS from our ReasonReact components, we'll install the
`style-loader` and `css-loader` webpack plugins[^1]:

```
npm install --save-dev css-loader style-loader
```

Add a new rule to `webpack.config.js` so that these plugins will be applied to
your `.css` files:

```javascript{5-12}
module.exports = {
  devServer: {
    historyApiFallback: true,
  },
  module: {
    rules: [
      {
        test: /\.css$/i,
        use: ["style-loader", "css-loader"],
      },
    ],
  },
};
```

## Add the first CSS file

Add a new file
`src/order-confirmation/order-item.css` and give it these styles:

<!-- This file gets renamed to order-item.module.css later -->
<<< @/../src/styling-with-css/order-item.module.css

## Import using `mel.raw`

In OCaml, there is no syntax to import from files, because all modules within a
project are visible to all other modules[^2]. However, we can make use of
JavaScript's `import` syntax by using the [mel.raw extension
node](https://melange.re/v2.1.0/communicate-with-javascript/#generate-raw-javascript),
which allows us to embed raw JavaScript in our OCaml code. Add the following
line to the top of `Order.re`:

```reason
[%%mel.raw {|import "./order-item.css"|}];
```

The `{||}` string doesn't require you to escape characters like double-quote and
newline. They are similar to the `{js||js}` strings we first saw in the [Celsius
Converter](/celsius-converter-exception/#solutions) chapter, with the
difference that they won't handle Unicode correctly.

Unfortunately, in the terminal where we're running `make serve`, we see this
webpack compilation error:

```
ERROR in ./_build/default/src/order-confirmation/output/src/order-confirmation/Order.js 7:0-8:1
Module not found: Error: Can't resolve './order-item.css' in '~/melange-for-react-devs/_build/default/src/order-confirmation/output/src/order-confirmation'
```

## Tell Dune to copy CSS files

The problem is that webpack is serving the app from the build directory at
`_build/default/src/order-confirmation/output/src/order-confirmation`, and the
`order-item.css` file isn't in the build directory.

To solve this, we can add the [runtime_deps
field](https://melange.re/v2.1.0/build-system/#handling-assets) to our
`melange.emit` stanza in `src/order-confirmation/dune`:

```clj{7}
(melange.emit
 (target output)
 (libraries reason-react)
 (preprocess
  (pps melange.ppx reason-react-ppx))
 (module_systems es6)
 (runtime_deps order-item.css))
```

We also want to add styles for the `Order` component, so add a new file
`src/order-confirmation/order.css` with these styles:

<!-- This file gets renamed to order.module.css later -->
<<< @/../src/styling-with-css/order.module.css

To ensure that `order.css` is also copied to the build directory, we can add
`order.css` to the value of `runtime_deps`:

```clj
(runtime_deps item.css order.css)
```

If you have many `.css` files, you can just tell `runtime_deps` to copy all
`.css` files over:

```clj
(runtime_deps (glob_files *.css))
```

## Add classes to JSX

Now we can add the appropriate classes to `OrderItem.make`'s JSX:

<<< Order.re#order-item{4-6}

As well as `Order.make`'s JSX:

<<< Order.re#make{6,13}

Finally, add a `mel.raw` extension node at the top of `Order.re`:

```reason
[%%mel.raw {|import "./order.css"|}];
```

## Problems with `mel.raw`

This solution works well for our current build configuration, but falls apart if
we change the `module_systems` field of the `melange.emit` stanza from `es6` to
`commonjs`. This results in a subtle runtime error caused by the fact
that CommonJS uses `require` instead of `import` to import modules.

Another issue is that you can't easily use `mel.raw` extension nodes in
submodules.

The `mel.raw` extension node is unsafe, but it is still useful for prototyping.
Fortunately, Melange provides an official and more reliable way to import
frontend assets.

## Import using `external`

At the top of `Order.re`, replace our first `mel.raw` extension node with an
[external](https://melange.re/v2.2.0/communicate-with-javascript/#external-functions)
declaration:

```reason
[%%mel.raw {|import "./order-item.css"|}]; // [!code --]
[@mel.module "./order-item.css"] external _css: unit = "default"; // [!code ++]
```

This essentially tells OCaml to assign the default export of the
`order-item.css` module to the variable `_css`. The generated JavaScript looks
like this:

```javascript
import OrderItemCss from "./order-item.css";
var _css;
```

Let's break down the individual parts of the `external` declaration:

```reason
[@mel.module "./order-item.css"] external _css: unit = "default";
```

- [mel.module](https://melange.re/v2.2.0/communicate-with-javascript/#using-functions-from-other-javascript-modules)
  is an
  [attribute](https://melange.re/v2.2.0/communicate-with-javascript/#attributes)
  that tells the `external` declaration which module to import from
- The `external` keyword tells OCaml this is a declaration for a value defined
  outside of OCaml, i.e. it comes from JavaScript
- `_css: unit` means the object we get back from the import is named `_css` and
  has type `unit`. We put an underscore in front of the name because we don't
  intend to use this variable. Likewise, we give it a type of
  [unit](https://reasonml.github.io/docs/en/overview#unit) because it doesn't
  have a meaningful value.
- The `"default"` at the end tells OCaml to import the default export of the
  module.

::: tip

A quick way to check what an `external` declaration compiles to is to use
the [Melange Playground](https://melange.re/v2.2.0/playground/). For example,
here's a
[link](https://melange.re/v2.2.0/playground/?language=Reason&code=W0BtZWwubW9kdWxlICIuL29yZGVyLWl0ZW0uY3NzIl0gZXh0ZXJuYWwgX2NzczogdW5pdCA9ICJkZWZhdWx0Ijs%3D&live=off)
to the `external` declaration we just added.

:::

## Use CSS modules

Right now, the classes defined in the CSS files we're importing are in the
global scope. For non-trivial projects, it's better to use [CSS
modules](https://css-tricks.com/css-modules-part-1-need/), which give us access
to locally scoped classes.

First, rename `order-item.css` to `order-item.module.css`, which turns it into a
CSS module. Then change the corresponding `external` declaration:

```reason
[@mel.module "./order-item.css"] external _css: unit = "default"; // [!code --]
[@mel.module "./order-item.module.css"] external css: Js.t({..}) = "default"; // [!code ++]
```

There are three changes of note:

- We change the payload of the `mel.module` attribute to
  `./order-item.module.css` to reflect the new name of the file
- We rename the `_css` variable to `css`, since we intend to use the variable
  later
- We change the type of `css` from `unit` to `Js.t({..})`[^3]

If you look at your compiled app in the browser right now, you'll see that this
change breaks the styles, because the classes defined in `order-item.module.css`
can no longer be accessed by the names we originally gave them. To access the
locally-scoped classes, we must refactor the `OrderItem` component so that it
accesses the class names through the `css` variable:

<<< Order.re#order-item-css{7-9}

Recall that `##` is the access operator for `Js.t` objects, so
`className=css##item` is equivalent to `className={css.item}` in JavaScript.
Note that we also moved the `external` declaration for `./order-item.module.css`
inside the `OrderComponent` module[^4], since that's the only place it's used.

We have not seen the last of `external` declarations, as they are the primary
way in which OCaml interacts with code written in JavaScript. See the [Melange
docs](https://melange.re/v2.2.0/communicate-with-javascript/#external-functions)
for more details.

## Class names must be the same

The class names you use in `.css` files must be the same as the ones you use in
your `.re` files. Try changing `css##emoji` in the `OrderItem` component to
`css##emojis`.

What happens is that the styling for emojis silently breaks. This is the
weakness of the CSS module approach, which requires that you manually keep all
class names in sync. In a [future chapter](/todo), we'll introduce a type-safe
approach to styling that doesn't have this problem.

---

Excelsior! Madame Jellobutter likes how the order confirmation widget looks so
far. But she plans to add more options for her current menu items, for example
she'd like to have more than one type of sandwich. We'll tackle that in the next
chapter.

## Exercises


<b>1.</b> Extension nodes like `mel.raw` can also be prefixed with `%` instead
of `%%`. What happens if you replace `%%mel.raw` with `%mel.raw`?

<b>2.</b> Refactor the `Order` component so that it also uses an `external`
declaration instead of `mel.raw`.

<b>3.</b> Replace your usage of `mel.module` with `bs.module`. What happens?

## Overview

- The `mel.raw` extension node embeds raw JavaScript inside OCaml code. Use with
  caution.
- The `runtime_deps` field of `melange.emit` copies assets like `.css` files to
  the build directory.
- `external` declarations are used to import CSS or JS files.

## Solutions

<b>1.</b> Change the `%%mel.raw` to `%mel.raw` will cause a compile error in
webpack because the generated JS code changes to

```
((import "./order.css"));
```

which isn't valid JavaScript syntax. Changing it back to `%%mel.raw` will
produce syntactically valid JS:

```javascript
import "./order.css"
;
```

The general rule is that you should use `%%mel.raw` for statements, and
`%mel.raw` for expressions.

<b>2.</b> After you refactor the `Order` component to use an `external`
declaration, it should look something like this:

<<< Order.re#order-external

<b>3.</b> If you replace `mel.modlue` with `bs.module`, your code will continue
to compile but you get a morning from Melange:

```
File "src/styling-with-css/Order.re", line 4, characters 4-13:
Alert deprecated: The `[@bs.*]' attributes are deprecated and will be removed in the
next release.
Use `[@mel.*]' instead.
```

Basically, `bs.module` was the old name for the attribute, but it has been
replaced by `mel.module`. This is worth mentioning because there's still a
decent amount of code out in the wild that uses `bs.module`.

-----

[Source code for this
chapter](https://github.com/melange-re/melange-for-react-devs/blob/main/src/styling-with-css/)
can be found in the [Melange for React Developers
repo](https://github.com/melange-re/melange-for-react-devs).

[^1]: The `css-loader` plugin allows you to import CSS files and the
    `style-loader` plugin injects imported CSS into the DOM.

[^2]: Recall that in the `Index` modules you've written so far, you've never had
    to import any of the components you used that were defined in other files.

[^3]: ` Js.t({..})` is a `Js.t` object, which we [first
    encountered](/celsius-converter-exception/#js-t-object) in the Celsius
    Converter chapter.
