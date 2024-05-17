module DiscountLabel = {
  module Css = {
    let error = [%cx {|color: red;|}];
  };

  [@react.component]
  let make = (~discount: result(float, Discount.error('a))) => {
    switch (discount) {
    | Error(InvalidCode) =>
      <div className=Css.error> {RR.s("Invalid code")} </div>
    | Error(ExpiredCode) =>
      <div className=Css.error> {RR.s("Expired code")} </div>
    | Error(Buy(code)) =>
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
    | Ok(0.0) => React.null
    | Ok(discount) =>
      <div className=[%cx {|color: green;|}]>
        {discount |> Float.neg |> RR.currency}
      </div>
    };
  };
};

let applyDiscount_switch = (~code, ~date, ~items) => {
  switch (Discount.getDiscountFunction(~code, ~date)) {
  | Error(_) as err => err
  | Ok(discountFunc) =>
    switch (discountFunc(items)) {
    | Ok(_) as ok => ok
    | Error(err) => Error(Discount.Buy(err))
    }
  };
};

let applyDiscount_map_error = (~code, ~date, ~items) => {
  switch (Discount.getDiscountFunction(~code, ~date)) {
  | Error(_) as err => err
  | Ok(discountFunc) =>
    discountFunc(items) |> Result.map_error(err => Discount.Buy(err))
  };
};

let applyDiscount_bind = (~code, ~date, ~items) => {
  Result.bind(Discount.getDiscountFunction(~code, ~date), discountFunc =>
    discountFunc(items) |> Result.map_error(err => Discount.Buy(err))
  );
};

let resultFlatMap = (fn, result) => Result.bind(result, fn);

let applyDiscount = (~code, ~date, ~items) => {
  Discount.getDiscountFunction(~code, ~date)
  |> resultFlatMap(discountFunc =>
       discountFunc(items) |> Result.map_error(err => Discount.Buy(err))
     );
};

let ( let* ) = Result.bind;

let applyDiscount_let = (~code, ~date, ~items) => {
  let* discountFunc = Discount.getDiscountFunction(~code, ~date);
  discountFunc(items) |> Result.map_error(err => Discount.Buy(err));
};

[@react.component]
let make = (~items: list(Item.t), ~date: Js.Date.t, ~onApply: float => unit) => {
  let (code, setCode) = RR.useStateValue("");
  let (discount, setDiscount) = RR.useStateValue(Ok(0.0));

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
      let discount = applyDiscount(~code, ~date, ~items);
      setDiscount(discount);
      discount |> Result.iter(onApply);
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
        setDiscount(Ok(0.0));
      }}
    />
    <DiscountLabel discount />
  </form>;
};
