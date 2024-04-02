module Burger = {
  type t = {
    lettuce: bool,
    onions: int,
    cheese: int,
    tomatoes: bool,
    bacon: int,
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
};

type t =
  | Sandwich(Sandwich.t)
  | Burger(Burger.t)
  | Hotdog;

let toPrice = t => {
  switch (t) {
  | Sandwich(sandwich) => Sandwich.toPrice(sandwich, ~date=Js.Date.make())
  | Burger(burger) => Burger.toPrice(burger)
  | Hotdog => 5.
  };
};
