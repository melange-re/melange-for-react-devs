module Item = {
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
};

module Format = {
  let currency = _value => React.null;
};

// #region order-item
module OrderItem = {
  [@react.component]
  let make = (~item: Item.t) =>
    <tr className="item">
      <td className="emoji"> {item |> Item.toEmoji |> React.string} </td>
      <td className="price"> {item |> Item.toPrice |> Format.currency} </td>
    </tr>;
};
// #endregion order-item

type t = array(Item.t);

// #region order-make
[@react.component]
let make = (~items: t) => {
  let total =
    items
    |> Js.Array.reduce(~init=0., ~f=(acc, order) =>
         acc +. Item.toPrice(order)
       );

  <table className="order">
    <tbody>
      {items
       |> Js.Array.mapi(~f=(item, index) =>
            <OrderItem key={"item-" ++ string_of_int(index)} item />
          )
       |> React.array}
      <tr className="total">
        <td> {React.string("Total")} </td>
        <td> {total |> Format.currency} </td>
      </tr>
    </tbody>
  </table>;
};
// #endregion order-make

module OrderItemV2 = {
  // #region order-item-css
  module OrderItem = {
    [@mel.module "./order-item.module.css"]
    external css: Js.t({..}) = "default";

    [@react.component]
    let make = (~item: Item.t) =>
      <tr className=css##item>
        <td className=css##emoji> {item |> Item.toEmoji |> React.string} </td>
        <td className=css##price>
          {item |> Item.toPrice |> Format.currency}
        </td>
      </tr>;
  };
  // #endregion order-item-css
};

module OrderV2 = {
  // #region order-external
  [@mel.module "./order.module.css"] external css: Js.t({..}) = "default";

  [@react.component]
  let make = (~items: t) => {
    let total =
      items
      |> Js.Array.reduce(~init=0., ~f=(acc, order) =>
           acc +. Item.toPrice(order)
         );

    <table className=css##order>
      <tbody>
        {items
         |> Js.Array.mapi(~f=(item, index) =>
              <OrderItem key={"item-" ++ string_of_int(index)} item />
            )
         |> React.array}
        <tr className=css##total>
          <td> {React.string("Total")} </td>
          <td> {total |> Format.currency} </td>
        </tr>
      </tbody>
    </table>;
  };
  // #endregion order-external
};
