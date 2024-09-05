type t = list(Item.t);

module OrderItem = {
  module Style = {
    let item = [%cx {|border-top: 1px solid lightgray;|}];
    let emoji = [%cx {|font-size: 2em;|}];
    let price = [%cx {|text-align: right;|}];
  };

  [@react.component]
  let make = (~item: Item.t, ~date: Js.Date.t) =>
    <tr className=Style.item>
      <td className=Style.emoji> {item |> Item.toEmoji |> RR.s} </td>
      <td className=Style.price>
        {item |> Item.toPrice(~date) |> RR.currency}
      </td>
    </tr>;
};

module Style = {
  let order = [%cx
    {|
    border-collapse: collapse;

    td {
      padding: 0.5em;
    }
    |}
  ];

  let total = [%cx
    {|
    border-top: 1px solid gray;
    font-weight: bold;
    text-align: right;
    |}
  ];

  let promo = [%cx
    {|
    border-top: 1px solid gray;
    text-align: right;
    vertical-align: top;
  |}
  ];
};

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
            <OrderItem key={"item-" ++ string_of_int(index)} item date />
          )
       |> RR.list}
      <tr className=Style.total>
        <td> {RR.s("Subtotal")} </td>
        <td> {subtotal |> RR.currency} </td>
      </tr>
      <tr className=Style.promo>
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
