module Css = {
  let promo = [%cx
    {|
    border-top: 1px solid gray;
    text-align: right;
    vertical-align: top;
    |}
  ];

  let input = [%cx
    {|
    width: 8em;
    font-family: monospace;
    text-transform: uppercase;
    |}
  ];

  let form = [%cx
    {|
    display: flex;
    flex-direction: column;
    align-items: flex-end;
    gap: 0.5em;
    width: 15em;
    |}
  ];

  let error = [%cx {|color: red;|}];

  let success = [%cx {|color: green;|}];
};

[@react.component]
let make = (~items: list(Item.t), ~onApply: float => unit) => {
  let (code, setCode) = RR.useStateValue("");
  let (discount, setDiscount) = RR.useStateValue(Some(0.0));

  <tr className=Css.promo>
    <Td> {RR.s("Promo code")} </Td>
    <Td>
      <form
        className=Css.form
        onSubmit={evt => {
          evt |> React.Event.Form.preventDefault;
          let discount =
            Discount.applyDiscount(~code, ~items, ~date=Js.Date.make());
          setDiscount(discount);
          discount |> Option.iter(onApply);
        }}>
        <input
          className=Css.input
          value=code
          onChange={evt => {
            evt |> RR.getValueFromEvent |> setCode;
            setDiscount(Some(0.0));
          }}
        />
        {switch (discount) {
         | None =>
           <div className=Css.error>
             {RR.s("Error processing promo code")}
           </div>
         | Some(0.0) => React.null
         | Some(discount) =>
           <div className=Css.success>
             {discount |> Float.neg |> RR.currency}
           </div>
         }}
      </form>
    </Td>
  </tr>;
};
