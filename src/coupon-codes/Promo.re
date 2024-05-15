module Css = {
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
};

module DiscountLabel = {
  module Css = {
    let error = [%cx {|color: red;|}];

    let success = [%cx {|color: green;|}];

    let buyMore = [%cx {|color: purple|}];
    let buyWhat = [%cx {|text-decoration: underline;|}];
  };

  [@react.component]
  let make = (~discount: result(float, Discount.error('a))) => {
    switch (discount) {
    | Error(InvalidCode) =>
      <div className=Css.error> {RR.s("Invalid code")} </div>
    | Error(ExpiredCode) =>
      <div className=Css.error> {RR.s("Expired code")} </div>
    | Error(Buy(code)) =>
      <div className=Css.buyMore>
        {let buyWhat =
           switch (code) {
           | `OneBurger => "at least 1 more burger"
           | `TwoBurgers => "at least 2 burgers"
           | `MegaBurger => "a burger with every topping"
           | `MissingSandwichTypes(missing) =>
             (missing |> Stdlib.Array.of_list |> Js.Array.join(~sep=", "))
             ++ " sandwiches"
           };

         {j|Buy $buyWhat to enjoy this promo.|j} |> RR.s}
      </div>
    | Ok(0.0) => React.null
    | Ok(discount) =>
      <div className=Css.success> {discount |> Float.neg |> RR.currency} </div>
    };
  };
};

let applyDiscount = (~code, ~date, ~items) => {
  switch (Discount.getDiscountFunction(~code, ~date)) {
  | Error(_) as err => err
  | Ok(discountFunc) =>
    discountFunc(items) |> Result.map_error(err => Discount.Buy(err))
  };
};

[@react.component]
let make = (~items: list(Item.t), ~date: Js.Date.t, ~onApply: float => unit) => {
  let (code, setCode) = RR.useStateValue("");
  let (discount, setDiscount) = RR.useStateValue(Ok(0.0));

  <form
    className=Css.form
    onSubmit={evt => {
      evt |> React.Event.Form.preventDefault;
      let discount = applyDiscount(~code, ~date, ~items);
      setDiscount(discount);
      discount |> Result.iter(onApply);
    }}>
    <input
      className=Css.input
      value=code
      onChange={evt => {
        evt |> RR.getValueFromEvent |> setCode;
        setDiscount(Ok(0.0));
      }}
    />
    <DiscountLabel discount />
  </form>;
};
