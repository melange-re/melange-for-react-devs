type t = list(Item.t);

module OrderItem = {
  module Style = {
    let item = [%cx {|border-top: 1px solid lightgray;|}];
    let emoji = [%cx {|font-size: 2em;|}];
    let price = [%cx {|text-align: right;|}];
  };

  [@react.component]
  let make = (~item: Item.t) =>
    <tr className=Style.item>
      <td className=Style.emoji> {item |> Item.toEmoji |> RR.s} </td>
      <td className=Style.price> {item |> Item.toPrice |> RR.currency} </td>
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
};

[@react.component]
let make = (~items: t) => {
  let total =
    items
    |> ListLabels.fold_left(~init=0., ~f=(acc, order) =>
         acc +. Item.toPrice(order)
       );

  <table className=Style.order>
    <tbody>
      {items
       |> List.mapi((index, item) =>
            <OrderItem key={"item-" ++ string_of_int(index)} item />
          )
       |> RR.list}
      <tr className=Style.total>
        <td> {RR.s("Total")} </td>
        <td> {total |> RR.currency} </td>
      </tr>
    </tbody>
  </table>;
};
