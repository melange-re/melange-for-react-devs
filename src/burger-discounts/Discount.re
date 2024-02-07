let getBurgerDiscountOriginal = (items: array(Item.t)) => {
  let burgers =
    items
    |> Js.Array.filter(
         ~f=
           fun
           | Item.Burger(_) => true
           | Sandwich(_)
           | Hotdog => false,
       )
    |> Js.Array.sortInPlaceWith(~f=(a, b) => {
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

// This function only works when Array module isn't present
// let getBurgerDiscountCatchException = (items: array(Item.t)) => {
//   let burgers =
//     items
//     |> Js.Array.filter(
//          ~f=((item: Item.t) =>
//            switch (item) {
//            | Burger(_) => true
//            | Sandwich(_)
//            | Hotdog => false
//            },
//        )
//     |> Js.Array.sortInPlaceWith(~f=(a, b) => {
//          let a = Item.toPrice(a);
//          let b = Item.toPrice(b);
//          compare(b, a);
//        });

//   switch (burgers[0], burgers[1]) {
//   | exception _ => None
//   | (Burger(_), Burger(cheaperBurger)) =>
//     let price = Item.Burger.toPrice(cheaperBurger);
//     Some(price);
//   | _ => None
//   };
// };

let getBurgerDiscount = (items: array(Item.t)) => {
  let burgers =
    items
    |> Js.Array.filter(
         ~f=
           fun
           | Item.Burger(_) => true
           | Sandwich(_)
           | Hotdog => false,
       )
    |> Js.Array.sortInPlaceWith(~f=(a, b) => {
         let a = Item.toPrice(a);
         let b = Item.toPrice(b);
         compare(b, a);
       });

  switch (burgers[0], burgers[1]) {
  | (Some(Burger(_)), Some(Burger(cheaperBurger))) =>
    let price = Item.Burger.toPrice(cheaperBurger);
    Some(price);
  | _ => None
  };
};
