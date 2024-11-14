/**

// #region inferred-type
[> `CodeError(Discount.error)
 | `Discount(float)
 | `DiscountError([> `MissingSandwichTypes
                   | `NeedMegaBurger
                   | `NeedOneBurger
                   | `NeedTwoBurgers ])
 | `NoSubmittedCode ]
// #endregion inferred-type

 */

/**

// #region bad-discount-type
type discount =
  | CodeError(Discount.error)
  | Discount(float)
  | DiscountError(
      [>
        | `MissingSandwichTypes
        | `NeedMegaBurger
        | `NeedOneBurger
        | `NeedTwoBurgers
      ],
    )
  | NoSubmittedCode;
// #endregion bad-discount-type

*/

// #region delete-refinement
type discount =
  | CodeError(Discount.error)
  | Discount(float)
  | DiscountError(
      [
        | `MissingSandwichTypes
        | `NeedMegaBurger
        | `NeedOneBurger
        | `NeedTwoBurgers
      ],
    )
  | NoSubmittedCode;
// #endregion delete-refinement

module TypeVar = {
  // #region type-variable
  type discount('a) =
    | CodeError(Discount.error)
    | Discount(float)
    | DiscountError('a)
    | NoSubmittedCode;
  // #endregion type-variable
};

/**

// #region explicit-type-var
type discount =
  | CodeError(Discount.error)
  | Discount(float)
  | DiscountError(
      [>
        | `MissingSandwichTypes
        | `NeedMegaBurger
        | `NeedOneBurger
        | `NeedTwoBurgers
      ] as 'a,
    )
  | NoSubmittedCode;
// #endregion explicit-type-var

*/
module AddTypeArg = {
  // #region add-type-arg
  type discount('a) =
    | CodeError(Discount.error)
    | Discount(float)
    | DiscountError(
        [>
          | `MissingSandwichTypes
          | `NeedMegaBurger
          | `NeedOneBurger
          | `NeedTwoBurgers
        ] as 'a,
      )
    | NoSubmittedCode;
  // #endregion add-type-arg
};

module MustBePoly = {
  // #region must-be-poly
  type discount('a) =
    | CodeError(Discount.error)
    | Discount(float)
    | DiscountError([> ] as 'a)
    | NoSubmittedCode;
  // #endregion must-be-poly
};
