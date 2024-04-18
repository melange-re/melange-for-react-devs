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
