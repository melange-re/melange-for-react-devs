// #region first-version
[@react.component]
let make = () => {
  let (code, setCode) = React.useState(() => "");

  <form onSubmit={evt => evt |> React.Event.Form.preventDefault}>
    <input
      value=code
      onChange={evt => setCode(_ => RR.getValueFromEvent(evt))}
    />
    {RR.s("Todo: Discount value or error")}
  </form>;
};
// #endregion first-version

ignore(make);

// #region use-reducer
[@react.component]
let make = () => {
  let (code, dispatch) =
    React.useReducer((_state, newState) => newState, "");

  <form onSubmit={evt => evt |> React.Event.Form.preventDefault}>
    <input
      value=code
      onChange={evt => dispatch(RR.getValueFromEvent(evt))}
    />
    {RR.s("Todo: Discount value or error")}
  </form>;
};
// #endregion use-reducer

ignore(make);

// #region set-code
[@react.component]
let make = () => {
  let (code, setCode) =
    React.useReducer((_state, newState) => newState, "");

  <form onSubmit={evt => evt |> React.Event.Form.preventDefault}>
    <input
      value=code
      onChange={evt => evt |> RR.getValueFromEvent |> setCode}
    />
    {RR.s("Todo: Discount value or error")}
  </form>;
};
// #endregion set-code

ignore(make);

// #region ignore
[@react.component]
let make = (~items: list(Item.t), ~date: Js.Date.t) => {
  ignore(items);
  ignore(date);
  // #endregion ignore
  <div />;
};

// #region style-submodule
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
};
// #endregion style-submodule

let _ =
  () => {
    let (code, setCode) = RR.useStateValue("");
    // #region updated-jsx
    <form
      className=Style.form
      onSubmit={evt => evt |> React.Event.Form.preventDefault}>
      <input
        className=Style.input
        value=code
        onChange={evt => evt |> RR.getValueFromEvent |> setCode}
      />
      {RR.s("Todo: Discount value or error")}
    </form>;
    // #endregion updated-jsx
  };

[@warning "-27"]
let _ =
  () => {
    // #region submitted-code
    let (code, setCode) = RR.useStateValue("");
    let (submittedCode, setSubmittedCode) = RR.useStateValue(None);

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
      {RR.s("Todo: Discount value or error")}
    </form>;
    // #endregion submitted-code
  };

let _ =
  date => {
    // #region discount-function
    let (submittedCode, setSubmittedCode) = RR.useStateValue(None);

    let discountFunction =
      submittedCode
      |> Option.map(code => Discount.getDiscountFunction(code, date));
    // #endregion discount-function

    ignore(setSubmittedCode);
    ignore(discountFunction);
  };

module Css = {
  let input = "";
};

module Style' = {
  let error = "";
};

let _ =
  () => {
    module Style = Style';
    let discountFunction = Some(Error(Discount.InvalidCode));

    <>
      // #region render-discount-function
      <input /* ... */ />
      {switch (discountFunction) {
       | None
       | Some(Ok(_)) => React.null
       | Some(Error(error)) =>
         <div className=Style.error>
           {let errorType =
              switch (error) {
              | Discount.InvalidCode => "Invalid"
              | ExpiredCode => "Expired"
              };
            {j|$errorType promo code|j} |> RR.s}
         </div>
       }}
      // #endregion render-discount-function
    </>;
  };

let _ = {
  // #region error-class-name
  let error = [%cx {|color: red|}];
  // #endregion error-class-name

  ignore(error);
};
