let getBurgerDiscount = (items: array(Item.t)) => {
  let burgers =
    items
    |> Js.Array.filter(item =>
         switch (item) {
         | Item.Burger(_) => true
         | Sandwich(_)
         | Hotdog => false
         }
       )
    |> Js.Array.sortInPlaceWith((a, b) => {
         let a = Item.toPrice(a);
         let b = Item.toPrice(b);
         compare(b, a);
       });

  switch (burgers) {
  | [|Burger(_), Burger(cheaperBurger)|] =>
    let price = Item.Burger.toPrice(cheaperBurger);
    Some(price);
  | _ => None
  };
};
