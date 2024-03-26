type t = array(Item.t);

module OrderItem = {
  [@mel.module "./order-item.module.css"]
  external css: Js.t({..}) = "default";

  [@react.component]
  let make = (~item: Item.t) =>
    <tr className=css##item>
      <td className=css##emoji> {item |> Item.toEmoji |> React.string} </td>
      <td className=css##price> {item |> Item.toPrice |> Format.currency} </td>
    </tr>;
};

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
