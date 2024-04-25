// [@mel.module "./Menu.jsx"] [@react.component]
// external make: unit => React.element = "default";

type options('a) = {
  item: option('a),
  quantity: int,
};

type cartItem = {
  item: Item.t,
  quantity: int,
};

type state = {
  hotdogQty: int,
  sandwich: options(Item.Sandwich.t),
  burger: options(Item.Burger.t),
  cart: list(cartItem),
};

module Row = {
  [@react.component]
  let make = (~title, ~quantity, ~enabled, ~onAdd) =>
    <tr>
      <td> {React.string(title)} </td>
      <td> <input type_="number" value={quantity |> string_of_int} /> </td>
      <td>
        <button disabled={!enabled} onClick={_ => onAdd()}>
          {React.string("Add")}
        </button>
      </td>
    </tr>;
};

[@react.component]
let make = () => {
  let (state, setState) =
    RR.useStateValue({
      hotdogQty: 1,
      sandwich: {
        item: None,
        quantity: 1,
      },
      burger: {
        item: None,
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
          enabled={state.sandwich.item != None}
          onAdd={() =>
            state.sandwich.item
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
          }
        />
        <Row
          title="Burger"
          quantity={state.burger.quantity}
          enabled={state.burger.item != None}
          onAdd={() =>
            state.burger.item
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
          }
        />
      </tbody>
    </table>
    <button>
      {let cartCount = state.cart |> List.length |> string_of_int;
       React.string({j|Checkout ($cartCount)|j})}
    </button>
  </div>;
};
