let datasets = {
  let burger =
    Item.Burger.{
      lettuce: false,
      tomatoes: false,
      onions: 0,
      cheese: 0,
      bacon: 0,
    };
  [
    (
      "No burgers",
      Item.[
        Sandwich(Unicorn),
        Hotdog,
        Sandwich(Ham),
        Sandwich(Turducken),
        Hotdog,
      ],
    ),
    (
      "5 burgers",
      [
        Burger({...burger, tomatoes: true}),
        Burger({...burger, lettuce: true}),
        Burger({...burger, bacon: 2}),
        Burger({...burger, cheese: 3, onions: 9, tomatoes: true}),
        Burger({...burger, onions: 2}),
      ],
    ),
    (
      "1 burger with at least one of every topping",
      [
        Hotdog,
        Burger({
          lettuce: true,
          tomatoes: true,
          onions: 1,
          cheese: 2,
          bacon: 3,
        }),
        Sandwich(Turducken),
      ],
    ),
  ];
};

[@react.component]
let make = () => {
  let (items, setItems) =
    datasets
    |> ListSafe.head
    |> Option.map(snd)
    |> Option.value(~default=[])
    |> RR.useStateValue;

  <div>
    <h1> {RR.s("Order Confirmation")} </h1>
    <select
      onChange={evt => {
        let selectedLabel = evt |> RR.getValueFromEvent;
        datasets
        |> List.find_opt(((label, _items)) => label == selectedLabel)
        |> Option.iter(((_label, items)) => setItems(items));
      }}>
      {datasets
       |> List.map(((label, _items)) =>
            <option key={"option-" ++ label} value=label>
              {RR.s(label)}
            </option>
          )
       |> RR.list}
    </select>
    <Order items />
  </div>;
};
