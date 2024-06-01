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

  let discountFunction =
    submittedCode
    |> Option.map(code => Discount.getDiscountFunction(code, date));

  let discount =
    switch (discountFunction) {
    | None
    | Some(Error(_)) => None
    | Some(Ok(discountFunction)) => Some(discountFunction(items))
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
    {switch (discountFunction) {
     | None
     | Some(Ok(_)) => React.null
     | Some(Error(error)) =>
       <div className=Style.codeError>
         {let errorType =
            switch (error) {
            | Discount.InvalidCode => "Invalid"
            | ExpiredCode => "Expired"
            };
          {j|$errorType promo code|j} |> RR.s}
       </div>
     }}
    {switch (discount) {
     | None => React.null
     | Some(Ok(value)) => value |> Float.neg |> string_of_float |> RR.s
     | Some(Error(_code)) =>
       <div className=Style.discountError>
         {RR.s("Todo: discount error message")}
       </div>
     }}
  </form>;
};
