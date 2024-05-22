module DiscountLabel = {
  module Css = {
    let error = [%cx {|color: red;|}];
  };

  [@react.component]
  let make = (~discount: [> ]) => {
    switch (discount) {
    | `Discount(discount) =>
      <div className=[%cx {|color: green;|}]>
        {discount |> Float.neg |> RR.currency}
      </div>
    | `CodeError(Discount.InvalidCode) =>
      <div className=Css.error> {RR.s("Invalid code")} </div>
    | `CodeError(ExpiredCode) =>
      <div className=Css.error> {RR.s("Expired code")} </div>
    | `DiscountError(code) =>
      <div className=[%cx {|color: purple|}]>
        {RR.s("Buy ")}
        <span className=[%cx {|text-decoration: underline;|}]>
          {(
             switch (code) {
             | `OneBurger => "at least 1 more burger"
             | `TwoBurgers => "at least 2 burgers"
             | `MegaBurger => "a burger with every topping"
             | `MissingSandwichTypes(missing) =>
               (missing |> Stdlib.Array.of_list |> Js.Array.join(~sep=", "))
               ++ " sandwiches"
             }
           )
           |> RR.s}
        </span>
        {RR.s(" to enjoy this promo.")}
      </div>
    };
  };
};

[@react.component]
let make = (~items: list(Item.t), ~date: Js.Date.t, ~onApply: float => unit) => {
  let (code, setCode) = RR.useStateValue("");
  let (submittedCode, setSubmittedCode) = RR.useStateValue(None);

  let getDiscount = code =>
    switch (Discount.getDiscountFunction(~code, ~date)) {
    | Error(err) => `CodeError(err)
    | Ok(discountFunc) =>
      switch (discountFunc(items)) {
      | Error(err) => `DiscountError(err)
      | Ok(value) => `Discount(value)
      }
    };

  <form
    className=[%cx
      {|
      display: flex;
      flex-direction: column;
      align-items: flex-end;
      gap: 0.5em;
      width: 15em;
      |}
    ]
    onSubmit={evt => {
      evt |> React.Event.Form.preventDefault;
      setSubmittedCode(Some(code));
      switch (getDiscount(code)) {
      | `CodeError(_)
      | `DiscountError(_) => ()
      | `Discount(value) => onApply(value)
      };
    }}>
    <input
      className=[%cx
        {|
        width: 8em;
        font-family: monospace;
        text-transform: uppercase;
        |}
      ]
      value=code
      onChange={evt => {
        evt |> RR.getValueFromEvent |> setCode;
        setSubmittedCode(None);
      }}
    />
    {submittedCode
     |> Option.map(getDiscount)
     |> RR.option(discount => <DiscountLabel discount />)}
  </form>;
};
