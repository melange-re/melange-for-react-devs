type t =
  | Sandwich
  | Burger
  | Hotdog;

let toPrice =
  fun
  | Sandwich => 10.
  | Burger => 15.
  | Hotdog => 5.;

let toEmoji =
  fun
  | Sandwich => {js|🥪|js}
  | Burger => {js|🍔|js}
  | Hotdog => {js|🌭|js};
