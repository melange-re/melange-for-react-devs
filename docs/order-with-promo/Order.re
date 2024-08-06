type t = list(Item.t);

module OrderItem = {
  [@react.component]
  let make = (~item as _: Item.t) => <div />;
};

module Style = {
  let order = "";
  let total = "";

  // #region promo-class
  let promo = [%cx
    {|
    border-top: 1px solid gray;
    text-align: right;
    vertical-align: top;
    |}
  ];
  // #endregion promo-class
};

// #region make
[@react.component]
let make = (~items: t, ~date: Js.Date.t) => {
  let (discount, setDiscount) = RR.useStateValue(0.0);

  let subtotal =
    items
    |> ListLabels.fold_left(~init=0., ~f=(acc, order) =>
         acc +. Item.toPrice(order, ~date)
       );

  <table className=Style.order>
    <tbody>
      {items
       |> List.mapi((index, item) =>
            <OrderItem key={"item-" ++ string_of_int(index)} item />
          )
       |> RR.list}
      <tr className=Style.total>
        <td> {RR.s("Subtotal")} </td>
        <td> {subtotal |> RR.currency} </td>
      </tr>
      <tr>
        <td> {RR.s("Promo code")} </td>
        <td> <Promo items date onApply=setDiscount /> </td>
      </tr>
      <tr className=Style.total>
        <td> {RR.s("Total")} </td>
        <td> {subtotal -. discount |> RR.currency} </td>
      </tr>
    </tbody>
  </table>;
};
// #endregion make

let _ =
  (setDiscount, items, date) => {
    <>
      // #region set-promo-class
      <tr className=Style.promo>
        <td> {RR.s("Promo code")} </td>
        <td> <Promo items date onApply=setDiscount /> </td>
      </tr>
      // #endregion set-promo-class
    </>;
  };
