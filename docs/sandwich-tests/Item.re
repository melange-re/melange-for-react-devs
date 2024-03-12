module Sandwich = {
  type t =
    | Portabello
    | Ham
    | Unicorn
    | Turducken;

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

  // #region to-price-with-date
  let toPrice = (~date: Js.Date.t, t) => {
    let day = date |> Js.Date.getDay |> int_of_float;

    switch (t) {
    | Portabello
    | Ham => 10.
    | Unicorn => 80.
    | Turducken when day == 2 => 10.
    | Turducken => 20.
    };
  };
  // #endregion to-price-with-date
};

module Burger = {
  type t = {
    lettuce: bool,
    onions: int,
    cheese: int,
    tomatoes: bool,
    bacon: int,
  };

  let toPrice = _t => 0.;
};

type t =
  | Sandwich(Sandwich.t)
  | Burger(Burger.t)
  | Hotdog;

// #region to-price
let toPrice = t => {
  switch (t) {
  | Sandwich(sandwich) => Sandwich.toPrice(sandwich, ~date=Js.Date.make())
  | Burger(burger) => Burger.toPrice(burger)
  | Hotdog => 5.
  };
};
// #endregion to-price
