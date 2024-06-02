module Style = {
  let form = [%cx {|
    display: flex;
    flex-direction: column;
  |}];

  let input = [%cx
    {|
    font-family: monospace;
    text-transform: uppercase;
    |}
  ];

  let codeError = [%cx {|color: red|}];

  let discountError = [%cx {|color: purple|}];
};

[@react.component]
let make = (~items: list(Item.t), ~date: Js.Date.t) => {
  let (code, setCode) = RR.useStateValue("");
  let (submittedCode, setSubmittedCode) = RR.useStateValue(None);

  let discount =
    switch (submittedCode) {
    | None => `NoSubmittedCode
    | Some(code) =>
      switch (Discount.getDiscountFunction(code, date)) {
      | Error(error) => `CodeError(error)
      | Ok(discountFunction) =>
        switch (discountFunction(items)) {
        | Error(error) => `DiscountError(error)
        | Ok(value) => `Discount(value)
        }
      }
    };

  <form
    className=Style.form
    onSubmit={evt => {
      evt |> React.Event.Form.preventDefault;
      setSubmittedCode(Some(code));
    }}>
    <input
      className=Style.input
      value=code
      onChange={evt => {
        evt |> RR.getValueFromEvent |> setCode;
        setSubmittedCode(None);
      }}
    />
    {switch (discount) {
     | `NoSubmittedCode => React.null
     | `Discount(discount) => discount |> Float.neg |> RR.currency
     | `CodeError(error) =>
       <div className=Style.codeError>
         {let errorType =
            switch (error) {
            | Discount.InvalidCode => "Invalid"
            | ExpiredCode => "Expired"
            };
          {j|$errorType promo code|j} |> RR.s}
       </div>
     | `DiscountError(code) =>
       let buyWhat =
         switch (code) {
         | `NeedOneBurger => "at least 1 more burger"
         | `NeedTwoBurgers => "at least 2 burgers"
         | `NeedMegaBurger => "a burger with every topping"
         | `MissingSandwichTypes => "every sandwich"
         };
       <div className=Style.discountError>
         {RR.s({j|Buy $buyWhat to enjoy this promotion|j})}
       </div>;
     }}
  </form>;
};
