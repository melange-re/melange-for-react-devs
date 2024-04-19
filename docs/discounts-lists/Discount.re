// #region get-free-burger
/** Buy 2 burgers, get 1 free */
let getFreeBurger = (items: list(Item.t)) => {
  let prices =
    items
    |> List.filter(item =>
         switch (item) {
         | Item.Burger(_) => true
         | Sandwich(_)
         | Hotdog => false
         }
       )
    |> List.map(Item.toPrice)
    |> List.sort((x, y) => - compare(x, y));

  switch (prices) {
  | [_, cheaperPrice, ..._] => Some(cheaperPrice)
  | _ => None
  };
};
// #endregion get-free-burger

// #region get-half-off
/** Buy 1+ burger with 1+ of every topping, get half off */
let getHalfOff = (items: list(Item.t)) => {
  let meetsCondition =
    items
    |> List.exists(
         fun
         | Item.Burger({lettuce: true, tomatoes: true, onions, cheese, bacon})
             when onions > 0 && cheese > 0 && bacon > 0 =>
           true
         | Burger(_)
         | Sandwich(_)
         | Hotdog => false,
       );

  switch (meetsCondition) {
  | false => None
  | true =>
    let total =
      items
      |> List.fold_left((total, item) => total +. Item.toPrice(item), 0.0);
    Some(total /. 2.0);
  };
};
// #endregion get-half-off
