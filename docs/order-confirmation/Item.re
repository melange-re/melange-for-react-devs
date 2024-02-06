// #region type-t
type t =
  | Sandwich
  | Burger;
// #endregion type-t

let _ = {
  // #region to-price
  let toPrice = t =>
    switch (t) {
    | Sandwich => 10.
    | Burger => 15.
    };
  // #endregion to-price
  toPrice(Sandwich) |> ignore;
};

// #region to-price-fun
let toPrice =
  fun
  | Sandwich => 10.
  | Burger => 15.;
// #endregion to-price-fun

// #region to-emoji
let toEmoji =
  fun
  | Sandwich => {js|🥪|js}
  | Burger => {js|🍔|js};
// #endregion to-emoji

// #region make
[@react.component]
let make = (~item: t) =>
  <tr>
    <td> {item |> toEmoji |> React.string} </td>
    <td>
      {item |> toPrice |> Js.Float.toFixed(~digits=2) |> React.string}
    </td>
  </tr>;
// #endregion make

module AddHotdog = {
  // #region hotdog
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
  // #endregion hotdog
};
