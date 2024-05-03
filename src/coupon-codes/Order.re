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
      <Td className=Css.emoji> {item |> Item.toEmoji |> React.string} </Td>
      <Td className=Css.price> {item |> Item.toPrice |> Format.currency} </Td>
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
};

[@react.component]
let make = (~items: t) => {
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
        <Td> {React.string("Subtotal")} </Td>
        <Td> {subtotal |> Format.currency} </Td>
      </tr>
      <Promo items onApply=setDiscount />
      <tr className=Css.total>
        <Td> {React.string("Total")} </Td>
        <Td> {subtotal -. discount |> Format.currency} </Td>
      </tr>
    </tbody>
  </table>;
};
