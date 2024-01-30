let getBurgerDiscount = (items: array(Item.t)) => {
  let burgers =
    items
    |> Js.Array.filter(
         fun
         | Item.Burger(_) => true
         | Sandwich(_)
         | Hotdog => false,
       )
    |> Js.Array.sortInPlaceWith((a, b) => {
         let a = Item.toPrice(a);
         let b = Item.toPrice(b);
         compare(b, a);
       });

  Js.Array.length(burgers) < 2
    ? None
    : (
      switch (burgers |> Js.Array.slice(~start=0, ~end_=2)) {
      | [|Burger(_), Burger(cheaperBurger)|] =>
        let price = Item.Burger.toPrice(cheaperBurger);
        Some(price);
      | _ => None // unreachable
      }
    );
};
