module Burger = {
  type t = {
    lettuce: bool,
    onions: int,
    cheese: int,
    tomatoes: bool,
    bacon: int,
  };

  // #region to-emoji
  let toEmoji = t => {
    let multiple = (emoji, count) =>
      switch (count) {
      | 0 => ""
      | 1 => emoji
      | count => Printf.sprintf({js|%s×%d|js}, emoji, count)
      };

    switch (t) {
    | {lettuce: false, onions: 0, cheese: 0, tomatoes: false, bacon: 0} => {js|🍔|js}
    | {lettuce, onions, cheese, tomatoes, bacon} =>
      let toppingsCount =
        (lettuce ? 1 : 0) + (tomatoes ? 1 : 0) + onions + cheese + bacon;

      Printf.sprintf(
        {js|🍔%s{%s}|js},
        toppingsCount > 12 ? {js|🥣|js} : "",
        [|
          lettuce ? {js|🥬|js} : "",
          tomatoes ? {js|🍅|js} : "",
          multiple({js|🧅|js}, onions),
          multiple({js|🧀|js}, cheese),
          multiple({js|🥓|js}, bacon),
        |]
        |> Js.Array.filter(~f=str => str != "")
        |> Js.Array.join(~sep=","),
      );
    };
    // #endregion to-emoji
  };
};
