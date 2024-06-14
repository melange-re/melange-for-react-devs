type t = list(Item.t);

module OrderItem = {
  module Css = {
    let item = [%cx {|
      border-top: 1px solid lightgray;
    |}];

    let emoji = [%cx {|
      font-size: 2em;
    |}];

    let price = [%cx {|
      text-align: right;
    |}];
  };

  [@react.component]
  let make = (~item: Item.t) =>
    <tr className=Css.item>
      <Td className=Css.emoji> {item |> Item.toEmoji |> RR.s} </Td>
      <Td className=Css.price> {item |> Item.toPrice |> RR.currency} </Td>
    </tr>;
};

module Css = {
  let order = [%cx {|
    border-collapse: collapse;
    |}];

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
         acc +. Item.toPrice(order)
       );

  <table className=Css.order>
    <tbody>
      {items
       |> List.mapi((index, item) =>
            <OrderItem key={"item-" ++ string_of_int(index)} item />
          )
       |> RR.list}
      <tr className=Css.total>
        <Td> {RR.s("Subtotal")} </Td>
        <Td> {subtotal |> RR.currency} </Td>
      </tr>
      <tr className=Css.promo>
        <Td> {RR.s("Promo code")} </Td>
        <Td> <PromoNormalVariant items date onApply=setDiscount /> </Td>
      </tr>
      <tr className=Css.total>
        <Td> {RR.s("Total")} </Td>
        <Td> {subtotal -. discount |> RR.currency} </Td>
      </tr>
    </tbody>
  </table>;
};
