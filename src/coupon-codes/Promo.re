module Css = {
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
};

[@react.component]
let make = (~items: list(Item.t), ~onApply: float => unit) => {
  let (code, setCode) = RR.useStateValue("");
  let (discount, setDiscount) = RR.useStateValue(Some(0.0));

  <tr className=Css.promoRow>
    <Td> {React.string("Promo code")} </Td>
    <Td>
      <form
        className=Css.promoForm
        onSubmit={evt => {
          evt |> React.Event.Form.preventDefault;
          let discount = Discount.applyDiscount(code, items);
          setDiscount(discount);
          discount |> Option.iter(onApply);
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
  </tr>;
};
