type t = list(Item.t);

module Format = {
  let currency = _ => React.null;
};

module OrderItem = {
  [@react.component]
  let make = (~item as _: Item.t) => <div />;
};

let css = {"order": "", "total": ""};

// #region make
[@react.component]
let make = (~items: t) => {
  let total =
    items
    |> ListLabels.fold_left(~init=0., ~f=(acc, order) =>
         acc +. Item.toPrice(order)
       );

  <table className=css##order>
    <tbody>
      {items
       |> List.mapi((index, item) =>
            <OrderItem key={"item-" ++ string_of_int(index)} item />
          )
       |> Stdlib.Array.of_list
       |> React.array}
      <tr className=css##total>
        <td> {React.string("Total")} </td>
        <td> {total |> Format.currency} </td>
      </tr>
    </tbody>
  </table>;
};
// #endregion make
