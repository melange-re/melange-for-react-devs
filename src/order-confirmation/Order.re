type t = array(Item.t);

module OrderItem = {
  [@react.component]
  let make = (~item: Item.t) =>
    <tr>
      <td> {item |> Item.toEmoji |> React.string} </td>
      <td> {item |> Item.toPrice |> Format.currency} </td>
    </tr>;
};

[@react.component]
let make = (~items: t) => {
  let total =
    items |> Js.Array.reduce((acc, order) => acc +. Item.toPrice(order), 0.);

  <table>
    <tbody>
      {items
       |> Js.Array.mapi((item, index) =>
            <OrderItem key={"item-" ++ string_of_int(index)} item />
          )
       |> React.array}
      <tr>
        <td> {React.string("Total")} </td>
        <td> {total |> Format.currency} </td>
      </tr>
    </tbody>
  </table>;
};
