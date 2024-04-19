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

// Buy 1+ burger with 1+ of every topping, get half off
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
      |> ListLabels.fold_left(~init=0.0, ~f=(total, item) =>
           total +. Item.toPrice(item)
         );
    Some(total /. 2.0);
  };
};
