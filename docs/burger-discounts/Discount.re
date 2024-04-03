// Mock function just to get code to compile
let getFreeBurger = (_items: array(Item.t)) => None;

let _ =
  (items: array(Item.t)) => {
    let burgers =
      items
      // #region type-annotate-argument
      |> Js.Array.filter(~f=(item: Item.t) =>
           switch (item) {
           | Burger(_) => true
           | Sandwich(_)
           | Hotdog => false
           }
         );
    // #endregion type-annotate-argument
    ignore(burgers);
  };

let _ =
  (items: array(Item.t)) => {
    let burgers =
      items
      // #region full-name-constructors
      |> Js.Array.filter(~f=item =>
           switch (item) {
           | Item.Burger(_) => true
           | Item.Sandwich(_)
           | Item.Hotdog => false
           }
         );
    // #endregion full-name-constructors
    ignore(burgers);
  };

let _ =
  (items: array(Item.t)) => {
    let burgers =
      items
      // #region full-name-constructor
      |> Js.Array.filter(~f=item =>
           switch (item) {
           | Item.Burger(_) => true
           | Sandwich(_)
           | Hotdog => false
           }
         );
    // #endregion full-name-constructor
    ignore(burgers);
  };

let _ =
  (items: array(Item.t)) => {
    let burgers =
      items
      // #region full-name-fun
      |> Js.Array.filter(
           ~f=
             fun
             | Item.Burger(_) => true
             | Sandwich(_)
             | Hotdog => false,
         );
    // #endregion full-name-fun
    ignore(burgers);
  };

let _ =
  (items: array(Item.t)) => {
    // #region swap-function-order
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
       )
    // #endregion swap-function-order
    |> ignore;
  };

let _ = {
  let burgers: array(Item.t) = [||];

  // #region match-on-tuple
  switch (burgers[0], burgers[1]) {
  | (Burger(_), Burger(cheaperBurger)) =>
    Some(Item.Burger.toPrice(cheaperBurger))
  | _ => None
  };
  // #endregion match-on-tuple
};

let _ = {
  let burgers: array(Item.t) = [||];

  // #region catch-exception
  switch (burgers[0], burgers[1]) {
  | exception (Invalid_argument(_)) => None
  | (Burger(_), Burger(cheaperBurger)) =>
    Some(Item.Burger.toPrice(cheaperBurger))
  | _ => None
  };
  // #endregion catch-exception
};

module Array = {
  // #region module-array
  // Safe array access function
  let get = (array: array('a), index): option('a) =>
    switch (index) {
    | index when index < 0 || index >= Js.Array.length(array) => None
    | index => Some(Stdlib.Array.get(array, index))
    };
  // #endregion module-array
};

let _ = {
  let burgers: array(Item.t) = [||];

  // #region custom-array-get
  switch (burgers[0], burgers[1]) {
  | (Some(Burger(_)), Some(Burger(cheaperBurger))) =>
    Some(Item.Burger.toPrice(cheaperBurger))
  | _ => None
  };
  // #endregion custom-array-get
};
