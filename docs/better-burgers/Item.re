module ItemV1 = {
  // #region starting-item-t
  type sandwich =
    | Portabello
    | Ham
    | Unicorn
    | Turducken;

  type t =
    | Sandwich(sandwich)
    | Burger
    | Hotdog;
  // #endregion starting-item-t
};

module ItemV2 = {
  type sandwich =
    | Portabello
    | Ham
    | Unicorn
    | Turducken;

  // #region add-burger-type
  type burger = {
    lettuce: bool,
    onions: int,
    cheese: int,
  };

  type t =
    | Sandwich(sandwich)
    | Burger(burger)
    | Hotdog;
  // #endregion add-burger-type

  // #region to-emoji
  let toEmoji =
    fun
    | Hotdog => {js|🌭|js}
    | Sandwich(sandwich) =>
      Printf.sprintf(
        {js|🥪(%s)|js},
        switch (sandwich) {
        | Portabello => {js|🍄|js}
        | Ham => {js|🐷|js}
        | Unicorn => {js|🦄|js}
        | Turducken => {js|🦃🦆🐓|js}
        },
      )
    | Burger(burger) =>
      Printf.sprintf(
        {js|🍔{%s}|js},
        [|
          burger.lettuce ? {js|🥬|js} : "",
          {js|🧅×|js} ++ string_of_int(burger.onions),
          {js|🧀×|js} ++ string_of_int(burger.cheese),
        |]
        |> Js.Array.filter(str => str != "")
        |> Js.Array.joinWith(", "),
      );
  // #endregion to-emoji

  let toEmojiBurgerDestructure =
    fun
    | Hotdog
    | Sandwich(_) => "fake"
    // #region destructure-burger
    | Burger(burger) => {
        let {lettuce, onions, cheese} = burger;
        Printf.sprintf(
          {js|🍔{%s}|js},
          [|
            lettuce ? {js|🥬|js} : "",
            {js|🧅×|js} ++ string_of_int(onions),
            {js|🧀×|js} ++ string_of_int(cheese),
          |]
          |> Js.Array.filter(str => str != "")
          |> Js.Array.joinWith(", "),
        );
      };
  // #endregion destructure-burger

  let toEmojiBurgerDestructureBranch =
    fun
    | Hotdog
    | Sandwich(_) => "fake"
    // #region destructure-burger-branch
    | Burger({lettuce, onions, cheese}) =>
      Printf.sprintf(
        {js|🍔{%s}|js},
        [|
          lettuce ? {js|🥬|js} : "",
          {js|🧅×|js} ++ string_of_int(onions),
          {js|🧀×|js} ++ string_of_int(cheese),
        |]
        |> Js.Array.filter(str => str != "")
        |> Js.Array.joinWith(", "),
      );
  // #endregion destructure-burger-branch

  let toEmojiBurgerMultiple =
    fun
    | Hotdog
    | Sandwich(_) => "fake"
    // #region multiple
    | Burger({lettuce, onions, cheese}) => {
        let multiple = (emoji, count) =>
          Printf.sprintf({js|%s×%d|js}, emoji, count);

        Printf.sprintf(
          {js|🍔{%s}|js},
          [|
            lettuce ? {js|🥬|js} : "",
            multiple({js|🧅|js}, onions),
            multiple({js|🧀|js}, cheese),
          |]
          |> Js.Array.filter(str => str != "")
          |> Js.Array.joinWith(", "),
        );
      };
  // #endregion multiple
};

module ItemV3 = {
  // #region burger-submodule
  module Burger = {
    type t = {
      lettuce: bool,
      onions: int,
      cheese: int,
    };

    let toEmoji = ({lettuce, onions, cheese}) => {
      let multiple = (emoji, count) =>
        Printf.sprintf({js|%s×%d|js}, emoji, count);

      Printf.sprintf(
        {js|🍔{%s}|js},
        [|
          lettuce ? {js|🥬|js} : "",
          multiple({js|🧅|js}, onions),
          multiple({js|🧀|js}, cheese),
        |]
        |> Js.Array.filter(str => str != "")
        |> Js.Array.joinWith(", "),
      );
    };

    let toPrice = _burger => 15.;
  };
  // #endregion burger-submodule

  type sandwich =
    | Portabello
    | Ham
    | Unicorn
    | Turducken;

  // #region t-and-functions
  type t =
    | Sandwich(sandwich)
    | Burger(Burger.t)
    | Hotdog;

  let toEmoji =
    fun
    | Hotdog => {js|🌭|js}
    | Burger(burger) => Burger.toEmoji(burger)
    | Sandwich(sandwich) =>
      Printf.sprintf(
        {js|🥪(%s)|js},
        switch (sandwich) {
        | Portabello => {js|🍄|js}
        | Ham => {js|🐷|js}
        | Unicorn => {js|🦄|js}
        | Turducken => {js|🦃🦆🐓|js}
        },
      );

  let toPrice = t => {
    let day = Js.Date.make() |> Js.Date.getDay |> int_of_float;

    switch (t) {
    | Sandwich(Portabello | Ham) => 10.
    | Sandwich(Unicorn) => 80.
    | Sandwich(Turducken) when day == 2 => 10.
    | Sandwich(Turducken) => 20.
    | Burger(burger) => Burger.toPrice(burger)
    | Hotdog => 5.
    };
  };
  // #endregion t-and-functions

  module BurgerFunctions = {
    type t = {
      lettuce: bool,
      onions: int,
      cheese: int,
    };

    // #region record-wildcard
    let toPrice = ({onions, cheese, _}) =>
      15.  // base cost
      +. float_of_int(onions)
      *. 0.2
      +. float_of_int(cheese)
      *. 0.1;
    // #endregion record-wildcard

    toPrice({lettuce: false, onions: 0, cheese: 0}) |> ignore;

    // #region record-lettuce-wildcard
    let toPrice = ({onions, cheese, lettuce: _}) =>
      15.  // base cost
      +. float_of_int(onions)
      *. 0.2
      +. float_of_int(cheese)
      *. 0.1;
    // #endregion record-lettuce-wildcard

    // #region ternary
    let toEmoji = ({lettuce, onions, cheese}) => {
      let multiple = (emoji, count) =>
        Printf.sprintf({js|%s×%d|js}, emoji, count);

      !lettuce && onions == 0 && cheese == 0
        ? {js|🍔|js}
        : Printf.sprintf(
            {js|🍔{%s}|js},
            [|
              lettuce ? {js|🥬|js} : "",
              multiple({js|🧅|js}, onions),
              multiple({js|🧀|js}, cheese),
            |]
            |> Js.Array.filter(str => str != "")
            |> Js.Array.joinWith(", "),
          );
    };
    // #endregion ternary

    toEmoji({lettuce: false, onions: 0, cheese: 0}) |> ignore;

    // #region match-tuple
    let toEmoji = ({lettuce, onions, cheese}) => {
      let multiple = (emoji, count) =>
        Printf.sprintf({js|%s×%d|js}, emoji, count);

      switch (lettuce, onions, cheese) {
      | (false, 0, 0) => {js|🍔|js}
      | (lettuce, onions, cheese) =>
        Printf.sprintf(
          {js|🍔{%s}|js},
          [|
            lettuce ? {js|🥬|js} : "",
            multiple({js|🧅|js}, onions),
            multiple({js|🧀|js}, cheese),
          |]
          |> Js.Array.filter(str => str != "")
          |> Js.Array.joinWith(", "),
        )
      };
    };
    // #endregion match-tuple

    toEmoji({lettuce: false, onions: 0, cheese: 0}) |> ignore;

    // #region match-record
    let toEmoji = t => {
      let multiple = (emoji, count) =>
        Printf.sprintf({js|%s×%d|js}, emoji, count);

      switch (t) {
      | {lettuce: false, onions: 0, cheese: 0} => {js|🍔|js}
      | {lettuce, onions, cheese} =>
        Printf.sprintf(
          {js|🍔{%s}|js},
          [|
            lettuce ? {js|🥬|js} : "",
            multiple({js|🧅|js}, onions),
            multiple({js|🧀|js}, cheese),
          |]
          |> Js.Array.filter(str => str != "")
          |> Js.Array.joinWith(", "),
        )
      };
    };
    // #endregion match-record
  };
};

// #region sandwich-module
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
    Printf.sprintf(
      {js|🥪(%s)|js},
      switch (t) {
      | Portabello => {js|🍄|js}
      | Ham => {js|🐷|js}
      | Unicorn => {js|🦄|js}
      | Turducken => {js|🦃🦆🐓|js}
      },
    );
};
// #endregion sandwich-module

module BurgerTomatoBacon = {
  // #region to-price-tomato-bacon
  type t = {
    lettuce: bool,
    onions: int,
    cheese: int,
    tomatoes: bool,
    bacon: int,
  };

  let toPrice = ({onions, cheese, tomatoes, bacon, lettuce: _}) =>
    15.  // base cost
    +. float_of_int(onions)
    *. 0.2
    +. float_of_int(cheese)
    *. 0.1
    +. (tomatoes ? 0.05 : 0.0)
    +. float_of_int(bacon)
    *. 0.5;
  // #endregion to-price-tomato-bacon

  toPrice({lettuce: false, onions: 0, cheese: 0, tomatoes: false, bacon: 0})
  |> ignore;

  // #region to-price-topping-cost
  let toPrice = ({onions, cheese, tomatoes, bacon, lettuce: _}) => {
    let toppingCost = (quantity, cost) => float_of_int(quantity) *. cost;

    15.  // base cost
    +. toppingCost(onions, 0.2)
    +. toppingCost(cheese, 0.1)
    +. (tomatoes ? 0.05 : 0.0)
    +. toppingCost(bacon, 0.5);
  };
  // #endregion to-price-topping-cost

  // #region to-emoji-multiple
  let multiple = (emoji, count) =>
    switch (count) {
    | 0 => ""
    | 1 => emoji
    | count => Printf.sprintf({js|%s×%d|js}, emoji, count)
    };
  // #endregion to-emoji-multiple

  multiple("", 0) |> ignore;

  // #region to-emoji-multiple-fun
  let multiple = emoji =>
    fun
    | 0 => ""
    | 1 => emoji
    | count => Printf.sprintf({js|%s×%d|js}, emoji, count);
  // #endregion to-emoji-multiple-fun

  multiple("", 0) |> ignore;

  // #region to-emoji-multiple-fun-annotated
  let multiple: (string, int) => string =
    emoji =>
      fun
      | 0 => ""
      | 1 => emoji
      | count => Printf.sprintf({js|%s×%d|js}, emoji, count);
  // #endregion to-emoji-multiple-fun-annotated
};
