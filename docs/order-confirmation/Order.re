// #region order
type t = array(Item.t);

[@react.component]
let make = (~items: t) => {
  let total =
    items |> Js.Array.reduce((acc, order) => acc +. Item.toPrice(order), 0.);

  <table>
    <tbody>
      {items |> Js.Array.map(item => <Item item />) |> React.array}
      <tr>
        <td> {React.string("Total")} </td>
        <td>
          {total |> Js.Float.toFixedWithPrecision(~digits=2) |> React.string}
        </td>
      </tr>
    </tbody>
  </table>;
};
// #endregion order

let _ = {
  let items = [||];
  // #region mapi
  items
  |> Js.Array.mapi((item, index) =>
       <Item key={"item-" ++ string_of_int(index)} item />
     )
  |> React.array
  // #endregion mapi
  |> ignore;
};

let _ = {
  let items = [||];
  // #region order-make-item-rows
  let total =
    items |> Js.Array.reduce((acc, order) => acc +. Item.toPrice(order), 0.);

  let itemRows: array(React.element) =
    items |> Js.Array.map(item => <Item item />);

  <table>
    <tbody>
      {itemRows |> React.array}
      <tr>
        <td> {React.string("Total")} </td>
        <td>
          {total |> Js.Float.toFixedWithPrecision(~digits=2) |> React.string}
        </td>
      </tr>
    </tbody>
  </table>;
  // #endregion order-make-item-rows
};
