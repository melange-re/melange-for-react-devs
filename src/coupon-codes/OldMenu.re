// [@mel.module "./Menu.jsx"] [@react.component]
// external make: unit => React.element = "default";

type menuItem('a) = {
  options: option('a),
  quantity: int,
};

type cartItem = {
  item: Item.t,
  quantity: int,
};

type state = {
  hotdogQty: int,
  sandwich: menuItem(Item.Sandwich.t),
  burger: menuItem(Item.Burger.t),
  cart: list(cartItem),
};

module Row = {
  [@react.component]
  let make =
      (
        ~title,
        ~quantity,
        ~enabled,
        ~onQuantityChange: int => unit,
        ~onAdd: unit => unit,
        ~children=React.null,
      ) =>
    <tr>
      <td> {React.string(title)} </td>
      <td>
        <input
          type_="number"
          value={quantity |> string_of_int}
          onChange={evt => {
            evt
            |> RR.getValueFromEvent
            |> int_of_string_opt
            |> Option.iter(value =>
                 if (value > 0) {
                   onQuantityChange(value);
                 }
               )
          }}
        />
      </td>
      <td> children </td>
      <td>
        <button disabled={!enabled} onClick={_ => onAdd()}>
          {React.string("Add")}
        </button>
      </td>
    </tr>;
};

module SandwichSelect = {
  [@react.component]
  let make =
      (
        ~value: option(Item.Sandwich.t),
        ~onChange: option(Item.Sandwich.t) => unit,
      ) => {
    <select
      onChange={evt => {
        let sandwich =
          switch (RR.getValueFromEvent(evt)) {
          | "Portabello" => Some(Item.Sandwich.Portabello)
          | "Ham" => Some(Ham)
          | "Unicorn" => Some(Unicorn)
          | "Turducken" => Some(Turducken)
          | _ => None
          };

        onChange(sandwich);
      }}>
      <option value="" selected={value == None}>
        {"Select a sandwich" |> RR.s}
      </option>
      <option value="Portabello" selected={value == Some(Portabello)}>
        {"Portabello" |> RR.s}
      </option>
      <option value="Ham" selected={value == Some(Ham)}>
        {"Ham" |> RR.s}
      </option>
      <option value="Unicorn" selected={value == Some(Unicorn)}>
        {"Unicorn" |> RR.s}
      </option>
      <option value="Turducken" selected={value == Some(Turducken)}>
        {"Turducken" |> RR.s}
      </option>
    </select>;
  };
};

[@react.component]
let make = () => {
  // let (hotdogQty, setHotdogQty) = RR.useStateValue(1);
  // let (sandwich, setSandwich) = RR.useStateValue({options: None, })

  let (state, setState) =
    RR.useStateValue({
      hotdogQty: 1,
      sandwich: {
        options: None,
        quantity: 1,
      },
      burger: {
        options: None,
        quantity: 1,
      },
      cart: [],
    });

  <div>
    <h1> {React.string("Menu")} </h1>
    <table>
      <tbody>
        <Row
          title="Hotdog"
          quantity={state.hotdogQty}
          enabled=true
          onQuantityChange={value => setState({...state, hotdogQty: value})}
          onAdd={() =>
            setState({
              ...state,
              cart: state.cart @ [{item: Hotdog, quantity: state.hotdogQty}],
            })
          }
        />
        <Row
          title="Sandwich"
          quantity={state.sandwich.quantity}
          enabled={state.sandwich.options != None}
          onQuantityChange={quantity => {
            let sandwich = {...state.sandwich, quantity};
            setState({...state, sandwich});
          }}
          onAdd={() =>
            state.sandwich.options
            |> Option.iter(sandwich =>
                 setState({
                   ...state,
                   cart:
                     state.cart
                     @ [
                       {
                         item: Sandwich(sandwich),
                         quantity: state.sandwich.quantity,
                       },
                     ],
                 })
               )
          }>
          {"Sandwich" |> RR.s}
        </Row>
        <Row
          title="Burger"
          quantity={state.burger.quantity}
          enabled={state.burger.options != None}
          onQuantityChange={quantity => {
            let burger = {...state.burger, quantity};
            setState({...state, burger});
          }}
          onAdd={() =>
            state.burger.options
            |> Option.iter(burger =>
                 setState({
                   ...state,
                   cart:
                     state.cart
                     @ [
                       {
                         item: Burger(burger),
                         quantity: state.burger.quantity,
                       },
                     ],
                 })
               )
          }>
          {"Burger" |> RR.s}
        </Row>
      </tbody>
    </table>
    <button>
      {let cartCount = state.cart |> List.length |> string_of_int;
       React.string({j|Checkout ($cartCount)|j})}
    </button>
  </div>;
};
