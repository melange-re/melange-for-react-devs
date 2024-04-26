type pane =
  | Main
  | Hotdog
  | Sandwich
  | Burger;

module Row = {
  [@react.component]
  let make = (~title, ~onClick: unit => unit) =>
    <tr>
      <td> {RR.s(title)} </td>
      <td>
        <button onClick={_ => onClick()}> {RR.s({js|➕|js})} </button>
      </td>
    </tr>;
};

module Quantity = {
  [@react.component]
  let make = (~value, ~onChange: int => unit) =>
    <div>
      <button onClick={_ => onChange(max(value - 1, 1))}>
        {RR.s({js|➖|js})}
      </button>
      <span> {value |> string_of_int |> RR.s} </span>
      <button onClick={_ => onChange(value + 1)}>
        {RR.s({js|➕|js})}
      </button>
    </div>;
};

module Pane = {
  [@react.component]
  let make =
      (
        ~emoji,
        ~onSubmit: int => unit,
        ~onClose: unit => unit,
        ~children=React.null,
      ) => {
    let (quantity, setQuantity) = RR.useStateValue(1);

    <div>
      <h1> {RR.s(emoji)} </h1>
      <div> children </div>
      <Quantity value=quantity onChange=setQuantity />
      <div>
        <button onClick={_ => onClose()}> {RR.s({js|❌|js})} </button>
        <button
          onClick={_ => {
            onSubmit(quantity);
            onClose();
          }}>
          {RR.s({js|➕🛒|js})}
        </button>
      </div>
    </div>;
  };
};

module SandwichCustomizer = {
  let choices =
    [Item.Sandwich.Portabello, Ham, Unicorn, Turducken]
    |> List.map(item => (item, item |> Item.Sandwich.toEmoji));

  [@react.component]
  let make = (~value: Item.Sandwich.t, ~onChange: Item.Sandwich.t => unit) => {
    <fieldset>
      {choices
       |> List.map(((item, label)) =>
            <div key={"radio-" ++ label}>
              <label>
                <input
                  type_="radio"
                  checked={item == value}
                  onClick={_ => onChange(item)}
                />
                {RR.s(label)}
              </label>
            </div>
          )
       |> RR.list}
    </fieldset>;
  };
};

module BurgerCustomizer = {
  [@react.component]
  let make = () => <div> {RR.s("Burger")} </div>;
};

[@react.component]
let make = () => {
  let (pane, setPane) = RR.useStateValue(Main);
  let (order: list(Item.t), setOrder) = RR.useStateValue([]);
  let (sandwich, setSandwich) = RR.useStateValue(Item.Sandwich.Portabello);
  let (burger, _setBurger) =
    RR.useStateValue({
      Item.Burger.lettuce: false,
      tomatoes: false,
      bacon: 0,
      onions: 0,
      cheese: 0,
    });
  let onClose = () => setPane(Main);

  <div>
    {switch (pane) {
     | Main =>
       <div>
         <h1> {{js|🍽️|js} |> RR.s} </h1>
         <table>
           <tbody />
           <Row title={js|🌭|js} onClick={() => setPane(Hotdog)} />
           <Row title={js|🥪|js} onClick={() => setPane(Sandwich)} />
           <Row title={js|🍔|js} onClick={() => setPane(Burger)} />
         </table>
         <button>
           {let orderSize = order |> List.length |> string_of_int;
            RR.s({j|➡️🛒($orderSize)|j})}
         </button>
       </div>
     | Hotdog =>
       <Pane
         emoji={js|🌭|js}
         onClose
         onSubmit={quantity =>
           setOrder(order @ List.init(quantity, _ => Item.Hotdog))
         }
       />
     | Sandwich =>
       <Pane
         emoji={js|🥪|js}
         onClose
         onSubmit={quantity =>
           setOrder(
             order @ List.init(quantity, _ => Item.Sandwich(sandwich)),
           )
         }>
         <SandwichCustomizer value=sandwich onChange=setSandwich />
       </Pane>
     | Burger =>
       <Pane
         emoji={js|🍔|js}
         onClose
         onSubmit={quantity =>
           setOrder(order @ List.init(quantity, _ => Item.Burger(burger)))
         }>
         <BurgerCustomizer />
       </Pane>
     }}
  </div>;
};
