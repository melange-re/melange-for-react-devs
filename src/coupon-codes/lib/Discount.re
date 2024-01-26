let getBurgerDiscount = (items: array(Item.t)) => {
  let burgers =
    items
    |> Js.Array.sortInPlaceWith((a, b) => {
         let a = Item.toPrice(a);
         let b = Item.toPrice(b);
         compare(b, a);
       })
    |> Js.Array.filter(item =>
         switch (item) {
         | Item.Burger(_) => true
         | Sandwich(_)
         | Hotdog => false
         }
       );

  Array.length(burgers) < 2 ? None : Some(Item.toPrice(burgers[1]));
};
