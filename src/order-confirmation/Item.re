[%%mel.raw {|import "./item.css"|}];

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

[@react.component]
let make = (~item: t) =>
  <tr className="item">
    <td className="emoji"> {item |> toEmoji |> React.string} </td>
    <td className="price"> {item |> toPrice |> Format.currency} </td>
  </tr>;
