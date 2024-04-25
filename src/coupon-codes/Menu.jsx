// Fill this out after the ReasonReact version is done

import React, { useState } from 'react';
// import css from "./menu.module.css";

export default function Menu() {
  const [state, setState] = useState({
    hotdog: {},
    sandwich: {},
    burger: {},
  })

  return (<div>
    <h1>Menu</h1>

    <table>
      <tbody>
      </tbody>
    </table>

    <button> Checkout </button>
  </div>);
}
