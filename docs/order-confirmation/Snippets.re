module ItemV1 = {
  // #region type-t
  type t =
    | Sandwich
    | Burger;
  // #endregion type-t

  // #region to-price
  let toPrice = t =>
    switch (t) {
    | Sandwich => 10.
    | Burger => 15.
    };
  // #endregion to-price
};

module Item = {
  type t =
    | Sandwich
    | Burger;

  // #region to-price-fun
  let toPrice =
    fun
    | Sandwich => 10.
    | Burger => 15.;
  // #endregion to-price-fun

  // #region to-emoji
  let toEmoji =
    fun
    | Sandwich => {js|🥪|js}
    | Burger => {js|🍔|js};
  // #endregion to-emoji

  // #region make
  [@react.component]
  let make = (~item: t) =>
    <tr>
      <td> {item |> toEmoji |> React.string} </td>
      <td>
        {item
         |> toPrice
         |> Js.Float.toFixedWithPrecision(~digits=2)
         |> React.string}
      </td>
    </tr>;
  // #endregion make
};

module Order = {
  // #region order
  type t = array(Item.t);

  [@react.component]
  let make = (~items: t) => {
    let total =
      items
      |> Js.Array.reduce((acc, order) => acc +. Item.toPrice(order), 0.);

    <table>
      <tbody>
        {items |> Js.Array.map(item => <Item item />) |> React.array}
        <tr>
          <td> {React.string("Total")} </td>
          <td>
            {total |> Js.Float.toFixedWithPrecision(~digits=2) |> React.string}
          </td>
        </tr>
      </tbody>
    </table>;
  };
  // #endregion order

  let makeWithItemRows = (~items: t) => {
    // #region order-make-item-rows
    let total =
      items
      |> Js.Array.reduce((acc, order) => acc +. Item.toPrice(order), 0.);

    let itemRows = items |> Js.Array.map(item => <Item item />);

    <table>
      <tbody>
        {itemRows |> React.array}
        <tr>
          <td> {React.string("Total")} </td>
          <td>
            {total |> Js.Float.toFixedWithPrecision(~digits=2) |> React.string}
          </td>
        </tr>
      </tbody>
    </table>;
    // #endregion order-make-item-rows
  };
};

module Index = {
  // #region index
  module App = {
    let items: Order.t = [|Sandwich, Burger, Sandwich|];

    [@react.component]
    let make = () =>
      <div>
        <h1> {React.string("Order confirmation")} </h1>
        <Order items />
      </div>;
  };

  let node = ReactDOM.querySelector("#root");
  switch (node) {
  | None =>
    Js.Console.error("Failed to start React: couldn't find the #root element")
  | Some(root) => ReactDOM.render(<App />, root)
  };
  // #endregion index
};

let _ = {
  let items = [||];
  // #region mapi
  items
  |> Js.Array.mapi((item, index) =>
       <Item key={"item-" ++ string_of_int(index)} item />
     )
  |> React.array
  // #endregion mapi
  |> ignore;
};

module ItemV2 = {
  // #region hotdog
  type t =
    | Sandwich
    | Burger
    | Hotdog;

  let toPrice =
    fun
    | Sandwich => 10.
    | Burger => 15.
    | Hotdog => 5.;

  let toEmoji =
    fun
    | Sandwich => {js|🥪|js}
    | Burger => {js|🍔|js}
    | Hotdog => {js|🌭|js};
  // #endregion hotdog
};
