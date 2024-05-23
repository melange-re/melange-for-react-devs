module Css = {
  let form = [%cx
    {|
    display: flex;
    flex-direction: column;
    align-items: flex-end;
    gap: 0.5em;
    width: 15em;
    |}
  ];
  let input = [%cx
    {|
    width: 8em;
    font-family: monospace;
    text-transform: uppercase;
    |}
  ];
  let error = [%cx {|color: red;|}];
};

[@react.component]
let make = (~items: list(Item.t), ~date: Js.Date.t) => {
  let (code, setCode) = RR.useStateValue("");
  let (submittedCode, setSubmittedCode) = RR.useStateValue(None);

  let discount =
    switch (submittedCode) {
    | None => `NoSubmittedCode
    | Some(code) =>
      switch (Discount.getDiscountFunction(~code, ~date)) {
      | Error(error) => `CodeError(error)
      | Ok(discountFunc) =>
        switch (discountFunc(items)) {
        | Error(error) => `DiscountError(error)
        | Ok(value) => `Discount(value)
        }
      }
    };

  <form
    className=Css.form
    onSubmit={evt => {
      evt |> React.Event.Form.preventDefault;
      setSubmittedCode(Some(code));
    }}>
    <input
      className=Css.input
      value=code
      onChange={evt => {
        evt |> RR.getValueFromEvent |> setCode;
        setSubmittedCode(None);
      }}
    />
    {switch (discount) {
     | `NoSubmittedCode => React.null
     | `Discount(discount) =>
       <div className=[%cx {|color: green;|}]>
         {discount |> Float.neg |> RR.currency}
       </div>
     | `CodeError(error) =>
       <div className=[%cx {|color: red|}]>
         {let errorType =
            switch (error) {
            | Discount.InvalidCode => "Invalid"
            | ExpiredCode => "Expired"
            };
          {j|$errorType promo code|j} |> RR.s}
       </div>
     | `DiscountError(_code) =>
       <div className=[%cx {|color: purple|}]>
         {RR.s("Todo: discount error message")}
       </div>
     }}
  </form>;
};
