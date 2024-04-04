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
    |> Js.Array.map(~f=Item.toPrice)
    |> Js.Array.sortInPlaceWith(~f=(x, y) => - compare(x, y));

  switch (burgers[0], burgers[1]) {
  | (Some(_), Some(cheaperPrice)) => Some(cheaperPrice)
  | _ => None
  };
};
