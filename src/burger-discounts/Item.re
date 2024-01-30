module Burger = {
  type t = {
    lettuce: bool,
    onions: int,
    cheese: int,
    tomatoes: bool,
    bacon: int,
  };

  let toEmoji = t => {
    let multiple = (emoji, count) =>
      switch (count) {
      | 0 => ""
      | 1 => emoji
      // todo: Move unicode characters back into format string after Melange 3 is released
      | count => Printf.sprintf({js|%s%s%d|js}, emoji, {js|×|js}, count)
      };

    switch (t) {
    | {lettuce: false, onions: 0, cheese: 0, tomatoes: false, bacon: 0} => {js|🍔|js}
    | {lettuce, onions, cheese, tomatoes, bacon} =>
      // todo: Move unicode characters back into format string after Melange 3 is released
      Printf.sprintf(
        {js|%s{%s}|js},
        {js|🍔|js},
        [|
          lettuce ? {js|🥬|js} : "",
          tomatoes ? {js|🍅|js} : "",
          multiple({js|🧅|js}, onions),
          multiple({js|🧀|js}, cheese),
          multiple({js|🥓|js}, bacon),
        |]
        |> Js.Array.filter(str => str != "")
        |> Js.Array.joinWith(", "),
      )
    };
  };

  let toPrice = ({onions, cheese, tomatoes, bacon, lettuce: _}) => {
    let toppingCost = (quantity, cost) => float_of_int(quantity) *. cost;

    15.  // base cost
    +. toppingCost(onions, 0.2)
    +. toppingCost(cheese, 0.1)
    +. (tomatoes ? 0.05 : 0.0)
    +. toppingCost(bacon, 0.5);
  };
};

module Sandwich = {
  type t =
    | Portabello
    | Ham
    | Unicorn
    | Turducken;

  let toPrice = t => {
    let day = Js.Date.make() |> Js.Date.getDay |> int_of_float;

    switch (t) {
    | Portabello
    | Ham => 10.
    | Unicorn => 80.
    | Turducken when day == 2 => 10.
    | Turducken => 20.
    };
  };

  let toEmoji = t =>
    // todo: Put 🥪 in format string after Melange 3 is released
    // https://github.com/melange-re/melange-for-react-devs/issues/12
    Printf.sprintf(
      "%s(%s)",
      {js|🥪|js},
      switch (t) {
      | Portabello => {js|🍄|js}
      | Ham => {js|🐷|js}
      | Unicorn => {js|🦄|js}
      | Turducken => {js|🦃🦆🐓|js}
      },
    );
};

type t =
  | Sandwich(Sandwich.t)
  | Burger(Burger.t)
  | Hotdog;

let toPrice = t => {
  switch (t) {
  | Sandwich(sandwich) => Sandwich.toPrice(sandwich)
  | Burger(burger) => Burger.toPrice(burger)
  | Hotdog => 5.
  };
};

let toEmoji =
  fun
  | Hotdog => {js|🌭|js}
  | Burger(burger) => Burger.toEmoji(burger)
  | Sandwich(sandwich) => Sandwich.toEmoji(sandwich);
