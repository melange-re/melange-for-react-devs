let datasets = {
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
      {
        let burger =
          Item.Burger.{
            lettuce: false,
            tomatoes: false,
            onions: 0,
            cheese: 0,
            bacon: 0,
          };
        [
          Burger({...burger, tomatoes: true}),
          Burger({...burger, lettuce: true}),
          Burger({...burger, bacon: 2}),
          Burger({...burger, cheese: 3, onions: 9, tomatoes: true}),
          Burger({...burger, onions: 2}),
        ];
      },
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
let make = () =>
  <div>
    <h1> {RR.s("Order Confirmation")} </h1>
    {datasets
     |> List.map(((label, items)) => {
          let slug =
            label
            |> Js.String.toLowerCase
            |> Js.String.replaceByRe(~regexp=[%re "/ /g"], ~replacement="-");
          <div key={"order-" ++ slug}>
            <a name=slug href={"#" ++ slug}> <h2> {label |> RR.s} </h2> </a>
            <Order items />
          </div>;
        })
     |> RR.list}
  </div>;
