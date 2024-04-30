type t = list(Item.t);

module OrderItem = {
  [@mel.module "./order-item.module.css"]
  external css: Js.t({..}) = "default";

  [@react.component]
  let make = (~item: Item.t) =>
    <tr className=css##item>
      <td className=css##emoji> {item |> Item.toEmoji |> React.string} </td>
      <td className=css##price> {item |> Item.toPrice |> Format.currency} </td>
    </tr>;
};

[@mel.module "./order.module.css"] external css: Js.t({..}) = "default";

let applyDiscount = (items, code) =>
  switch (code |> Js.String.toLowerCase) {
  | "2burgers1free" => Discount.getFreeBurgers(items)
  | "halfoff" => Discount.getHalfOff(items)
  | _ => None
  };

type discount =
  | NotApplied
  | Applied(float)
  | Error;

[@react.component]
let make = (~items: t) => {
  let (code, setCode) = RR.useStateValue("");
  let (discount, setDiscount) = RR.useStateValue(NotApplied);

  let subtotal =
    items
    |> ListLabels.fold_left(~init=0., ~f=(acc, order) =>
         acc +. Item.toPrice(order)
       );

  <table className=css##order>
    <tbody>
      {items
       |> List.mapi((index, item) =>
            <OrderItem key={"item-" ++ string_of_int(index)} item />
          )
       |> RR.list}
      <tr className=css##total>
        <td> {React.string("Subtotal")} </td>
        <td> {subtotal |> Format.currency} </td>
      </tr>
      <tr className=css##couponCode>
        <td> {React.string("Coupon code")} </td>
        <td>
          <form
            className=css##couponCodeForm
            onSubmit={evt => {
              evt |> React.Event.Form.preventDefault;
              let discount =
                applyDiscount(items, code)
                |> Option.map(value => Applied(value))
                |> Option.value(~default=Error);
              setDiscount(discount);
            }}>
            <input
              className=css##couponCodeInput
              value=code
              onChange={evt => {
                evt |> RR.getValueFromEvent |> setCode;
                setDiscount(NotApplied);
              }}
            />
            {switch (discount) {
             | NotApplied => React.null
             | Error => "Could not apply discount" |> RR.s
             | Applied(discount) => discount |> Float.neg |> Format.currency
             }}
          </form>
        </td>
      </tr>
      <tr className=css##total>
        <td> {React.string("Total")} </td>
        <td> {subtotal |> Format.currency} </td>
      </tr>
    </tbody>
  </table>;
};
