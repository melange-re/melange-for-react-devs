type t = list(Item.t);

module Td = [%styled.td {|
  padding: 0.5em;
|}];

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

  let promoRow = [%cx
    {|
    border-top: 1px solid gray;
    text-align: right;
    vertical-align: top;
    |}
  ];

  let promoInput = [%cx
    {|
    width: 8em;
    font-family: monospace;
    text-transform: uppercase;
    |}
  ];

  let promoForm = [%cx
    {|
    display: flex;
    flex-direction: column;
    align-items: flex-end;
    gap: 0.5em;
    width: 15em;
    |}
  ];

  let promoError = [%cx {|color: red;|}];

  let promoSuccess = [%cx {|color: green;|}];

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
  let (code, setCode) = RR.useStateValue("");
  let (discount, setDiscount) = RR.useStateValue(Some(0.0));

  let subtotal =
    items
    |> ListLabels.fold_left(~init=0., ~f=(acc, order) =>
         acc +. Item.toPrice(order)
       );
  let total = subtotal -. Option.value(~default=0.0, discount);

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
      <tr className=Css.promoRow>
        <Td> {React.string("Coupon code")} </Td>
        <Td>
          <form
            className=Css.promoForm
            onSubmit={evt => {
              evt |> React.Event.Form.preventDefault;
              Discount.applyDiscount(code, items) |> setDiscount;
            }}>
            <input
              className=Css.promoInput
              value=code
              onChange={evt => {
                evt |> RR.getValueFromEvent |> setCode;
                setDiscount(Some(0.0));
              }}
            />
            {switch (discount) {
             | None =>
               <div className=Css.promoError>
                 {"Error processing promo code" |> RR.s}
               </div>
             | Some(0.0) => React.null
             | Some(discount) =>
               <div className=Css.promoSuccess>
                 {discount |> Float.neg |> Format.currency}
               </div>
             }}
          </form>
        </Td>
      </tr>
      <tr className=Css.total>
        <Td> {React.string("Total")} </Td>
        <Td> {total |> Format.currency} </Td>
      </tr>
    </tbody>
  </table>;
};
