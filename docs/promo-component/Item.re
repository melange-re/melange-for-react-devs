module Burger = {
  type t = {
    lettuce: bool,
    onions: int,
    cheese: int,
    tomatoes: bool,
    bacon: int,
  };
};

module Sandwich = {
  type t =
    | Portabello
    | Ham
    | Unicorn
    | Turducken;
};

type t =
  | Sandwich(Sandwich.t)
  | Burger(Burger.t)
  | Hotdog;

let toPrice = (_t: t) => 0.;

let toEmoji = (_t: t) => "";
