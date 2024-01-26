// type t =
//   | TwoBurgerOneFree;

// type discount =
//   | Fixed(int)
//   | Percentage(int);

let getDiscount = (items: array(Item.t)) => {
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
         compare(a, b);
       });

  Array.length(burgers) < 2 ? None : Some(Item.toPrice(burgers[1]));
};
