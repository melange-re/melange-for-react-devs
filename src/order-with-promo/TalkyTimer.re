module Voice = SpeechSynthesis.Voice;

let moveToFront = (pred, lst) => {
  lst
  |> List.find_opt(pred)
  |> Option.map(element => {
       [element, ...lst |> List.filter(e => !pred(e))]
     })
  |> Option.value(~default=lst);
};

let moveToFrontUsingRef = (pred, lst) => {
  let element = ref(None);

  let withoutElement =
    lst
    |> List.filter(e =>
         pred(e)
           ? {
             element := Some(e);
             false;
           }
           : true
       );

  switch (element^) {
  | None => lst
  | Some(element) => [element, ...withoutElement]
  };
};

let getVoices = () => {
  SpeechSynthesis.getVoices()
  |> Promise.map(voices => {
       voices
       |> Array.toList
       |> List.filter(v =>
            v |> Voice.getLang |> Js.String.startsWith(~prefix="en-")
          )
       |> moveToFrontUsingRef(v => Voice.getName(v) == "Zarvox")
     });
};

let speak = (voice, text) => {
  Js.Promise.make((~resolve, ~reject as _) => {
    SpeechSynthesis.cancel(); // cancel active utterance

    module Utterance = SpeechSynthesis.Utterance;
    let utterance = Utterance.make(text);
    Utterance.setVoice(utterance, voice);
    utterance
    |> Utterance.addOnEndListener(() => {
         let unitValue = (); // refmt bug forces you to do this
         resolve(. unitValue);
       });
    SpeechSynthesis.speak(utterance);
  });
};

module Inner = {
  type timerState =
    | Stopped
    | Running(int)
    | Alarm;

  type state = {
    duration: int,
    timerState,
    voice: Voice.t,
  };

  type action =
    | ChangeDuration(int)
    | ChangeVoice(Voice.t)
    | Start
    | Stop
    | Tick
    | SoundAlarm;

  let reducer = (state, action) => {
    switch (action) {
    | ChangeDuration(duration) => {...state, duration}
    | ChangeVoice(voice) => {...state, voice}
    | Start => {...state, timerState: Running(state.duration)}
    | Tick => {
        ...state,
        timerState:
          switch (state.timerState) {
          | (Stopped | Alarm) as t => t
          | Running(counter) when counter <= 0 => Alarm
          | Running(counter) => Running(counter - 1)
          },
      }
    | Stop => {...state, timerState: Stopped}
    | SoundAlarm => {...state, timerState: Alarm}
    };
  };

  [@react.component]
  let make = (~voice as initialVoice: Voice.t, ~voices: list(Voice.t)) => {
    let (state, dispatch) =
      React.useReducer(
        reducer,
        {duration: 10, timerState: Stopped, voice: initialVoice},
      );

    let intervalId = React.useRef(None);

    // let soundAlarm = () => {
    //   speak(state.voice, "Time's up!") |> ignore;
    // };

    let tick = () => dispatch @@ Tick;

    <div>
      <NumberInput
        disabled={
          switch (state.timerState) {
          | Running(_)
          | Alarm => true
          | Stopped => false
          }
        }
        value={
          switch (state.timerState) {
          | Alarm => 0
          | Stopped => state.duration
          | Running(counter) => counter
          }
        }
        onChange={duration => dispatch @@ ChangeDuration(duration)}
      />
      <select
        value={Voice.getName(state.voice)}
        onChange={evt => {
          let name = RR.getValueFromEvent(evt);
          voices
          |> List.find_opt(v => Voice.getName(v) == name)
          |> Option.iter(voice => dispatch @@ ChangeVoice(voice));
        }}>
        {voices
         |> List.map(v => {
              let name = v |> Voice.getName;
              <option key=name value=name> {name |> RR.s} </option>;
            })
         |> RR.list}
      </select>
      <button
        onClick={_evt =>
          switch (state.timerState) {
          | Stopped =>
            dispatch @@ Start;
            intervalId.current =
              Js.Global.setInterval(1000, ~f=tick) |> Option.some;
          | Alarm
          | Running(_) =>
            dispatch @@ Stop;
            intervalId.current |> Option.iter(Js.Global.clearInterval);
            intervalId.current = None;
          }
        }>
        {(
           switch (state.timerState) {
           | Running(_)
           | Alarm => "Stop"
           | Stopped => "Start"
           }
         )
         |> RR.s}
      </button>
    </div>;
  };
};

[@react.component]
let make = () => {
  let (voices, setVoices) = RR.useStateValue([]);

  React.useEffect0(() => {
    getVoices() |> Promise.iter(setVoices);
    None;
  });

  switch (voices) {
  | [] => React.null
  | [voice, ..._] => <Inner voice voices />
  };
};
