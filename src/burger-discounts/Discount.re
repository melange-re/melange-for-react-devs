// Buy 2 burgers, get 1 free
let getFreeBurger = (items: array(Item.t)) => {
  let burgers =
    items
    |> Js.Array.filter(~f=item =>
         switch (item) {
         | Item.Burger(_) => true
         | Sandwich(_)
         | Hotdog => false
         }
       )
    |> Js.Array.sortInPlaceWith(~f=(item1, item2) =>
         - compare(Item.toPrice(item1), Item.toPrice(item2))
       );

  switch (burgers) {
  | [|Burger(_), Burger(cheaperBurger)|] =>
    Some(Item.Burger.toPrice(cheaperBurger))
  | _ => None
  };
};
